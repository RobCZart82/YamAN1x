#pragma once
#include "h8_itu_counter.h"
#include "h8_itu_status.h"
#include <array>
namespace yamanix {
// Internal-clock A/B auto-clear only;no external clear,capture,IRQ/deadline/MMIO.
class H8ItuAutoClear {
public:
 struct State {H8ItuCounter::State counter;std::array<uint16_t,2> compare;unsigned clear_index;uint8_t status,enables;
  bool operator==(const State&)const=default;};
 State state()const{return {counter_.state(),compare_,clear_,status_.status(),status_.enables()};}
 void advance(uint64_t now){
  const auto delta=counter_.elapsed_ticks(now);const auto before=counter_.state().count;const auto cycle=counter_.state().cycle;
  uint8_t events=0;
  for(unsigned i=0;i<2;++i){const auto target=uint16_t(compare_[i]+1u);const auto d=distance(before,cycle,target);
   if(d&&delta>=d)events|=uint8_t(1u<<i);
  }
  if(before>=cycle&&delta>=uint32_t(65536u-before))events|=H8ItuStatus::overflow;
  if(cycle==65536&&delta>=uint32_t(65536u-before))events|=H8ItuStatus::overflow;
  counter_.advance(now);status_.latch(events);
 }
 void configure(uint64_t now,unsigned shift,unsigned phase){auto next=*this;next.advance(now);next.counter_.configure(now,shift,phase,next.counter_.state().cycle);*this=next;}
 void set_clear_index(uint64_t now,unsigned index){
  if(index>1)throw std::out_of_range("Unsupported ITU clear source");advance(now);clear_=index;refresh_cycle(now);
 }
 void write_compare(uint64_t now,unsigned index,uint16_t value){
  if(index>1)throw std::out_of_range("Unsupported ITU compare index");advance(now);compare_[index]=value;refresh_cycle(now);
 }
 void write_count(uint64_t now,uint16_t count){advance(now);counter_.write_count(now,count);}
 void set_enabled(uint64_t now,bool enabled){advance(now);counter_.set_enabled(now,enabled);}
 void write_status(uint64_t now,uint8_t value){advance(now);status_.write_status(value);}
 void write_enables(uint64_t now,uint8_t value){advance(now);status_.write_enables(value);}
 uint8_t read_status(uint64_t now){advance(now);return status_.status();}
 uint16_t read_count(uint64_t now){advance(now);return counter_.state().count;}
 uint8_t pending()const{return status_.pending();}
private:
 // Zero means unreachable;finite distance is always positive,including wrap.
 static uint32_t distance(uint16_t count,uint32_t cycle,uint16_t target){
  if(count<cycle){if(target==0)return cycle-count;if(target>cycle)return 0;
   return target>count?target-count:cycle-count+target;}
  if(target>count)return target-count;
  if(target==0)return 65536u-count;
  if(target<=cycle)return 65536u-count+target;
  return 0;
 }
 void refresh_cycle(uint64_t now){const auto s=counter_.state();counter_.configure(now,s.shift,s.phase,uint32_t(compare_[clear_])+1);}
 H8ItuCounter counter_;H8ItuStatus status_;std::array<uint16_t,2>compare_={0xffff,0xffff};unsigned clear_=0;
};
}
