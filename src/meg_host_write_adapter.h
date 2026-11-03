#pragma once
#include "meg_register_storage.h"
namespace yamanix {
// Canonical lower-byte host window only;no chip-select mirrors or word access.
// Stub-control observation is explicit and supplies no actual command effects.
class MegHostWriteAdapter {
public:
 enum class ControlPolicy { reject, observe_original_stubs };
 explicit MegHostWriteAdapter(ControlPolicy policy=ControlPolicy::reject):policy_(policy){
  if(policy!=ControlPolicy::reject&&policy!=ControlPolicy::observe_original_stubs)
   throw std::invalid_argument("Unknown MEG control policy");
 }
 struct Observation {
  uint64_t count=0;uint8_t offset=0,value=0;
  bool operator==(const Observation&)const=default;
 };
 static int offset(uint32_t address){
  if(address<0x400001||address>0x400031||!(address&1))return -1;
  return int((address-0x400001)/2);
 }
 void write_byte(uint32_t address,uint8_t value){
  const auto reg=offset(address);
  if(reg<0)throw std::out_of_range("Unsupported MEG host byte address");
  if(reg==1||reg==2||reg==3||reg==7||reg==10||reg==21){
   if(policy_!=ControlPolicy::observe_original_stubs)
    throw std::logic_error("Unimplemented MEG control effects");
   if(observation_.count==UINT64_MAX)throw std::overflow_error("MEG observation count overflow");
   observation_={observation_.count+1,uint8_t(reg),value};return;
  }
  storage_.write(unsigned(reg),value);
 }
 const MegRegisterStorage::State& state()const{return storage_.state();}
 Observation observation()const{return observation_;}
 void reset_retaining_state(){storage_.reset_retaining_state();}
private:
 ControlPolicy policy_;
 MegRegisterStorage storage_;
 Observation observation_;
};
}
