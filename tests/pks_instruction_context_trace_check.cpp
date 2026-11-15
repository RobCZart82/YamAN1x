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
}
