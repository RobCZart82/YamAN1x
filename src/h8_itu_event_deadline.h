#pragma once
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
namespace yamanix {
struct H8ItuDeadline {uint64_t time;uint8_t events;bool operator==(const H8ItuDeadline&)const=default;};
// Pure next enabled event for supported internal-clock counter snapshots.
// Latched flags do not suppress recurring events. No IRQ dispatch or state update.
template<class State>std::optional<H8ItuDeadline> next_itu_event(const State& state){
 const auto& c=state.counter;
 if(c.shift>3||c.phase>=(1u<<c.shift)||(c.phase!=0&&c.phase!=(1u<<(c.shift-1)))||c.cycle==0||c.cycle>65536)
  throw std::invalid_argument("Unsupported ITU deadline profile");
 if(!c.enabled||!(state.enables&7))return std::nullopt;
 uint32_t best=0;uint8_t events=0;
 auto candidate=[&](uint32_t distance,uint8_t bit){
  if(!distance||!(state.enables&bit))return;
  if(!best||distance<best){best=distance;events=bit;}else if(distance==best)events|=bit;
 };
 for(unsigned i=0;i<2;++i){const auto target=uint16_t(state.compare[i]+1u);uint32_t d=0;
  if(c.count<c.cycle){
   if(target==0)d=c.cycle-c.count;
   else if(target<=c.cycle)d=target>c.count?target-c.count:c.cycle-c.count+target;
  }else if(target>c.count)d=target-c.count;
  else if(target==0)d=65536u-c.count;
  else if(target<=c.cycle)d=65536u-c.count+target;
  candidate(d,uint8_t(1u<<i));
 }
 if(c.cycle==65536||c.count>=c.cycle)candidate(65536u-c.count,4);
 if(!best)return std::nullopt;
 const uint64_t period=uint64_t(1)<<c.shift;
 const uint64_t remainder=((c.time&(period-1))+c.phase)&(period-1);
 const uint64_t delay=period-remainder+(uint64_t(best)-1)*period;
 if(c.time>std::numeric_limits<uint64_t>::max()-delay)throw std::overflow_error("ITU deadline exceeds absolute time range");
 return H8ItuDeadline{c.time+delay,events};
}
}
