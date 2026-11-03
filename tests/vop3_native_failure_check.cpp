#include "vop3_native_backend.h"
#include <cstdio>
struct yamanix_vop3 {};
static unsigned creates=0,destroys=0,calls=0;
static uint32_t abi=2;
static bool allocation_failure=false;
static int code=0;
extern "C" uint32_t yamanix_vop3_abi_version(){return abi;}
extern "C" yamanix_vop3* yamanix_vop3_create(){++creates;return allocation_failure?nullptr:new yamanix_vop3;}
extern "C" void yamanix_vop3_destroy(yamanix_vop3* p){++destroys;delete p;}
extern "C" int yamanix_vop3_write(yamanix_vop3*,uint32_t,uint32_t,uint16_t,uint16_t){++calls;return code;}
extern "C" int yamanix_vop3_read(yamanix_vop3*,uint32_t,uint32_t,uint16_t,uint16_t*){++calls;return code;}
extern "C" int yamanix_vop3_snapshot(yamanix_vop3*,uint8_t*,size_t){++calls;return code;}
extern "C" int yamanix_vop3_get_metrics(yamanix_vop3*,yamanix_vop3_metrics*){++calls;return code;}
void check(bool ok){if(!ok)throw std::runtime_error("Native backend failure contract");}
int main(){using B=yamanix::Vop3NativeBackend;
 abi=3;bool bad=false;try{B b;}catch(const std::runtime_error&){bad=true;}check(bad&&creates==0);abi=2;
 allocation_failure=true;bad=false;try{B b;}catch(const std::runtime_error&){bad=true;}check(bad&&destroys==0);allocation_failure=false;
 for(unsigned operation=0;operation<4;++operation){B b;code=-3;uint16_t value=0;
  const auto old=destroys;
  if(operation==0)check(b.write(0,0,0,0xffff)==-3);
  if(operation==1)check(b.read(0,0,0xffff,value)==-3);
  if(operation>=2){bad=false;try{if(operation==2)(void)b.snapshot();else(void)b.metrics();}catch(const std::runtime_error&){bad=true;}check(bad);}
  check(b.failed()&&destroys==old+1);auto count=calls;
  check(b.write(0,0,0,0xffff)==-3&&b.read(0,0,0xffff,value)==-3);
  bad=false;try{(void)b.snapshot();}catch(const std::logic_error&){bad=true;}check(bad);
  bad=false;try{(void)b.metrics();}catch(const std::logic_error&){bad=true;}check(bad&&calls==count&&destroys==old+1);
 }
 check(destroys==4);code=0;{B b;check(!b.failed());}check(destroys==5);
 std::puts("Native backend ABI/allocation failure,4 internal failure paths,no post-failure callback,exact destruction PASS");
}
