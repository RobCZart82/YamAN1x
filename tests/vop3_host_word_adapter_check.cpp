#include "vop3_host_word_adapter.h"
#include <cstdio>
#include <functional>
#include <initializer_list>
struct Backend {unsigned writes=0,reads=0,chip=0,reg=0;uint16_t value=0;int result=0;std::function<void()> callback;
 int write(unsigned c,unsigned r,uint16_t v,uint16_t){++writes;chip=c;reg=r;value=v;if(callback)callback();return result;}
 int read(unsigned c,unsigned r,uint16_t,uint16_t&v){++reads;chip=c;reg=r;v=uint16_t(0xa500+c);return result;}};
using A=yamanix::Vop3HostWordAdapter<Backend>;
void check(bool ok){if(!ok)throw std::runtime_error("VOP3 word adapter failed");}
int main(){Backend b;A a(b,A::ControlPolicy::experimental_firmware_contract);unsigned cases=0;
 for(unsigned chip=0;chip<2;++chip)for(unsigned reg=0;reg<128;++reg)for(unsigned value=0;value<256;++value){
  if(reg==1)continue;const uint32_t address=(chip?0x480000:0x440000)+2*reg;
  a.write_word(address,uint16_t(value*257));check(b.chip==chip&&b.reg==reg&&b.value==value*257);++cases;}
 for(unsigned chip=0;chip<2;++chip){const uint32_t base=chip?0x480000:0x440000;
  a.write_word(base+2,1);check(a.read_word(base+2)==0x8000&&a.read_word((chip?0x440000:0x480000)+2)==0);
  auto writes=b.writes;bool bad=false;try{a.write_word(base+2,2);}catch(const std::invalid_argument&){bad=true;}
  check(bad&&b.writes==writes&&a.read_word(base+2)==0x8000&&!a.failed());
  a.write_word(base+2,0);check(a.read_word(base+2)==0&&a.read_word(base+0x42)==0xa500+chip);}
 for(auto address:{0x43fffeu,0x440001u,0x440100u,0x480100u,0x1440000u}){bool bad=false;auto count=b.writes;
  try{a.write_word(address,0);}catch(const std::exception&){bad=true;}check(bad&&b.writes==count&&!a.failed());}
 for(auto mask:{uint16_t(0),uint16_t(0xff),uint16_t(0xff00)}){bool bad=false;auto n=b.writes;
  try{a.write_word(0x440000,0,mask);}catch(const std::invalid_argument&){bad=true;}check(bad&&b.writes==n);}
 {Backend backend;A strict(backend);bool bad=false;try{strict.read_word(0x440002);}catch(const std::logic_error&){bad=true;}
  check(bad&&backend.reads==0);bad=false;try{strict.read_word(0x440044);}catch(const std::out_of_range&){bad=true;}check(bad&&backend.reads==0);}
 {Backend backend;A failed(backend);backend.result=-3;bool bad=false;try{failed.write_word(0x440000,1);}catch(const std::runtime_error&){bad=true;}check(bad&&failed.failed());}
 {Backend backend;A nested(backend);backend.callback=[&]{try{nested.write_word(0x440000,2);}catch(const std::logic_error&) {}};
  bool bad=false;try{nested.write_word(0x440000,1);}catch(const std::runtime_error&){bad=true;}check(bad&&nested.failed()&&backend.writes==1);}
 std::printf("VOP3 host:%u route/value cases,chip controls,strict bounds/masks/read/fail-stop/reentry PASS\n",cases);
}
