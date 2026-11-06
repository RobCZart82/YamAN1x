#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
struct AnalogSink : Sink {
 unsigned samples = 0;
 uint64_t sampled_at = 0;
 uint16_t level = 0x2ab;
 uint16_t adc_an3_sample(uint64_t cycle) {
  ++samples;
  sampled_at = cycle;
  return level;
 }
};
struct ScanSink : AnalogSink {
 std::vector<std::pair<unsigned, uint64_t>> events;
 uint16_t adc_scan_sample(unsigned channel, uint64_t cycle) {
  events.emplace_back(channel, cycle);
  return uint16_t(100 + channel);
 }
};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
int main(){std::vector<uint8_t>rom(0x100000);unsigned accepted=0;
 for(unsigned raw=0;raw<256;++raw){Sink s;Bus b(rom,{},s);bool bad=false;try{b.write_byte(0x12ffffe8,uint8_t(raw));}catch(const std::logic_error&){bad=true;}bool supported=raw<32||(raw>=128&&raw<160);check(bad!=supported&&b.failed()==!supported&&s.effects==0,"256 writes:inactive channel/scan/clock config only,ADF cannot software-set");if(supported){++accepted;check(b.read_byte(0xffffe8)==raw%32,"Independent five-bit idle readback");b.update(1000);check(b.read_byte(0xffffe8)==raw%32&&b.next_bus_event()==0&&s.effects==0,"No synthetic conversion,IRQ or analog input");b.write_byte(0x200000,0x5a);b.reset_supported_peripherals();check(b.read_byte(0xffffe8)==0&&b.cycle()==1000&&b.read_byte(0x200000)==0x5a,"Reset idle ADC preserves time and RAM");}}
 check(accepted==64,"64 accepted raw control encodings");
 for(uint32_t address:{0xffffe0u,0xffffe7u,0xffffe9u}){Sink s;Bus b(rom,{},s);bool bad=false;try{(void)b.read_byte(address);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed(),"Results and trigger control remain unsupported");}
 for(bool write:{false,true}){Sink s;Bus b(rom,{},s);bool bad=false;try{if(write)b.write_word(0xffffe8,0);else(void)b.read_word(0xffffe8);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&s.effects==0,"Paired ADC word rejects before partial effects");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xffe8,0xfa03,0x688a,0x40fe};for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink s;Machine whole(rom,{},s);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Machine part(rom,{},s);part.run(int(cut),10000);part.run(128-int(cut),10000);check(part.snapshot()==whole.snapshot()&&part.read_byte(0xffffe8)==3,"129 CPU partitions preserve idle ADC configuration");}
 for (unsigned cut = 0; cut <= 267; ++cut) {
  AnalogSink analog;
  yamanix::An1xSciBus<AnalogSink> bus(rom, {}, analog);
  bus.update(40);
  bus.write_byte(0x12ffffe8, 0x23);
  check(analog.samples == 1 && analog.sampled_at == 40 &&
        bus.read_byte(0xffffe8) == 0x23 && bus.next_bus_event() == 306,
        "AN3 samples once at the bus write and publishes its deadline");
  analog.level = 0;
  bus.write_byte(0xffffe8, 0xa3);
  bus.update(40 + cut);
  if (cut < 266)
   check(bus.read_word(0xffffe6) == 0 && bus.read_byte(0xffffe6) == 0 && bus.read_byte(0xffffe8) == 0x23,
         "Result and ADF are unchanged before completion");
  bus.update(306 + (cut == 267));
  check(bus.read_byte(0xffffe8) == 0x83 && bus.read_byte(0x12ffffe6) == 0xaa &&
        bus.read_byte(0xffffe7) == 0xc0 && bus.read_word(0x12ffffe6) == 0xaac0 && bus.next_bus_event() == 0 &&
        analog.samples == 1 && analog.effects == 0,
        "Completion commits the original sample without a new deadline");
  bus.write_byte(0xffffe8, 3);
  check(bus.read_byte(0xffffe8) == 3, "Write zero clears ADF");
  bus.write_byte(0xffffe8, 0x23);
  bus.reset_supported_peripherals();
  bus.update(600);
  check(bus.read_byte(0xffffe8) == 0 && bus.read_byte(0xffffe6) == 0 &&
        bus.next_bus_event() == 0, "Reset cancels the conversion");
 }
 for (uint32_t address : {0xffffe5u, 0xffffe9u}) {
  AnalogSink analog;
  yamanix::An1xSciBus<AnalogSink> bus(rom, {}, analog);
  bool bad = false;
  try { (void)bus.read_byte(address); } catch (const std::out_of_range&) { bad = true; }
  check(bad && analog.samples == 0, "Only ADDRD byte lanes are added");
 }
 for (uint32_t address : {0xffffe4u, 0xffffe5u, 0xffffe7u, 0xffffe8u}) {
  AnalogSink analog;
  yamanix::An1xSciBus<AnalogSink> bus(rom, {}, analog);
  bool bad = false;
  try {
   (void)bus.read_word(address);
  } catch (const std::invalid_argument&) {
   bad = (address & 1) != 0;
  } catch (const std::out_of_range&) {
   bad = (address & 1) == 0;
  }
  check(bad && analog.samples == 0, "Only the aligned ADDRD word is mapped");
 }
 {
  AnalogSink analog;
  yamanix::An1xSciBus<AnalogSink> bus(rom, {}, analog);
  bool bad = false;
  try { bus.write_word(0xffffe6, 0); } catch (const std::out_of_range&) { bad = true; }
  check(bad && analog.samples == 0, "ADDRD word stays read only");
 }
 {
  Sink idle;
  Bus bus(rom, {}, idle);
  bool bad = false;
  try { (void)bus.read_word(0xffffe6); } catch (const std::out_of_range&) { bad = true; }
  check(bad, "Idle sinks still reject ADDRD words");
 }
 rom[263] = 0x23;
 using AnalogMachine = yamanix::An1xResearchMachine<AnalogSink>;
 AnalogSink whole_analog;
 AnalogMachine active(rom, {}, whole_analog);
 active.run(320, 10000);
 for (unsigned cut = 0; cut <= 320; ++cut) {
  AnalogSink analog;
  AnalogMachine part(rom, {}, analog);
  part.run(int(cut), 10000);
  part.run(320 - int(cut), 10000);
  check(part.snapshot() == active.snapshot() && analog.samples == 1 &&
        analog.sampled_at == whole_analog.sampled_at && part.read_byte(0xffffe8) == 0x83 &&
        part.read_byte(0xffffe6) == 0xaa && part.read_byte(0xffffe7) == 0xc0,
        "CPU slice boundaries preserve AN3 conversion");
 }
 for (uint64_t cut = 266; cut <= 1300; ++cut) {
  ScanSink analog;
  yamanix::An1xSciBus<ScanSink> bus(rom, {}, analog);
  bus.write_byte(0xffffe8, 0x23);
  bus.update(266);
  bus.write_byte(0x12ffffe8, 0xb3);
  check(bus.read_word(0xffffe6) == 0xaac0 && bus.read_byte(0xffffe8) == 0xb3 &&
        bus.next_bus_event() == 532, "Scan preserves previous AN3 and ADF at start");
  bus.write_byte(0xffffe8, 0x33);
  bus.update(cut);
  if (cut < 1300)
   check(bus.read_byte(0xffffe8) == 0x33, "ADF stays clear before AN3 commit");
  bus.update(1300);
  check(analog.events == std::vector<std::pair<unsigned, uint64_t>>{
      {0,266}, {1,532}, {2,788}, {3,1044}, {0,1300}},
      "Exact scan sample order and cycles");
  for (unsigned channel = 0; channel < 4; ++channel) {
   const auto address = 0xffffe0 + 2 * channel;
   const auto expected = (100 + channel) * 64;
   check(bus.read_word(address) == expected && bus.read_byte(address) == expected / 256 &&
         bus.read_byte(address + 1) == expected % 256, "Independent scan result lanes");
  }
  check(bus.read_byte(0xffffe8) == 0xb3 && bus.next_bus_event() == 1556,
        "AN3 commit sets ADF and keeps scanning");
  bus.reset_supported_peripherals();
  bus.update(1600);
  check(bus.read_byte(0xffffe8) == 0 && bus.next_bus_event() == 0 &&
        analog.events.size() == 5, "Reset cancels scan and clears ADF");
 }
 std::printf("Idle ADC control:256 encodings,reset/alias/no-sample/word,129 idle,268 bus and 321 active CPU partitions:%u failures\n",failures);return failures?1:0;}
