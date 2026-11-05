#include "h8_itu_bounded_dispatch.h"
#include "h8_itu_auto_clear.h"
#include "h8_itu_free_running.h"
#include <cstdio>
#include <vector>
#include <utility>
using D=yamanix::H8ItuBoundedDispatch<yamanix::H8ItuAutoClear>;
using Events=std::vector<std::pair<unsigned,uint64_t>>;
void check(bool ok){if(!ok)throw std::runtime_error("ITU dispatch contract failed");}
void setup(D& d){d.write_compare(0,6);d.write_compare(1,2);d.write_enables(3);d.set_enabled(true);}
int main(){
 for(unsigned channel=0;channel<5;++channel){D d(channel);setup(d);Events events;
  check(d.run_until(22,20,[&](unsigned v,uint64_t t){events.emplace_back(v,t);})==6);
  const unsigned a=24+4*channel,b=a+1;check(events==Events{{b,3},{a,7},{b,10},{a,14},{b,17},{a,21}});
 }
 for(unsigned cut=0;cut<=128;++cut){D a,b;setup(a);setup(b);Events x,y;
  a.run_until(128,100,[&](unsigned v,uint64_t t){x.emplace_back(v,t);});
  auto record=[&](unsigned v,uint64_t t){y.emplace_back(v,t);};b.run_until(cut,100,record);b.run_until(128,100,record);
  check(x==y&&a.state()==b.state());
 }
 {yamanix::H8ItuBoundedDispatch<yamanix::H8ItuFreeRunning>d;d.write_count(65535);d.write_enables(7);d.set_enabled(true);Events e;
  check(d.run_until(1,1,[&](unsigned v,uint64_t t){e.emplace_back(v,t);})==1);check(e==Events{{24,1},{25,1},{26,1}});}
 {D d;setup(d);unsigned calls=0;bool bad=false;try{d.run_until(22,1,[&](unsigned,uint64_t){++calls;});}catch(const std::runtime_error&){bad=true;}
  check(bad&&d.failed()&&calls==1&&d.state().counter.time==3);}
 {D d;setup(d);bool bad=false;try{d.run_until(7,10,[&](unsigned,uint64_t){try{d.write_status(0);}catch(const std::logic_error&) {}});}catch(const std::logic_error&){bad=true;}check(bad&&d.failed());}
 {D d;setup(d);bool bad=false;try{d.run_until(7,10,[](unsigned,uint64_t){throw std::runtime_error("sink failure");});}catch(const std::runtime_error&){bad=true;}check(bad&&d.failed());}
 {D d;const auto before=d.state();bool bad=false;try{d.run_until(1,0,[](unsigned,uint64_t){});}catch(const std::invalid_argument&){bad=true;}check(bad&&!d.failed()&&d.state()==before);
  d.run_until(8,1,[](unsigned,uint64_t){});bad=false;try{d.run_until(7,1,[](unsigned,uint64_t){});}catch(const std::invalid_argument&){bad=true;}check(bad&&!d.failed());}
 {D d;setup(d);Events e;auto record=[&](unsigned v,uint64_t t){e.emplace_back(v,t);};
  d.run_until(3,10,record);d.write_compare(1,4);d.run_until(7,10,record);
  check(e==Events{{25,3},{25,5},{24,7}});d.write_enables(0);d.run_until(14,10,record);
  check(e.size()==3&&(d.state().status&3)==3);d.write_status(0);d.write_enables(3);
  d.run_until(21,10,record);check(e==Events{{25,3},{25,5},{24,7},{25,19},{24,21}});}
 {D d;setup(d);try{d.run_until(7,10,[](unsigned,uint64_t){throw std::runtime_error("failure");});}catch(const std::runtime_error&){}
  unsigned calls=0;bool bad=false;try{d.run_until(14,10,[&](unsigned,uint64_t){++calls;});}catch(const std::logic_error&){bad=true;}check(bad&&calls==0);}
 std::puts("ITU bounded dispatch:5 vector maps,129 splits,simultaneous/budget/reentry/callback/input guards PASS");
}
