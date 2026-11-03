#pragma once
#include "vop3_bridge.h"
#include <array>
#include <memory>
#include <stdexcept>
namespace yamanix {
// Register-only owner. No sample clock, table ROM, CPU bus or audio acceptance.
class Vop3NativeBackend {
public:
 using Snapshot = std::array<uint8_t,14340>;
 Vop3NativeBackend() {
  if(yamanix_vop3_abi_version()!=2)throw std::runtime_error("Unsupported VOP3 ABI");
  handle_.reset(yamanix_vop3_create());
  if(!handle_)throw std::runtime_error("Cannot create VOP3 backend");
 }
 Vop3NativeBackend(const Vop3NativeBackend&)=delete;
 Vop3NativeBackend& operator=(const Vop3NativeBackend&)=delete;
 Vop3NativeBackend(Vop3NativeBackend&&)=delete;
 Vop3NativeBackend& operator=(Vop3NativeBackend&&)=delete;
 int write(unsigned chip,unsigned reg,uint16_t value,uint16_t mask){
  if(!handle_)return -3;
  return result(yamanix_vop3_write(handle_.get(),chip,reg,value,mask));
 }
 int read(unsigned chip,unsigned reg,uint16_t mask,uint16_t& value){
  if(!handle_)return -3;
  return result(yamanix_vop3_read(handle_.get(),chip,reg,mask,&value));
 }
 bool failed()const{return !handle_;}
 Snapshot snapshot(){Snapshot value{};require_live();
  require_success(yamanix_vop3_snapshot(handle_.get(),value.data(),value.size()));return value;}
 yamanix_vop3_metrics metrics(){yamanix_vop3_metrics value{};require_live();
  require_success(yamanix_vop3_get_metrics(handle_.get(),&value));return value;}
private:
 int result(int code){if(code==-3)handle_.reset();return code;}
 void require_live()const{if(!handle_)throw std::logic_error("Failed VOP3 native backend");}
 void require_success(int code){if(result(code)!=0)throw std::runtime_error("VOP3 native observation failed");}
 std::unique_ptr<yamanix_vop3,decltype(&yamanix_vop3_destroy)> handle_{nullptr,&yamanix_vop3_destroy};
};
}
