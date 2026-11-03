#include "lc7985_controller.h"
#include "an1x_lcd_gpio.h"
#include <cstdio>
#include <initializer_list>
using C=yamanix::Lc7985Controller;
void check(bool value){if(!value)throw std::runtime_error("LCD controller check failed");}
int main(){
 unsigned cases=0;
 for(bool two:{false,true})for(bool up:{false,true})for(unsigned start=0;start<80;++start){
  C c(1);uint64_t t=10000;
  c.instruction_write(t,two?0x28:0x20);t+=40;
  c.instruction_write(t,up?6:4);t+=40;
  const unsigned address=two&&start>=40?start+24:start;
  c.instruction_write(t,uint8_t(0x80|address));
  // Data during instruction busy must still read/write and preserve deadline.
  const auto end=c.timing().deadline;
  for(unsigned value=0;value<256;++value){
   const auto index=up?(start+value)%80:(start+80-value%80)%80;
   c.data_write(t,uint8_t(value));check(c.state().ddram[index]==value);
   const auto next=up?(index+1)%80:(index+79)%80;
   check(c.state().ddac==(two&&next>=40?next+24:next));
   check(c.timing().deadline==end);++cases;
  }
 }
 for(bool up:{false,true})for(unsigned start=0;start<64;++start){
  C c(1);uint64_t t=10000;c.instruction_write(t,up?6:4);t+=40;
  c.instruction_write(t,uint8_t(0x40|start));
  for(unsigned v=0;v<256;++v){const auto index=up?(start+v)%64:(start+64-v%64)%64;
   c.data_write(t,uint8_t(v));check(c.state().cgram[index]==v);++cases;}
 }
 // All address commands/status,including addresses for which data is unsafe.
 for(unsigned v=0;v<256;++v){C c(1);c.instruction_write(10000,uint8_t(v));
  auto saved=c.state();c.instruction_write(10001,0xff);
  if(v)check(c.state()==saved); //00 establishes no busy;new command can succeed.
  if(v>=128){check(c.status_read(10040)==(v&127));}
 }
 for(unsigned start=0;start<80;++start){C c(1);uint64_t t=10000;
  c.instruction_write(t,uint8_t(0x80|start));c.data_write(t,0x5a);
  t+=40;c.instruction_write(t,uint8_t(0x80|start));
  check(c.data_read(t)==0x5a&&c.state().ddac==(start+1)%80);
 }
 C c(1);check(c.status_read(9999)==128&&c.status_read(10000)==0);
 c.instruction_write(10000,0x85);c.data_write(10000,0xab);
 c.instruction_write(10040,7);c.data_write(10040,0xcd);check(c.state().shift==1);
 c.instruction_write(10080,2);check(c.state().ddac==0&&c.state().shift==0&&c.state().ddram_selected);
 c.instruction_write(26480,0x42);c.instruction_write(26520,1);
 check(!c.state().ddram_selected&&c.state().cgac==2&&c.state().ddram[5]==32);
 // Clear retains CGRAM selection/address (pinned original behavior).
 c.instruction_write(42920,0xff);auto before=c.state();auto time=c.timing();
 bool bad=false;try{c.data_write(42920,1);}catch(const std::out_of_range&){bad=true;}
 check(bad&&c.state()==before&&c.timing()==time);
 c.reset(42920);check(c.state()==C(1).state()&&c.status_read(42920)==128);
 // End-to-end split GPIO:status latch busy80 then ready00 at exact reset deadline.
 C wired(1);yamanix::An1xLcdGpio<C> gpio(wired);
 gpio.write(11,1,0);gpio.write(9,1,1);gpio.write(9,1,0x21);check((gpio.read(11)&128)==128);
 gpio.write(9,10000,1);gpio.write(9,10000,0x21);check((gpio.read(11)&128)==0);
 std::printf("LCD controller:%u memory cases,address/wrap/mode/busy/GPIO PASS\n",cases);
}
