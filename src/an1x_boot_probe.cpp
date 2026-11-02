#include "an1x_sci_bus.h"
#include "h8_first_instructions.h"
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
struct Sink {
 uint64_t tx_count=0,clock_count=0,gpio_writes=0;
 void tx(unsigned,uint64_t,bool){++tx_count;}
 void clock(unsigned,uint64_t,bool){++clock_count;}
 bool dma_request(unsigned,uint64_t){return false;}
 uint8_t gpio_read(unsigned,uint64_t){throw std::logic_error("Unmodeled external GPIO input");}
 void gpio_write(unsigned,uint64_t,uint8_t,uint8_t){++gpio_writes;}
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
 uint64_t next_bus_event()const{return bus.next_bus_event();}
 uint8_t syscr_value()const{return bus.syscr_value();}
 void sync_cpu_ccr(uint8_t v){bus.sync_cpu_ccr(v);}
};
uint64_t number(const char*s,uint64_t maximum){
 const std::string_view v(s);uint64_t n=0;auto [end,error]=std::from_chars(v.data(),v.data()+v.size(),n);
 if(error!=std::errc{}||end!=v.data()+v.size()||n==0||n>maximum)throw std::invalid_argument("Invalid bounded numeric argument");
 return n;
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
  if(argc!=6)throw std::invalid_argument("Usage: yamanix_an1x_boot_probe ROM cycles slice callbacks raw|swap16");
  const auto cycles=number(argv[2],100000000),slice=number(argv[3],1000000),limit=number(argv[4],1000000);
  const std::string view=argv[5];if(view!="raw"&&view!="swap16")throw std::invalid_argument("Expected raw or swap16 ROM view");
  auto rom=load(argv[1]);if(view=="swap16")for(std::size_t i=0;i<rom.size();i+=2)std::swap(rom[i],rom[i+1]);
  const auto reset_target=((uint32_t(rom[1])<<16)|(uint32_t(rom[2])<<8)|rom[3]);
  if(reset_target&1)throw std::invalid_argument("Odd reset target;verify explicit ROM byte order");
  Sink sink;Host host(rom,sink);yamanix::H8FirstInstructions<Host>cpu(host,0,&host.bus.irq());
  uint64_t completed=0,calls=0;std::string error;
  try{while(completed<cycles){const auto count=std::min(slice,cycles-completed);
   calls+=cpu.run(completed,int(count),std::size_t(limit-calls));completed+=count;
  }}catch(const std::exception&e){error=e.what();}
  const auto state=cpu.snapshot();
  std::cout<<"{\"schema_version\":1,\"status\":"<<std::quoted(error.empty()?"cycle_limit":"stopped")
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
