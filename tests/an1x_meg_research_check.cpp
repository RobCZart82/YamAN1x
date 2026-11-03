#include "an1x_meg_research_sink.h"
#include "an1x_research_machine.h"
#include <cstdio>
#include <initializer_list>
using Sink=yamanix::An1xMegResearchSink;
using Bus=yamanix::An1xSciBus<Sink>;
void check(bool ok){if(!ok)throw std::runtime_error("MEG research integration failed");}
int main(){std::vector<uint8_t>rom(0x100000);Sink sink(16);Bus bus(rom,{},sink);
 for(unsigned value=0;value<256;++value){bus.write_byte(0x12400001,uint8_t(value));bus.write_byte(0x400009,uint8_t(value));
  bus.write_byte(0x400007,uint8_t(value));check(sink.meg_state().bytes[0][value]==value&&sink.meg_observation().count==value+1);}
 check(bus.access_cycles(0x400007,1)==2);bus.write_byte(0xffffed,4);bus.write_byte(0xffffef,0);
 check(bus.access_cycles(0x400007,1)==3&&bus.access_cycles(0x12400007,1)==3);auto state=sink.meg_state();auto obs=sink.meg_observation();
 bus.reset_supported_peripherals();check(sink.meg_state()==state&&sink.meg_observation()==obs);
 for(auto address:{0x400000u,0x40000du,0x400021u,0x400033u}){Sink s(16);Bus b(rom,{},s);bool bad=false;
  try{b.write_byte(address,0x5a);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&s.failed());}
 {Sink s(16);Bus b(rom,{},s);bool bad=false;try{(void)b.read_byte(0x400005);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed());}
 {Sink s(16);Bus b(rom,{},s);bool bad=false;try{b.write_word(0x400006,0x1234);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&s.meg_observation().count==0);}
 {yamanix::An1xLcdResearchSink s(16);yamanix::An1xSciBus<yamanix::An1xLcdResearchSink>b(rom,{},s);bool bad=false;
  try{b.write_byte(0x400007,1);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed());}
 {Sink s(16);s.meg_write_byte(0x400007,2,1);bool bad=false;try{s.meg_write_byte(0x400007,1,2);}catch(const std::invalid_argument&){bad=true;}
  check(bad&&s.failed()&&s.meg_observation().count==1);s.cold_reset();check(!s.failed()&&s.meg_observation().count==0);}
 rom[2]=1;const uint16_t code[]={0x7a00,0x40,7,0xfa5a,0x688a,0x40fe};
 for(unsigned i=0;i<6;++i){rom[256+i*2]=uint8_t(code[i]>>8);rom[257+i*2]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink whole_sink(16);Machine whole(rom,{},whole_sink);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Sink ps(16);Machine part(rom,{},ps);part.run(int(cut),10000);part.run(int(128-cut),10000);
  check(part.snapshot()==whole.snapshot()&&ps.meg_observation()==whole_sink.meg_observation()&&ps.meg_observation().count==1);}
 std::puts("MEG research bus:256 storage/control values,aliases/timing/reset/strict/fail-stop,129 CPU cuts PASS");
}
