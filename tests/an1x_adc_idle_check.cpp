#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{unsigned effects=0;void tx(unsigned,uint64_t,bool){++effects;}void clock(unsigned,uint64_t,bool){++effects;}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++effects;}};
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
 std::printf("Idle ADC control:256 encodings,reset/alias/no-sample/word,129 partitions:%u failures\n",failures);return failures?1:0;}
