#pragma once
#include "h8_itu_counter.h"
#include "h8_itu_status.h"
#include <array>
namespace yamanix {
// Free-running internal-clock profile only:cycle65536,no auto-clear/capture.
// Polling latches flags even with IRQ disabled;no deadline/delivery/MMIO.
class H8ItuFreeRunning {
public:
 struct State {H8ItuCounter::State counter;std::array<uint16_t,2> compare;uint8_t status,enables;
  bool operator==(const State&)const=default;};
 State state()const{return {counter_.state(),compare_,status_.status(),status_.enables()};}
 void advance(uint64_t now){
  const auto delta=counter_.elapsed_ticks(now);const auto before=counter_.state().count;
  uint8_t events=0;
  for(unsigned i=0;i<2;++i){const auto target=uint16_t(compare_[i]+1u);
   const auto distance=uint32_t(uint16_t(target-before));
   if(delta>=(distance?distance:65536u))events|=uint8_t(1u<<i);
  }
  if(delta>=uint32_t(65536u-before))events|=H8ItuStatus::overflow;
  counter_.advance(now);status_.latch(events);
 }
 void configure(uint64_t now,unsigned shift,unsigned phase){
  auto next=*this;next.advance(now);next.counter_.configure(now,shift,phase,65536);*this=next;
 }
 void set_enabled(uint64_t now,bool enabled){advance(now);counter_.set_enabled(now,enabled);}
 void write_count(uint64_t now,uint16_t count){advance(now);counter_.write_count(now,count);}
 void write_compare(uint64_t now,unsigned index,uint16_t value){
  if(index>1)throw std::out_of_range("Unsupported ITU compare index");advance(now);compare_[index]=value;
 }
 void write_enables(uint64_t now,uint8_t value){advance(now);status_.write_enables(value);}
 void write_status(uint64_t now,uint8_t value){advance(now);status_.write_status(value);}
 uint8_t read_status(uint64_t now){advance(now);return status_.status();}
 uint16_t read_count(uint64_t now){advance(now);return counter_.state().count;}
 uint8_t pending()const{return status_.pending();}
private:
 H8ItuCounter counter_;H8ItuStatus status_;std::array<uint16_t,2>compare_={0xffff,0xffff};
};
}
