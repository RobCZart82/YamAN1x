#include "an1x_sci_bus.h"
#include "an1x_lcd_research_sink.h"
#include "an1x_meg_research_sink.h"
#include "an1x_vop3_research_sink.h"
#include "h8_first_instructions.h"
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>

namespace {
struct Sink {
 std::unique_ptr<yamanix::An1xLcdResearchSink> lcd;
 std::unique_ptr<yamanix::An1xMegResearchSink> meg;
 std::unique_ptr<yamanix::An1xVop3ResearchSink> vop3;
 bool idle_panel;
 std::optional<uint16_t> an3_sample;
 uint64_t an3_sample_count=0,an3_first_sample_cycle=0;
 explicit Sink(bool enabled,bool meg_enabled,bool source_status,bool native,bool level,bool panel):idle_panel(panel){
  if(native)vop3=std::make_unique<yamanix::An1xVop3ResearchSink>(16,
   yamanix::An1xVop3ResearchSink::Adapter::ControlPolicy::experimental_firmware_contract,
   yamanix::MegSourceStatus::Policy::original_zero_stubs,
   level?yamanix::Vop3LevelReadPolicy::experimental_an1x_level:yamanix::Vop3LevelReadPolicy::reject);
  else if(meg_enabled)meg=std::make_unique<yamanix::An1xMegResearchSink>(16,source_status?yamanix::MegSourceStatus::Policy::original_zero_stubs:yamanix::MegSourceStatus::Policy::reject);
  else if(enabled)lcd=std::make_unique<yamanix::An1xLcdResearchSink>(16);
 }
 uint16_t adc_an3_sample(uint64_t cycle){
  if(!an3_sample)throw std::logic_error("Unsupported ADC conversion or IRQ enable");
  if(an3_sample_count++==0)an3_first_sample_cycle=cycle;
  return *an3_sample;
 }
 bool adc_an3_enabled()const{return an3_sample.has_value();}
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
 void tx(unsigned,uint64_t,bool){++tx_count;}
 void clock(unsigned,uint64_t,bool){++clock_count;}
 bool dma_request(unsigned,uint64_t){return false;}
 uint8_t gpio_read(unsigned p,uint64_t t){if(meg_sink())return meg_sink()->gpio_read(p,t);if(lcd)return lcd->gpio_read(p,t);throw std::logic_error("Unmodeled external GPIO input");}
 void gpio_write(unsigned p,uint64_t t,uint8_t v,uint8_t d){++gpio_writes;if(meg_sink())meg_sink()->gpio_write(p,t,v,d);else if(lcd)lcd->gpio_write(p,t,v,d);}
};
struct Host {
 yamanix::An1xSciBus<Sink> bus;
 bool fault=false;const char*kind="none";uint32_t address=0;unsigned width=0;
 Host(std::span<const uint8_t>rom,Sink&s):bus(rom,{},s){}
 template<class F>decltype(auto) access(const char*k,uint32_t a,unsigned w,F f){
  try{return f();}catch(...){fault=true;kind=k;address=a&0xffffff;width=w;throw;}
 }
 uint16_t read_instruction_word(uint32_t a){return access("instruction_read",a,2,[&]{return bus.read_instruction_word(a);});}
 uint8_t read_byte(uint32_t a){return access("data_read",a,1,[&]{return bus.read_byte(a);});}
 uint16_t read_word(uint32_t a){return access("data_read",a,2,[&]{return bus.read_word(a);});}
 void write_byte(uint32_t a,uint8_t v){access("data_write",a,1,[&]{bus.write_byte(a,v);});}
 void write_word(uint32_t a,uint16_t v){access("data_write",a,2,[&]{bus.write_word(a,v);});}
 void bus_cycle(uint64_t t){bus.bus_cycle(t);}
 uint64_t update(uint64_t t){return bus.update(t);}
 int access_cycles(uint32_t a,unsigned w)const{return bus.access_cycles(a,w);}
 uint64_t next_bus_event()const{return bus.next_bus_event();}
 uint8_t syscr_value()const{return bus.syscr_value();}
 void sync_cpu_ccr(uint8_t v){bus.sync_cpu_ccr(v);}
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
std::vector<uint8_t>load(const char*path){
 std::ifstream in(path,std::ios::binary|std::ios::ate);
 if(!in||in.tellg()!=std::streamoff(yamanix::An1xMemoryMap::rom_size))throw std::invalid_argument("Expected exactly 1048576 ROM bytes");
 std::vector<uint8_t>rom(yamanix::An1xMemoryMap::rom_size);in.seekg(0);
 if(!in.read(reinterpret_cast<char*>(rom.data()),std::streamsize(rom.size()))||in.peek()!=std::char_traits<char>::eof())
  throw std::runtime_error("ROM read failed or size changed");
 return rom;
}
}
int main(int argc,char**argv){
 try{
  if(argc<6||argc>8)throw std::invalid_argument("Usage: yamanix_an1x_boot_probe ROM cycles slice callbacks raw|swap16 [LCD_PROFILE] [--adc-an3=0..1023]");
  const auto cycles=number(argv[2],100000000),slice=number(argv[3],1000000),limit=number(argv[4],20000000);
  const std::string view=argv[5];if(view!="raw"&&view!="swap16")throw std::invalid_argument("Expected raw or swap16 ROM view");
  std::string_view profile="disabled";bool profile_seen=false;std::optional<uint16_t> adc_sample;
  for(int i=6;i<argc;++i){
   const std::string_view option(argv[i]);
   if(option.starts_with("--adc-an3=")){if(adc_sample)throw std::invalid_argument("Duplicate AN3 sample");adc_sample=parse_adc_an3(option);}
   else if(option=="lcd-16mhz"||option=="lcd-meg-observe-16mhz"||
           option=="lcd-meg-source-stubs-16mhz"||option=="lcd-meg-vop3-register-experiment-16mhz"||
           option=="lcd-meg-vop3-level-experiment-16mhz"||option=="lcd-meg-vop3-idle-panel-experiment-16mhz"){
    if(profile_seen)throw std::invalid_argument("Duplicate LCD profile");profile=option;profile_seen=true;
   }
   else throw std::invalid_argument("Unexpected or duplicate probe option");
  }
  const bool lcd_enabled=profile_seen;
  const bool panel=profile=="lcd-meg-vop3-idle-panel-experiment-16mhz";
  const bool level=panel||profile=="lcd-meg-vop3-level-experiment-16mhz";
  const bool native=level||profile=="lcd-meg-vop3-register-experiment-16mhz";
  const bool source_status=native||profile=="lcd-meg-source-stubs-16mhz";
  const bool meg_enabled=source_status||profile=="lcd-meg-observe-16mhz";
  if(lcd_enabled&&!meg_enabled&&profile!="lcd-16mhz")throw std::invalid_argument("Unknown LCD profile");
  auto rom=load(argv[1]);if(view=="swap16")for(std::size_t i=0;i<rom.size();i+=2)std::swap(rom[i],rom[i+1]);
  const auto reset_target=((uint32_t(rom[1])<<16)|(uint32_t(rom[2])<<8)|rom[3]);
  if(reset_target&1)throw std::invalid_argument("Odd reset target;verify explicit ROM byte order");
  Sink sink(lcd_enabled,meg_enabled,source_status,native,level,panel);sink.an3_sample=adc_sample;Host host(rom,sink);yamanix::H8FirstInstructions<Host>cpu(host,0,&host.bus.irq());
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
   <<",\"adc_an3_input_profile\":"<<std::quoted(adc_sample?"explicit-constant":"disabled")
   <<",\"adc_an3_value\":";
  if(adc_sample)std::cout<<*adc_sample;else std::cout<<"null";
  std::cout<<",\"adc_an3_samples\":"<<sink.an3_sample_count<<",\"adc_an3_first_sample_cycle\":";
  if(sink.an3_sample_count)std::cout<<sink.an3_first_sample_cycle;else std::cout<<"null";
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
   <<",\"completed_slice_callbacks\":"<<calls<<",\"peripheral_cycle\":"<<host.bus.cycle()
   <<",\"pc\":"<<state.pc<<",\"instruction_state\":"<<state.state<<",\"instruction_substate\":"<<state.substate
   <<",\"cpu_failed\":"<<(cpu.failed()?"true":"false")<<",\"fault_access\":";
  if(host.fault)std::cout<<"{\"kind\":"<<std::quoted(host.kind)<<",\"address\":"<<host.address<<",\"width\":"<<host.width<<'}';
  else std::cout<<"null";
  std::cout<<",\"tx_edges\":"<<sink.tx_count<<",\"clock_edges\":"<<sink.clock_count<<",\"gpio_writes\":"<<sink.gpio_writes
   <<",\"error\":"<<std::quoted(error)<<"}\n";
  return error.empty()?0:2;
 }catch(const std::exception&e){std::cout<<"{\"schema_version\":1,\"status\":\"input_error\",\"firmware_boot_accepted\":false,\"error\":"<<std::quoted(e.what())<<"}\n";return 1;}
}
