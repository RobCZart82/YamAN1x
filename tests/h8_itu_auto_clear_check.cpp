#include "h8_itu_auto_clear.h"
#include <cstdio>
#include <initializer_list>
#include <limits>
#include <source_location>
using C=yamanix::H8ItuAutoClear;
void check(bool ok,std::source_location at=std::source_location::current()){if(!ok){std::fprintf(stderr,"ITU check line%u\n",at.line());throw std::runtime_error("ITU auto-clear event contract failed");}}
int main(){unsigned cases=0;
 for(unsigned initial=0;initial<65536;++initial)for(unsigned cycle:{1u,7u,65535u,65536u})for(unsigned target:{0u,1u,17u,65535u}){
  C c;c.write_compare(0,0,uint16_t(cycle-1));c.write_compare(0,1,uint16_t(target-1));c.write_count(0,uint16_t(initial));c.set_enabled(0,true);
  unsigned count=initial;uint8_t flags=0;
  for(unsigned tick=0;tick<19;++tick){const unsigned raw=count+1;count=raw==cycle||raw==65536?0:raw;
   if(count==uint16_t(cycle)||raw==cycle)flags|=1;
   if(count==target||(raw==target&&raw==cycle))flags|=2;
   if(raw==65536)flags|=4;
  }
  check(c.read_count(19)==count&&c.read_status(19)==(0xf8|flags)&&c.pending()==0);++cases;
 }
 for(unsigned index=0;index<2;++index)for(unsigned shift=0;shift<=3;++shift)for(unsigned phase:{0u,(1u<<shift)/2})for(unsigned cut=0;cut<=257;++cut){C a,b;
  for(auto c:{&a,&b}){c->configure(11,shift,phase);c->write_compare(11,0,6);c->write_compare(11,1,16);c->set_clear_index(11,index);c->write_count(11,65530);c->set_enabled(11,true);}
  a.advance(268);b.advance(11+cut);b.advance(268);check(a.state()==b.state());}
 {C c;c.write_compare(0,0,6);c.write_compare(0,1,0xffff);c.set_enabled(0,true);check(c.read_status(7)==0xfb&&c.read_count(7)==0);
  c.write_status(7,0);c.write_compare(8,0,0);check(c.read_count(9)==2&&c.read_status(9)==0xf8);
  c.write_count(9,0);check(c.read_status(10)==0xfb);c.write_enables(10,3);check(c.pending()==3);
  const auto before=c.state();bool bad=false;try{c.set_clear_index(11,2);}catch(const std::out_of_range&){bad=true;}check(bad&&c.state()==before);
 }
 {C c;c.write_compare(0,0,6);c.write_compare(0,1,16);c.set_enabled(0,true);c.advance(std::numeric_limits<uint64_t>::max());check(c.state().status==0xf9);}
 std::printf("ITU auto-clear:%u independent tick/event cases,4128 source/phase/splits,polling/FFFF/reconfigure/uint64 PASS\n",cases);
}
