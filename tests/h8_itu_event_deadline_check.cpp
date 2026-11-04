#include "h8_itu_auto_clear.h"
#include "h8_itu_free_running.h"
#include "h8_itu_event_deadline.h"
#include <cstdio>
#include <limits>
using A=yamanix::H8ItuAutoClear;
void check(bool ok){if(!ok)throw std::runtime_error("ITU deadline contract failed");}
int main(){unsigned cases=0;
 for(unsigned shift=0;shift<4;++shift)for(unsigned phase:{0u,(1u<<shift)/2})for(unsigned initial=0;initial<32;++initial)for(unsigned enable=0;enable<8;++enable){
  A a;a.configure(11,shift,phase);a.write_compare(11,0,6);a.write_compare(11,1,16);a.write_count(11,uint16_t(initial));a.write_enables(11,uint8_t(enable));a.set_enabled(11,true);
  const auto before=a.state();const auto deadline=yamanix::next_itu_event(before);check(before==a.state());
  unsigned count=initial;bool found=false;uint64_t when=0;uint8_t mask=0;
  for(uint64_t t=12;t<=11+8*65553;++t){if((t+phase)%(1u<<shift))continue;
   const unsigned raw=count+1;count=raw==7||raw==65536?0:raw;uint8_t flags=0;
   if(raw==7)flags|=1;if(count==17)flags|=2;if(raw==65536)flags|=4;
   flags&=uint8_t(enable);if(flags){found=true;when=t;mask=flags;break;}
  }
  check(bool(deadline)==found);if(found)check(deadline->time==when&&deadline->events==mask);++cases;
 }
 {yamanix::H8ItuFreeRunning c;c.write_count(0,65535);c.write_enables(0,7);c.set_enabled(0,true);
  const auto d=yamanix::next_itu_event(c.state());check(d&&d->time==1&&d->events==7);
  c.advance(1);check(c.state().status==0xff);const auto next=yamanix::next_itu_event(c.state());check(next&&next->time==65537&&next->events==7);}
 {A a;check(!yamanix::next_itu_event(a.state()));a.write_enables(0,1);a.set_enabled(0,true);a.advance(std::numeric_limits<uint64_t>::max());
  const auto before=a.state();bool bad=false;try{(void)yamanix::next_itu_event(before);}catch(const std::overflow_error&){bad=true;}check(bad&&a.state()==before);}
 {A a;for(unsigned kind=0;kind<3;++kind){auto s=a.state();if(kind==0)s.counter.shift=4;else if(kind==1){s.counter.shift=2;s.counter.phase=1;}else s.counter.cycle=0;
  bool bad=false;try{(void)yamanix::next_itu_event(s);}catch(const std::invalid_argument&){bad=true;}check(bad);}}
 std::printf("ITU deadline:%u independent tick searches,pure query,simultaneous/recurring/overflow PASS\n",cases);
}
