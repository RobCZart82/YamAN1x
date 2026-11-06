#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
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
 // A permissive timer-start fallback would invalidate the stopped-only promise.
 {Sink s;Bus b(rom,{},s);b.write_byte(0xffff94,7);bool bad=false;
  try{b.write_byte(0xffff60,0x10);}catch(const std::logic_error&){bad=true;}check(bad&&b.failed()&&s.effects==0);}
 // CPU enables all three TIER4 bits;129 split positions preserve flags and storage.
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xff94,0xfaff,0x688a,0x40fe};
 for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink s;Machine whole(rom,{},s);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Sink ps;Machine part(rom,{},ps);part.run(int(cut),10000);part.run(int(128-cut),10000);
  check(part.snapshot()==whole.snapshot()&&part.read_byte(0xffff94)==255&&part.read_byte(0xffff95)==248&&ps.effects==0);}
 std::puts("Stopped TIER:1280 values,owners/reset/flags/time/start rejection,129 CPU cuts PASS");
}
