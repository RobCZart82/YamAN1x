#include "an1x_lcd_research_sink.h"
#include "an1x_research_machine.h"
#include <cstdio>
using Sink=yamanix::An1xLcdResearchSink;
void check_at(bool ok,unsigned line){if(!ok){std::fprintf(stderr,"LCD check failed at line%u\n",line);throw std::runtime_error("LCD machine integration failed");}}
#define check(value) check_at((value),__LINE__)
int main(){
 std::vector<uint8_t>rom(0x100000);Sink sink(1);yamanix::An1xSciBus<Sink> bus(rom,{},sink);
 // PB2 RS output;LCD data pins input.P9 RW/E output.
 bus.write_byte(0xffffd4,4);bus.write_byte(0xffffd6,0);
 bus.write_byte(0xffffd0,0x21);bus.write_byte(0xffffd2,1);
 bus.update(1);bus.write_byte(0xffffd2,0x21);
 check((bus.read_byte(0xffffd6)&128)==128); // latched reset busy
 bus.update(10000);check((bus.read_byte(0xffffd6)&128)==128); // same E does not resample
 bus.write_byte(0xffffd2,1);bus.write_byte(0xffffd2,0x21);
 check((bus.read_byte(0xffffd6)&128)==0); // new edge samples ready
 // Configure split output data and commit set-DDRAM-address85 on E falling.
 bus.write_byte(0xffffd2,1);bus.write_byte(0xffffd2,0);bus.write_byte(0xffffd1,0x3d);
 bus.write_byte(0xffffd4,0xc6);bus.write_byte(0xffffd3,5);bus.write_byte(0xffffd6,0x80);
 bus.write_byte(0xffffd2,0x20);bus.write_byte(0xffffd2,0);
 check(sink.lcd_state().ddram_selected&&sink.lcd_state().ddac==5);
 auto saved=sink.lcd_state();bus.reset_supported_peripherals();check(sink.lcd_state()==saved);
 // Unknown input poisons bus and sink;only explicit cold reconstruction recovers.
 bool bad=false;try{(void)bus.read_byte(0xffffcf);}catch(const std::logic_error&){bad=true;}
 check(bad&&sink.failed()&&bus.failed());sink.cold_reset();check(!sink.failed()&&sink.lcd_state().ddac==0);
 bad=false;try{(void)sink.gpio_read(11,2);(void)sink.gpio_read(11,1);}catch(const std::invalid_argument&){bad=true;}
 check(bad&&sink.failed());sink.cold_reset();
 // Original CPU program drives PA through DDR/data;129 split-run comparisons.
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xffd1,0xfaff,0x688a,0x40fe};
 for(unsigned i=0;i<6;++i){rom[256+i*2]=uint8_t(code[i]>>8);rom[257+i*2]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink whole_sink(16);Machine whole(rom,{},whole_sink);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Sink part_sink(16);Machine part(rom,{},part_sink);
  part.run(int(cut),10000);part.run(int(128-cut),10000);
  check(part.snapshot()==whole.snapshot()&&part_sink.lcd_state()==whole_sink.lcd_state()&&!part_sink.failed());}
 whole_sink.cold_reset();whole.cold_restart(Machine::NvramPolicy::preserve);whole.run(128,10000);check(!whole.failed());
 std::puts("LCD machine:GPIO busy/latch/write/reset/fail-stop and129 CPU partitions PASS");
}
