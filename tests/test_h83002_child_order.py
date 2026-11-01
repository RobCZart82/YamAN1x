import sys
from pathlib import Path
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from h83002_child_order import extract
class ChildOrderTests(unittest.TestCase):
 def test_order_identity_and_duplicate(self):
  def source(body):return ('void h83002_device::device_add_mconfig(machine_config &config)\n{\n'+body+'\n}\n').encode()
  a=' H8_SCI(config, m_sci[0], 0);\n H8_SCI(config, m_sci[1], 1);'
  result=extract(source(a))
  self.assertEqual([d['member'] for d in result['declarations']],['m_sci[0]','m_sci[1]'])
  self.assertNotEqual(result,extract(source('\n'.join(reversed(a.splitlines())))))
  with self.assertRaises(ValueError):extract(source(a.replace('m_sci[1]','m_sci[0]')))
 def test_missing_method(self):
  with self.assertRaises(ValueError):extract(b'void other() {}')
