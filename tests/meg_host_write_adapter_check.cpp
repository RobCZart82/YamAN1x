#include "meg_host_write_adapter.h"
#include <cstdio>
#include <initializer_list>
using A=yamanix::MegHostWriteAdapter;
void check(bool v){if(!v)throw std::runtime_error("MEG host write adapter failed");}
int main(){
 for(uint32_t address=0x3ffff0;address<=0x400050;++address){
  const int expected=(address>=0x400001&&address<=0x400031&&(address%2))?int((address-0x400000)/2):-1;
  check(A::offset(address)==expected);
 }
 A strict,observe(A::ControlPolicy::observe_original_stubs);unsigned count=0;
 for(unsigned reg:{1u,2u,3u,7u,10u,21u})for(unsigned value=0;value<256;++value){
  const uint32_t address=0x400000+reg*2+1;auto state=strict.state();bool bad=false;
  try{strict.write_byte(address,uint8_t(value));}catch(const std::logic_error&){bad=true;}
  check(bad&&strict.state()==state&&strict.observation().count==0);
  observe.write_byte(address,uint8_t(value));++count;
  check(observe.observation().count==count&&observe.observation().offset==reg&&observe.observation().value==value);
 }
 // Independent host addresses:selector400001,bank4 at400009,constant C/D at19/1B.
 for(unsigned slot=0;slot<256;++slot){observe.write_byte(0x400001,uint8_t(slot));
  for(unsigned value=0;value<256;++value){observe.write_byte(0x400009,uint8_t(value));
   observe.write_byte(0x400019,uint8_t(value));observe.write_byte(0x40001b,uint8_t(255-value));
   check(observe.state().bytes[0][slot]==value&&observe.state().constants[slot]==value*256+255-value);}}
 auto before=observe.state();auto observations=observe.observation();
 for(auto address:{0x400000u,0x400002u,0x40000du,0x400017u,0x400021u,0x400023u,0x400033u,0x1400007u,0x440007u}){
  bool bad=false;try{observe.write_byte(address,0x5a);}catch(const std::out_of_range&){bad=true;}
  check(bad&&observe.state()==before&&observe.observation()==observations);}
 observe.reset_retaining_state();check(observe.state()==before&&observe.observation()==observations);
 std::puts("MEG host:canonical lane/window,1536 explicit stub controls,65536 storage pairs,strict holes/mirrors/reset PASS");
}
