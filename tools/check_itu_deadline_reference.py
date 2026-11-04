"""Next-event deadline differential against original recalc_event;no IRQ delivery proof."""
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
    matches=re.findall(r'^void h8_timer16_channel_device::recalc_event\(u64 cur_time\)\n\{.*?^\}',raw.decode(),re.M|re.S)
    if len(matches)!=1:raise ValueError('Expected one update_counter')
    method=matches[0]
    identity=next(m for m in expected['methods'] if m['name'].endswith('::recalc_event'))
    if hashlib.sha256(method.encode()).hexdigest()!=identity['sha256']:raise ValueError('Original counter method differs')
    prefix=r'''
#include "h8_itu_auto_clear.h"
#include "h8_itu_event_deadline.h"
#include <cstdio>
#include <cstdlib>
#include <initializer_list>
using u64=uint64_t;using u16=uint16_t;using u32=uint32_t;
void logerror(const char*,...){}
bool BIT(unsigned value,int bit){return (value>>bit)&1;}
struct Cpu {u64 now=0;u64 total_cycles(){return now;} void internal_update(){throw std::runtime_error("Unexpected CPU callback");}};
struct Intc {void internal_interrupt(int){throw std::runtime_error("IRQ outside value-only reference profile");}};
struct h8_timer16_channel_device {
 enum {DIV_1=0,IRQ_V=16};Cpu cpu;Intc intc;Cpu*m_cpu=&cpu;Intc*m_intc=&intc;
 unsigned m_clock_type=DIV_1,m_clock_divider=0,m_phase=0,m_counter_cycle=65536;
 u16 m_tcnt=0,m_tgr[2]{};u64 m_last_clock_update=0;bool m_channel_active=true,m_counter_incrementing=true;
 int m_tgr_count=2,m_interrupt[6]{};unsigned m_ier=0,m_isr=0;
 u64 m_event_time=0;int m_tgr_clearing=0;void recalc_event(u64);
};
'''
    suffix=r'''
int main(){uint64_t seed=0x123abc;unsigned count=0;
 for(unsigned shift=0;shift<=3;++shift)for(unsigned phase:{0u,(1u<<shift)/2})for(unsigned n=0;n<16384;++n){
  seed=seed*6364136223846793005ULL+1;
  yamanix::H8ItuAutoClear adapted;auto state=adapted.state();
  state.counter.time=1+(seed&1023);state.counter.count=uint16_t(seed>>32);state.counter.shift=shift;state.counter.phase=phase;state.counter.enabled=(n&8)!=0;
  state.compare={uint16_t(seed),uint16_t(seed>>48)};state.enables=uint8_t(0xf8|(n&7));
  const int clear=int(n%3)-1;state.counter.cycle=clear<0?65536:uint32_t(state.compare[clear])+1;
  h8_timer16_channel_device original;original.m_clock_divider=shift;original.m_phase=phase;original.m_channel_active=state.counter.enabled;
  original.m_tcnt=state.counter.count;original.m_tgr[0]=state.compare[0];original.m_tgr[1]=state.compare[1];original.m_tgr_clearing=clear;
  original.m_ier=(n&3)|((n&4)?16:0);original.recalc_event(state.counter.time);
  const auto deadline=yamanix::next_itu_event(state);
  if((deadline?deadline->time:0)!=original.m_event_time)throw std::runtime_error("Original deadline mismatch");++count;
 }
 std::printf("%u original deadline comparisons PASS;explicit current time,no CPU/IRQ delivery/hardware proof\n",count);
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-itu-deadline-') as directory:
        source=Path(directory)/'check.cpp';binary=Path(directory)/'check'
        source.write_text(prefix+method+suffix)
        subprocess.run([args.compiler,'-std=c++20','-O1','-fsanitize=address,undefined','-fno-sanitize-recover=all','-I'+str(root/'src'),str(source),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)

if __name__=='__main__':main()
