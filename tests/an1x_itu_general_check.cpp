#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
int main(){std::vector<uint8_t>rom(0x100000);const uint32_t addresses[]={0xffff6a,0xffff6c,0xffff74,0xffff76,0xffff7e,0xffff80,0xffff88,0xffff8a,0xffff98,0xffff9a};
 {Sink s;Bus b(rom,{},s);for(auto a:addresses){check(b.read_word(a)==65535,"Ten independent cold FFFF words");for(unsigned v=0;v<65536;++v){b.write_word(0x12000000|a,uint16_t(v));check(b.read_byte(a)==v/256&&b.read_byte(a+1)==v%256,"655360 word writes expose independent big-endian bytes");b.write_byte(a,uint8_t(v));check(b.read_word(a)==uint16_t((v%256)*257),"High-byte write preserves low byte");b.write_byte(a+1,uint8_t(v>>8));check(b.read_word(a)==uint16_t((v%256)*256+v/256),"Low-byte write preserves high byte");}b.write_word(a,uint16_t(a));}
 for(auto a:addresses)check(b.read_word(a)==uint16_t(a),"All ten owners isolated");b.update(10000);check(s.effects==0&&b.next_bus_event()==0,"Stopped general writes schedule no compare/output events");b.write_byte(0x200000,0x5a);b.reset_supported_peripherals();for(auto a:addresses)check(b.read_word(a)==65535,"Supported reset restores all FFFF words");check(b.cycle()==10000&&b.read_byte(0x200000)==0x5a,"Reset retains absolute time and RAM");}
 for(auto a:addresses)for(bool write:{false,true}){Sink s;Bus b(rom,{},s);bool bad=false;try{if(write)b.write_word(a+1,0);else(void)b.read_word(a+1);}catch(const std::invalid_argument&){bad=true;}check(bad&&b.failed()&&s.effects==0,"Odd words reject before partial effects");}
 for(uint32_t a:{0xffff8cu,0xffff9cu}){Sink s;Bus b(rom,{},s);bool bad=false;try{b.write_word(a,0);}catch(const std::out_of_range&){bad=true;}check(bad,"Buffer registers remain unmapped");}
 {
  Sink sink;
  Bus bus(rom, {}, sink);
  constexpr uint32_t counters[] = {0xffff68, 0xffff72, 0xffff7c, 0xffff86, 0xffff96};
  for (unsigned i = 0; i < 5; ++i) {
   const auto address = counters[i];
   check(bus.read_word(address) == 0, "Cold stopped counter is zero");
   for (unsigned value = 0; value < 65536; ++value) {
    bus.write_word(address | 0xab000000, uint16_t(value));
    check(bus.read_byte(address) == value / 256 && bus.read_byte(address+1) == value % 256,
          "Counter word maps to both byte lanes");
    bus.write_byte(address, uint8_t(value));
    bus.write_byte(address+1, uint8_t(value / 256));
    check(bus.read_word(address) == uint16_t((value % 256)*256 + value / 256),
          "Counter byte writes preserve the other lane");
   }
   bus.write_word(address, uint16_t(0x1234+i));
  }
  bus.update(10000);
  for (unsigned i = 0; i < 5; ++i)
   check(bus.read_word(counters[i]) == 0x1234+i, "Stopped counters retain independent values");
  check(bus.next_bus_event() == 0 && sink.effects == 0, "Stopped counts schedule no events");
  bus.reset_supported_peripherals();
  for (auto address : counters) check(bus.read_word(address) == 0, "Reset clears counter");
  check(bus.cycle() == 10000, "Counter reset retains the bus cycle");
  for (auto address : counters) for (bool write : {false, true}) {
   Bus fresh(rom, {}, sink);
   bool rejected = false;
   try {
    if (write) fresh.write_word(address+1, 0);
    else (void)fresh.read_word(address+1);
   } catch (const std::invalid_argument&) { rejected = true; }
   check(rejected && fresh.failed(), "Odd counter words reject");
  }
 }
 for (uint16_t target : {0xff68, 0xff6a}) {
  rom[2] = 1;
  const uint16_t code[] = {0x7a00, 0xff, target, 0x7901, 0xbeef, 0x6981, 0x40fe};
  for (unsigned i = 0; i < 7; ++i) {
   rom[256+2*i] = uint8_t(code[i] >> 8);
   rom[257+2*i] = uint8_t(code[i]);
  }
  using Machine = yamanix::An1xResearchMachine<Sink>;
  Sink sink;
  Machine whole(rom, {}, sink);
  whole.run(128, 10000);
  const uint32_t address = 0xff0000 | target;
  for (unsigned cut = 0; cut <= 128; ++cut) {
   Machine part(rom, {}, sink);
   part.run(int(cut), 10000);
   part.run(128-int(cut), 10000);
   check(part.snapshot() == whole.snapshot() && part.read_byte(address) == 0xbe &&
         part.read_byte(address+1) == 0xef, "CPU partitions preserve ITU word write");
  }
 }
 std::printf("ITU general and counter words:983040 values,byte lanes,reset/alias/guards,258 partitions:%u failures\n",failures);return failures?1:0;}
