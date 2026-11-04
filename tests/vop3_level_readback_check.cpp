#include "vop3_native_backend.h"
#include "vop3_host_word_adapter.h"
#include <algorithm>
#include <cstdio>
using P=yamanix::Vop3LevelReadPolicy;
using B=yamanix::Vop3NativeBackend;
using A=yamanix::Vop3HostWordAdapter<B>;
void check(bool ok){if(!ok)throw std::runtime_error("Experimental VOP3 readback failed");}
int main(){B backend(P::experimental_an1x_level);A a(backend,A::ControlPolicy::reject,P::experimental_an1x_level);unsigned cases=0;
 for(unsigned chip=0;chip<2;++chip){const unsigned base=chip?0x480000:0x440000;
  a.write_word(base,3);
  for(unsigned raw=0;raw<65536;++raw){a.write_word(base+0x58,uint16_t(raw));
   const int signed_level=raw<32768?int(raw):int(raw)-65536;
   const auto expected=uint16_t(std::clamp(signed_level*64,-32768,32767));
   check(a.read_word(base+0x44)==expected&&a.read_word(base+0x42)==0);++cases;
  }
 }
 a.write_word(0x440000,4);check(a.read_word(0x440044)==0);
 a.write_word(0x480000,3);check(a.read_word(0x480044)==0xffc0);
 for(unsigned chip=0;chip<2;++chip)for(unsigned index=0;index<16;++index){const unsigned base=chip?0x480000:0x440000;
  a.write_word(base,uint16_t(index));a.write_word(base+0x58,uint16_t(10*index+chip));}
 for(unsigned chip=0;chip<2;++chip)for(unsigned index=0;index<16;++index){const unsigned base=chip?0x480000:0x440000;
  a.write_word(base,uint16_t(index));check(a.read_word(base+0x44)==64*(10*index+chip));}
 {bool rejected=false;try{(void)a.read_word(0x440044,0xff);}catch(const std::invalid_argument&){rejected=true;}check(rejected&&!a.failed());}
 auto metrics=backend.metrics();check(metrics.sample_passes[0]==0&&metrics.sample_passes[1]==0&&metrics.unsupported_reads[0]==0&&metrics.unsupported_reads[1]==0);
 for(unsigned index=16;index<512;++index){A bad(backend,A::ControlPolicy::reject,P::experimental_an1x_level);bad.write_word(0x440000,uint16_t(index));auto before=backend.metrics().reads[0];
  bool rejected=false;try{(void)bad.read_word(0x440044);}catch(const std::runtime_error&){rejected=true;}check(rejected&&bad.failed()&&backend.metrics().reads[0]==before);}
 {B strict;bool rejected=false;try{A invalid(strict,A::ControlPolicy::reject,P::experimental_an1x_level);}catch(const std::invalid_argument&){rejected=true;}check(rejected);}
 {A strict(backend);bool rejected=false;try{(void)strict.read_word(0x440044);}catch(const std::out_of_range&){rejected=true;}check(rejected&&!strict.failed());}
 {B strict;bool rejected=false;try{B invalid(static_cast<P>(99));}catch(const std::invalid_argument&){rejected=true;}check(rejected);}
 std::printf("Experimental level:%u signed input/chip cases,496 invalid indices,independent channels/strict defaults,zero samples PASS\n",cases);
}
