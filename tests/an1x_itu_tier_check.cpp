#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;bool consume=false;std::vector<std::pair<unsigned,uint64_t>> requests;std::vector<int> exceptions;void exception_hook(int v){exceptions.push_back(v);}void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned v,uint64_t at){requests.emplace_back(v,at);return consume;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
void check(bool v){if(!v)throw std::runtime_error("Stopped TIER storage contract failed");}
int main(){
 using Bus=yamanix::An1xSciBus<Sink>;std::vector<uint8_t>rom(0x100000);
 const uint32_t addresses[]={0xffff66,0xffff70,0xffff7a,0xffff84,0xffff94};
 Sink sink;Bus bus(rom,{},sink);
 for(unsigned ch=0;ch<5;++ch)for(unsigned v=0;v<256;++v){
  bus.write_byte(addresses[ch],uint8_t(v));
  check(bus.read_byte(0x12000000|addresses[ch])==248+v%8);
  for(auto a:addresses)check(bus.read_byte(a+1)==248);
  bus.update(ch*256+v+1);check(bus.next_bus_event()==0&&sink.effects==0);
 }
 for(unsigned ch=0;ch<5;++ch)bus.write_byte(addresses[ch],uint8_t(ch));
 for(unsigned ch=0;ch<5;++ch)check(bus.read_byte(addresses[ch])==248+ch);
 bus.write_byte(0x200000,0x5a);bus.reset_supported_peripherals();
 for(auto a:addresses)check(bus.read_byte(a)==248&&bus.read_byte(a+1)==248);
 check(bus.cycle()==1280&&bus.read_byte(0x200000)==0x5a);
 // CPU enables all three TIER4 bits;129 split positions preserve flags and storage.
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xff94,0xfaff,0x688a,0x40fe};
 for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink s;Machine whole(rom,{},s);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Sink ps;Machine part(rom,{},ps);part.run(int(cut),10000);part.run(int(128-cut),10000);
  check(part.snapshot()==whole.snapshot()&&part.read_byte(0xffff94)==255&&part.read_byte(0xffff95)==248&&ps.effects==0);}
 constexpr uint32_t tgr[] = {0xffff6a, 0xffff74, 0xffff7e, 0xffff88, 0xffff98};
 for (unsigned channel = 0; channel < 5; ++channel) {
  Sink output;
  Bus timer(rom, {}, output);
  timer.irq().set_filter(0, -1);
  timer.update(10);
  timer.write_word(tgr[channel], 6);
  timer.write_byte(addresses[channel], 1);
  timer.write_byte(0xffff60, uint8_t(1u << channel));
  check(timer.next_bus_event() == 17);
  timer.update(16);
  check(output.requests.empty() && timer.read_byte(addresses[channel]+1) == 0xf8);
  timer.update(17);
  const unsigned vector = 24+4*channel;
  check(output.requests == decltype(output.requests){{vector,17}} &&
        timer.irq().current().vector == int(vector) && timer.read_byte(addresses[channel]+1) == 0xf9);
  timer.irq().taken(vector);
  check(timer.read_byte(addresses[channel]+1) == 0xf9);
  timer.reset_supported_peripherals();
  check(timer.next_bus_event() == 0 && timer.read_byte(addresses[channel]+1) == 0xf8);
 }
 {
  Sink output;
  output.consume = true;
  Bus timer(rom, {}, output, 1);
  timer.irq().set_filter(0, -1);
  timer.write_byte(0xffff64, 0x20);
  timer.write_word(tgr[0], 1);
  timer.write_byte(0xffff60, 1);
  timer.write_byte(addresses[0], 1);
  bool exhausted = false;
  try { timer.update(4); } catch (const std::runtime_error&) { exhausted = true; }
  check(exhausted && timer.failed() && timer.cycle() == 2 &&
        output.requests == decltype(output.requests){{24,2}} && timer.irq().current().level == -1);
 }
 auto install = [&](unsigned at, std::initializer_list<uint16_t> words) {
  for (auto word : words) {
   rom[at++] = uint8_t(word >> 8);
   rom[at++] = uint8_t(word);
  }
 };
 rom[4*24+2] = 1;
 rom[4*24+3] = 0x80;
 install(0x100, {0x7a07,0xff,0xfefe,0x0700,0x0180,0xf85a,0x40fe});
 install(0x180, {0x7a03,0x20,0x400,0xfaa5,0x68ba,0x5670});
 auto configure = [](Machine& machine) {
  machine.write_byte(0xffff6a, 0);
  machine.write_byte(0xffff6b, 99);
  machine.write_byte(0xffff66, 1);
  machine.write_byte(0xffff60, 1);
 };
 Sink continuous;
 Machine reference(rom, {}, continuous);
 configure(reference);
 reference.run(220, 10000);
 check(continuous.requests == decltype(continuous.requests){{24,100}} &&
       continuous.exceptions == std::vector<int>{24} && reference.read_byte(0x200400) == 0xa5);
 for (unsigned cut = 0; cut <= 220; ++cut) {
  Sink output;
  Machine split(rom, {}, output);
  configure(split);
  split.run(int(cut), 10000);
  split.run(220-int(cut), 10000);
  check(split.snapshot() == reference.snapshot() && output.requests == continuous.requests &&
        output.exceptions == continuous.exceptions && split.read_byte(0x200400) == 0xa5);
 }
 std::puts("ITU IRQ: five timed vectors, DMA/budget guard and 221 CPU splits PASS");
}
