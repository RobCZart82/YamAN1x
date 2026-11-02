#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{
 void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}
 bool dma_request(unsigned,uint64_t){return false;}
 uint8_t gpio_read(unsigned,uint64_t){return 0;}
 void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){}
};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
int main(){
 std::vector<uint8_t>rom(0x100000);Sink sink;
 for(unsigned value=0;value<256;++value){
  Bus b(rom,{},sink);check(b.read_byte(0xffffec)==0,"Research reset profile reads zero");
  bool bad=false;try{b.write_byte(0xffffec,uint8_t(value));}catch(const std::logic_error&){bad=true;}
  check(bad==bool(value&3)&&b.failed()==bool(value&3),"192 ROM/SRAM8 configurations reject;64 unimplemented-area configurations latch");
  if(!(value&3)){check(b.read_byte(0xffffec)==value,"Accepted bits read back unchanged");b.update(1000);b.reset_supported_peripherals();check(b.read_byte(0xffffec)==0&&b.cycle()==1000,"Reset/time preserve fixed16 profile");}
 }
 for(uint32_t alias:{0xffffecu,0x12ffffecu,0xffffffecu}){Bus b(rom,{},sink);b.write_byte(alias,0);check(b.read_byte(alias)==0&&!b.failed(),"24-bit address aliases decode ABWCR");}
 for(bool write:{false,true}){Bus b(rom,{},sink);bool bad=false;try{if(write)b.write_word(0xffffec,0);else(void)b.read_word(0xffffec);}catch(const std::out_of_range&){bad=true;}
  check(bad&&b.failed(),"ABWCR/ASTCR word access remains deliberately unsupported");}
 {Bus b(rom,{},sink);bool bad=false;try{b.write_byte(0xfffff3,0);}catch(const std::out_of_range&){bad=true;}check(bad,"Unimplemented BRCR is not a permissive fallback");}
 // Literal synthetic program:MOV.L #00FFFFEC,ER0;MOV.B #8,R2L;MOV.B R2L,@ER0;BRA .
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xffec,0xfa08,0x688a,0x40fe};
 for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;
 Machine whole(rom,{},sink);whole.run(128,10000);
 for(unsigned cut=0;cut<=128;++cut){Machine part(rom,{},sink);part.run(int(cut),10000);part.run(128-int(cut),10000);
  check(!part.failed()&&part.snapshot()==whole.snapshot()&&part.read_byte(0xffffec)==8,"129 CPU partitions execute supported ABWCR write identically");}
 std::printf("Mapped-area ABWCR profile:256 values,aliases,word/neighbor rejection,129 CPU partitions:%u failures\n",failures);return failures?1:0;
}
