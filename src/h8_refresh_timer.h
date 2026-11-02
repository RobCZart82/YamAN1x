#pragma once
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
namespace yamanix {
// Standalone pinned-MAME timer policy,not yet attached to the machine/IRQ owner.
class H8RefreshTimer {
public:
 struct State {uint64_t cycle;uint8_t count,compare,control;bool operator==(const State&)const=default;};
 explicit H8RefreshTimer(uint64_t at=0):cycle_(at){}
 State snapshot()const{return {cycle_,count_,compare_,control_};}
 uint8_t count()const{return count_;}
 uint8_t compare()const{return compare_;}
 uint8_t control()const{return uint8_t(control_|7);}
 bool irq_requested()const{return (control_&0xc0)==0xc0;}
 void advance_to(uint64_t at){
  if(at<cycle_)throw std::invalid_argument("Refresh timer time moves backwards");
  const int shift=shifts_[(control_>>3)&7];
  if(shift>=0){
   const uint64_t elapsed=(at>>shift)-(cycle_>>shift);
   if(elapsed){
    const unsigned period=unsigned(compare_)+1;
    // Reduce before adding so long gaps cannot overflow count+elapsed.
    if(elapsed>=period||unsigned(count_)+elapsed>=period)control_|=0x80;
    count_=uint8_t((unsigned(count_)+elapsed%period)%period);
   }
  }
  cycle_=at;
 }
 void write_count(uint8_t value){count_=value;}
 void write_compare(uint8_t value){compare_=value;}
 void write_control(uint8_t value){
  // Original source clears CMF on write-zero without a read-history gate.
  control_=uint8_t((control_&value&0x80)|(value&0x78));
 }
 void reset(){count_=0;compare_=255;control_=0;}
 std::optional<uint64_t> next_compare()const{
  const int shift=shifts_[(control_>>3)&7];if(shift<0)return std::nullopt;
  // Arbitrary counter writes above compare are accepted by the source. Its
  // next elapsed tick normalizes them;do not underflow an unsigned deadline.
  const uint64_t ticks=count_>compare_?1:uint64_t(compare_)+1-count_;
  const auto position=cycle_>>shift;
  const auto max=std::numeric_limits<uint64_t>::max()>>shift;
  if(ticks>max-position)return std::nullopt;
  return (position+ticks)<<shift;
 }
private:
 inline static constexpr int shifts_[8]={-1,1,3,5,7,9,11,12};
 uint64_t cycle_;uint8_t count_=0,compare_=255,control_=0;
};
}
