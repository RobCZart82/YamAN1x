#include "cycle_dispatcher.h"
#include "h8_execution_runner.h"
#include <cstdio>
#include "h8h_bus_access.h"
#include <functional>
struct D {yamanix::CycleDispatcher& d;bool done=false;std::optional<uint64_t>next_event(){return done?std::nullopt:std::optional<uint64_t>{1};}void service(uint64_t){try{d.advance_to(1,1,*this);}catch(const std::logic_error&){}done=true;}};
struct R {yamanix::H8ExecutionRunner&r;bool tried=false;uint64_t update(uint64_t){if(!tried){tried=true;try{r.run(0,0,1,*this);}catch(const std::logic_error&){}}return 0;}bool has_partial(){return false;}void full(yamanix::H8ExecutionBudget&b){b.counter()=0;}void partial(yamanix::H8ExecutionBudget&){} };
struct B {std::function<void()> callback;std::function<void()> timed=[]{};unsigned reads=0;
 void bus_cycle(uint64_t){timed();}
 uint16_t read_instruction_word(uint32_t){++reads;callback();return 0;}};
int main(){
 unsigned failures=0;
 yamanix::CycleDispatcher d;D o{d};
 try{d.advance_to(1,2,o);++failures;}catch(const std::logic_error&){}
 if(!d.failed())++failures;
 try{d.advance_to(1,2,o);++failures;}catch(const std::logic_error&){}
 yamanix::H8ExecutionRunner r;R q{r};
 try{r.run(0,1,10,q);++failures;}catch(const std::logic_error&){}
 if(!r.failed())++failures;
 try{r.run(0,1,10,q);++failures;}catch(const std::logic_error&){}
 int remaining=20;B b;yamanix::H8hBusAccess<B> bus(b,remaining);
 bus.set_slice_end(20);
 b.callback=[&]{try{bus.read16i(0);}catch(const std::logic_error&){}};
 try{bus.read16i(0);++failures;}catch(const std::logic_error&){}
 if(!bus.failed()||remaining!=18)++failures;
 try{bus.read16i(0);++failures;}catch(const std::logic_error&){}
 int count=20;B timed;yamanix::H8hBusAccess<B> guarded(timed,count);guarded.set_slice_end(20);
 timed.callback=[]{};timed.timed=[&]{try{guarded.read16i(0);}catch(const std::logic_error&){}};
 try{guarded.read16i(0);++failures;}catch(const std::logic_error&){}
 if(!guarded.failed()||timed.reads!=0)++failures;
 std::printf("Swallowed reentry fail-stop:%u failures\n",failures);return failures?1:0;
}
