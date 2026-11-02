#include "an1x_sci_bus.h"
#include <cstdio>
#include <functional>
#include <tuple>
struct Sink{
 using Event=std::tuple<char,unsigned,uint64_t,unsigned,unsigned>;
 std::vector<Event> events;std::function<void()> callback;
 void tx(unsigned c,uint64_t t,bool v){events.emplace_back('T',c,t,v,0);if(callback)callback();}
 void clock(unsigned c,uint64_t t,bool v){events.emplace_back('C',c,t,v,0);if(callback)callback();}
 bool dma_request(unsigned,uint64_t){return false;}
 uint8_t gpio_read(unsigned,uint64_t){return 0;}
 void gpio_write(unsigned p,uint64_t t,uint8_t v,uint8_t d){events.emplace_back('G',p,t,v,d);if(callback)callback();}
};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){
 for(uint64_t origin:{uint64_t{0},uint64_t{1000},uint64_t{1}<<40}){
  std::vector<uint8_t>rom(0x100000);std::vector<Bus::RxEvent>input{{origin+120,0,false},{origin+140,0,true}};
  Sink s;Bus b(rom,input,s);b.update(origin+100);
  b.write_byte(0x200000,0x5a);b.write_byte(0xfffd10,0xa5);b.write_byte(0xffffda,0x39);
  b.write_byte(0xffffc5,0xff);b.write_byte(0xffffc7,0x6b);
  b.write_byte(0xfffff2,1);b.irq().irq_input(2,true);b.write_byte(0xfffff5,0xff);
  b.write_byte(0xffffb1,0);b.write_byte(0xffffb2,0x20);b.write_byte(0xffffb3,0xa5);
  b.write_byte(0xffffb4,0);s.events.clear();b.reset_supported_peripherals();
  const auto now=origin+100;
  const std::vector<Sink::Event>expected={{'G',6,now,127,0},{'G',7,now,255,0},{'G',8,now,15,16},{'G',9,now,63,0},{'G',10,now,255,0},{'G',11,now,255,0},
      {'C',0,now,1,0},{'T',0,now,1,0},{'C',1,now,1,0},{'T',1,now,1,0}};
  check(s.events==expected,"Retained GPIO suppresses unchanged output;first outputs precede clock/TX per SCI channel");
  check(b.cycle()==now&&b.consumed_input()==0&&b.next_bus_event()==origin+120,"Reset keeps absolute time and queued RX deadlines;old TX event cancelled");
  check(b.read_byte(0x200000)==0x5a&&b.read_byte(0xfffd10)==0xa5&&b.read_byte(0xffffda)==0x39&&b.read_byte(0xffffc7)==0x6b,"Memory and GPIO latch/DDR/PCR retained");
  check(b.syscr_value()==9&&b.irq().ier()==0&&b.irq().isr()==4&&b.irq().current().level==-1,"SYSCR reset,held IRQ relatches ISR,but disabled requests unpublished");
  for(uint32_t base:{0xffffb0u,0xffffb8u})check(b.peek_sci(base)==0&&b.peek_sci(base+1)==255&&b.peek_sci(base+2)==0&&b.peek_sci(base+3)==255&&b.peek_sci(base+4)==0x84&&b.peek_sci(base+5)==0,"Both SCI register reset profiles");
  s.events.clear();b.reset_supported_peripherals();check(s.events==std::vector<Sink::Event>{{'C',0,now,1,0},{'T',0,now,1,0},{'C',1,now,1,0},{'T',1,now,1,0}},"Repeated reset retains GPIO publication cache and SCI order");
  b.update(origin+150);check(b.consumed_input()==2&&b.cycle()==origin+150,"Future physical RX schedule survives reset");
 }
 {std::vector<uint8_t>rom(0x100000);Sink s;Bus b(rom,{},s);b.reset_supported_peripherals();s.events.clear();
  s.callback=[&]{try{b.reset_supported_peripherals();}catch(const std::logic_error&){}};
  bool bad=false;try{b.reset_supported_peripherals();}catch(const std::logic_error&){bad=true;}
  check(bad&&b.failed()&&s.events==std::vector<Sink::Event>{{'C',0,0,1,0}},"Swallowed reset reentry fails before further TX/channel effects");
 }
 {std::vector<uint8_t>rom(0x100000);Sink s;Bus b(rom,{},s);
  s.callback=[&]{try{b.read_byte(0x200000);}catch(const std::logic_error&){}};
  bool bad=false;try{b.reset_supported_peripherals();}catch(const std::logic_error&){bad=true;}
  check(bad&&b.failed()&&s.events==std::vector<Sink::Event>{{'G',4,0,255,0}},"First GPIO callback reentry stops before remaining ports and SCI");
 }
 std::printf("Supported bus reset:3 absolute origins,registers/memory/held IRQ/pending RX/reentry:%u failures\n",failures);return failures?1:0;
}
