#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){
 std::vector<uint8_t>rom(0x100000);unsigned accepted=0,rejected=0;
 for(unsigned value=0;value<256;++value){Sink s;Bus b(rom,{},s);check(b.read_byte(0xffff61)==0xe0,"Retained TSNC profile reads E0");
  bool bad=false;try{b.write_byte(0xffff61,uint8_t(value));}catch(const std::logic_error&){bad=true;}
  const bool supported=value%32==0;check(bad==!supported&&b.failed()==!supported&&s.effects==0,"All encodings reject unsupported synchronization modes before output");
  if(!supported){++rejected;continue;}++accepted;check(b.read_byte(0xffff61)==0xe0,"Reserved bits read one;accepted profile stays E0");
  b.write_byte(0x200000,0x5a);b.update(1000);b.reset_supported_peripherals();check(b.read_byte(0xffff61)==0xe0&&b.read_byte(0x200000)==0x5a&&b.cycle()==1000,"Supported reset retains logical map,memory,time and E0 profile");
 }
 check(accepted==8&&rejected==248,"Exact normalized profile coverage");
 for(uint32_t alias:{0xffff61u,0x12ffff61u}){Sink s;Bus b(rom,{},s);b.write_byte(alias,0xe0);check(b.read_byte(alias)==0xe0,"24-bit aliases and reserved-bit normalization");}
 for(bool write:{false,true}){Sink s;Bus b(rom,{},s);bool bad=false;try{if(write)b.write_word(0xffff60,0xe0e0);else(void)b.read_word(0xffff60);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&b.syscr_value()==9&&s.effects==0,"Unsupported TSTR/TSNC word has no unrelated SYSCR/output effect");}
 {Sink s;Bus b(rom,{},s);bool bad=false;try{b.write_byte(0xfffff0,0);}catch(const std::out_of_range&){bad=true;}check(bad,"No generic neighboring MMIO fallback");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xff61,0xfae0,0x688a,0x40fe};for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink sink;Machine whole(rom,{},sink);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Machine part(rom,{},sink);part.run(int(cut),10000);part.run(128-int(cut),10000);check(part.snapshot()==whole.snapshot()&&!part.failed()&&part.read_byte(0xffff61)==0xe0,"129 CPU partitions execute retained TSNC write identically");}
 constexpr uint32_t counters[] = {0xffff68, 0xffff72, 0xffff7c, 0xffff86, 0xffff96};
 for (unsigned mask = 0; mask < 256; ++mask) {
  Sink output;
  Bus bus(rom, {}, output);
  check(bus.read_byte(0xffff60) == 0xe0, "TSTR cold value");
  bus.update(10);
  bus.write_byte(0x12ffff60, uint8_t(mask));
  check(bus.read_byte(0xffff60) == mask, "TSTR retains all written bits");
  bus.update(15);
  for (unsigned channel = 0; channel < 5; ++channel)
   check(bus.read_word(counters[channel]) == ((mask & (1u << channel)) ? 5 : 0),
         "TSTR advances only selected channels from the write cycle");
  bus.write_byte(0xffff60, 0);
  bus.update(20);
  for (unsigned channel = 0; channel < 5; ++channel)
   check(bus.read_word(counters[channel]) == ((mask & (1u << channel)) ? 5 : 0),
         "TSTR stop preserves count");
  check(bus.next_bus_event() == 0 && output.effects == 0, "Polling emits no IRQ or pin activity");
 }
 {
  Sink output;
  Bus bus(rom, {}, output);
  bus.write_word(0xffff68, 0xfffe);
  bus.write_byte(0xffff60, 1);
  bus.update(1);
  check(bus.read_word(0xffff68) == 0xffff && bus.read_byte(0xffff67) == 0xf8, "Before wrap");
  bus.update(2);
  check(bus.read_word(0xffff68) == 0 && bus.read_byte(0xffff67) == 0xff, "Wrap and compare flags");
  bus.write_byte(0xffff67, 0xfd);
  check(bus.read_byte(0xffff67) == 0xfd, "TSR clears only selected flags");
  bus.write_byte(0xffff64, 0x21);
  bus.write_word(0xffff6a, 2);
  bus.update(8);
  check(bus.read_word(0xffff68) == 0, "Running TCR changes divider and clear mode");
  bus.reset_supported_peripherals();
  bus.update(20);
  check(bus.read_byte(0xffff60) == 0xe0 && bus.read_word(0xffff68) == 0 &&
        bus.read_byte(0xffff67) == 0xf8, "Reset stops and clears active channel");
 }
 for (bool running : {false, true}) {
  Sink output;
  Bus bus(rom, {}, output);
  if (running) bus.write_byte(0xffff60, 1);
  else bus.write_byte(0xffff64, 4);
  bool rejected_mode = false;
  try {
   if (running) bus.write_byte(0xffff64, 4);
   else bus.write_byte(0xffff60, 1);
  } catch (const std::logic_error&) { rejected_mode = true; }
  check(rejected_mode && bus.failed(), "Unsupported clock rejects at start and while running");
 }
 rom[261] = 0x60;
 rom[263] = 0x01;
 Machine active(rom, {}, sink);
 active.run(128, 10000);
 check(active.read_byte(0xffff60) == 1 && active.read_byte(0xffff69) > 0, "CPU starts timer");
 for (unsigned cut = 0; cut <= 128; ++cut) {
  Machine part(rom, {}, sink);
  part.run(int(cut), 10000);
  part.run(128-int(cut), 10000);
  check(part.snapshot() == active.snapshot() && part.read_byte(0xffff69) == active.read_byte(0xffff69) &&
        part.read_byte(0xffff67) == active.read_byte(0xffff67), "Split CPU runs preserve ITU state");
 }
 std::printf("TSNC/TSTR:256 synchronization encodings,256 start masks,258 CPU partitions:%u failures\n",failures);return failures?1:0;
}
