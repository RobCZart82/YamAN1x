#include "h8_itu_status.h"
#include <cstdio>
#include <stdexcept>
using S=yamanix::H8ItuStatus;
void check(bool ok){if(!ok)throw std::runtime_error("ITU status contract failed");}
int main(){unsigned cases=0;
 for(unsigned event=0;event<8;++event)for(unsigned enable=0;enable<256;++enable)for(unsigned clear=0;clear<256;++clear){
  S s;s.write_enables(uint8_t(enable));s.latch(uint8_t(event));
  check(s.status()==(0xf8|event)&&s.enables()==(0xf8|(enable&7)));
  check(s.pending()==(event&enable&7));s.write_status(uint8_t(clear));
  check(s.status()==(0xf8|(event&clear))&&s.pending()==(event&clear&enable&7));
  s.reset();check(s.status()==0xf8&&s.enables()==0xf8&&s.pending()==0);++cases;
 }
 S s;s.latch(S::compare_a);s.latch(S::overflow);check(s.status()==0xfd&&s.pending()==0);
 s.write_enables(5);check(s.pending()==5);s.write_enables(0);check(s.pending()==0&&s.status()==0xfd);
 s.latch(S::compare_b);s.write_status(0xfe);check(s.status()==0xfe);s.latch(S::compare_a);check(s.status()==0xff);
 for(unsigned events=8;events<256;++events){const auto before=s.status();bool bad=false;
  try{s.latch(uint8_t(events));}catch(const std::invalid_argument&){bad=true;}check(bad&&s.status()==before);}
 std::printf("ITU status:%u independent event/enable/clear cases,disabled latch/reenable/reset/invalid PASS\n",cases);
}
