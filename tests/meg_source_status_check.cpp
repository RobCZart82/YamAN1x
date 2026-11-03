#include "an1x_meg_research_sink.h"
#include "an1x_research_machine.h"
#include <cstdio>
#include <initializer_list>
using Status=yamanix::MegSourceStatus;using Sink=yamanix::An1xMegResearchSink;
void check(bool ok){if(!ok)throw std::runtime_error("MEG source stub experiment failed");}
int main(){Status strict,source(Status::Policy::original_zero_stubs);
 for(auto a:{0x400005u,0x400021u,0x400023u})for(unsigned value=0;value<256;++value){
  bool bad=false;try{strict.read_byte(a);}catch(const std::logic_error&){bad=true;}check(bad&&strict.reads()==0);
  check(source.read_byte(a)==0);}
 check(source.reads()==768);
 for(auto a:{0x400004u,0x400007u,0x400025u,0x1400005u}){auto n=source.reads();bool bad=false;
  try{source.read_byte(a);}catch(const std::out_of_range&){bad=true;}check(bad&&source.reads()==n);}
 std::vector<uint8_t>rom(0x100000);Sink sink(16,Status::Policy::original_zero_stubs);yamanix::An1xSciBus<Sink>bus(rom,{},sink);
 bus.write_byte(0x400001,7);bus.write_byte(0x400009,0x5a);auto before=sink.meg_state();
 check(bus.read_byte(0x12400005)==0&&sink.meg_status_reads()==1&&sink.meg_state()==before);
 bus.reset_supported_peripherals();check(sink.meg_status_reads()==1&&sink.meg_state()==before);
 sink.cold_reset();check(sink.meg_read_enabled()&&sink.meg_status_reads()==0&&sink.meg_state().bytes[0][7]==0);
 {Sink s(16);yamanix::An1xSciBus<Sink>b(rom,{},s);bool bad=false;try{b.read_byte(0x400005);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&s.meg_status_reads()==0);}
 {Sink s(16,Status::Policy::original_zero_stubs);yamanix::An1xSciBus<Sink>b(rom,{},s);bool bad=false;
  try{b.read_word(0x400004);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&s.meg_status_reads()==0);}
 rom[2]=1;const uint16_t code[]={0x7a00,0x40,5,0x680a,0x40fe};for(unsigned i=0;i<5;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink ws(16,Status::Policy::original_zero_stubs);Machine whole(rom,{},ws);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Sink ps(16,Status::Policy::original_zero_stubs);Machine part(rom,{},ps);
  part.run(int(cut),10000);part.run(int(128-cut),10000);check(part.snapshot()==whole.snapshot()&&ps.meg_status_reads()==1);}
 std::puts("MEG explicit original zero stubs:768 reads,strict policies/holes/word/reset,129 CPU cuts PASS");
}
