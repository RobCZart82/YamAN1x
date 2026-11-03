"""Local-only locked H8H TIER/empty-TSR method differential;not active timer proof."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import tempfile
from h8_reset_contract import verified_archive


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--compiler', default='c++')
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    lock = json.loads((root/'research/mame-source-lock.json').read_text())
    with verified_archive(args.archive, lock) as archive:
        key = 'src/devices/cpu/h8/h8_timer16.cpp'
        names = [n for n in archive.namelist() if n.endswith('/'+key)]
        if len(names) != 1:
            raise ValueError('Expected one original timer source')
        source = archive.read(names[0]).decode()
    methods = []
    for name in ['tier_update','isr_update','isr_to_sr']:
        found = re.findall(r'^(?:void|u8) h8h_timer16_channel_device::'+name+
                           r'\([^\n]*\)(?: const)?\n\{.*?^\}',source,re.M|re.S)
        if len(found) != 1:
            raise ValueError('Expected one H8H method: '+name)
        methods.append(found[0])
    prefix = r'''
#include "an1x_sci_bus.h"
#include <cstdio>
using u8=uint8_t;
struct h8h_timer16_channel_device {
 enum {IRQ_A=1,IRQ_B=2,IRQ_V=16};
 u8 m_tier=0xf8,m_ier=0,m_isr=0;
 void tier_update();void isr_update(u8);u8 isr_to_sr()const;
};
struct Sink {void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}
 bool dma_request(unsigned,uint64_t){return false;}};
'''
    suffix = r'''
int main(){std::vector<u8>rom(0x100000);Sink sink;yamanix::An1xSciBus<Sink>bus(rom,{},sink);
 for(uint32_t address:{0xffff66u,0xffff70u,0xffff7au,0xffff84u,0xffff94u})
  for(unsigned value=0;value<256;++value){
   h8h_timer16_channel_device original;original.m_tier=u8(value);original.tier_update();
   bus.write_byte(address,u8(value));bus.write_byte(address+1,u8(value));original.isr_update(u8(value));
   if(bus.read_byte(address)!=original.m_tier||bus.read_byte(address+1)!=original.isr_to_sr())
    throw std::runtime_error("Original stopped TIER/empty-TSR mismatch");
  }
 std::puts("1280 locked original TIER/empty-TSR comparisons PASS;no active counter/event/IRQ proof");
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-tier-reference-') as temp:
        source_path = Path(temp)/'check.cpp'
        binary = Path(temp)/'check'
        source_path.write_text(prefix+'\n'.join(methods)+suffix)
        subprocess.run([args.compiler,'-std=c++20','-O1','-fsanitize=address,undefined',
                        '-fno-sanitize-recover=all','-I'+str(root/'src'),str(source_path),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)


if __name__ == '__main__':
    main()
