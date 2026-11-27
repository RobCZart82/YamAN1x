#include "an1x_sci_bus.h"
#include "an1x_lcd_research_sink.h"
#include "an1x_meg_research_sink.h"
#include "an1x_vop3_research_sink.h"
#include "h8_first_instructions.h"
#include <charconv>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>

namespace {
constexpr uint64_t max_probe_cycles=600000000;
constexpr uint64_t max_probe_slice_cycles=1000000;
constexpr uint64_t max_probe_callbacks=125000000;
constexpr std::size_t max_tstr_observations=64;
class An1xSciTxObserver {
public:
 static constexpr std::size_t capacity=4096;
 struct Event {
  unsigned channel;uint64_t cycle;bool level;bool first_observation;
  friend bool operator==(const Event&,const Event&)=default;
 };
 // The first callback per channel records the first observed level; later
 // callbacks are retained only when that level changes.
 void tx(unsigned channel,uint64_t cycle,bool level){
  if(channel>=last_level.size())throw std::out_of_range("SCI TX observer channel");
  const bool first=!last_level[channel].has_value();
  if(!first&&*last_level[channel]==level)return;
  last_level[channel]=level;
  if(count==events.size()){++dropped_count;return;}
  events[count++]={channel,cycle,level,first};
 }
 std::span<const Event> retained()const{return {events.data(),count};}
 std::size_t dropped()const{return dropped_count;}
private:
 std::array<Event,capacity> events{};
 std::array<std::optional<bool>,2> last_level{};
 std::size_t count=0,dropped_count=0;
};
// Records successful byte writes only; callers invoke this after bus completion.
class An1xSciAccessObserver {
public:
 static constexpr std::size_t capacity=256;
 struct Event {
  uint32_t address;uint16_t value;uint64_t cycle;unsigned width=1;
  friend bool operator==(const Event&,const Event&)=default;
 };
 void record(uint32_t raw,uint16_t value,uint64_t cycle,unsigned width=1){
  const auto address=raw&0xffffff;
  if(!((address>=0xffffb0&&address<=0xffffb5)||(address>=0xffffb8&&address<=0xffffbd)))return;
  if(count==events.size()){++dropped_count;return;}
  events[count++]={address,value,cycle,width};
 }
 std::span<const Event> retained()const{return {events.data(),count};}
 std::size_t dropped()const{return dropped_count;}
private:
 std::array<Event,capacity> events{};
 std::size_t count=0,dropped_count=0;
};
// MIDI at 31,250 bits/s on the probe's 16 MHz timeline. RXD0 is MIDI IN.
constexpr uint64_t midi_bit_cycles=512;
using MidiByteSchedule=std::array<yamanix::SciCycleTimeline::RxEvent,10>;
MidiByteSchedule schedule_midi_byte(uint64_t start,uint8_t byte){
 if(start==0||start>max_probe_cycles-10*midi_bit_cycles)
  throw std::invalid_argument("MIDI byte must fit within the probe cycle range");
 MidiByteSchedule events{};
 events.front()={start,0,false};
 for(unsigned bit=0;bit<8;++bit)
  events[bit+1]={start+(bit+1)*midi_bit_cycles,0,bool(byte&(1u<<bit))};
 events.back()={start+9*midi_bit_cycles,0,true};
 return events;
}
struct CpuContext {uint32_t pc=0,ppc=0,state=0,substate=0;uint8_t ccr=0;};
struct CycleWindow {
 uint64_t first,last;
 bool contains(uint64_t cycle)const{return cycle>=first&&cycle<=last;}
};
CycleWindow parse_cycle_window(std::string_view window){
 const auto split=window.find(':');
 if(split==std::string_view::npos)throw std::invalid_argument("Expected FIRST:LAST");
 auto endpoint=[](std::string_view token){
  uint64_t value=0;
  const auto [end,error]=std::from_chars(token.data(),token.data()+token.size(),value);
  if(error!=std::errc{}||end!=token.data()+token.size()||value>max_probe_cycles)
   throw std::invalid_argument("Invalid cycle endpoint");
  return value;
 };
 const CycleWindow result{endpoint(window.substr(0,split)),endpoint(window.substr(split+1))};
 if(result.first>result.last)throw std::invalid_argument("Reversed cycle window");
 return result;
}
uint8_t parse_panel_pressed_mask(std::string_view token){
 uint64_t value=0;
 const auto [end,error]=std::from_chars(token.data(),token.data()+token.size(),value);
 if(token.empty()||error!=std::errc{}||end!=token.data()+token.size()||value>255)
  throw std::invalid_argument("Invalid panel pressed mask");
 return uint8_t(value);
}
struct PanelReadObserver {
 static constexpr std::size_t capacity=256;
 struct Event {uint8_t selector,value;uint64_t cycle;CpuContext context;};
 bool enabled=false;
 std::optional<CycleWindow> window;
 std::array<Event,capacity> events{};
 std::size_t count=0,dropped=0;
 void record(uint8_t selector,uint8_t value,uint64_t cycle,const CpuContext&context){
  if(!enabled||(window&&!window->contains(cycle)))return;
  if(count==capacity){++dropped;return;}
  events[count++]={selector,value,cycle,context};
 }
 void write()const{
  if(!enabled){std::cout<<"null";return;}
  std::cout<<"{\"capacity\":"<<capacity<<",\"dropped\":"<<dropped<<",\"events\":[";
  for(std::size_t i=0;i<count;++i){if(i)std::cout<<',';const auto&e=events[i];
   std::cout<<"{\"selector\":"<<unsigned(e.selector)<<",\"value\":"<<unsigned(e.value)
    <<",\"cycle\":"<<e.cycle<<",\"pc\":"<<e.context.pc<<",\"ppc\":"<<e.context.ppc
    <<",\"instruction_state\":"<<e.context.state<<",\"instruction_substate\":"<<e.context.substate
    <<",\"raw_ccr\":"<<unsigned(e.context.ccr)<<'}';
  }
  std::cout<<"]}";
 }
};
struct StartupPrefix {
 struct Instruction {uint32_t pc;uint16_t opcode;uint8_t ccr;uint64_t bus_cycle;};
 std::array<Instruction,128> instructions{};
 std::size_t count=0,dropped=0;
 std::optional<CycleWindow> window;
 void record(uint32_t pc,uint16_t opcode,uint8_t ccr,uint64_t cycle){
  if(window&&!window->contains(cycle))return;
  if(count<instructions.size())instructions[count++]={pc,opcode,ccr,cycle};
  else if(window)++dropped;
 }
 void write()const{
  std::cout<<'[';
  for(std::size_t i=0;i<count;++i){
   if(i)std::cout<<',';
   const auto&e=instructions[i];
   std::cout<<"{\"pc\":"<<e.pc<<",\"opcode\":"<<e.opcode<<",\"raw_ccr\":"<<unsigned(e.ccr)
    <<",\"bus_cycle\":"<<e.bus_cycle<<'}';
  }
  std::cout<<']';
 }
 void write_window()const{
  if(!window){std::cout<<"null";return;}
  std::cout<<"{\"first_cycle\":"<<window->first<<",\"last_cycle\":"<<window->last
    <<",\"capacity\":"<<instructions.size()<<",\"dropped\":"<<dropped<<'}';
 }
};
struct TstrObservation {
 uint32_t address=0;uint8_t width=1,value=0;uint64_t cycle=0;CpuContext before{};
 bool continuation_available=false;CpuContext continuation{};
};
struct IrqObserver {
 static constexpr std::size_t capacity=512;
 struct Event {unsigned vector;uint64_t cycle;std::optional<uint32_t> handler;uint8_t ccr;};
 bool enabled=false;
 std::array<Event,capacity> events{};
 std::size_t count=0,dropped=0;
 void record(unsigned vector,uint64_t cycle,std::optional<uint32_t> handler={},uint8_t ccr=0){
  if(!enabled)return;
  if(count==capacity){++dropped;return;}
  events[count++]={vector,cycle,handler,ccr};
 }
 void write()const{
  if(!enabled){std::cout<<"null";return;}
  std::cout<<"{\"capacity\":"<<capacity<<",\"dropped\":"<<dropped<<",\"events\":[";
  for(std::size_t i=0;i<count;++i){
   if(i)std::cout<<',';
   const auto&e=events[i];
   std::cout<<"{\"vector\":"<<e.vector<<",\"bus_cycle\":"<<e.cycle<<",\"kind\":"
    <<std::quoted(e.handler?"accepted":"request");
   if(e.handler)std::cout<<",\"handler_pc\":"<<*e.handler<<",\"raw_ccr\":"<<unsigned(e.ccr);
   std::cout<<'}';
  }
  std::cout<<"]}";
 }
};
struct Sink {
 IrqObserver irq_observer;
 PanelReadObserver panel_observer;
 CpuContext panel_cpu_context{};
 std::unique_ptr<yamanix::An1xLcdResearchSink> lcd;
 std::unique_ptr<yamanix::An1xMegResearchSink> meg;
 std::unique_ptr<yamanix::An1xVop3ResearchSink> vop3;
 bool idle_panel;
 bool service_panel=false;
 uint64_t panel_chord_end=67200000;
 uint8_t panel_selector=7;
 std::array<uint64_t,8> panel_counts{};
 uint8_t panel_rows5=0x08,panel_rows6=0x88;
 std::optional<CycleWindow> panel_window;
 std::array<uint64_t,8> panel_first{};
 uint64_t panel_pressed_reads=0;
 uint8_t panel_read(uint64_t cycle) {
  if(panel_counts[panel_selector]++==0)panel_first[panel_selector]=cycle;
  const auto value=panel_value(cycle);
  panel_observer.record(panel_selector,value,cycle,panel_cpu_context);
  if(value!=0xff)++panel_pressed_reads;
  return value;
 }
 uint8_t panel_value(uint64_t cycle) const {
  if(!service_panel|| (panel_window&&!panel_window->contains(cycle)))return 0xff;
  if(cycle<panel_chord_end)return panel_selector==5?uint8_t(~panel_rows5):panel_selector==6?uint8_t(~panel_rows6):0xff;
  if(cycle>=80000000&&cycle<96000000&&((cycle-80000000)%3200000)<1600000)
   return panel_selector==5?uint8_t(~panel_rows5):0xff;
  if(cycle>=99200000&&cycle<101120000)return panel_selector==5?0xbf:0xff;
  return 0xff;
 }
 std::optional<uint16_t> an3_sample;
 uint64_t an3_sample_count=0,an3_first_sample_cycle=0;
 std::optional<std::array<uint16_t,8>> adc_scan_samples;
 std::array<uint64_t,8> adc_scan_sample_counts{};
 std::array<std::optional<uint64_t>,8> adc_scan_first_sample_cycles{};
 bool observe_sci_tx=false;
 An1xSciTxObserver sci_tx_observer;
 explicit Sink(bool enabled,bool meg_enabled,bool source_status,bool native,bool level,bool panel):idle_panel(panel){
  if(native)vop3=std::make_unique<yamanix::An1xVop3ResearchSink>(16,
   yamanix::An1xVop3ResearchSink::Adapter::ControlPolicy::experimental_firmware_contract,
   yamanix::MegSourceStatus::Policy::original_zero_stubs,
   level?yamanix::Vop3LevelReadPolicy::experimental_an1x_level:yamanix::Vop3LevelReadPolicy::reject);
  else if(meg_enabled)meg=std::make_unique<yamanix::An1xMegResearchSink>(16,source_status?yamanix::MegSourceStatus::Policy::original_zero_stubs:yamanix::MegSourceStatus::Policy::reject);
  else if(enabled)lcd=std::make_unique<yamanix::An1xLcdResearchSink>(16);
 }
 uint16_t adc_an3_sample(uint64_t cycle){
  const auto sample=an3_sample?an3_sample:(adc_scan_samples?std::optional<uint16_t>(adc_scan_samples->at(3)):std::nullopt);
  if(!sample)throw std::logic_error("Unsupported ADC conversion or IRQ enable");
  if(an3_sample_count++==0)an3_first_sample_cycle=cycle;
  return *sample;
 }
 uint16_t adc_scan_sample(unsigned channel,uint64_t cycle){
  if(!adc_scan_samples||channel>=adc_scan_sample_counts.size())
   throw std::logic_error("Unsupported ADC control mode");
  if(adc_scan_sample_counts[channel]++==0)adc_scan_first_sample_cycles[channel]=cycle;
  return adc_scan_samples->at(channel);
 }
 bool adc_an3_enabled()const{return an3_sample.has_value()||adc_scan_samples.has_value();}
 yamanix::An1xMegResearchSink* meg_sink()const{return vop3?static_cast<yamanix::An1xMegResearchSink*>(vop3.get()):meg.get();}
 bool idle_panel_read_enabled()const{return idle_panel;}
 bool vop3_word_enabled()const{return bool(vop3);}
 void vop3_write_word(uint32_t a,uint64_t t,uint16_t v){if(!vop3)throw std::out_of_range("VOP3 profile disabled");vop3->vop3_write_word(a,t,v);}
 uint16_t vop3_read_word(uint32_t a,uint64_t t){if(!vop3)throw std::out_of_range("VOP3 profile disabled");return vop3->vop3_read_word(a,t);}
 bool meg_read_enabled()const{return meg_sink()&&meg_sink()->meg_read_enabled();}
 uint8_t meg_read_byte(uint32_t a,uint64_t t){if(!meg_sink())throw std::out_of_range("MEG profile disabled");return meg_sink()->meg_read_byte(a,t);}
 bool meg_write_enabled()const{return bool(meg_sink());}
 void meg_write_byte(uint32_t a,uint64_t t,uint8_t v){if(!meg_sink())throw std::out_of_range("MEG profile disabled");meg_sink()->meg_write_byte(a,t,v);}
 uint64_t tx_count=0,clock_count=0,gpio_writes=0;
 void tx(unsigned channel,uint64_t cycle,bool level){++tx_count;if(observe_sci_tx)sci_tx_observer.tx(channel,cycle,level);}
 void clock(unsigned,uint64_t,bool){++clock_count;}
 bool dma_request(unsigned vector,uint64_t at){irq_observer.record(vector,at);return false;}
 uint8_t gpio_read(unsigned p,uint64_t t){if(meg_sink())return meg_sink()->gpio_read(p,t);if(lcd)return lcd->gpio_read(p,t);throw std::logic_error("Unmodeled external GPIO input");}
 void gpio_write(unsigned p,uint64_t t,uint8_t v,uint8_t d){++gpio_writes;if(p==6)panel_selector=v&7;if(meg_sink())meg_sink()->gpio_write(p,t,v,d);else if(lcd)lcd->gpio_write(p,t,v,d);}
};
struct Host {
 Sink& sink;
 yamanix::An1xSciBus<Sink> bus;
 bool fault=false;const char*kind="none";uint32_t address=0;unsigned width=0;
 bool fault_has_value=false;uint16_t fault_value=0;
 bool observe_sci_writes=false;An1xSciAccessObserver sci_writes;
 bool observe_sci0_rx_reads=false,observe_sci1_rx_reads=false;
 An1xSciAccessObserver sci0_rx_reads,sci1_rx_reads;
 uint64_t sci_first_cycle=0,sci_last_cycle=UINT64_MAX;
 bool observe_startup_prefix=false;StartupPrefix startup_prefix;
 bool observe_panel_reads=false;
 bool observe_tstr=false;CpuContext cpu_context{};std::array<TstrObservation,max_tstr_observations> tstr_observations{};
 std::size_t tstr_observation_count=0,tstr_observation_dropped=0;
 Host(std::span<const uint8_t>rom,Sink&s,bool observe=false,std::span<const yamanix::SciCycleTimeline::RxEvent> input={}):sink(s),bus(rom,input,s),observe_tstr(observe){}
 void observe_cpu_dispatch(uint32_t pc,uint32_t ppc,uint32_t state,uint32_t substate,uint8_t ccr){
  if(observe_startup_prefix&&state<0x10000&&substate==0)
   startup_prefix.record(ppc&0xffffff,uint16_t(state),ccr,bus.cycle());
  if(observe_panel_reads)
   sink.panel_cpu_context={pc&0xffffff,ppc&0xffffff,state,substate,ccr};
  if(!observe_tstr)return;
  cpu_context={pc&0xffffff,ppc&0xffffff,state,substate,ccr};
 }
 void observe_cpu_dispatch_continuation(uint32_t pc,uint32_t ppc,uint32_t state,uint32_t substate,uint8_t ccr,bool completed){
  if(!observe_tstr||!completed)return;
  if(substate!=0)return;
  const CpuContext continuation{pc&0xffffff,ppc&0xffffff,state,substate,ccr};
  for(std::size_t i=0;i<tstr_observation_count;++i){
   if(tstr_observations[i].continuation_available)continue;
   tstr_observations[i].continuation_available=true;
   tstr_observations[i].continuation=continuation;
  }
 }
 void observe_tstr_write(uint32_t a,uint8_t v){
  if(!observe_tstr||(a&0xffffff)!=0xffff60)return;
  if(tstr_observation_count==tstr_observations.size()){++tstr_observation_dropped;return;}
  tstr_observations[tstr_observation_count++]={a&0xffffff,1,v,bus.cycle(),cpu_context,false,{}};
 }
 void write_tstr_observations()const{
  if(!observe_tstr){std::cout<<"null";return;}
  auto write_context=[](const CpuContext&c){std::cout<<"{\"pc\":"<<c.pc<<",\"ppc\":"<<c.ppc<<",\"instruction_state\":"<<c.state<<",\"instruction_substate\":"<<c.substate<<",\"raw_ccr\":"<<unsigned(c.ccr)<<'}';};
  std::cout<<"{\"capacity\":"<<max_tstr_observations<<",\"dropped\":"<<tstr_observation_dropped<<",\"writes\":[";
  for(std::size_t i=0;i<tstr_observation_count;++i){if(i)std::cout<<',';const auto&e=tstr_observations[i];
   std::cout<<"{\"address\":"<<e.address<<",\"width\":"<<unsigned(e.width)<<",\"value\":"<<unsigned(e.value)<<",\"cycle\":"<<e.cycle<<",\"raw_ccr\":"<<unsigned(e.before.ccr)<<",\"before\":{\"pc\":"<<e.before.pc<<",\"ppc\":"<<e.before.ppc<<",\"instruction_state\":"<<e.before.state<<",\"raw_ccr\":"<<unsigned(e.before.ccr)<<'}';
   std::cout<<",\"continuation\":";if(e.continuation_available)write_context(e.continuation);else std::cout<<"null";std::cout<<'}';
  }
  std::cout<<"]}";
 }
 template<class F>decltype(auto) access(const char*k,uint32_t a,unsigned w,F f,std::optional<uint16_t> write_value=std::nullopt){
  try{return f();}catch(...){fault=true;kind=k;address=a&0xffffff;width=w;if(write_value){fault_has_value=true;fault_value=*write_value;}throw;}
 }
 uint16_t read_instruction_word(uint32_t a){return access("instruction_read",a,2,[&]{return bus.read_instruction_word(a);});}
 uint8_t read_byte(uint32_t a){
  return access("data_read",a,1,[&]{
   const auto value=bus.read_byte(a);
   const auto address=a&0xffffff;
   if(observe_sci0_rx_reads&&(address==0xffffb4||address==0xffffb5))
    sci0_rx_reads.record(address,value,bus.cycle());
   if(observe_sci1_rx_reads&&(address==0xffffbc||address==0xffffbd))
    sci1_rx_reads.record(address,value,bus.cycle());
   return value;
  });
 }
 uint16_t read_word(uint32_t a){
  return access("data_read",a,2,[&]{
   const auto value=bus.read_word(a);
   if(observe_sci0_rx_reads&&(a&0xffffff)==0xffffb4)
    sci0_rx_reads.record(a,value,bus.cycle(),2);
   if(observe_sci1_rx_reads&&(a&0xffffff)==0xffffbc)
    sci1_rx_reads.record(a,value,bus.cycle(),2);
   return value;
  });
 }
 void write_byte(uint32_t a,uint8_t v){access("data_write",a,1,[&]{bus.write_byte(a,v);observe_tstr_write(a,v);const auto cycle=bus.cycle();if(observe_sci_writes&&cycle>=sci_first_cycle&&cycle<=sci_last_cycle)sci_writes.record(a,v,cycle);},v);}
 void write_word(uint32_t a,uint16_t v){access("data_write",a,2,[&]{bus.write_word(a,v);},v);}
 void bus_cycle(uint64_t t){bus.bus_cycle(t);}
 uint64_t update(uint64_t t){return bus.update(t);}
 int access_cycles(uint32_t a,unsigned w)const{return bus.access_cycles(a,w);}
 uint64_t next_bus_event()const{return bus.next_bus_event();}
 uint8_t syscr_value()const{return bus.syscr_value();}
 void sync_cpu_ccr(uint8_t v){bus.sync_cpu_ccr(v);}
 // The bus timestamp is from the last vector read, before the handler fetch.
 void observe_irq_taken(unsigned vector,uint32_t handler,uint8_t ccr){
  sink.irq_observer.record(vector,bus.cycle(),handler&0xffffff,ccr);
 }
};
uint64_t number(const char*s,uint64_t maximum){
 const std::string_view v(s);uint64_t n=0;auto [end,error]=std::from_chars(v.data(),v.data()+v.size(),n);
 if(error!=std::errc{}||end!=v.data()+v.size()||n==0||n>maximum)throw std::invalid_argument("Invalid bounded numeric argument");
 return n;
}
uint16_t parse_adc_an3(std::string_view argument){
 constexpr std::string_view prefix="--adc-an3=";
 if(!argument.starts_with(prefix))throw std::invalid_argument("Expected --adc-an3=0..1023");
 const auto value=argument.substr(prefix.size());uint64_t n=0;
 const auto [end,error]=std::from_chars(value.data(),value.data()+value.size(),n);
 if(error!=std::errc{}||end!=value.data()+value.size()||n>1023)
  throw std::invalid_argument("AN3 sample must be an integer from 0 through 1023");
 return uint16_t(n);
}
std::array<uint16_t,8> parse_adc_scan(std::string_view argument){
 constexpr std::string_view prefix="--adc-scan=";
 if(!argument.starts_with(prefix))throw std::invalid_argument("Expected --adc-scan=AN0,AN1,AN2,AN3,AN4,AN5,AN6,AN7");
 std::array<uint16_t,8> values{};auto rest=argument.substr(prefix.size());
 for(std::size_t channel=0;channel<values.size();++channel){
  const auto comma=rest.find(',');const auto token=rest.substr(0,comma);
  uint64_t value=0;const auto [end,error]=std::from_chars(token.data(),token.data()+token.size(),value);
  if(token.empty()||error!=std::errc{}||end!=token.data()+token.size()||value>1023)
   throw std::invalid_argument("ADC scan samples must contain eight integers from 0 through 1023");
  values[channel]=uint16_t(value);
  if(channel+1<values.size()){
   if(comma==std::string_view::npos)throw std::invalid_argument("ADC scan requires exactly eight channel samples");
   rest.remove_prefix(comma+1);
  }else if(comma!=std::string_view::npos)throw std::invalid_argument("ADC scan requires exactly eight channel samples");
 }
 return values;
}
std::vector<uint8_t>load(const char*path){
 std::ifstream in(path,std::ios::binary|std::ios::ate);
 if(!in||in.tellg()!=std::streamoff(yamanix::An1xMemoryMap::rom_size))throw std::invalid_argument("Expected exactly 1048576 ROM bytes");
 std::vector<uint8_t>rom(yamanix::An1xMemoryMap::rom_size);in.seekg(0);
 if(!in.read(reinterpret_cast<char*>(rom.data()),std::streamsize(rom.size()))||in.peek()!=std::char_traits<char>::eof())
  throw std::runtime_error("ROM read failed or size changed");
 return rom;
}
}
#ifndef YAMANIX_AN1X_BOOT_PROBE_OBSERVER_TEST
int main(int argc,char**argv){
 try{
  if(argc<6||argc>18)throw std::invalid_argument("Usage: yamanix_an1x_boot_probe ROM cycles slice callbacks raw|swap16 [LCD_PROFILE] [--adc-an3=0..1023|--adc-scan=AN0,AN1,AN2,AN3,AN4,AN5,AN6,AN7] [--observe-tstr] [--observe-sci-tx] [--observe-sci-writes[=FIRST:LAST]] [--midi-byte=CYCLE:BYTE] [--observe-sci0-rx-reads] [--observe-sci1-rx-reads] [--observe-irq] [--observe-startup-prefix[=FIRST:LAST]] [--observe-panel=FIRST:LAST]");
  const auto cycles=number(argv[2],max_probe_cycles),slice=number(argv[3],max_probe_slice_cycles),limit=number(argv[4],max_probe_callbacks);
  const std::string view=argv[5];if(view!="raw"&&view!="swap16")throw std::invalid_argument("Expected raw or swap16 ROM view");
  std::string_view profile="disabled";bool profile_seen=false,observe_startup_prefix=false,observe_irq=false,observe_tstr=false,observe_sci_tx=false,observe_sci_writes=false;std::optional<uint16_t> adc_sample;std::optional<std::array<uint16_t,8>> adc_scan_samples;
  uint64_t sci_first_cycle=0,sci_last_cycle=UINT64_MAX;
  std::optional<CycleWindow> prefix_window;
  std::optional<CycleWindow> panel_observation_window;
  std::optional<MidiByteSchedule> midi;
  std::optional<uint64_t> panel_chord_end;uint8_t panel_rows5=8,panel_rows6=136;
  std::optional<CycleWindow> panel_window;
  bool panel_rows_seen=false;
  bool observe_sci0_rx_reads=false,observe_sci1_rx_reads=false;
  for(int i=6;i<argc;++i){
   const std::string_view option(argv[i]);
   if(option.starts_with("--adc-an3=")){if(adc_sample)throw std::invalid_argument("Duplicate AN3 sample");adc_sample=parse_adc_an3(option);}
   else if(option.starts_with("--adc-scan=")){if(adc_scan_samples)throw std::invalid_argument("Duplicate ADC scan samples");adc_scan_samples=parse_adc_scan(option);}
   else if(option.starts_with("--panel-window=")){
    if(panel_window)throw std::invalid_argument("Duplicate panel window");
    panel_window=parse_cycle_window(option.substr(std::string_view("--panel-window=").size()));
   }
   else if(option.starts_with("--panel-rows=")){
    if(panel_rows_seen)throw std::invalid_argument("Duplicate panel row mask option");
    panel_rows_seen=true;
    const std::string fields(option.substr(13));const auto colon=fields.find(':');
    if(colon==std::string::npos)throw std::invalid_argument("Expected --panel-rows=SF:SG");
    const auto a=parse_panel_pressed_mask(std::string_view(fields).substr(0,colon));
    const auto b=parse_panel_pressed_mask(std::string_view(fields).substr(colon+1));
    panel_rows5=uint8_t(a);panel_rows6=uint8_t(b);
   }
   else if(option.starts_with("--observe-panel=")){
    if(panel_observation_window)throw std::invalid_argument("Duplicate panel observer option");
    panel_observation_window=parse_cycle_window(option.substr(std::string_view("--observe-panel=").size()));
   }
   else if(option.starts_with("--panel-chord-end=")){
    if(panel_chord_end)throw std::invalid_argument("Duplicate panel chord end");
    panel_chord_end=number(std::string(option.substr(std::string_view("--panel-chord-end=").size())).c_str(),max_probe_cycles);
   }
   else if(option.starts_with("--midi-byte=")){
    if(midi)throw std::invalid_argument("Duplicate MIDI byte");
    const std::string fields(option.substr(12));
    const auto separator=fields.find(':');
    if(separator==std::string::npos)throw std::invalid_argument("Expected MIDI CYCLE:BYTE");
    const auto start=number(fields.substr(0,separator).c_str(),max_probe_cycles);
    const auto digits=std::string_view(fields).substr(separator+1);
    unsigned byte=0;
    const auto [end,error]=std::from_chars(digits.data(),digits.data()+digits.size(),byte);
    if(error!=std::errc{}||end!=digits.data()+digits.size()||byte>255||
       cycles<10*midi_bit_cycles||start>cycles-10*midi_bit_cycles)
     throw std::invalid_argument("Invalid MIDI byte or frame exceeds requested cycles");
    midi=schedule_midi_byte(start,uint8_t(byte));
   }
   else if(option=="--observe-sci0-rx-reads"){
    if(observe_sci0_rx_reads)throw std::invalid_argument("Duplicate SCI0 receive read observer option");
    observe_sci0_rx_reads=true;
   }
   else if(option=="--observe-sci1-rx-reads"){
    if(observe_sci1_rx_reads)throw std::invalid_argument("Duplicate SCI1 receive read observer option");
    observe_sci1_rx_reads=true;
   }
   else if(option=="--observe-startup-prefix"){if(observe_startup_prefix)throw std::invalid_argument("Duplicate startup prefix option");observe_startup_prefix=true;}
   else if(option.starts_with("--observe-startup-prefix=")){
    if(observe_startup_prefix)throw std::invalid_argument("Duplicate startup prefix option");
    prefix_window=parse_cycle_window(option.substr(std::string_view("--observe-startup-prefix=").size()));
    observe_startup_prefix=true;
   }
   else if(option=="--observe-irq"){if(observe_irq)throw std::invalid_argument("Duplicate IRQ observer option");observe_irq=true;}
   else if(option=="--observe-tstr"){if(observe_tstr)throw std::invalid_argument("Duplicate TSTR observer option");observe_tstr=true;}
   else if(option=="--observe-sci-writes"){if(observe_sci_writes)throw std::invalid_argument("Duplicate SCI write observer option");observe_sci_writes=true;}
   else if(option.starts_with("--observe-sci-writes=")){
    if(observe_sci_writes)throw std::invalid_argument("Duplicate SCI write observer option");
    const auto window=parse_cycle_window(option.substr(std::string_view("--observe-sci-writes=").size()));
    sci_first_cycle=window.first;
    sci_last_cycle=window.last;
    observe_sci_writes=true;
   }
   else if(option=="--observe-sci-tx"){if(observe_sci_tx)throw std::invalid_argument("Duplicate SCI TX observer option");observe_sci_tx=true;}
   else if(option=="lcd-16mhz"||option=="lcd-meg-observe-16mhz"||
           option=="lcd-meg-source-stubs-16mhz"||option=="lcd-meg-vop3-register-experiment-16mhz"||
           option=="lcd-meg-vop3-level-experiment-16mhz"||option=="lcd-meg-vop3-idle-panel-experiment-16mhz"||option=="lcd-meg-vop3-t6-panel-experiment-16mhz"){
    if(profile_seen)throw std::invalid_argument("Duplicate LCD profile");profile=option;profile_seen=true;
   }
   else throw std::invalid_argument("Unexpected or duplicate probe option");
  }
  if(adc_sample&&adc_scan_samples)throw std::invalid_argument("Choose either --adc-an3 or --adc-scan, not both");
  const bool lcd_enabled=profile_seen;
  const bool service_panel=profile=="lcd-meg-vop3-t6-panel-experiment-16mhz";
  if(panel_chord_end&&!service_panel)throw std::invalid_argument("Panel chord requires T6 panel profile");
  const bool panel=service_panel||profile=="lcd-meg-vop3-idle-panel-experiment-16mhz";
  if((panel_window||panel_rows_seen)&&!service_panel)throw std::invalid_argument("Panel stimulus options require the T6 panel profile");
  if(panel_observation_window&&!panel)throw std::invalid_argument("Panel observer requires a panel profile");
  const bool level=panel||profile=="lcd-meg-vop3-level-experiment-16mhz";
  const bool native=level||profile=="lcd-meg-vop3-register-experiment-16mhz";
  const bool source_status=native||profile=="lcd-meg-source-stubs-16mhz";
  const bool meg_enabled=source_status||profile=="lcd-meg-observe-16mhz";
  if(lcd_enabled&&!meg_enabled&&profile!="lcd-16mhz")throw std::invalid_argument("Unknown LCD profile");
  auto rom=load(argv[1]);if(view=="swap16")for(std::size_t i=0;i<rom.size();i+=2)std::swap(rom[i],rom[i+1]);
  const auto reset_target=((uint32_t(rom[1])<<16)|(uint32_t(rom[2])<<8)|rom[3]);
  if(reset_target&1)throw std::invalid_argument("Odd reset target;verify explicit ROM byte order");
  Sink sink(lcd_enabled,meg_enabled,source_status,native,level,panel);sink.service_panel=service_panel;if(panel_chord_end)sink.panel_chord_end=*panel_chord_end;sink.panel_rows5=uint8_t(panel_rows5);sink.panel_rows6=uint8_t(panel_rows6);sink.panel_window=panel_window;sink.panel_observer.enabled=panel_observation_window.has_value();sink.panel_observer.window=panel_observation_window;sink.irq_observer.enabled=observe_irq;sink.an3_sample=adc_sample;sink.adc_scan_samples=adc_scan_samples;sink.observe_sci_tx=observe_sci_tx;Host host(rom,sink,observe_tstr,midi ? std::span<const yamanix::SciCycleTimeline::RxEvent>(*midi) : std::span<const yamanix::SciCycleTimeline::RxEvent>{});host.observe_startup_prefix=observe_startup_prefix;host.startup_prefix.window=prefix_window;host.observe_panel_reads=panel_observation_window.has_value();host.observe_sci0_rx_reads=observe_sci0_rx_reads;host.observe_sci1_rx_reads=observe_sci1_rx_reads;host.observe_sci_writes=observe_sci_writes;host.sci_first_cycle=sci_first_cycle;host.sci_last_cycle=sci_last_cycle;yamanix::H8FirstInstructions<Host>cpu(host,0,&host.bus.irq());
  uint64_t completed=0,calls=0;std::string error;
  try{while(completed<cycles){const auto count=std::min(slice,cycles-completed);
   calls+=cpu.run(completed,int(count),std::size_t(limit-calls));completed+=count;
  }}catch(const std::exception&e){error=e.what();}
  const auto state=cpu.snapshot();
  yamanix_vop3_metrics metrics{};bool metrics_available=true;
  try{if(sink.vop3)metrics=sink.vop3->vop3_metrics();}catch(const std::exception&){metrics_available=false;}
  auto pair=[&](const uint64_t* values){if(metrics_available)std::cout<<'['<<values[0]<<','<<values[1]<<']';else std::cout<<"null";};
  const char*stop_reason=error.empty()?"cycle_limit":error=="H8 execution callback limit"?"callback_limit":"execution_error";
  std::cout<<"{\"schema_version\":1,\"status\":"<<std::quoted(error.empty()?"cycle_limit":"stopped")
   <<",\"stop_reason\":"<<std::quoted(stop_reason)<<",\"callback_limit_requested\":"<<limit
   <<",\"lcd_profile\":"<<std::quoted(profile)
   <<",\"adc_an3_input_profile\":"<<std::quoted(adc_sample?"explicit-constant":adc_scan_samples?"explicit-constant-from-eight-channel-scan":"disabled")
   <<",\"adc_an3_value\":";
  if(adc_sample)std::cout<<*adc_sample;else if(adc_scan_samples)std::cout<<adc_scan_samples->at(3);else std::cout<<"null";
  std::cout<<",\"adc_an3_samples\":"<<sink.an3_sample_count<<",\"adc_an3_first_sample_cycle\":";
  if(sink.an3_sample_count)std::cout<<sink.an3_first_sample_cycle;else std::cout<<"null";
  std::cout<<",\"adc_scan_input_profile\":"<<std::quoted(adc_scan_samples?"explicit-constant-eight-channel":"disabled")<<",\"adc_scan_values\":";
  if(adc_scan_samples){std::cout<<'[';for(std::size_t i=0;i<adc_scan_samples->size();++i){if(i)std::cout<<',';std::cout<<adc_scan_samples->at(i);}std::cout<<']';}else std::cout<<"null";
  std::cout<<",\"adc_scan_sample_counts\":[";for(std::size_t i=0;i<sink.adc_scan_sample_counts.size();++i){if(i)std::cout<<',';std::cout<<sink.adc_scan_sample_counts[i];}std::cout<<']';
  std::cout<<",\"adc_scan_first_sample_cycles\":[";for(std::size_t i=0;i<sink.adc_scan_first_sample_cycles.size();++i){if(i)std::cout<<',';if(sink.adc_scan_first_sample_cycles[i])std::cout<<*sink.adc_scan_first_sample_cycles[i];else std::cout<<"null";}std::cout<<']';
  std::cout
   <<",\"meg_observed_controls\":"<<(sink.meg_sink()?sink.meg_sink()->meg_observation().count:0)
   <<",\"meg_source_status_reads\":"<<(sink.meg_sink()?sink.meg_sink()->meg_status_reads():0)
   <<",\"vop3_profile\":"<<std::quoted(level?"native-registers-experimental-upload-and-level":native?"native-registers-experimental-upload":"disabled")
   <<",\"vop3_metrics_available\":"<<(metrics_available?"true":"false")
   <<",\"vop3_writes\":";pair(metrics.writes);
  std::cout<<",\"vop3_reads\":";pair(metrics.reads);
  std::cout<<",\"vop3_sample_passes\":";pair(metrics.sample_passes);
  std::cout
   <<",\"audio_accepted\":false"
   <<",\"firmware_boot_accepted\":false,\"rom_view\":"<<std::quoted(view)<<",\"reset_target\":"<<reset_target<<",\"cycles_requested\":"<<cycles<<",\"completed_slice_cycles\":"<<completed
   <<",\"midi_input_events_consumed\":"<<host.bus.consumed_input()
   <<",\"completed_slice_callbacks\":"<<calls<<",\"peripheral_cycle\":"<<host.bus.cycle()
   <<",\"pc\":"<<state.pc<<",\"instruction_state\":"<<state.state<<",\"instruction_substate\":"<<state.substate
   <<",\"cpu_failed\":"<<(cpu.failed()?"true":"false")<<",\"fault_access\":";
  if(host.fault){std::cout<<"{\"kind\":"<<std::quoted(host.kind)<<",\"address\":"<<host.address<<",\"width\":"<<host.width;
   if(host.fault_has_value)std::cout<<",\"value\":"<<host.fault_value;
   std::cout<<'}';}
  else std::cout<<"null";
  std::cout<<",\"startup_prefix\":";if(observe_startup_prefix)host.startup_prefix.write();else std::cout<<"null";
  std::cout<<",\"startup_prefix_window\":";host.startup_prefix.write_window();
  std::cout<<",\"irq_observations\":";sink.irq_observer.write();
  std::cout<<",\"tstr_observations\":";host.write_tstr_observations();
  std::cout<<",\"sci_write_observations\":";
  if(!observe_sci_writes)std::cout<<"null";
  else{
   std::cout<<"{\"capacity\":"<<An1xSciAccessObserver::capacity<<",\"dropped\":"<<host.sci_writes.dropped()<<",\"events\":[";
   bool first=true;for(const auto&e:host.sci_writes.retained()){
    if(!first)std::cout<<',';first=false;
    std::cout<<"{\"address\":"<<e.address<<",\"value\":"<<unsigned(e.value)<<",\"cycle\":"<<e.cycle<<'}';
   }std::cout<<"]}";
  }
  auto print_rx_trace=[](bool enabled,const An1xSciAccessObserver& trace){
  if(!enabled)std::cout<<"null";
  else{
   std::cout<<"{\"capacity\":"<<An1xSciAccessObserver::capacity<<",\"dropped\":"<<trace.dropped()<<",\"events\":[";
   bool first=true;
   for(const auto& event:trace.retained()){
    if(!first)std::cout<<',';
    first=false;
    std::cout<<"{\"address\":"<<event.address<<",\"width\":"<<event.width<<",\"value\":"<<unsigned(event.value)<<",\"cycle\":"<<event.cycle<<'}';
   }
   std::cout<<"]}";
  }
  };
  std::cout<<",\"sci0_rx_read_observations\":";
  print_rx_trace(observe_sci0_rx_reads,host.sci0_rx_reads);
  std::cout<<",\"sci1_rx_read_observations\":";
  print_rx_trace(observe_sci1_rx_reads,host.sci1_rx_reads);
  std::cout<<",\"sci_tx_observations\":";
  if(!observe_sci_tx)std::cout<<"null";
  else{
   const auto events=sink.sci_tx_observer.retained();
   std::cout<<"{\"capacity\":"<<An1xSciTxObserver::capacity<<",\"dropped\":"<<sink.sci_tx_observer.dropped()<<",\"events\":[";
   for(std::size_t i=0;i<events.size();++i){if(i)std::cout<<',';const auto&e=events[i];
    std::cout<<"{\"channel\":"<<e.channel<<",\"cycle\":"<<e.cycle<<",\"level\":"<<(e.level?"true":"false")<<",\"first_observation\":"<<(e.first_observation?"true":"false")<<'}';
   }
   std::cout<<"]}";
  }
  if(panel){
   std::cout<<",\"panel_pressed_reads\":"<<sink.panel_pressed_reads<<",\"panel_counts\":[";
   for(unsigned i=0;i<8;++i){if(i)std::cout<<',';std::cout<<sink.panel_counts[i];}
   std::cout<<"],\"panel_first_cycles\":[";
   for(unsigned i=0;i<8;++i){if(i)std::cout<<',';if(sink.panel_counts[i])std::cout<<sink.panel_first[i];else std::cout<<"null";}
   std::cout<<"]";
   const auto& state=sink.meg_sink()->lcd_state();
   std::cout<<",\"panel_lcd_ddram\":[";
   for(unsigned i=0;i<state.ddram.size();++i){if(i)std::cout<<',';std::cout<<unsigned(state.ddram[i]);}
   std::cout<<"],\"panel_read_observations\":";
   sink.panel_observer.write();
  }
  std::cout<<",\"tx_edges\":"<<sink.tx_count<<",\"clock_edges\":"<<sink.clock_count<<",\"gpio_writes\":"<<sink.gpio_writes
   <<",\"error\":"<<std::quoted(error)<<"}\n";
  return error.empty()?0:2;
 }catch(const std::exception&e){std::cout<<"{\"schema_version\":1,\"status\":\"input_error\",\"firmware_boot_accepted\":false,\"error\":"<<std::quoted(e.what())<<"}\n";return 1;}
}
#endif
