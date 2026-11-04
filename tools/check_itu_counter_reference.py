"""Counter-value differential against locked original update_counter;no IRQ/MMIO proof."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tempfile
from h8_reset_contract import verified_archive

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive',required=True,type=Path)
    parser.add_argument('--compiler',default='c++')
    args=parser.parse_args()
    root=Path(__file__).resolve().parents[1]
    lock=json.loads((root/'research/mame-source-lock.json').read_text())
    manifest=json.loads((root/'research/h83002-active-itu-contract.json').read_text())
    with verified_archive(args.archive,lock) as archive:
        names=[n for n in archive.namelist() if n.endswith('/src/devices/cpu/h8/h8_timer16.cpp')]
        if len(names)!=1:raise ValueError('Expected one original timer source')
        raw=archive.read(names[0])
    expected=next(f for f in manifest['files'] if f['path'].endswith('/h8_timer16.cpp'))
    if hashlib.sha256(raw).hexdigest()!=expected['sha256']:raise ValueError('Original timer source differs')
    matches=re.findall(r'^void h8_timer16_channel_device::update_counter\(u64 cur_time\)\n\{.*?^\}',raw.decode(),re.M|re.S)
    if len(matches)!=1:raise ValueError('Expected one update_counter')
    method=matches[0]
    identity=next(m for m in expected['methods'] if m['name'].endswith('::update_counter'))
    if hashlib.sha256(method.encode()).hexdigest()!=identity['sha256']:raise ValueError('Original counter method differs')
    prefix=r'''
#include "h8_itu_counter.h"
#include <cstdio>
#include <cstdlib>
#include <initializer_list>
using u64=uint64_t;using u16=uint16_t;using u32=uint32_t;
void logerror(const char*,...){}
bool BIT(unsigned value,int bit){return (value>>bit)&1;}
struct Cpu {u64 now=0;u64 total_cycles(){return now;}};
struct Intc {void internal_interrupt(int){throw std::runtime_error("IRQ outside value-only reference profile");}};
struct h8_timer16_channel_device {
 enum {DIV_1=0,IRQ_V=16};Cpu cpu;Intc intc;Cpu*m_cpu=&cpu;Intc*m_intc=&intc;
 unsigned m_clock_type=DIV_1,m_clock_divider=0,m_phase=0,m_counter_cycle=65536;
 u16 m_tcnt=0,m_tgr[2]{};u64 m_last_clock_update=0;bool m_channel_active=true,m_counter_incrementing=true;
 int m_tgr_count=0,m_interrupt[6]{};unsigned m_ier=0,m_isr=0;
 void update_counter(u64);
};
'''
    suffix=r'''
int main(){uint64_t seed=0xabc123;unsigned count=0;
 for(unsigned shift=0;shift<=3;++shift)for(unsigned phase:{0u,(1u<<shift)/2})
  for(unsigned cycle:{1u,7u,65535u,65536u})for(unsigned n=0;n<4096;++n){
   seed=seed*6364136223846793005ULL+1;const auto initial=uint16_t(seed>>32);
   const uint64_t start=1+(seed&1023),end=start+((seed>>16)&65535);
   yamanix::H8ItuCounter adapted;adapted.configure(start,shift,phase,cycle);adapted.write_count(start,initial);adapted.set_enabled(start,true);
   h8_timer16_channel_device original;original.m_clock_divider=shift;original.m_phase=phase;original.m_counter_cycle=cycle;
   original.m_tcnt=initial;original.m_last_clock_update=start;original.update_counter(end);adapted.advance(end);
   if(original.m_tcnt!=adapted.state().count)throw std::runtime_error("Original counter value mismatch");++count;
  }
 std::printf("%u original unchanged update_counter value comparisons PASS;TGR/IRQ callbacks excluded;not hardware proof\n",count);
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-itu-counter-') as directory:
        source=Path(directory)/'check.cpp';binary=Path(directory)/'check'
        source.write_text(prefix+method+suffix)
        subprocess.run([args.compiler,'-std=c++20','-O1','-fsanitize=address,undefined','-fno-sanitize-recover=all','-I'+str(root/'src'),str(source),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)

if __name__=='__main__':main()
