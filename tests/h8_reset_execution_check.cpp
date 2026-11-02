#include "h8_first_instructions.h"
#include <cstdio>
#include <functional>
#include <tuple>
#include <vector>
using namespace yamanix;
static unsigned failures=0;
static void check(bool ok,const char* message){if(!ok && ++failures<8)std::fprintf(stderr,"%s\n",message);}
struct Host {
    using Event=std::tuple<char,uint64_t,uint32_t,unsigned>;
    std::vector<Event> events;std::function<void()> callback;
    // For filter events this records the preceding bus completion,not a new event timestamp.
    uint64_t now=0;bool seed_register=false;
    static constexpr uint32_t target=0x123400;
    void bus_cycle(uint64_t cycle){now=cycle;}
    uint64_t update(uint64_t){return 0;}
    uint8_t syscr_value()const{return 9;}
    void sync_cpu_ccr(uint8_t ccr){events.emplace_back('F',now,0,ccr);}
    uint16_t read_instruction_word(uint32_t address){
        if(address!=0 && address!=2 && (address<target || address>target+64))
            throw std::runtime_error("Unexpected reset fetch");
        if(callback)callback();
        const uint16_t value=address==0?0x12:address==2?0x3400:address==target&&seed_register?0xf85a:0; // NOP after vector
        events.emplace_back('R',now,address,value);return value;
    }
    uint8_t read_byte(uint32_t){throw std::runtime_error("Unexpected data read");}
    uint16_t read_word(uint32_t){throw std::runtime_error("Unexpected data read");}
    void write_byte(uint32_t,uint8_t){throw std::runtime_error("Unexpected data write");}
    void write_word(uint32_t,uint16_t){throw std::runtime_error("Unexpected data write");}
};
int main(){
    // Independent literal bus sequence; no expected values taken from CPU snapshots.
    for(uint64_t origin:{uint64_t{0},uint64_t{1000},uint64_t{1}<<40})
    for(unsigned flags=0;flags<256;++flags) {
        const auto ccr=uint8_t(flags|0x80);
        const std::vector<Host::Event> expected={{'R',origin+2,0,0x12},
            {'R',origin+4,2,0x3400},{'F',origin+4,0,ccr},
            {'R',origin+6,Host::target,0}};
        Host whole;H8FirstInstructions<Host> cpu(whole,uint8_t(flags));
        cpu.run(origin,6,100);const auto reference=cpu.snapshot();
        check(whole.events==expected,"Vector words, filter and first prefetch order/time");
        check(reference.ccr==ccr && reference.exr==0x7f,"Reset preserves all CCR bits except setting I;EXR7F");
        check(reference.pc==Host::target && reference.npc==Host::target && reference.substate==8,
              "Reset suspends after first prefetch before PC increment");
        for(unsigned cut=0;cut<=6;++cut) {
            Host split;H8FirstInstructions<Host> part(split,uint8_t(flags));
            part.run(origin,int(cut),100);
            check(part.snapshot().debt==(cut%2),"Odd reset slice carries exactly one completed bus cycle");
            const auto reads_before=split.events.size();
            part.run(origin+cut,6-int(cut),100);
            check(part.snapshot()==reference && split.events==expected,"Every reset cut resumes without repeated bus/filter effects");
            if(cut==6)check(split.events.size()==reads_before,"Zero continuation adds no effect");
            part.run(origin+6,1,100);
            check(part.snapshot().debt==1 && part.snapshot().pc==Host::target+2,
                  "Reset continuation advances PC once and next NOP incurs debt");
            check(split.events.size()==5 && split.events.back()==Host::Event{'R',origin+8,Host::target+2,0},
                  "Next instruction fetch continues absolute time without duplicated reset");
        }
    }
    // Deferred reset retains already completed bus effects and repays debt.
    for(uint64_t origin:{uint64_t{0},uint64_t{1000},uint64_t{1}<<40})
    for(unsigned cut=0;cut<=20;++cut)
    for(unsigned split=0;split<=6;++split){
        Host host;host.seed_register=true;H8FirstInstructions<Host> cpu(host,0x35);
        cpu.run(origin,int(cut),100);const auto before=cpu.snapshot();
        const auto old_events=host.events;
        cpu.request_reset_after_debt();const auto queued=cpu.snapshot();
        check(host.events==old_events && queued.registers==before.registers && queued.ccr==before.ccr && queued.debt==before.debt,
              "Request preserves registers,CCR,debt and completed external effects");
        check(queued.state==0x10000 && queued.substate==0 && queued.taken_vector==0 && queued.taken_level==-1,
              "Request replaces old partial dispatch and clears taken IRQ bookkeeping");
        cpu.request_reset_after_debt();check(cpu.snapshot()==queued,"Repeated pending request is idempotent");
        host.events.clear();const auto start=origin+cut;const auto debt=uint64_t(before.debt);
        cpu.run(start,int(split),100);cpu.run(start+split,before.debt+6-int(split),100);
        const std::vector<Host::Event> expected={{'R',start+debt+2,0,0x12},{'R',start+debt+4,2,0x3400},
            {'F',start+debt+4,0,unsigned(before.ccr|0x80)},{'R',start+debt+6,Host::target,0xf85a}};
        check(host.events==expected,"Deferred vector/filter/prefetch has literal debt-adjusted absolute times");
        check(cpu.snapshot().registers==before.registers && cpu.snapshot().ccr==uint8_t(before.ccr|0x80) && cpu.snapshot().exr==0x7f,
              "Reset retains general registers and non-I CCR while applying reset sequence");
        if(cut>=9)check(before.registers[0]!=0,"Register retention includes a nonzero instruction result");
    }
    {
        Host host;H8FirstInstructions<Host> cpu(host);bool rejected=false;
        host.callback=[&]{try{cpu.request_reset_after_debt();}catch(const std::logic_error&){rejected=true;}};
        bool failed=false;try{cpu.run(0,6,100);}catch(const std::logic_error&){failed=true;}
        check(rejected && failed && cpu.failed(),"Swallowed reset reentry poisons active execution");
        host.callback={};const auto before=cpu.snapshot();rejected=false;
        try{cpu.request_reset_after_debt();}catch(const std::logic_error&){rejected=true;}
        check(rejected && cpu.snapshot()==before,"Failed runner cannot be recovered by deferred reset");
    }
    std::printf("Reset execution:256 CCR seeds,3 time origins,7 cold cuts,441 deferred split runs,reentry guard;%u failures\n",failures);
    return failures?1:0;
}
