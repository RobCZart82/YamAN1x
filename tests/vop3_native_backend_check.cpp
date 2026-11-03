#include "vop3_native_backend.h"
#include "vop3_host_word_adapter.h"
#include <cstdio>
#include <type_traits>
using B=yamanix::Vop3NativeBackend;
using A=yamanix::Vop3HostWordAdapter<B>;
static_assert(!std::is_copy_constructible_v<B> && !std::is_move_constructible_v<B>);
void check(bool value){if(!value)throw std::runtime_error("Native VOP3 integration failed");}
int main(){
 B backend,isolated;A adapter(backend,A::ControlPolicy::experimental_firmware_contract);
 const auto zero=isolated.snapshot();unsigned cases=0;
 // Independent fixed BE snapshot offsets: select2 + program512*5*2, then coef512*2.
 for(unsigned chip=0;chip<2;++chip){const unsigned base=chip?0x480000:0x440000;
  for(unsigned step=0;step<512;++step){
   const uint16_t value=uint16_t((step*131+chip*197)^0xa55a);
   adapter.write_word(base,uint16_t(step));adapter.write_word(base+0x16,value);
   const auto state=backend.snapshot();const unsigned offset=chip*7170+5122+step*2;
   check(state[offset]==uint8_t(value>>8)&&state[offset+1]==uint8_t(value));++cases;
  }
  adapter.write_word(base+2,1);check(adapter.read_word(base+2)==0x8000);
  adapter.write_word(base+2,0);check(adapter.read_word(base+2)==0);
  check(adapter.read_word(base+0x42)==0);
 }
 check(isolated.snapshot()==zero);
 auto before=backend.snapshot();auto metrics=backend.metrics();
 for(unsigned chip=0;chip<2;++chip){check(metrics.writes[chip]==1026&&metrics.reads[chip]==1);
  check(metrics.unsupported_reads[chip]==0&&metrics.sample_passes[chip]==0);}
 bool rejected=false;try{adapter.read_word(0x440044);}catch(const std::out_of_range&){rejected=true;}
 check(rejected&&!adapter.failed()&&backend.snapshot()==before&&backend.metrics().reads[0]==1);
 uint16_t value=0xbeef;check(backend.read(2,0x21,0xffff,value)==-1&&value==0xbeef);
 check(backend.write(0,0,1,0xff)==-2&&!backend.failed()&&backend.snapshot()==before);
 // Fresh ownership reconstructs zero state; destroying another owner cannot alter this one.
 for(unsigned epoch=0;epoch<32;++epoch){B fresh;A a(fresh);check(fresh.snapshot()==zero);
  a.write_word(0x480000,7);a.write_word(0x480016,0x1234);check(backend.snapshot()==before);}
 std::printf("Native VOP3: %u independent coefficient/address/value cases,metrics,strict reads,32 lifecycles,isolation PASS\n",cases);
}
