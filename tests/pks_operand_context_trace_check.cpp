#include "pks_operand_context_trace.h"
#include <array>
#include <cstdio>
#include <tuple>
#include <vector>

int main() {
    using Trace = yamanix::PksOperandContextTrace;
    using Registers = Trace::Registers;
    using Event = std::tuple<uint64_t, char, uint32_t, uint16_t, uint16_t,
                             uint32_t, uint64_t, Registers, uint8_t>;
    std::vector<Event> events;
    auto emit = [&](auto seq, auto op, auto address, auto data, auto mask,
                    auto pc, auto time_ns, const auto &er, auto ccr) {
        events.emplace_back(seq, op, address, data, mask, pc, time_ns, er, ccr);
    };
    int failures = 0;
    auto check = [&](bool ok, const char *message) {
        if (!ok) { std::fprintf(stderr, "%s\n", message); ++failures; }
    };

    Trace trace;
    const Registers er{0x10000000, 0x20000001, 0x30000002, 0x40000003,
                       0x50000004, 0x60000005, 0x70000006, 0x80000007};
    trace.observe('R', 0x237204, 0x1234, 0x00ff, 0x001fd8, 900, er, 0x80, emit);
    trace.observe('W', 0x2349e8, 0xabcd, 0xffff, 0x002008, 1200, er, 0x05, emit);
    check(events.size() == 2, "Selected RAM context events were not emitted");
    if (events.size() == 2) {
        check(events[0] == Event{0, 'R', 0x237204, 0x1234, 0x00ff,
                                  0x001fd8, 900, er, 0x80},
              "Read snapshot lost address, lane, PC, time, registers or CCR");
        check(events[1] == Event{1, 'W', 0x2349e8, 0xabcd, 0xffff,
                                  0x002008, 1200, er, 0x05},
              "Write snapshot or sequence order mismatch");
    }
    trace.observe('X', 0x237202, 0, 0xffff, 0, 0, er, 0, emit);
    trace.observe('R', 0x237206, 0, 0xffff, 0, 0, er, 0, emit);
    trace.observe('W', 0x2349e9, 0, 0xffff, 0, 0, er, 0, emit);
    check(trace.events() == 2, "Invalid op or unselected address was recorded");

    uint64_t emitted = 0;
    Trace bounded;
    for (uint64_t i = 0; i < Trace::capacity + 9; ++i)
        bounded.observe('W', 0x237202, 0, 0xffff, 0, i, er, 0,
                        [&](auto...) { ++emitted; });
    check(emitted == Trace::capacity && bounded.events() == Trace::capacity + 9 &&
          bounded.dropped() == 9,
          "Capacity must be bounded and overflow counted without extra emission");

    std::printf("PKS operand CPU-context trace: %s\n", failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
