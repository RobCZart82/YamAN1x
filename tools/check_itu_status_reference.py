"""Locked original H8H latched status/enable differential;no counter/IRQ dispatch proof."""
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
#include "h8_itu_status.h"
#include <cstdio>
using u8=uint8_t;
struct h8h_timer16_channel_device {
 enum {IRQ_A=1,IRQ_B=2,IRQ_V=16};
 u8 m_tier=0xf8,m_ier=0,m_isr=0;
 void tier_update();void isr_update(u8);u8 isr_to_sr()const;
};
'''
    suffix = r'''
int main(){unsigned cases=0;
 for(unsigned events=0;events<8;++events)for(unsigned enable=0;enable<256;++enable)for(unsigned clear=0;clear<256;++clear){
  h8h_timer16_channel_device original;original.m_tier=u8(enable);original.tier_update();
  original.m_isr=u8((events&3)|((events&4)?16:0));
  yamanix::H8ItuStatus adapted;adapted.write_enables(u8(enable));adapted.latch(u8(events));
  if(adapted.enables()!=original.m_tier||adapted.status()!=original.isr_to_sr())throw std::runtime_error("Original latched state mismatch");
  original.isr_update(u8(clear));adapted.write_status(u8(clear));
  const unsigned original_pending=original.m_isr&original.m_ier;
  const unsigned pending=(original_pending&3)|((original_pending&16)?4:0);
  if(adapted.status()!=original.isr_to_sr()||adapted.pending()!=pending)throw std::runtime_error("Original status clear/enable mismatch");
  ++cases;
 }
 std::printf("%u original H8H event/enable/clear comparisons PASS;events supplied explicitly,no counter/IRQ dispatch proof\n",cases);
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-itu-status-reference-') as temp:
        source_path = Path(temp)/'check.cpp'
        binary = Path(temp)/'check'
        source_path.write_text(prefix+'\n'.join(methods)+suffix)
        subprocess.run([args.compiler,'-std=c++20','-O1','-fsanitize=address,undefined',
                        '-fno-sanitize-recover=all','-I'+str(root/'src'),str(source_path),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)


if __name__ == '__main__':
    main()
