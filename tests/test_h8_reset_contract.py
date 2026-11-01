import importlib.util
from pathlib import Path
import unittest
spec=importlib.util.spec_from_file_location('reset_contract',Path(__file__).resolve().parents[1]/'tools/h8_reset_contract.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
class ResetIdentityTests(unittest.TestCase):
 def test_identity_and_mutation(self):
  raw=b'void cpu::device_reset()\n{\n if(x){\n  f();\n }\n}\n'
  a=m.method_identity(raw,'cpu::device_reset')
  self.assertNotEqual(a['sha256'],m.method_identity(raw.replace(b'f();',b'g();'),'cpu::device_reset')['sha256'])
  self.assertEqual(a['bytes'],len(raw)-1)
 def test_missing_and_duplicate(self):
  raw=b'void cpu::device_reset()\n{\n}\n'
  for bad in [b'',raw+raw]:
   with self.assertRaises(ValueError):m.method_identity(bad,'cpu::device_reset')
 def test_nonreset_cannot_substitute(self):
  with self.assertRaises(ValueError):m.method_identity(b'void cpu::device_start()\n{\n}\n','cpu::device_reset')
 def test_replacement_after_hash_uses_verified_snapshot(self):
  import tempfile,zipfile,hashlib
  from unittest.mock import patch
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)/'source.zip'
   with zipfile.ZipFile(p,'w') as z:
    for leaf,names in m.METHODS.items():
     z.writestr('root/src/devices/cpu/h8/'+leaf,'\n'.join('void '+n+'()\n{\n original();\n}\n' for n in names))
   lock={'archive_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'commit':'test'}
   expected=m.from_archive(p,lock);original=zipfile.ZipFile
   def swap_then_open(snapshot):
    p.write_bytes(b'replaced after verification')
    return original(snapshot)
   with patch.object(m.zipfile,'ZipFile',side_effect=swap_then_open):
    self.assertEqual(m.from_archive(p,lock),expected)
 def test_archive_hash_checked_before_zip(self):
  import tempfile
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)/'invalid.zip';p.write_bytes(b'not source')
   with self.assertRaisesRegex(ValueError,'Archive differs'):m.from_archive(p,{'archive_sha256':'0'*64,'commit':'test'})
