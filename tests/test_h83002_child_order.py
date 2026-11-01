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

 def test_nested_constructor_tags_and_missing_tag(self):
  raw=b'h83002_device::h83002_device() : m_dma(*this, "dma"), m_dma0(*this, "dma:0") {}\nvoid h83002_device::map() {}\nvoid h83002_device::device_add_mconfig(machine_config &config)\n{\n H8H_DMA(config, m_dma, *this);\n H8H_DMA_CHANNEL(config, m_dma0, *this);\n}\n'
  self.assertEqual(extract(raw)['constructor_tags'],{'m_dma':'dma','m_dma0':'dma:0'})
  with self.assertRaises(ValueError):extract(raw.replace(b'm_dma0(*this, "dma:0")',b'm_other(*this, "dma:0")'))
