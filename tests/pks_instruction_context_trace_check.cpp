#include "pks_instruction_context_trace.h"

#include <array>
#include <cstdlib>

using Trace = yamanix::PksInstructionContextTrace<8, 2>;

static void check(bool value) { if (!value) std::abort(); }

int main() {
    Trace trace;
    std::array<std::uint16_t, 16> registers{};
    check(!trace.trigger('W', 0x2349ea, 0x4000, 0xff00, 0x2a08, 0x84, 10));
    check(!trace.trigger('R', 0x2349ea, 0x4000, 0x00ff, 0x2a08, 0x84, 10));
    check(!trace.trigger('R', 0x2349e8, 0x4000, 0xff00, 0x2a08, 0x84, 10));
    check(trace.trigger('R', 0x2349ea, 0x4000, 0xff00, 0x2a08, 0x84, 10));
    registers[1] = 0x2240;
    trace.observe(0x2a08, 0x6910, 0x84, registers.data(), 11);
    registers[6] = 0x2240;
    trace.observe(0x2a0e, 0x0c16, 0x84, registers.data(), 12);
    check(trace.window_count() == 1 && trace.event_count() == 2);
    check(trace.window(0).input_data == 0x4000 && trace.window(0).input_mask == 0xff00);
    check(trace.window(0).input_ccr == 0x84 && trace.window(0).trigger_time_ns == 10);
    check(trace.event(1).pc == 0x2a0e && trace.event(1).registers[6] == 0x2240);
    check(trace.window(0).complete);

    check(trace.trigger('R', 0x237204, 0xff00, 0x00ff, 0x1fd8, 0x80, 20));
    trace.observe(0x1fde, 0x0c21, 0x80, registers.data(), 21);
    trace.observe(0x1fe0, 0x7b18, 0x84, registers.data(), 22);
    check(trace.window_count() == 2 && trace.window(1).complete);
    check(trace.window(1).input_address == 0x237204 && trace.window(1).postfetch_pc == 0x1fd8);

    using SmallTrace = yamanix::PksInstructionContextTrace<4, 2>;
    SmallTrace small;
    check(small.trigger('R', 0x2349ea, 0x4000, 0xff00, 0x2a08, 0x84, 30));
    small.observe(0x2a08, 0x6910, 0x84, registers.data(), 31);
    small.observe(0x2a0e, 0x0c16, 0x84, registers.data(), 32);
    check(small.trigger('R', 0x237204, 0xff00, 0x00ff, 0x1fd8, 0x80, 40));
    small.observe(0x1fde, 0x0c21, 0x80, registers.data(), 41);
    small.observe(0x1fe0, 0x7b18, 0x84, registers.data(), 42);
    check(!small.trigger('R', 0x2349ea, 0x4100, 0xff00, 0x2a08, 0x85, 50));
    check(small.dropped() == 0 && small.window_count() == 2);
    small.finish();
    check(small.finished());

    using PartialTrace = yamanix::PksInstructionContextTrace<2, 2>;
    PartialTrace partial;
    check(partial.trigger('R', 0x2349ea, 0x4000, 0xff00, 0x2a08, 0x84, 60));
    partial.observe(0x2a08, 0x6910, 0x84, registers.data(), 61);
    partial.finish();
    check(!partial.window(0).complete && partial.window(0).event_count == 1);

    using PreRollTrace = yamanix::PksInstructionContextTrace<8, 2, 2>;
    PreRollTrace pre_roll;
    pre_roll.observe(0x1000, 0x1111, 0x80, registers.data(), 1);
    pre_roll.observe(0x1002, 0x2222, 0x81, registers.data(), 2);
    pre_roll.observe(0x1004, 0x3333, 0x82, registers.data(), 3);
    check(pre_roll.trigger('R', 0x2349ea, 0x4000, 0xff00, 0x2a08, 0x84, 4));
    pre_roll.observe(0x2a08, 0x6910, 0x84, registers.data(), 5);
    pre_roll.observe(0x2a0e, 0x0c16, 0x84, registers.data(), 6);
    const auto &first = pre_roll.window(0);
    check(first.pre_roll_count == 2 && first.event_count == 4 && first.complete);
    check(pre_roll.event(first.first_event).pc == 0x1002);
    check(pre_roll.event(first.first_event).ccr == 0x81);
    check(pre_roll.event(first.first_event + 1).pc == 0x1004);
    check(pre_roll.event(first.first_event + 2).pc == 0x2a08);
    check(pre_roll.event(first.first_event + 3).pc == 0x2a0e);
    check(pre_roll.event(first.first_event).sequence + 1 ==
          pre_roll.event(first.first_event + 1).sequence);

    pre_roll.observe(0x1fd2, 0x6a27, 0x80, registers.data(), 7);
    pre_roll.observe(0x1fd4, 0x0000, 0x80, registers.data(), 8);
    check(pre_roll.trigger('R', 0x237204, 0xff00, 0x00ff, 0x1fd8, 0x80, 9));
    pre_roll.observe(0x1fde, 0x0c21, 0x80, registers.data(), 10);
    pre_roll.observe(0x1fe0, 0x7b18, 0x84, registers.data(), 11);
    const auto &second = pre_roll.window(1);
    check(second.pre_roll_count == 2 && second.event_count == 4 && second.complete);
    check(pre_roll.event(second.first_event).pc == 0x1fd2);
    check(pre_roll.event(second.first_event + 1).pc == 0x1fd4);
    check(pre_roll.event(second.first_event + 2).pc == 0x1fde);

    using ShortPreRollTrace = yamanix::PksInstructionContextTrace<3, 2, 2>;
    ShortPreRollTrace short_pre_roll;
    short_pre_roll.observe(0x1000, 0x1111, 0x80, registers.data(), 1);
    check(!short_pre_roll.trigger('R', 0x2349ea, 0x4000, 0xff00, 0x2a08, 0x84, 2));
    check(short_pre_roll.window_count() == 0 && short_pre_roll.event_count() == 0);
}
