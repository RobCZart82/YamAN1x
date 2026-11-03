#pragma once
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace yamanix {
// Headless timing component;not LCD memory,command execution or status synthesis.
// Explicit integral cycles/us. Caller must establish the machine clock conversion.
class Lc7985BusyClock {
public:
    struct State {
        uint64_t cycle, deadline;
        bool operator==(const State&) const = default;
    };
    explicit Lc7985BusyClock(uint64_t cycles_per_us, uint64_t origin=0)
        : scale_(cycles_per_us), state_{origin,origin} {
        if(!scale_ || scale_>std::numeric_limits<uint64_t>::max()/16400)
            throw std::invalid_argument("Invalid LCD clock scale");
        reset(origin);
    }
    State snapshot() const { return state_; }
    bool busy() const { return state_.cycle<state_.deadline; }
    void advance(uint64_t cycle) { require_time(cycle);state_.cycle=cycle; }
    void reset(uint64_t cycle) {
        require_time(cycle);
        const auto end=deadline(cycle,10000);
        state_={cycle,end};
    }
    // False means busy:the controller must ignore this instruction entirely.
    // True means accepted,including the no-op00. Data accesses do not call this.
    bool instruction(uint64_t cycle,uint8_t value) {
        require_time(cycle);
        if(cycle<state_.deadline) { state_.cycle=cycle;return false; }
        const auto duration=instruction_us(value);
        const auto end=duration?deadline(cycle,duration):state_.deadline;
        state_={cycle,end};return true;
    }
    static unsigned instruction_us(uint8_t value) {
        if(value>=4) return 40;
        return value?16400:0;
    }
private:
    void require_time(uint64_t cycle) const {
        if(cycle<state_.cycle)throw std::invalid_argument("LCD time cannot rewind");
    }
    uint64_t deadline(uint64_t cycle,unsigned us) const {
        const auto duration=scale_*us;
        if(cycle>std::numeric_limits<uint64_t>::max()-duration)
            throw std::overflow_error("LCD busy deadline overflow");
        return cycle+duration;
    }
    uint64_t scale_;
    State state_;
};
}
