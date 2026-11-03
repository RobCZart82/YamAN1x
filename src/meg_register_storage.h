#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
namespace yamanix {
// Storage-only MEG source profile. No CPU address map,status,commands or audio.
// Zero-filled cold state is project-owned;original start/reset do not initialize.
class MegRegisterStorage {
public:
 struct State {
  uint8_t selected=0;
  std::array<std::array<uint8_t,256>,10> bytes{};
  std::array<uint16_t,256> constants{},offsets{};
  bool operator==(const State&)const=default;
 };
 const State& state()const{return state_;}
 void write(unsigned offset,uint8_t value){
  if(offset==0){state_.selected=value;return;}
  const int bank=bank_index(offset);
  if(bank>=0){state_.bytes[bank][state_.selected]=value;return;}
  if(offset==0x0c||offset==0x0d||offset==0x12||offset==0x13){
   auto& word=(offset<0x10?state_.constants:state_.offsets)[state_.selected];
   word=offset%2?uint16_t((word&0xff00)|value):uint16_t((word&0xff)|(uint16_t(value)<<8));return;
  }
  throw std::out_of_range("Unsupported MEG storage write offset");
 }
 static int bank_index(unsigned offset){
  constexpr unsigned offsets[]={4,5,8,9,0x0e,0x0f,0x14,0x16,0x17,0x18};
  for(unsigned i=0;i<10;++i)if(offsets[i]==offset)return int(i);return -1;
 }
 // Mirrors empty original reset method:retain selector,bytes and word lanes.
 void reset_retaining_state(){}
private:
 State state_;
};
}
