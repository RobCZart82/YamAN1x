#include "an1x_lcd_research_sink.h"
#include "an1x_sci_bus.h"
#include <cstdio>
#include <initializer_list>
void check(bool ok){if(!ok)throw std::runtime_error("P9 input profile failed");}
int main(){
 std::vector<uint8_t>rom(0x100000);
 yamanix::An1xLcdResearchSink sweep_sink(1);yamanix::An1xSciBus<yamanix::An1xLcdResearchSink>bus(rom,{},sweep_sink);
 for(unsigned direction=0;direction<256;++direction)for(unsigned data=0;data<256;++data){
  // Exercise a single stopped bus across the complete sweep.
  bus.write_byte(0xffffd0,uint8_t(direction));bus.write_byte(0xffffd2,uint8_t(data));
  unsigned expected=0;
  for(unsigned bit=0;bit<8;++bit)if(bit>=6||!(direction&(1u<<bit))||(data&(1u<<bit)))expected|=1u<<bit;
  check(bus.read_byte(0x12ffffd2)==expected&&!sweep_sink.failed());
 }
 yamanix::An1xLcdResearchSink sink(1);check(sink.gpio_read(9,10)==255);
 bool bad=false;try{(void)sink.gpio_read(9,9);}catch(const std::invalid_argument&){bad=true;}
 check(bad&&sink.failed());sink.cold_reset();check(sink.gpio_read(9,0)==255);
 for(unsigned port:{4u,6u,7u,8u}){sink.cold_reset();bad=false;
  try{(void)sink.gpio_read(port,0);}catch(const std::logic_error&){bad=true;}check(bad&&sink.failed());}
 std::puts("P9 scoped research pull-ups:65536 DDR/data cases,alias/strict others/time/reset PASS");
}
