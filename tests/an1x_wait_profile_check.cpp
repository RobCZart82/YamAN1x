#include "an1x_research_machine.h"
#include <cstdio>
struct Sink{void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
struct Timed{
 int cost=2,data=0;std::vector<uint64_t>times;
 int access_cycles(uint32_t,unsigned)const{return cost;}
 void bus_cycle(uint64_t t){times.push_back(t);}
 uint16_t read_instruction_word(uint32_t){++data;return 1;}uint8_t read_byte(uint32_t){++data;return 2;}uint16_t read_word(uint32_t){++data;return 3;}
 void write_byte(uint32_t,uint8_t){++data;}void write_word(uint32_t,uint16_t){++data;}
};
int main(){
 for(int cost=2;cost<=6;++cost){Timed h;h.cost=cost;int count=14;yamanix::H8hBusAccess<Timed>a(h,count);a.set_slice_end(100);
  check(a.read16i(1)==1&&a.read8(3)==2&&a.read16(5)==3,"Read results unchanged under wait charges");a.write8(7,0);a.write16(9,0);
  check(count==14-5*cost&&h.data==5&&h.times==std::vector<uint64_t>{uint64_t(86+cost),uint64_t(86+2*cost),uint64_t(86+3*cost),uint64_t(86+4*cost),uint64_t(86+5*cost)},"Literal2..6-cycle charges and committed overrun timestamps");
  Timed low;low.cost=cost;int left=std::numeric_limits<int>::min()+cost-1;yamanix::H8hBusAccess<Timed>b(low,left);bool bad=false;try{b.read8(0);}catch(const std::overflow_error&){bad=true;}check(bad&&!b.failed()&&left==std::numeric_limits<int>::min()+cost-1&&low.data==0,"Every supported charge checks signed underflow before mutation");
 }
 for(bool overflow:{false,true}){Timed h;h.cost=6;int count=0;yamanix::H8hBusAccess<Timed>a(h,count);const auto max=std::numeric_limits<uint64_t>::max();a.set_slice_end(max-(overflow?5:6));bool bad=false;
  try{(void)a.read8(0);}catch(const std::overflow_error&){bad=true;}
  check(bad==overflow&&count==-6&&a.failed()==overflow&&h.data==(overflow?0:1),"Six-cycle absolute-time overflow stops data after committed charge");
  check(overflow?h.times.empty():h.times==std::vector<uint64_t>{max},"Exact UINT64_MAX completion accepted;one beyond rejected");
 }
 std::vector<uint8_t>rom(0x100000);Sink sink;
 for(unsigned value=0;value<256;++value){Bus b(rom,{},sink);bool bad=false;try{b.write_byte(0xffffee,uint8_t(value));}catch(const std::logic_error&){bad=true;}
  check(bad==bool(value&8)&&b.failed()==bool(value&8),"128 programmable/disabled encodings accept;128 pin-mode encodings reject");
  if(bad)continue;check(b.read_byte(0xffffee)==uint8_t(value|0xf0),"Reserved upper bits read one");
  for(unsigned enabled=0;enabled<4;++enabled){b.write_byte(0xffffef,uint8_t(0xfc|enabled));b.write_byte(0xffffed,3);
   const int wait=(value&4)?0:int(value&3);
   check(b.access_cycles(0,2)==3+((enabled&1)?wait:0)&&b.access_cycles(0x200000,1)==3+((enabled&2)?wait:0)&&b.access_cycles(0xfffd10,2)==2,"Independent WCR count/WCER area mask/internal timing");
   b.write_byte(0xffffed,0);check(b.access_cycles(0,2)==2&&b.access_cycles(0x200000,2)==2,"Two-state areas ignore wait controller");
  }
 }
 for(unsigned enabled=0;enabled<256;++enabled){Bus b(rom,{},sink);b.write_byte(0xffffed,3);b.write_byte(0xffffee,3);b.write_byte(0xffffef,uint8_t(enabled));
  check(b.read_byte(0xffffef)==enabled&&b.access_cycles(0,2)==((enabled&1)?6:3)&&b.access_cycles(0x200000,2)==((enabled&2)?6:3),"All256 enable masks latch and choose mapped waits");
  b.reset_supported_peripherals();check(b.read_byte(0xffffee)==0xf4&&b.read_byte(0xffffef)==0xff&&b.access_cycles(0,2)==2,"Explicit no-wait research reset profile restored");
 }
 {Bus b(rom,{},sink);b.write_byte(0x12ffffee,0x03);b.write_byte(0x12ffffef,0x01);b.write_byte(0xffffed,3);check(b.read_byte(0xffffee)==0xf3&&b.access_cycles(0,2)==6&&b.access_cycles(0x200000,2)==3,"24-bit aliases and area-independent enables");}
 {Bus b(rom,{},sink);b.write_byte(0xffffed,3);b.write_byte(0xffffee,0);b.write_byte(0xffffef,0);bool bad=false;try{b.write_word(0xffffee,0xf3ff);}catch(const std::out_of_range&){bad=true;}check(bad&&b.access_cycles(0,2)==3,"Unsupported paired word has no partial wait-mode effect");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xffed,0xfa03,0x688a,0x0b00,0xfa03,0x688a,0x0b00,0xfaff,0x688a,0x40fe};for(unsigned i=0;i<12;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Machine whole(rom,{},sink);whole.run(256,10000);
 for(unsigned cut=0;cut<=256;++cut){Machine part(rom,{},sink);part.run(int(cut),10000);part.run(256-int(cut),10000);check(part.snapshot()==whole.snapshot()&&!part.failed()&&part.read_byte(0xffffee)==0xf3&&part.read_byte(0xffffef)==255,"257 CPU partitions preserve six-cycle debt and continuation");}
 std::printf("Wait profile:256 WCR/256 WCER values,literal2..6 charges,257 CPU partitions:%u failures\n",failures);return failures?1:0;
}
