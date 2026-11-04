import hashlib
from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from check_active_itu_contract import identity

class ActiveItuIdentity(unittest.TestCase):
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
