// Project-owned bounded instruction-context snapshots for pinned MAME research.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace yamanix {

template<std::size_t Capacity, std::size_t WindowLength, std::size_t PreRollLength = 0>
class PksInstructionContextTrace {
    static_assert(Capacity > 0 && WindowLength > 0);
    static_assert(Capacity >= WindowLength);
public:
    static constexpr std::size_t capacity() { return Capacity; }
    static constexpr std::size_t window_length() { return WindowLength; }
    static constexpr std::size_t pre_roll_length() { return PreRollLength; }

    struct Event {
        std::uint64_t sequence = 0;
        std::uint64_t time_ns = 0;
        std::uint32_t pc = 0;
        std::uint16_t opcode = 0;
        std::array<std::uint16_t, 16> registers{};
        std::uint8_t ccr = 0;
    };

    struct Window {
        std::uint64_t trigger_sequence = 0;
        std::uint64_t trigger_time_ns = 0;
        std::uint32_t input_address = 0;
        std::uint32_t postfetch_pc = 0;
        std::uint16_t input_data = 0;
        std::uint16_t input_mask = 0;
        std::uint8_t input_ccr = 0;
        std::size_t first_event = 0;
        std::size_t pre_roll_count = 0;
        std::size_t event_count = 0;
        bool complete = false;
    };

    bool trigger(char op, std::uint32_t address, std::uint16_t data,
                 std::uint16_t mask, std::uint32_t postfetch_pc,
                 std::uint8_t ccr, std::uint64_t time_ns) {
        if (op != 'R' || active_window_ >= 0)
            return false;
        std::size_t site = 0;
        if (address == 0x2349ea && mask == 0xff00 && postfetch_pc == 0x002a08)
            site = 0;
        else if (address == 0x237204 && mask == 0x00ff && postfetch_pc == 0x001fd8)
            site = 1;
        else
            return false;
        if (site_triggers_[site] >= 2 || window_count_ >= windows_.size() ||
            pre_roll_count_ != PreRollLength ||
            event_count_ + PreRollLength + WindowLength > events_.size())
            return false;

        Window &window = windows_[window_count_];
        window.trigger_sequence = trigger_count_++;
        window.trigger_time_ns = time_ns;
        window.input_address = address;
        window.postfetch_pc = postfetch_pc;
        window.input_data = data;
        window.input_mask = mask;
        window.input_ccr = ccr;
        window.first_event = event_count_;
        window.pre_roll_count = pre_roll_count_;
        if constexpr (PreRollLength > 0) {
            for (std::size_t i = 0; i < pre_roll_count_; ++i) {
                const std::size_t index = (pre_roll_next_ + PreRollLength - pre_roll_count_ + i) % PreRollLength;
                append(pre_roll_[index]);
            }
        }
        window.event_count = window.pre_roll_count;
        ++site_triggers_[site];
        active_window_ = static_cast<int>(window_count_++);
        remaining_ = WindowLength;
        return true;
    }

    void observe(std::uint32_t pc, std::uint16_t opcode, std::uint8_t ccr,
                 const std::uint16_t *registers, std::uint64_t time_ns) {
        Event event;
        event.time_ns = time_ns;
        event.pc = pc;
        event.opcode = opcode;
        event.ccr = ccr;
        for (std::size_t i = 0; i < event.registers.size(); ++i)
            event.registers[i] = registers[i];

        if (active_window_ >= 0) {
            if (event_count_ >= events_.size()) {
                ++dropped_;
                windows_[active_window_].complete = false;
                active_window_ = -1;
                remaining_ = 0;
            } else {
                append(event);
                Window &window = windows_[active_window_];
                ++window.event_count;
                if (--remaining_ == 0) {
                    window.complete = true;
                    active_window_ = -1;
                }
            }
        }
        remember(event);
    }

    void finish() { finished_ = true; }
    std::size_t event_count() const { return event_count_; }
    std::size_t window_count() const { return window_count_; }
    std::uint64_t trigger_count() const { return trigger_count_; }
    std::uint64_t dropped() const { return dropped_; }
    bool finished() const { return finished_; }
    const Event &event(std::size_t index) const { return events_[index]; }
    const Window &window(std::size_t index) const { return windows_[index]; }

private:
    void append(const Event &source) {
        Event &event = events_[event_count_++];
        event = source;
        event.sequence = event_count_ - 1;
    }

    void remember(const Event &event) {
        if constexpr (PreRollLength > 0) {
            pre_roll_[pre_roll_next_] = event;
            pre_roll_next_ = (pre_roll_next_ + 1) % PreRollLength;
            if (pre_roll_count_ < PreRollLength)
                ++pre_roll_count_;
        }
    }

    std::array<Event, Capacity> events_{};
    std::array<Window, 4> windows_{};
    std::array<unsigned, 2> site_triggers_{};
    std::array<Event, PreRollLength> pre_roll_{};
    std::size_t event_count_ = 0;
    std::size_t window_count_ = 0;
    std::size_t pre_roll_count_ = 0;
    std::size_t pre_roll_next_ = 0;
    std::uint64_t trigger_count_ = 0;
    std::uint64_t dropped_ = 0;
    int active_window_ = -1;
    std::size_t remaining_ = 0;
    bool finished_ = false;
};

using PksInstructionContextTraceDefault = PksInstructionContextTrace<270336, 2048, 65536>;

} // namespace yamanix
