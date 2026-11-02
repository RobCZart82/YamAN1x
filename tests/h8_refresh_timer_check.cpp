#include "h8_refresh_timer.h"
#include <cstdio>
using Timer=yamanix::H8RefreshTimer;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
int main(){
 const unsigned divisors[]={0,2,8,32,128,512,2048,4096};
 for(unsigned mode=0;mode<8;++mode)for(unsigned compare:{0u,1u,7u,255u}){
  Timer timer;timer.write_compare(uint8_t(compare));timer.write_control(uint8_t((mode<<3)|0x40));unsigned count=0;bool flag=false;
  for(uint64_t at=1;at<=16384;++at){if(divisors[mode]&&at%divisors[mode]==0){++count;if(count==compare+1){count=0;flag=true;}}
   timer.advance_to(at);check(timer.count()==count&&timer.irq_requested()==flag,"Independent single-cycle prescaler/compare oracle");}
  for(unsigned cut=0;cut<=512;++cut){Timer split;split.write_compare(uint8_t(compare));split.write_control(uint8_t((mode<<3)|0x40));split.advance_to(cut);split.advance_to(16384);check(split.snapshot()==timer.snapshot(),"513 partitions per mode/compare preserve final state");}
 }
 for(unsigned raw=0;raw<256;++raw){Timer t;t.write_compare(0);t.write_control(8);t.advance_to(2);t.write_control(uint8_t(raw));check(t.control()==uint8_t((raw&0xf8)|7),"256 controls preserve only written-one CMF,read-one reserved bits");}
 {Timer t(3);t.write_compare(0);t.write_control(8);check(t.next_compare()==4,"Free-running divider phase,not time-since-enable");t.advance_to(4);check(!t.irq_requested()&&(t.control()&128),"Compare latches with IRQ disabled");t.write_control(0xc8);check(t.irq_requested(),"Enable with existing CMF exposes request");t.write_control(0x48);check(!t.irq_requested(),"Original write-zero clears CMF without read gate");t.write_count(255);t.write_compare(1);check(t.next_compare()==6,"Above-compare counter normalizes on next tick");t.advance_to(6);check(t.count()==0,"Above-compare source modulo behavior");auto old=t.snapshot();bool bad=false;try{t.advance_to(5);}catch(const std::invalid_argument&){bad=true;}check(bad&&t.snapshot()==old,"Backward time rejects without mutation");t.reset();check(t.snapshot()==Timer::State{6,0,255,0},"Reset preserves absolute time");}
 {Timer t;t.write_compare(255);t.write_control(8);t.advance_to(UINT64_MAX);check(t.count()==255&&(t.control()&128)&&!t.next_compare(),"Huge elapsed interval avoids overflow,unrepresentable deadline absent");}
 std::printf("Refresh timer:independent prescalers,16416 partitions,control/phase/reset/overflow:%u failures\n",failures);return failures?1:0;
}
