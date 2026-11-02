// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// H8H-only bus helpers adapted from pinned MAME h8.cpp (m_has_exr=false).
// Identities and scope: research/h8h-bus-provenance.json.
#pragma once
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace yamanix {
// Bus: read_instruction_word, read_byte, write_byte, read_word, write_word.
// Bus and CPU-owned signed remaining-cycle counter must outlive this adapter.
// Optional pure access_cycles(address,width) selects2/3 cycles;default2.
// No callback reentry or counter mutation. Failure requires discarding owner.
template<class Bus> class H8hBusAccess {
public:
    H8hBusAccess(Bus& bus, int& remaining) : bus_(bus), remaining_(remaining) {}
    H8hBusAccess(const H8hBusAccess&)=delete;
    H8hBusAccess& operator=(const H8hBusAccess&)=delete;
    // CPU owner supplies the validated absolute slice end before accesses.
    void set_slice_end(uint64_t end) { slice_end_=end; timed_=true; }
    bool failed() const { return failed_; }
    uint16_t read16i(uint32_t address) {
        return access(address & ~uint32_t{1},2,[&]{return bus_.read_instruction_word(address & ~uint32_t{1});});
    }
    uint8_t read8(uint32_t address) {
        return access(address,1,[&]{return bus_.read_byte(address);});
    }
    void write8(uint32_t address,uint8_t data) {
        access(address,1,[&]{bus_.write_byte(address,data);});
    }
    uint16_t read16(uint32_t address) {
        return access(address & ~uint32_t{1},2,[&]{return bus_.read_word(address & ~uint32_t{1});});
    }
    void write16(uint32_t address,uint16_t data) {
        access(address & ~uint32_t{1},2,[&]{bus_.write_word(address & ~uint32_t{1},data);});
    }
private:
    template<class F> decltype(auto) access(uint32_t address,unsigned width,F f) {
        if(busy_) {failed_=true;throw std::logic_error("Reentrant H8H bus access");}
        if(failed_) throw std::logic_error("Failed H8H bus owner");
        int cost=2;
        if constexpr(requires(Bus& b){b.access_cycles(address,width);}) {
            busy_=true;
            try {
                struct Guard{bool&busy;~Guard(){busy=false;}}guard{busy_};
                const auto selected=bus_.access_cycles(address,width);
                if(failed_)throw std::logic_error("Reentered H8H access-cost query");
                if(selected==2)cost=2;else if(selected==3)cost=3;
                else throw std::logic_error("Unsupported H8H access cost");
            }catch(...){failed_=true;throw;}
        }
        if(remaining_<std::numeric_limits<int>::min()+cost)
            throw std::overflow_error("H8H cycle counter underflow");
        remaining_-=cost; // Charge before calling the bus, including across zero.
        busy_=true;
        try {
            // A guard also handles void operations without duplicating charge.
            struct Guard { bool& busy; ~Guard(){busy=false;} } guard{busy_};
            if constexpr(requires(Bus& b,uint64_t cycle){b.bus_cycle(cycle);}) {
                if(!timed_) throw std::logic_error("Timed bus requires a slice end");
                uint64_t cycle;
                if(remaining_>=0) {
                    if(uint64_t(remaining_)>slice_end_) throw std::overflow_error("Bus time before zero");
                    cycle=slice_end_-uint64_t(remaining_);
                } else {
                    const auto overrun=uint64_t(-int64_t(remaining_));
                    if(slice_end_>std::numeric_limits<uint64_t>::max()-overrun)
                        throw std::overflow_error("Bus time overflow");
                    cycle=slice_end_+overrun;
                }
                bus_.bus_cycle(cycle);
            }
            if(failed_)throw std::logic_error("Reentered H8H bus access");
            if constexpr(std::is_void_v<std::invoke_result_t<F>>) {
                f();if(failed_)throw std::logic_error("Reentered H8H bus access");return;
            } else {
                auto result=f();if(failed_)throw std::logic_error("Reentered H8H bus access");return result;
            }
        } catch(...) { failed_=true; throw; }
    }
    Bus& bus_;
    int& remaining_;
    uint64_t slice_end_=0;
    bool timed_=false;
    bool busy_=false, failed_=false;
};
} // namespace yamanix
