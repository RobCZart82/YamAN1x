#pragma once
#include "h8_itu_event_deadline.h"
#include <cstddef>
namespace yamanix {
// Own one supported channel;callbacks must not reenter/mutate this owner.
// Budget counts timestamps,each delivering at most3 vectors(A,B,overflow).
template<class Channel>class H8ItuBoundedDispatch {
public:
 explicit H8ItuBoundedDispatch(unsigned channel=0):base_(24+4*channel){if(channel>4)throw std::invalid_argument("Unsupported H83002 ITU channel");}
 H8ItuBoundedDispatch(const H8ItuBoundedDispatch&)=delete;
 H8ItuBoundedDispatch& operator=(const H8ItuBoundedDispatch&)=delete;
 auto state()const{return channel_.state();}
 bool failed()const{return failed_;}
 void configure(unsigned shift,unsigned phase){mutate([&]{channel_.configure(time(),shift,phase);});}
 void set_enabled(bool value){mutate([&]{channel_.set_enabled(time(),value);});}
 void write_count(uint16_t value){mutate([&]{channel_.write_count(time(),value);});}
 void write_compare(unsigned index,uint16_t value){mutate([&]{channel_.write_compare(time(),index,value);});}
 void write_enables(uint8_t value){mutate([&]{channel_.write_enables(time(),value);});}
 void write_status(uint8_t value){mutate([&]{channel_.write_status(time(),value);});}
 void set_clear_index(unsigned index)requires requires(Channel& c){c.set_clear_index(0,index);}{mutate([&]{channel_.set_clear_index(time(),index);});}
 template<class Sink>std::size_t run_until(uint64_t target,std::size_t budget,Sink&& sink){
  require_live();if(target<time()||!budget)throw std::invalid_argument("Invalid bounded ITU dispatch request");
  Guard guard(*this);std::size_t used=0;
  try{
   while(time()<target){
    std::optional<H8ItuDeadline> next;
    // An unrepresentable future event cannot occur before a representable target.
    try{next=next_itu_event(channel_.state());}catch(const std::overflow_error&){break;}
    if(!next||next->time>target)break;
    if(next->time<=time())throw std::logic_error("Nonprogressing ITU deadline");
    if(used==budget)throw std::runtime_error("ITU dispatch event limit");
    channel_.advance(next->time);++used;
    for(unsigned bit=0;bit<3;++bit)if(next->events&(1u<<bit)){
     sink(base_+bit,next->time);if(failed_)throw std::logic_error("Reentered ITU dispatch");
    }
   }
   channel_.advance(target);return used;
  }catch(...){failed_=true;throw;}
 }
private:
 uint64_t time()const{return channel_.state().counter.time;}
 void require_live(){if(busy_){failed_=true;throw std::logic_error("ITU dispatch reentry");}if(failed_)throw std::logic_error("Failed ITU dispatch owner");}
 struct Guard {H8ItuBoundedDispatch& owner;explicit Guard(H8ItuBoundedDispatch& o):owner(o){owner.busy_=true;}~Guard(){owner.busy_=false;}};
 template<class F>void mutate(F operation){require_live();Guard guard(*this);operation();}
 Channel channel_;unsigned base_;bool failed_=false,busy_=false;
};
}
