"""Bounded vector/time dispatch differential;no CPU interrupt-controller or hardware proof."""
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
    methods=[]
    for name, result, argument in [('update_counter','void','cur_time'),('recalc_event','void','cur_time'),('internal_update','u64','current_time')]:
        matches=re.findall(r'^'+result+r' h8_timer16_channel_device::'+name+r'\(u64 '+argument+r'\)\n\{.*?^\}',raw.decode(),re.M|re.S)
        if len(matches)!=1:raise ValueError('Expected one original '+name)
        method=matches[0]
        identity=next(m for m in expected['methods'] if m['name'].endswith('::'+name))
        if hashlib.sha256(method.encode()).hexdigest()!=identity['sha256']:raise ValueError('Original method differs: '+name)
        methods.append(method)
    method='\n'.join(methods)
    prefix=r'''
#include "h8_itu_auto_clear.h"
#include "h8_itu_bounded_dispatch.h"
#include "h8_itu_channel_scheduler.h"
#include "h8_itu_free_running.h"
#include <algorithm>
#include <vector>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <initializer_list>
using u64=uint64_t;using u16=uint16_t;using u32=uint32_t;
void logerror(const char*,...){}
bool BIT(unsigned value,int bit){return (value>>bit)&1;}
struct Cpu {u64 now=0;u64 total_cycles(){return now;} void internal_update(){throw std::runtime_error("Unexpected CPU callback");}};
using Events=std::vector<std::pair<unsigned,u64>>;
struct Intc {u64* now=nullptr;Events events;void internal_interrupt(int v){events.emplace_back(v,*now);}};
struct h8_timer16_channel_device {
 enum {DIV_1=0,IRQ_V=16};Cpu cpu;Intc intc;Cpu*m_cpu=&cpu;Intc*m_intc=&intc;
 unsigned m_clock_type=DIV_1,m_clock_divider=0,m_phase=0,m_counter_cycle=65536;
 u16 m_tcnt=0,m_tgr[2]{};u64 m_last_clock_update=0;bool m_channel_active=true,m_counter_incrementing=true;
 int m_tgr_count=2,m_interrupt[6]{};unsigned m_ier=0,m_isr=0;
 u64 m_event_time=0;int m_tgr_clearing=0;void recalc_event(u64);void update_counter(u64);u64 internal_update(u64);
};
'''
    suffix=r'''
int main(){uint64_t seed=0x123abc;unsigned count=0;
 for(unsigned channel=0;channel<5;++channel)for(unsigned shift=0;shift<=3;++shift)for(unsigned phase:{0u,(1u<<shift)/2})for(unsigned n=0;n<256;++n){
  seed=seed*6364136223846793005ULL+1;
  const u64 start=1+(seed&1023),end=start+1024;
  yamanix::H8ItuBoundedDispatch<yamanix::H8ItuAutoClear> adapted(channel);
  adapted.run_until(start,4096,[](unsigned,u64){});adapted.configure(shift,phase);
  adapted.write_compare(0,uint16_t(7+(seed&127)));adapted.write_compare(1,uint16_t(3+((seed>>16)&127)));
  adapted.set_clear_index(n&1);adapted.write_count(uint16_t(seed>>32));adapted.write_enables(n&7);adapted.set_enabled((n&8)!=0);
  const auto state=adapted.state();
  h8_timer16_channel_device original;original.intc.now=&original.m_last_clock_update;
  original.m_last_clock_update=start;original.m_clock_divider=shift;original.m_phase=phase;original.m_channel_active=state.counter.enabled;
  original.m_tcnt=state.counter.count;original.m_tgr[0]=state.compare[0];original.m_tgr[1]=state.compare[1];original.m_tgr_clearing=n&1;
  original.m_interrupt[0]=24+4*channel;original.m_interrupt[1]=25+4*channel;original.m_interrupt[4]=26+4*channel;
  original.m_ier=(n&3)|((n&4)?16:0);original.recalc_event(start);
  original.internal_update(end);original.update_counter(end);
  Events events;adapted.run_until(end,4096,[&](unsigned v,u64 t){events.emplace_back(v,t);});
  const auto after=adapted.state();const unsigned flags=(original.m_isr&3)|((original.m_isr&16)?4:0);
  if(events!=original.intc.events||after.counter.count!=original.m_tcnt||(after.status&7)!=flags)throw std::runtime_error("Original dispatch mismatch");++count;
 }
 unsigned banks=0;
 for(unsigned shift=0;shift<=3;++shift)for(unsigned phase:{0u,(1u<<shift)/2})for(unsigned n=0;n<256;++n){
  yamanix::H8ItuChannelScheduler<yamanix::H8ItuAutoClear> bank;
  bank.run_until(1,4096,[](unsigned,u64){});Events expected,actual;
  for(unsigned channel=0;channel<5;++channel){
   seed=seed*6364136223846793005ULL+1;
   const unsigned mode=(channel+n)%3;
   bank.set_clear_mode(channel,static_cast<yamanix::H8ItuAutoClear::ClearMode>(mode));
   bank.configure(channel,shift,phase);bank.write_count(channel,uint16_t(seed>>32));
   bank.write_compare(channel,0,uint16_t(seed));bank.write_compare(channel,1,uint16_t(seed>>16));
   bank.write_enables(channel,n&7);bank.set_enabled(channel,(n&8)!=0);
   const auto state=bank.state(channel);h8_timer16_channel_device original;
   original.intc.now=&original.m_last_clock_update;original.m_last_clock_update=1;
   original.m_clock_divider=shift;original.m_phase=phase;original.m_channel_active=state.counter.enabled;
   original.m_tcnt=state.counter.count;original.m_tgr[0]=state.compare[0];original.m_tgr[1]=state.compare[1];
   original.m_tgr_clearing=mode==2?-1:int(mode);original.m_interrupt[0]=24+4*channel;original.m_interrupt[1]=25+4*channel;original.m_interrupt[4]=26+4*channel;
   original.m_ier=(n&3)|((n&4)?16:0);original.recalc_event(1);original.internal_update(4097);
   expected.insert(expected.end(),original.intc.events.begin(),original.intc.events.end());
  }
  std::sort(expected.begin(),expected.end(),[](auto a,auto b){return a.second==b.second?a.first<b.first:a.second<b.second;});
  bank.run_until(4097,8192,[&](unsigned v,u64 t){actual.emplace_back(v,t);});
  if(actual!=expected)throw std::runtime_error("Original five-channel chronology mismatch");++banks;
 }
 std::printf("%u original mixed-mode five-channel event-stream comparisons PASS;equal-time ordering is project policy\n",banks);
 std::printf("%u original dispatch vector/time/count/flag comparisons PASS;no CPU/controller/hardware acceptance\n",count);
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-itu-dispatch-') as directory:
        source=Path(directory)/'check.cpp';binary=Path(directory)/'check'
        source.write_text(prefix+method+suffix)
        subprocess.run([args.compiler,'-std=c++20','-O1','-fsanitize=address,undefined','-fno-sanitize-recover=all','-I'+str(root/'src'),str(source),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)

if __name__=='__main__':main()
