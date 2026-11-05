#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
int main(){std::vector<uint8_t>rom(0x100000);const uint32_t addresses[]={0xffff64,0xffff6e,0xffff78,0xffff82,0xffff92};
 {Sink s;Bus b(rom,{},s);for(auto a:addresses){check(b.read_byte(a)==128,"All five reset controls are80");for(unsigned value=0;value<256;++value){b.write_byte(0x12000000|a,uint8_t(value));b.update(1+(a&255)*256+value);check(b.read_byte(a)==128+value%128&&b.next_bus_event()==0&&s.effects==0,"1280 independent configs retain clock/edge/clear bits without running");}b.write_byte(a,uint8_t(a));}
 for(auto a:addresses)check(b.read_byte(a)==uint8_t(a|128),"Independent channel owners");b.write_word(0xffff6a,0x5aa5);auto at=b.cycle();b.reset_supported_peripherals();for(auto a:addresses)check(b.read_byte(a)==128,"Reset restores all five controls");check(b.cycle()==at&&b.read_word(0xffff6a)==65535,"Coordinated supported reset retains time and resets general storage");}
 {
  Sink sink;
  Bus bus(rom, {}, sink);
  using Mode = yamanix::H8ItuAutoClear::ClearMode;
  const Mode modes[] = {Mode::free_running, Mode::compare_a, Mode::compare_b};
  for (unsigned channel = 0; channel < 5; ++channel) {
   check(bus.stopped_itu_profile(channel) == yamanix::H8ItuControlProfile{0, 0, modes[0]}, "Cold free-running profile");
   for (unsigned value = 0; value < 256; ++value) {
    bus.update(bus.cycle() + 1);
    bus.write_byte(0xab000000 | addresses[channel], uint8_t(value));
    const unsigned clock = value % 8;
    const unsigned clear = value / 32 % 4;
    const auto profile = bus.stopped_itu_profile(channel);
    if (clock < 4 && clear < 3)
     check(profile == yamanix::H8ItuControlProfile{clock, 0, modes[clear]}, "TCR applies supported profile");
    else
     check(!profile, "Unsupported TCR remains inactive");
    check(bus.next_bus_event() == 0 && sink.effects == 0, "Stopped configuration creates no event");
   }
   bus.write_byte(addresses[channel], uint8_t(0x20 | (channel % 4)));
  }
  for (unsigned channel = 0; channel < 5; ++channel)
   check(bus.stopped_itu_profile(channel) == yamanix::H8ItuControlProfile{channel % 4, 0, modes[1]}, "Independent profiles");
  bus.reset_supported_peripherals();
  for (unsigned channel = 0; channel < 5; ++channel)
   check(bus.stopped_itu_profile(channel) == yamanix::H8ItuControlProfile{0, 0, modes[0]}, "Reset reapplies cold profile");
 }
 for(auto a:addresses)for(bool write:{false,true}){Sink s;Bus b(rom,{},s);bool bad=false;try{if(write)b.write_word(a,0);else(void)b.read_word(a);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&s.effects==0,"Paired TCR/TIOR words remain unsupported");}
 for(uint32_t a:{0xffff60u,0xffff8cu}){Sink s;Bus b(rom,{},s);bool bad=false;try{b.write_byte(a,0);}catch(const std::out_of_range&){bad=true;}check(bad,"Start/buffer registers still reject");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xff64,0xfa83,0x688a,0x40fe};for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink s;Machine whole(rom,{},s);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Machine part(rom,{},s);part.run(int(cut),10000);part.run(128-int(cut),10000);check(part.snapshot()==whole.snapshot()&&part.read_byte(0xffff64)==0x83,"129 CPU partitions retain stopped TCR0 selection");}
 std::printf("Stopped ITU controls:1280 configs,alias/reset/no-events/word guards,129 partitions:%u failures\n",failures);return failures?1:0;}
