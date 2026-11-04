#include "h8_itu_counter.h"
#include <cstdio>
#include <initializer_list>
#include <limits>
using C=yamanix::H8ItuCounter;
void check(bool ok){if(!ok)throw std::runtime_error("ITU counter value contract failed");}
uint16_t step(uint16_t count,unsigned cycle){return uint16_t(unsigned(count)+1>=65536?0:unsigned(count)+1==cycle?0:unsigned(count)+1);}
int main(){unsigned cases=0;
 for(unsigned shift=0;shift<=3;++shift)for(unsigned phase:{0u,(1u<<shift)/2})for(unsigned cycle:{1u,7u,65535u,65536u}){
  for(unsigned initial=0;initial<65536;++initial){C c;c.configure(0,shift,phase,cycle);c.write_count(0,uint16_t(initial));c.set_enabled(0,true);
   uint16_t expected=uint16_t(initial);for(unsigned t=1;t<=19;++t)if((t+phase)%(1u<<shift)==0)expected=step(expected,cycle);
   c.advance(19);check(c.state().count==expected);++cases;}
 }
 for(unsigned cut=0;cut<=257;++cut){C whole,part;for(auto c:{&whole,&part}){c->configure(11,3,4,37);c->write_count(11,65530);c->set_enabled(11,true);}
  whole.advance(268);part.advance(11+cut);part.advance(268);check(whole.state()==part.state());}
 {C c;c.set_enabled(0,true);c.advance(10);c.set_enabled(13,false);c.advance(100);check(c.state().count==13);
  c.set_enabled(101,true);c.advance(103);check(c.state().count==15);c.configure(105,2,2,17);check(c.state().count==17);
  c.advance(109);check(c.state().count==18); // already above new clear threshold:wrap first
  const auto before=c.state();bool bad=false;try{c.advance(108);}catch(const std::invalid_argument&){bad=true;}check(bad&&c.state()==before);
  for(unsigned kind=0;kind<4;++kind){bad=false;try{c.configure(110,kind==0?4:2,kind==1?1:0,kind==2?0:kind==3?65537:17);}catch(const std::invalid_argument&){bad=true;}check(bad&&c.state()==before);}
 }
 {C c;const auto max=std::numeric_limits<uint64_t>::max();c.configure(0,3,4,65536);c.set_enabled(0,true);c.advance(max);
  check(c.state().count==0); // ceil(max/8)=2^61,low16=0;no now+phase overflow
 }
 {C c;c.write_count(0,65535);c.set_enabled(0,true);c.advance(std::numeric_limits<uint64_t>::max());check(c.state().count==65534);}
 std::printf("ITU counter:%u independent tick/value cases,258 splits,stop/reconfigure/invalid/uint64 limits PASS\n",cases);
}
