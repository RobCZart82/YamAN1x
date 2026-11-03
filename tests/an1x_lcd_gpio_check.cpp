#include "an1x_lcd_gpio.h"
#include <cstdio>
#include <initializer_list>
#include <stdexcept>
struct Controller {
    unsigned calls=0,kind=0; uint64_t time=0; uint8_t value=0,reply=0;
    void record(unsigned k,uint64_t t){++calls;kind=k;time=t;}
    uint8_t status_read(uint64_t t){record(1,t);return reply;}
    uint8_t data_read(uint64_t t){record(2,t);return reply;}
    void instruction_write(uint64_t t,uint8_t v){record(3,t);value=v;}
    void data_write(uint64_t t,uint8_t v){record(4,t);value=v;}
};
void check(bool v){if(!v)throw std::runtime_error("LCD GPIO contract failed");}
int main(){
 for(unsigned v=0;v<256;++v)for(unsigned rs=0;rs<2;++rs){
  Controller c;yamanix::An1xLcdGpio<Controller> gpio(c);
  check(gpio.read(10)==255&&gpio.read(11)==255);
  // Assemble from individual pin positions,independent of adapter masks.
  unsigned pa=0,pb=rs?4:0;
  for(unsigned bit:{0u,2u,3u,4u,5u})pa|=v&(1u<<bit);
  for(unsigned bit:{1u,6u,7u})pb|=v&(1u<<bit);
  gpio.write(10,10,uint8_t(pa));gpio.write(11,11,uint8_t(pb));
  gpio.write(9,12,0x20);check(c.calls==0);
  gpio.write(9,13,0);check(c.calls==1&&c.kind==3+rs&&c.time==13&&c.value==v);
  gpio.write(9,14,0);check(c.calls==1);
  c.reply=uint8_t(v);gpio.write(9,15,1);gpio.write(9,16,0x21);
  check(c.calls==2&&c.kind==1+rs&&c.time==16);
  check(gpio.read(10)==uint8_t(pa|0xc2)&&gpio.read(11)==uint8_t((pb&0xc2)|0x3d));
  c.reply=uint8_t(~v);gpio.write(9,17,0x21);check(c.calls==2);
  check(gpio.read(10)==uint8_t(pa|0xc2)); // latched,not reread
  gpio.write(9,18,1);check(c.calls==2&&gpio.read(11)==255);
  check(!gpio.write(4,19,0)&&c.calls==2);
  bool rejected=false;try{(void)gpio.read(9);}catch(const std::logic_error&){rejected=true;}check(rejected);
 }
 std::puts("LCD GPIO:512 data/RS cases,edge/latch/time/routing PASS");
}
