#pragma once
#include "upload_control_contract.h"
#include <array>
#include <stdexcept>
namespace yamanix {
// Canonical full-word routing to the existing bridge-style backend.
// Backend outlives adapter. No samples,table ROM or physical status timing.
template<class Backend>class Vop3HostWordAdapter {
public:
 enum class ControlPolicy { reject, experimental_firmware_contract };
 struct Address {unsigned chip,reg;};
 explicit Vop3HostWordAdapter(Backend& backend,ControlPolicy policy=ControlPolicy::reject):backend_(backend),policy_(policy){
  if(policy!=ControlPolicy::reject&&policy!=ControlPolicy::experimental_firmware_contract)throw std::invalid_argument("Unknown VOP3 control policy");
 }
 Vop3HostWordAdapter(const Vop3HostWordAdapter&)=delete;
 Vop3HostWordAdapter& operator=(const Vop3HostWordAdapter&)=delete;
 bool failed()const{return failed_;}
 static Address decode(uint32_t address){
  if(address&1)throw std::invalid_argument("Unaligned VOP3 host word");
  if(address>=0x440000&&address<0x440100)return {0,unsigned((address-0x440000)/2)};
  if(address>=0x480000&&address<0x480100)return {1,unsigned((address-0x480000)/2)};
  throw std::out_of_range("Unsupported VOP3 host address");
 }
 void write_word(uint32_t address,uint16_t value,uint16_t mask=0xffff){
  require_live();const auto a=decode(address);require_mask(mask);
  auto next=controls_[a.chip];
  if(a.reg==1){require_control();if(!next.write(value,mask))throw std::invalid_argument("Unsupported experimental upload command");}
  Guard guard(*this);
  try{if(backend_.write(a.chip,a.reg,value,mask)!=0||failed_)throw std::runtime_error("VOP3 backend write failed");
   if(a.reg==1)controls_[a.chip]=next;
  }catch(...){failed_=true;throw;}
 }
 uint16_t read_word(uint32_t address,uint16_t mask=0xffff){
  require_live();const auto a=decode(address);require_mask(mask);
  if(a.reg==1){require_control();return controls_[a.chip].status();}
  // Existing bridge returns zeros for other unimplemented reads;do not expose them.
  if(a.reg!=0x21)throw std::out_of_range("Unimplemented VOP3 register read");
  Guard guard(*this);uint16_t value=0;
  try{if(backend_.read(a.chip,a.reg,mask,value)!=0||failed_)throw std::runtime_error("VOP3 backend read failed");return value;}
  catch(...){failed_=true;throw;}
 }
private:
 struct Guard {Vop3HostWordAdapter& owner;explicit Guard(Vop3HostWordAdapter& value):owner(value){if(owner.busy_){owner.failed_=true;throw std::logic_error("VOP3 adapter reentry");}owner.busy_=true;}~Guard(){owner.busy_=false;}};
 void require_live(){if(busy_){failed_=true;throw std::logic_error("VOP3 adapter reentry");}if(failed_)throw std::logic_error("Failed VOP3 adapter");}
 static void require_mask(uint16_t mask){if(mask!=0xffff)throw std::invalid_argument("Unsupported VOP3 partial word");}
 void require_control()const{if(policy_!=ControlPolicy::experimental_firmware_contract)throw std::logic_error("Unverified VOP3 upload status disabled");}
 Backend&backend_;ControlPolicy policy_;std::array<UploadControlContract,2>controls_;
 bool busy_=false,failed_=false;
};
}
