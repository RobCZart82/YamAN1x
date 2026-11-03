"""Local-only headless LC7985 differential against locked unmodified MAME methods."""
import argparse
import hashlib
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
    key = 'src/devices/video/lc7985.cpp'
    with verified_archive(args.archive, lock) as archive:
        names = [n for n in archive.namelist() if n.endswith('/'+key)]
        if len(names) != 1:
            raise ValueError('Expected one original LC7985 source')
        raw = archive.read(names[0])
    if hashlib.sha256(raw).hexdigest() != lock['inputs'][key]:
        raise ValueError('Original LC7985 file identity mismatch')
    methods = []
    for name in ['device_reset','inc_ddac','dec_ddac','shift_left','shift_right',
                 'ir_w','status_r','dr_w','dr_r']:
        found = re.findall(r'^(?:void|u8) lc7985_device::'+name+r'\([^\n]*\)\n\{.*?^\}',
                           raw.decode(), re.M | re.S)
        if len(found) != 1:
            raise ValueError('Expected one original method: '+name)
        methods.append(found[0].replace('lc7985_device::', 'Original::'))
    prefix = r'''
#include "lc7985_controller.h"
#include <cstring>
#include <cstdio>
#include <stdexcept>
using u8=uint8_t;
struct attotime { static uint64_t from_usec(uint64_t n){return n;}
 static uint64_t from_msec(uint64_t n){return n*1000;} };
struct Original {
 u8 m_ddram[80]{},m_cgram[64]{},m_ddac=0,m_cgac=0,m_shift=0,m_function=0,m_cds=0,m_display=0,m_entry=0;
 bool m_busy_flag=false,m_access_ddram=false;uint64_t now=0,end=0;
 void busy(uint64_t duration){m_busy_flag=true;end=now+duration;}
 void at(uint64_t t){now=t;if(now>=end)m_busy_flag=false;}
 void device_reset();void inc_ddac();void dec_ddac();void shift_left();void shift_right();
 void ir_w(u8);u8 status_r();void dr_w(u8);u8 dr_r();
};
'''
    suffix = r'''
int main(){
 Original ref;ref.device_reset();yamanix::Lc7985Controller model(1);
 uint64_t t=0;uint32_t rng=9281;
 auto next=[&](){rng=rng*1664525u+1013904223u;return rng;};
 for(unsigned step=0;step<50000;++step){
  t+=next()%20000;ref.at(t);const auto operation=next()%5;const auto value=u8(next()>>24);
  const unsigned index=(ref.m_function&8)&&ref.m_ddac>=64?ref.m_ddac-24:ref.m_ddac;
  const bool safe=!ref.m_access_ddram||index<80;
  if(operation==0){ref.ir_w(value);model.instruction_write(t,value);}
  else if(operation==1&&safe){ref.dr_w(value);model.data_write(t,value);}
  else if(operation==2&&safe){if(ref.dr_r()!=model.data_read(t))throw std::runtime_error("Data mismatch");}
  else if(operation==3){ref.device_reset();model.reset(t);}
  const auto status=model.status_read(t);const auto&s=model.state();
  if(status!=ref.status_r()||std::memcmp(s.ddram.data(),ref.m_ddram,80)||
   std::memcmp(s.cgram.data(),ref.m_cgram,64)||s.ddac!=ref.m_ddac||s.cgac!=ref.m_cgac||
   s.shift!=ref.m_shift||s.function!=ref.m_function||s.display!=ref.m_display||
   s.entry!=ref.m_entry||s.ddram_selected!=ref.m_access_ddram||model.timing().deadline!=ref.end)
   throw std::runtime_error("Original controller state mismatch");
 }
 std::puts("50000 locked original LCD operation/state comparisons PASS (safe address profile)");
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-lcd-reference-') as temp:
        source = Path(temp)/'check.cpp'
        binary = Path(temp)/'check'
        source.write_text(prefix+'\n'.join(methods)+suffix)
        subprocess.run([args.compiler,'-std=c++20','-O1','-g','-fsanitize=address,undefined',
                        '-fno-sanitize-recover=all','-I'+str(root/'src'),str(source),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)


if __name__ == '__main__':
    main()
