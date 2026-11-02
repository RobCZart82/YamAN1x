#include "an1x_research_machine.h"
#include <cstdio>
#include <functional>
#include <tuple>
struct Sink{
 using Event=std::tuple<char,unsigned,uint64_t,bool>;
 std::vector<Event>events;std::function<void()>callback;
 void tx(unsigned c,uint64_t t,bool v){events.emplace_back('T',c,t,v);if(callback)callback();}
 void clock(unsigned c,uint64_t t,bool v){events.emplace_back('C',c,t,v);if(callback)callback();}
 bool dma_request(unsigned,uint64_t){return false;}
 uint8_t gpio_read(unsigned,uint64_t){return 0;}
 void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){if(callback)callback();}
};
using Machine=yamanix::An1xResearchMachine<Sink>;unsigned failures=0;
void check(bool ok,const char*why){if(!ok&&++failures<12)std::fprintf(stderr,"%s\n",why);}
std::vector<uint8_t>image(){std::vector<uint8_t>r(0x100000);r[2]=1;r[0x100]=0xf8;r[0x101]=0x5a;r[0x102]=0x40;r[0x103]=0xfe;return r;}
int main(){
 unsigned accepted=0,rejected=0;
 for(unsigned cut=0;cut<=80;++cut){
  auto rom=image();Sink s;std::vector<Machine::RxEvent>input{{1000,0,false},{1020,0,true}};
  Machine m(rom,input,s);m.run(int(cut),10000);m.write_byte(0x200000,0x5a);m.write_byte(0xfffd10,0xa5);
  const auto before=m.snapshot();s.events.clear();
  if(before.debt){
   bool bad=false;try{m.reset_supported_state();}catch(const std::invalid_argument&){bad=true;}
   check(bad&&!m.failed()&&m.snapshot()==before&&m.cycle()==cut&&m.reset_count()==0&&s.events.empty(),"Debt boundary rejection is nonmutating and resumable");++rejected;
   m.run(before.debt,10000);m.reset_supported_state();
   check(!m.failed()&&m.cycle()==cut+unsigned(before.debt)&&m.reset_count()==1,"Rejected reset can resume,repay debt and then reset");continue;
  }
  m.reset_supported_state();++accepted;const auto queued=m.snapshot();
  check(m.cycle()==cut&&m.reset_count()==1&&m.cycles_since_reset()==0&&m.generation()==0,"Subset reset preserves cold epoch/absolute time and moves relative origin");
  check(queued.registers==before.registers&&queued.ccr==before.ccr&&queued.state==0x10000&&queued.substate==0,"CPU registers retained,partial dispatch replaced by reset");
  check(m.read_byte(0x200000)==0x5a&&m.read_byte(0xfffd10)==0xa5&&m.read_byte(0xfffff2)==9,"RAM retained and SYSCR reset");
  const std::vector<Sink::Event>expected={{'C',0,cut,true},{'T',0,cut,true},{'C',1,cut,true},{'T',1,cut,true}};
  check(s.events==expected,"SCI resets share literal current machine timestamp and order");
  m.run(6,10000);check(m.cycle()==cut+6&&m.cycles_since_reset()==6&&m.snapshot().pc==0x100&&m.snapshot().ccr==uint8_t(before.ccr|0x80),"CPU reset sequence resumes without absolute rewind");
  m.run(1100-int(cut+6),10000);check(m.consumed_input()==2&&m.cycle()==1100,"Original pending RX survives machine reset");
  m.reset_supported_state();check(m.reset_count()==2&&m.cycles_since_reset()==0&&m.cycle()==1100,"Repeated reset keeps absolute time");
  m.cold_restart(Machine::NvramPolicy::preserve);check(m.reset_count()==0&&m.cycles_since_reset()==0&&m.cycle()==0&&m.generation()==1,"Cold restart creates a separate zero-time epoch");
 }
 check(accepted>0&&rejected>0,"Both debt-free and debt-carrying request points covered");
 {auto rom=image();Sink s;Machine m(rom,{},s);m.reset_supported_state();s.events.clear();
  s.callback=[&]{try{m.reset_supported_state();}catch(const std::logic_error&){}};
  bool bad=false;try{m.reset_supported_state();}catch(const std::logic_error&){bad=true;}
  check(bad&&m.failed()&&m.reset_count()==1&&s.events==std::vector<Sink::Event>{{'C',0,0,true}},"Swallowed machine reentry stops before subsequent reset outputs");
  s.callback={};m.cold_restart(Machine::NvramPolicy::clear);check(!m.failed()&&m.reset_count()==0,"Cold reconstruction recovers failed reset");
 }
 {auto rom=image();Sink s;std::vector<Machine::RxEvent>input{{0,0,false},{1000,0,true}};Machine m(rom,input,s);
  m.reset_supported_state();check(m.consumed_input()==1&&m.cycle()==0,"Input exactly at reset time settles before peripheral reset");
  m.run(1100,10000);check(m.consumed_input()==2,"Settled boundary input is not replayed after reset");
 }
 std::printf("Machine supported reset:%u accepted,%u debt rejections,%u failures\n",accepted,rejected,failures);return failures?1:0;
}
