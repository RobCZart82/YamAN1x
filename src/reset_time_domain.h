#pragma once
#include <cstdint>
#include <limits>
#include <stdexcept>
namespace yamanix {
// Project-owned time policy: absolute peripheral time never rewinds.
// Reset origin is distinct from CPU instruction debt/counter and scheduler time.
// Call only at an owner-defined quiescent boundary;not a reset implementation.
class ResetTimeDomain {
public:
 explicit ResetTimeDomain(uint64_t initial=0):absolute_(initial),origin_(initial){}
 uint64_t absolute()const{return absolute_;}
 uint64_t since_reset()const{return absolute_-origin_;}
 uint64_t generation()const{return generation_;}
 void advance_to(uint64_t at){
  if(at<absolute_)throw std::invalid_argument("Reset time moves backwards");
  absolute_=at;
 }
 void reset_origin(){
  if(generation_==std::numeric_limits<uint64_t>::max())throw std::overflow_error("Reset generation overflow");
  origin_=absolute_;++generation_;
 }
 uint64_t absolute_from_reset(uint64_t relative)const{
  if(relative>std::numeric_limits<uint64_t>::max()-origin_)throw std::overflow_error("Reset time conversion overflow");
  return origin_+relative;
 }
private:
 uint64_t absolute_,origin_,generation_=0;
};
}
