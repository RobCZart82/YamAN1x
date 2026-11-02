#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){
 std::vector<uint8_t>rom(0x100000);unsigned accepted=0,rejected=0;
 for(unsigned value=0;value<256;++value){Sink s;Bus b(rom,{},s);check(b.read_byte(0xffff90)==0xff,"Cold TOER permission bits read FF");
  bool bad=false;try{b.write_byte(0xffff90,uint8_t(value));}catch(const std::logic_error&){bad=true;}
  const bool supported=value%64==0;check(bad==!supported&&b.failed()==!supported&&s.effects==0,"All encodings reject unsupported output enable modes before output");
  if(!supported){++rejected;continue;}++accepted;check(b.read_byte(0xffff90)==0xc0,"Reserved bits read one;accepted profile stays C0");
  b.write_byte(0x200000,0x5a);b.update(1000);b.reset_supported_peripherals();check(b.read_byte(0xffff90)==0xff&&b.read_byte(0x200000)==0x5a&&b.cycle()==1000,"Supported reset restores FF permission bits and retains memory/time");
 }
 check(accepted==4&&rejected==252,"Exact normalized profile coverage");
 for(uint32_t alias:{0xffff90u,0x12ffff90u}){Sink s;Bus b(rom,{},s);b.write_byte(alias,0xc0);check(b.read_byte(alias)==0xc0,"24-bit aliases and reserved-bit normalization");}
 for(bool write:{false,true}){Sink s;Bus b(rom,{},s);bool bad=false;try{if(write)b.write_word(0xffff90,0xc0ff);else(void)b.read_word(0xffff90);}catch(const std::out_of_range&){bad=true;}check(bad&&b.failed()&&b.syscr_value()==9&&s.effects==0,"Unsupported TOER/TOCR word has no unrelated SYSCR/output effect");}
 {Sink s;Bus b(rom,{},s);bool bad=false;try{b.write_byte(0xfffff0,0);}catch(const std::out_of_range&){bad=true;}check(bad,"No generic neighboring MMIO fallback");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xff90,0xfac0,0x688a,0x40fe};for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Sink sink;Machine whole(rom,{},sink);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Machine part(rom,{},sink);part.run(int(cut),10000);part.run(128-int(cut),10000);check(part.snapshot()==whole.snapshot()&&!part.failed()&&part.read_byte(0xffff90)==0xc0,"129 CPU partitions execute retained TOER write identically");}
 std::printf("Retained TOER profile:4 accepted,252 rejected,aliases/word/reset,129 CPU partitions:%u failures\n",failures);return failures?1:0;
}
