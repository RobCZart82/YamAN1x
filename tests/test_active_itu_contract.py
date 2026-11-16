import hashlib
import json
from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from check_active_itu_contract import identity

class ActiveItuIdentity(unittest.TestCase):
    def test_pinned_tstr_map_writer_identity(self):
        root=Path(__file__).resolve().parents[1]
        lock=json.loads((root/'research/mame-source-lock.json').read_text())
        contract=json.loads((root/'research/h83002-active-itu-contract.json').read_text())
        self.assertEqual(lock['commit'],'706dbb73281320c54d7c2f16262c1c3786f2be01')
        self.assertEqual(contract['upstream_commit'],lock['commit'])
        timer=next(item for item in contract['files'] if item['path'].endswith('h8_timer16.cpp'))
        mapping=next(item for item in contract['files'] if item['path'].endswith('h83002.cpp'))
        writer=next(item for item in timer['methods'] if item['name']=='h8_timer16_device::tstr_w')
        self.assertEqual(timer['sha256'],'7039beac99834b03ebd25fd20394c40f228cc9e33ee26c1e265a51c544da0c5c')
        self.assertEqual(mapping['sha256'],'2b422dc4081700cccc27d3a6f62307ae5f138f4f601181015ff1d6605ecf1e92')
        self.assertEqual(writer,{'name':'h8_timer16_device::tstr_w','bytes':181,'sha256':'238146e5b9de1139c94baf14c5e6471012b21f3d092dce594b03f41b516efc8f'})

    def test_exact_const_method(self):
        data=b'u8 timer::read() const\n{\n\treturn 7;\n}'
        self.assertEqual(identity(data+b'\n','timer::read'),{'name':'timer::read','bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
    def test_missing_or_ambiguous(self):
        data=b'void timer::start(bool enable)\n{\n}\n'
        for raw in (b'',data+data):
            with self.assertRaises(ValueError):identity(raw,'timer::start')
    def test_body_changes_identity(self):
        a=b'u64 timer::tick(u64 now)\n{\n return now;\n}'
        self.assertNotEqual(identity(a,'timer::tick')['sha256'],identity(a.replace(b'return now;',b'return now+1;'),'timer::tick')['sha256'])
