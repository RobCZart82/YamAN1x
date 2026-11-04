#pragma once
#include <cstdint>
#include <stdexcept>
namespace yamanix {
// Bounded internal-clock value component only;no flags,IRQ,deadline or MMIO.
class H8ItuCounter {
public:
 struct State {uint64_t time=0;uint16_t count=0;unsigned shift=0,phase=0;uint32_t cycle=65536;bool enabled=false;
  bool operator==(const State&)const=default;};
 const State& state()const{return state_;}
 void advance(uint64_t now){
  require_time(now);
  if(state_.enabled){const auto delta=ticks(now)-ticks(state_.time);
   if(state_.count>=state_.cycle){
    const auto until_wrap=uint64_t(65536-state_.count);
    state_.count=delta>=until_wrap?uint16_t((delta-until_wrap)%state_.cycle):uint16_t(state_.count+delta);
   }else state_.count=uint16_t((state_.count+delta%state_.cycle)%state_.cycle);
  }
  state_.time=now;
 }
 void set_enabled(uint64_t now,bool enabled){advance(now);state_.enabled=enabled;}
 void write_count(uint64_t now,uint16_t count){advance(now);state_.count=count;}
 void configure(uint64_t now,unsigned shift,unsigned phase,uint32_t cycle){
  require_time(now);
  if(shift>3||phase>=(1u<<shift)||(phase!=0&&phase!=(1u<<(shift-1)))||cycle==0||cycle>65536)
   throw std::invalid_argument("Unsupported internal ITU counter profile");
  advance(now);state_.shift=shift;state_.phase=phase;state_.cycle=cycle;
 }
private:
 void require_time(uint64_t now)const{if(now<state_.time)throw std::invalid_argument("ITU time cannot rewind");}
 uint64_t ticks(uint64_t now)const{
  // Avoid overflow in original (now+phase)>>shift at large absolute times.
  const auto mask=(uint64_t(1)<<state_.shift)-1;
  return (now>>state_.shift)+(((now&mask)+state_.phase)>>state_.shift);
 }
 State state_;
};
}
