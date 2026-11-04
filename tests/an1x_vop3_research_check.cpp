#include "an1x_vop3_research_sink.h"
#include "an1x_research_machine.h"
#include <cstdio>
#include <initializer_list>
#include <functional>
using S=yamanix::An1xVop3ResearchSink;
using B=yamanix::An1xSciBus<S>;
struct HookSink:yamanix::An1xMegResearchSink {
 HookSink():An1xMegResearchSink(16){} bool enabled=false;unsigned calls=0;std::function<void()> callback;
 bool vop3_word_enabled()const{return enabled;}
 void vop3_write_word(uint32_t,uint64_t,uint16_t){++calls;if(callback)callback();}
 uint16_t vop3_read_word(uint32_t,uint64_t){++calls;if(callback)callback();return 0x1234;}
};
void check(bool ok){if(!ok)throw std::runtime_error("VOP3 bus integration failed");}
int main(){std::vector<uint8_t>rom(0x100000);S s(16);B bus(rom,{},s);unsigned cases=0;
 for(unsigned chip=0;chip<2;++chip)for(unsigned step=0;step<512;++step){
  const unsigned base=chip?0x480000:0x440000;const uint16_t v=uint16_t(step*179+chip*71);
  bus.write_word(base|0x12000000,uint16_t(step));bus.write_word(base+0x16,v);
  const auto state=s.vop3_snapshot();const unsigned offset=chip*7170+5122+step*2;
  check(state[offset]==uint8_t(v>>8)&&state[offset+1]==uint8_t(v));++cases;
 }
 bus.write_byte(0xffffed,4);bus.write_byte(0xffffef,0);
 check(bus.access_cycles(0x440000,2)==3&&bus.access_cycles(0x12480000,2)==3&&bus.access_cycles(0x440000,1)==2);
 for(unsigned ast:{0u,4u})for(unsigned enable:{0u,4u})for(unsigned wcr=0;wcr<8;++wcr){
  bus.write_byte(0xffffed,uint8_t(ast));bus.write_byte(0xffffef,uint8_t(enable));bus.write_byte(0xffffee,uint8_t(wcr));
  const int expected=ast==0?2:enable==0||(wcr&4)?3:3+int(wcr&3);
  check(bus.access_cycles(0x440000,2)==expected&&bus.access_cycles(0x4800fe,2)==expected);
 }
 {HookSink sink;yamanix::An1xSciBus<HookSink>b(rom,{},sink);bool bad=false;
  try{b.write_word(0x440000,1);}catch(const std::out_of_range&){bad=true;}check(bad&&sink.calls==0&&b.failed());}
 for(bool read:{false,true}){HookSink sink;sink.enabled=true;yamanix::An1xSciBus<HookSink>b(rom,{},sink);
  sink.callback=[&]{try{b.write_word(0x440000,2);}catch(const std::logic_error&) {}};
  bool bad=false;try{if(read)(void)b.read_word(0x440042);else b.write_word(0x440000,1);}catch(const std::logic_error&){bad=true;}
  check(bad&&b.failed()&&sink.calls==1);}
 auto state=s.vop3_snapshot();bus.reset_supported_peripherals();check(s.vop3_snapshot()==state);
 for(unsigned chip=0;chip<2;++chip){auto m=s.vop3_metrics();check(m.writes[chip]==1024&&m.sample_passes[chip]==0);}
 for(auto address:{0x440001u,0x440100u,0x480100u,0x440044u}){S sink(16);B b(rom,{},sink);bool bad=false;
  try{(void)b.read_word(address);}catch(const std::exception&){bad=true;}check(bad&&b.failed());}
 for(unsigned kind=0;kind<3;++kind){S sink(16);B b(rom,{},sink);bool bad=false;
  try{if(kind==0)b.write_byte(0x440000,1);else if(kind==1)(void)b.read_byte(0x440000);else(void)b.read_instruction_word(0x440000);}catch(const std::out_of_range&){bad=true;}
  check(bad&&b.failed()&&sink.vop3_metrics().writes[0]==0);}
 {yamanix::An1xMegResearchSink sink(16);yamanix::An1xSciBus<yamanix::An1xMegResearchSink>b(rom,{},sink);bool bad=false;
  try{b.write_word(0x440000,0);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed());}
 {S sink(16);B b(rom,{},sink);bool bad=false;try{b.write_word(0x440002,1);}catch(const std::logic_error&){bad=true;}check(bad&&b.failed()&&sink.failed());}
 {S sink(16,S::Adapter::ControlPolicy::experimental_firmware_contract);B b(rom,{},sink);
  b.bus_cycle(20);b.write_word(0x440002,1);check(b.read_word(0x440002)==0x8000&&b.read_word(0x480002)==0);
  bool bad=false;try{sink.vop3_write_word(0x440000,19,1);}catch(const std::invalid_argument&){bad=true;}check(bad&&sink.failed());
  sink.cold_reset();check(!sink.failed()&&sink.vop3_metrics().writes[0]==0);}
 {S sink(16,S::Adapter::ControlPolicy::reject,yamanix::MegSourceStatus::Policy::reject,yamanix::Vop3LevelReadPolicy::experimental_an1x_level);B b(rom,{},sink);
  b.write_word(0x440000,14);b.write_word(0x440058,0xff00);check(b.read_word(0x440044)==0xc000);
  b.reset_supported_peripherals();check(b.read_word(0x440044)==0xc000);
  sink.cold_reset();check(!sink.failed()&&sink.vop3_metrics().reads[0]==0);
  check(sink.vop3_read_word(0x440044,0)==0); // policy survives reconstruction
 }
 // MOV.L immediate address to ER0;MOV.W immediate to R1;MOV.W R1,@ER0;BRA self.
 rom[2]=1;const uint16_t code[]={0x7a00,0x44,0x16,0x7901,0x5a3c,0x6981,0x40fe};
 for(unsigned i=0;i<7;++i){rom[256+i*2]=uint8_t(code[i]>>8);rom[257+i*2]=uint8_t(code[i]);}
 using M=yamanix::An1xResearchMachine<S>;S whole_sink(16);M whole(rom,{},whole_sink);whole.run(128,10000);
 check(whole_sink.vop3_metrics().writes[0]==1);const auto expected=whole_sink.vop3_snapshot();check(expected[5122]==0x5a&&expected[5123]==0x3c);
 for(unsigned cut=0;cut<=128;++cut){S sink(16);M part(rom,{},sink);part.run(int(cut),10000);part.run(int(128-cut),10000);
  check(part.snapshot()==whole.snapshot()&&sink.vop3_snapshot()==expected&&sink.vop3_metrics().writes[0]==1);}
 std::printf("VOP3 bus:%u address/value cases,24-bit aliases,wait/reset/strict/epoch,129 CPU cuts PASS\n",cases);
}
