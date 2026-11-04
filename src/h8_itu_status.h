#pragma once
#include <cstdint>
#include <stdexcept>
namespace yamanix {
// H8H A/B/overflow flag latch only. Events are supplied explicitly by its owner.
// No counter comparison detection,deadline,IRQ delivery or MMIO integration.
class H8ItuStatus {
public:
 static constexpr uint8_t compare_a=1,compare_b=2,overflow=4;
 void latch(uint8_t events){
  if(events&0xf8)throw std::invalid_argument("Unsupported ITU status event");
  flags_|=events;
 }
 uint8_t status()const{return uint8_t(0xf8|flags_);}
 uint8_t enables()const{return uint8_t(0xf8|enabled_);}
 uint8_t pending()const{return uint8_t(flags_&enabled_);}
 void write_enables(uint8_t value){enabled_=uint8_t(value&7);}
 void write_status(uint8_t value){flags_&=value;}
 void reset(){flags_=enabled_=0;}
private:
 uint8_t flags_=0,enabled_=0;
};
}
