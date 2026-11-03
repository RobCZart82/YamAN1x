#include "lc7985_busy_clock.h"
#include <cstdio>
#include <initializer_list>
using Clock=yamanix::Lc7985BusyClock;
void check(bool ok){if(!ok)throw std::runtime_error("LCD busy clock contract failed");}
int main(){
 for(uint64_t scale:{1u,16u,24u})for(unsigned value=0;value<256;++value){
  Clock c(scale,37);const auto power=37+10000*scale;
  check(c.busy()&&c.snapshot().deadline==power);
  check(!c.instruction(power-1,uint8_t(value))&&c.snapshot().deadline==power);
  c.advance(power);check(!c.busy());
  check(c.instruction(power,uint8_t(value)));
  // Independent decode by highest command bits,including03 home precedence.
  const unsigned delay=(value&0xfc)?40:(value&3)?16400:0;
  check(c.snapshot().deadline==power+delay*scale);
  if(delay){c.advance(power+delay*scale-1);check(c.busy());}
  c.advance(power+delay*scale);check(!c.busy());
  auto before=c.snapshot();bool bad=false;
  try{c.advance(before.cycle-1);}catch(const std::invalid_argument&){bad=true;}
  check(bad&&c.snapshot()==before);
  c.reset(before.cycle);check(c.snapshot().deadline==before.cycle+10000*scale);
 }
 for(unsigned cut=0;cut<=10000;++cut){
  Clock whole(1),part(1);whole.advance(10000);part.advance(cut);part.advance(10000);
  check(whole.snapshot()==part.snapshot()&&!part.busy());
 }
 const auto max=std::numeric_limits<uint64_t>::max();
 Clock c(1,max-20000);c.advance(max-10000);const auto before=c.snapshot();
 bool bad=false;try{c.instruction(max-10000,1);}catch(const std::overflow_error&){bad=true;}
 check(bad&&c.snapshot()==before);
 bad=false;try{c.reset(max-9999);}catch(const std::overflow_error&){bad=true;}
 check(bad&&c.snapshot()==before);
 for(auto scale:{uint64_t{0},max}){bad=false;try{Clock invalid(scale);}catch(const std::invalid_argument&){bad=true;}check(bad);}
 // An accepted00 does not establish or extend busy. Data access has no clock API.
 check(c.instruction(max,0)&&!c.busy());
 std::puts("LCD busy clock:768 command/rate cases,10001 partitions,boundaries/overflow PASS");
}
