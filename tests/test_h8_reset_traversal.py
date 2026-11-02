import importlib.util
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
import zipfile
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
import h8_reset_traversal as m
class TraversalTests(unittest.TestCase):
 def test_identity_change_and_missing_method(self):
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)/'source.zip'
   def write(change=False,missing=False):
    with zipfile.ZipFile(p,'w') as z:
     for leaf,names in m.METHODS.items():
      text='\n'.join('void '+n+'()\n{\n '+('changed();' if change else 'original();')+'\n}\n' for n in names if not missing or n!=names[0])
      if leaf=='diexec.cpp':text+='\nTIMER_CALLBACK_MEMBER(device_execute_interface::device_input::empty_event_queue)\n{\n}\n'
      z.writestr('root/src/emu/'+leaf,text)
   with patch.object(m,'verified_archive',side_effect=lambda p,l:zipfile.ZipFile(p)) as verify:
    write();a=m.report(p,{'commit':'test'});verify.assert_called_once()
    write(True);self.assertNotEqual(a,m.report(p,{'commit':'test'}))
    write(missing=True)
    with self.assertRaises(ValueError):m.report(p,{'commit':'test'})
 def test_hash_failure_prevents_zip_open(self):
  with patch.object(m,'verified_archive',side_effect=ValueError('bad archive')),patch.object(m.zipfile,'ZipFile') as z:
   with self.assertRaises(ValueError):m.report(Path('unused'),{})
   z.assert_not_called()

 def test_queued_callback_identity(self):
  raw=b'TIMER_CALLBACK_MEMBER(device_execute_interface::device_input::empty_event_queue)\n{\n reset();\n}\n'
  a=m.queued_reset_identity(raw)
  self.assertNotEqual(a,m.queued_reset_identity(raw.replace(b'reset();',b'resume();')))
  for invalid in [b'',raw+raw]:
   with self.assertRaises(ValueError):m.queued_reset_identity(invalid)
