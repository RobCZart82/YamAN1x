#pragma once
#include "lc7985_busy_clock.h"
#include <array>

namespace yamanix {
// Headless source-compatible controller subset;no character ROM or rendering.
class Lc7985Controller {
public:
    struct State {
        std::array<uint8_t,80> ddram{};
        std::array<uint8_t,64> cgram{};
        uint8_t ddac=0,cgac=0,shift=0,function=0,display=0,entry=2;
        bool ddram_selected=false;
        bool operator==(const State&) const = default;
    };
    explicit Lc7985Controller(uint64_t cycles_per_us,uint64_t origin=0)
        : clock_(cycles_per_us,origin) { state_.ddram.fill(0x20); }
    const State& state() const { return state_; }
    Lc7985BusyClock::State timing() const { return clock_.snapshot(); }
    void reset(uint64_t cycle) {
        clock_.reset(cycle);state_=State{};state_.ddram.fill(0x20);
    }
    uint8_t status_read(uint64_t cycle) {
        clock_.advance(cycle);
        return uint8_t((state_.ddram_selected?state_.ddac:state_.cgac)|(clock_.busy()?0x80:0));
    }
    void instruction_write(uint64_t cycle,uint8_t value) {
        if(!clock_.instruction(cycle,value))return;
        if(value&0x80){state_.ddac=value&0x7f;state_.ddram_selected=true;}
        else if(value&0x40){state_.cgac=value&0x3f;state_.ddram_selected=false;}
        else if(value&0x20)state_.function=value;
        else if(value&0x10){
            state_.ddram_selected=true;
            const auto operation=(value>>2)&3;
            if(operation<2)step_address(operation==1);
            else step_shift(operation==2);
        }
        else if(value&8)state_.display=value;
        else if(value&4)state_.entry=value;
        else if(value&2){state_.ddac=0;state_.shift=0;state_.ddram_selected=true;}
        else if(value&1){state_.ddram.fill(0x20);state_.ddac=0;state_.entry|=2;}
    }
    // Original data methods are not busy-gated and do not start a busy interval.
    uint8_t data_read(uint64_t cycle) {
        const auto index=data_index();clock_.advance(cycle);
        const auto value=state_.ddram_selected?state_.ddram[index]:state_.cgram[index];
        step_address((state_.entry&2)!=0);return value;
    }
    void data_write(uint64_t cycle,uint8_t value) {
        const auto index=data_index();clock_.advance(cycle);
        if(state_.ddram_selected)state_.ddram[index]=value;else state_.cgram[index]=value;
        step_address((state_.entry&2)!=0);
        if(state_.ddram_selected&&(state_.entry&1))step_shift((state_.entry&2)!=0);
    }
private:
    unsigned data_index() const {
        if(!state_.ddram_selected)return state_.cgac;
        const unsigned index=((state_.function&8)&&state_.ddac>=64)?state_.ddac-24:state_.ddac;
        if(index>=state_.ddram.size())throw std::out_of_range("Unmodeled LCD DDRAM address");
        return index;
    }
    void step_address(bool increment) {
        if(!state_.ddram_selected){state_.cgac=uint8_t((state_.cgac+(increment?1:63))&63);return;}
        if(increment){
            if(state_.ddac==((state_.function&8)?103:79))state_.ddac=0;
            else if((state_.function&8)&&state_.ddac==39)state_.ddac=64;
            else ++state_.ddac;
        }else{
            if(!state_.ddac)state_.ddac=(state_.function&8)?103:79;
            else if((state_.function&8)&&state_.ddac==64)state_.ddac=39;
            else --state_.ddac;
        }
    }
    void step_shift(bool left){state_.shift=uint8_t((state_.shift+(left?1:79))%80);}
    Lc7985BusyClock clock_;
    State state_;
};
}
