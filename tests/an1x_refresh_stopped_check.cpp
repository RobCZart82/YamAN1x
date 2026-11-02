#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){std::vector<uint8_t>rom(0x100000);unsigned accepted=0;
 for(unsigned v=0;v<256;++v){Sink s;Bus b(rom,{},s);bool bad=false;try{b.write_byte(0xffffad,uint8_t(v));}catch(const std::logic_error&){bad=true;}
  bool supported=v<8||(v>=128&&v<136);check(bad!=supported&&b.failed()==!supported&&s.effects==0,"256 controls:only stopped clock,no IRQ;CMF cannot be software-set");if(supported){++accepted;check(b.read_byte(0xffffad)==7,"Read-one reserved bits and clear CMF");}}
 check(accepted==16,"16 supported raw controls");
 {Sink s;Bus b(rom,{},s);check(b.read_byte(0xffffad)==7&&b.read_byte(0xffffae)==0&&b.read_byte(0xffffaf)==255,"Independent reset bytes");
  for(unsigned count=0;count<256;++count)for(unsigned compare=0;compare<256;++compare){b.write_byte(0x12ffffae,uint8_t(count));b.write_byte(0xffffaf,uint8_t(compare));b.update(1+256*count+compare);check(b.read_byte(0xffffae)==count&&b.read_byte(0x12ffffaf)==compare&&b.read_byte(0xffffad)==7&&b.next_bus_event()==0&&s.effects==0,"65536 count/compare pairs remain unchanged without a clock or match IRQ");}
  b.write_byte(0x200000,0x5a);b.reset_supported_peripherals();check(b.read_byte(0xffffae)==0&&b.read_byte(0xffffaf)==255&&b.cycle()==65536&&b.read_byte(0x200000)==0x5a,"Supported reset restores timer only,keeps time and RAM");}
 for(bool write:{false,true})for(uint32_t a:{0xffffacu,0xffffadu,0xffffaeu}){Sink s;Bus b(rom,{},s);bool bad=false;try{if(write)b.write_word(a,0);else(void)b.read_word(a);}catch(const std::out_of_range&){bad=true;}catch(const std::invalid_argument&){bad=true;}check(bad&&b.failed()&&s.effects==0,"All overlapping word accesses still reject");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xffae,0xfa5a,0x688a,0x40fe};for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink s;Machine whole(rom,{},s);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Machine part(rom,{},s);part.run(int(cut),10000);part.run(128-int(cut),10000);check(part.snapshot()==whole.snapshot()&&part.read_byte(0xffffae)==0x5a,"129 CPU partitions preserve stopped counter writes");}
 std::printf("Stopped refresh timer:256 controls,65536 count/compare pairs,129 partitions:%u failures\n",failures);return failures?1:0;}
