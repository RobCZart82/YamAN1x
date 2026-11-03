#pragma once
#include <cstdint>
#include <stdexcept>
namespace yamanix {
// Explicit original-MAME zero-read stub experiment. NOT a physical ready model.
class MegSourceStatus {
public:
 enum class Policy { reject, original_zero_stubs };
 explicit MegSourceStatus(Policy policy=Policy::reject):policy_(policy){
  if(policy!=Policy::reject&&policy!=Policy::original_zero_stubs)throw std::invalid_argument("Unknown MEG status policy");
 }
 bool enabled()const{return policy_==Policy::original_zero_stubs;}
 uint64_t reads()const{return reads_;}
 uint8_t read_byte(uint32_t address){
  if(address!=0x400005&&address!=0x400021&&address!=0x400023)throw std::out_of_range("Unsupported MEG status address");
  if(!enabled())throw std::logic_error("Unimplemented physical MEG status");
  if(reads_==UINT64_MAX)throw std::overflow_error("MEG status observation count overflow");
  ++reads_;return 0;
 }
private:
 Policy policy_;
 uint64_t reads_=0;
};
}
