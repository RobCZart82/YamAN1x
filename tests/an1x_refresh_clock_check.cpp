#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<10)std::fprintf(stderr,"%s\n",why);}
int main(){std::vector<uint8_t>rom(0x100000);const unsigned divisors[]={2,8,32,128,512,2048,4096};
 for(unsigned mode=1;mode<=7;++mode){Sink s;Bus b(rom,{},s);b.write_byte(0xffffaf,7);b.write_byte(0xffffad,uint8_t(mode<<3));unsigned count=0;bool flag=false;
  for(uint64_t at=1;at<=16384;++at){if(at%divisors[mode-1]==0){if(++count==8){count=0;flag=true;}}b.update(at);check(b.read_byte(0xffffae)==count&&bool(b.read_byte(0xffffad)&128)==flag&&s.effects==0&&b.next_bus_event()==0,"Seven independent cycle-by-cycle clocks,CMF without outward IRQ/events");}
  b.reset_supported_peripherals();check(b.read_byte(0xffffad)==7&&b.read_byte(0xffffae)==0&&b.read_byte(0xffffaf)==255&&b.cycle()==16384,"Reset restores timer registers at current absolute cycle");b.write_byte(0xffffad,8);b.update(16386);check(b.read_byte(0xffffae)==1,"Restart after reset keeps absolute divider phase");}
 {Sink s;Bus b(rom,{},s);b.write_byte(0xffffaf,0);b.write_byte(0xffffad,8);b.bus_cycle(2);check(b.read_byte(0xffffad)==0x8f,"Bus completion sees compare flag");b.write_byte(0xffffad,8);check(b.read_byte(0xffffad)==15,"Mapped original clear-on-zero control");b.update(1);check(b.cycle()==2,"CPU debt does not rewind bus timer");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xffad,0xfa08,0x688a,0x40fe};for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink sink;Machine whole(rom,{},sink);whole.run(256,10000);whole.run(256,10000);
 for(unsigned cut=0;cut<=256;++cut){Machine part(rom,{},sink);part.run(int(cut),10000);part.run(256-int(cut),10000);part.run(256,10000);check(part.snapshot()==whole.snapshot()&&part.read_byte(0xffffae)==whole.read_byte(0xffffae)&&part.read_byte(0xffffad)==whole.read_byte(0xffffad),"257 CPU partitions preserve active timer across bus debt");}
 std::printf("Bus refresh clock:seven prescalers,reset,CMF,debt,257 CPU partitions:%u failures\n",failures);return failures?1:0;}
