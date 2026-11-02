#include "an1x_research_machine.h"
#include <cstdio>
#include <functional>
struct Sink{void tx(unsigned,uint64_t,bool){}void clock(unsigned,uint64_t,bool){}bool dma_request(unsigned,uint64_t){return false;}uint8_t gpio_read(unsigned,uint64_t){return 0;}void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){}};
using Bus=yamanix::An1xSciBus<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
struct Timed{
 int cost=3,data=0;std::function<void()>callback;std::vector<uint64_t>times;std::vector<std::pair<uint32_t,unsigned>>queries;
 int access_cycles(uint32_t a,unsigned w){queries.emplace_back(a,w);if(callback)callback();return cost;}
 void bus_cycle(uint64_t t){times.push_back(t);}
 uint16_t read_instruction_word(uint32_t){++data;return 0x1234;}
 uint8_t read_byte(uint32_t){++data;return 0x56;}
 uint16_t read_word(uint32_t){++data;return 0x789a;}
 void write_byte(uint32_t,uint8_t){++data;}
 void write_word(uint32_t,uint16_t){++data;}
};
int main(){
 for(int cost:{2,3}){Timed h;h.cost=cost;int count=14;yamanix::H8hBusAccess<Timed>a(h,count);a.set_slice_end(100);
  check(a.read16i(0x101)==0x1234&&a.read8(0x103)==0x56&&a.read16(0x105)==0x789a,"All read helper return values");a.write8(0x107,0);a.write16(0x109,0);
  check(count==14-5*cost&&h.data==5,"Exactly selected charge per access");
  check(h.times==std::vector<uint64_t>{uint64_t(86+cost),uint64_t(86+2*cost),uint64_t(86+3*cost),uint64_t(86+4*cost),uint64_t(86+5*cost)},"Literal completion times include negative-counter overrun");
  check(h.queries==std::vector<std::pair<uint32_t,unsigned>>{{0x100,2},{0x103,1},{0x104,2},{0x107,1},{0x108,2}},"Cost query uses aligned words/unmodified bytes");
  Timed low;low.cost=cost;int left=std::numeric_limits<int>::min()+cost-1;yamanix::H8hBusAccess<Timed>b(low,left);bool bad=false;
  try{b.read8(0);}catch(const std::overflow_error&){bad=true;}
  check(bad&&!b.failed()&&low.data==0&&left==std::numeric_limits<int>::min()+cost-1,"Underflow before charge/data remains nonmutating");
 }
 for(int invalid:{-1,0,1,7}){Timed h;h.cost=invalid;int count=10;yamanix::H8hBusAccess<Timed>a(h,count);bool bad=false;try{a.read8(0);}catch(const std::logic_error&){bad=true;}check(bad&&a.failed()&&count==10&&h.data==0,"Unsupported costs fail before charge/data");}
 {Timed h;int count=10;yamanix::H8hBusAccess<Timed>a(h,count);h.callback=[&]{try{a.read8(0);}catch(const std::logic_error&){}};bool bad=false;try{a.read8(0);}catch(const std::logic_error&){bad=true;}check(bad&&a.failed()&&count==10&&h.data==0,"Swallowed cost-query reentry poisons without charging or data effects");}
 std::vector<uint8_t>rom(0x100000);Sink sink;
 for(unsigned value=0;value<256;++value){Bus b(rom,{},sink);b.write_byte(0xffffed,uint8_t(value));check(b.read_byte(0xffffed)==value,"All ASTCR bits read back");
  for(unsigned width:{1u,2u})check(b.access_cycles(0,width)==2+int(value&1)&&b.access_cycles(0x200000,width)==2+int((value>>1)&1)&&b.access_cycles(0xfffd10,width)==2&&b.access_cycles(0xffffb0,width)==2,"Mapped external areas select2/3;internal remains2");
  b.reset_supported_peripherals();check(b.read_byte(0xffffed)==0&&b.access_cycles(0,2)==2,"Explicit no-wait research reset profile");
 }
 for(uint32_t alias:{0xffffedu,0x12ffffedu}){Bus b(rom,{},sink);b.write_byte(alias,0x0f);check(b.read_byte(alias)==15&&b.access_cycles(0,2)==3,"24-bit ASTCR alias");}
 {Bus b(rom,{},sink);bool bad=false;try{b.write_word(0xffffec,0x080f);}catch(const std::out_of_range&){bad=true;}check(bad&&b.access_cycles(0,2)==2,"Unsupported paired word rejects before ASTCR lane mutation");}
 rom[2]=1;const uint16_t code[]={0x7a00,0xff,0xffed,0xfa0f,0x688a,0x40fe};for(unsigned i=0;i<6;++i){rom[256+2*i]=uint8_t(code[i]>>8);rom[257+2*i]=uint8_t(code[i]);}
 using Machine=yamanix::An1xResearchMachine<Sink>;Machine whole(rom,{},sink);whole.run(192,10000);
 for(unsigned cut=0;cut<=192;++cut){Machine part(rom,{},sink);part.run(int(cut),10000);part.run(192-int(cut),10000);check(part.snapshot()==whole.snapshot()&&!part.failed()&&part.read_byte(0xffffed)==15,"193 CPU partitions preserve variable access debt and continuation");}
 std::printf("ASTCR:256 values,literal2/3-cycle helpers,guards,193 CPU partitions:%u failures\n",failures);return failures?1:0;
}
