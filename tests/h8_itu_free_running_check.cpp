#include "h8_itu_free_running.h"
#include <cstdio>
#include <initializer_list>
#include <limits>
using C=yamanix::H8ItuFreeRunning;
void check(bool ok){if(!ok)throw std::runtime_error("ITU free-running event contract failed");}
int main(){unsigned cases=0;
 for(unsigned initial=0;initial<65536;++initial)for(unsigned target:{0u,1u,17u,65535u}){
  C c;c.write_count(0,uint16_t(initial));c.write_compare(0,0,uint16_t(target-1));c.write_compare(0,1,uint16_t(65535-target));c.set_enabled(0,true);
  uint16_t count=uint16_t(initial);uint8_t flags=0;
  for(unsigned tick=0;tick<19;++tick){count=uint16_t(count+1);if(count==target)flags|=1;if(count==uint16_t(65536-target))flags|=2;if(count==0)flags|=4;}
  check(c.read_count(19)==count&&c.read_status(19)==(0xf8|flags)&&c.pending()==0);++cases;
 }
 for(unsigned shift=0;shift<=3;++shift)for(unsigned phase:{0u,(1u<<shift)/2})for(unsigned cut=0;cut<=257;++cut){
  C a,b;for(auto c:{&a,&b}){c->configure(11,shift,phase);c->write_count(11,65520);c->write_compare(11,0,65525);c->write_compare(11,1,3);c->set_enabled(11,true);}
  a.advance(268);b.advance(11+cut);b.advance(268);check(a.state()==b.state());}
 {C c;c.write_compare(0,0,2);c.set_enabled(0,true);check(c.read_status(3)==0xf9);c.write_status(3,0xfe);check(c.read_status(3)==0xf8);
  c.write_enables(4,1);check(c.pending()==0);c.advance(65539);check(c.pending()==1);
  c.set_enabled(65539,false);const auto count=c.state().counter.count;c.advance(99999);check(c.state().counter.count==count);
  auto before=c.state();bool bad=false;try{c.configure(100000,4,0);}catch(const std::invalid_argument&){bad=true;}check(bad&&c.state()==before);
  bad=false;try{c.write_compare(100000,2,0);}catch(const std::out_of_range&){bad=true;}check(bad&&c.state()==before);
 }
 {C c;c.set_enabled(0,true);c.advance(std::numeric_limits<uint64_t>::max());check(c.state().status==0xff&&c.state().counter.count==65535);}
 std::printf("ITU free-running:%u independent tick/event cases,2064 phase/split cases,disabled polling/clear/reconfigure/uint64 PASS\n",cases);
}
