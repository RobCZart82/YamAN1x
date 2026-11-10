#include "pks_source_context_trace.h"
#include <cstdio>
#include <tuple>
#include <vector>

int main() {
    using Trace = yamanix::PksSourceContextTrace;
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
    trace.observe('W', 0x22d9ee, 0x00e0, 0x00ff, 0x0057d16, 900, er, 0x80, emit);
    trace.observe('R', 0x234acc, 0x00e0, 0x00ff, 0x0057d42, 1200, er, 0x05, emit);
    trace.observe('R', 0x234ace, 0x0020, 0x00ff, 0x0057d42, 1300, er, 0x05, emit);
    trace.observe('R', 0x234ad0, 0x00e0, 0x00ff, 0x0057d42, 1400, er, 0x05, emit);
    trace.observe('R', 0x23f7fc, 0x0001, 0x00ff, 0x0023f7fd, 1500, er, 0x01, emit);
    check(events.size() == 5, "Selected source/control context events were not emitted");
    if (events.size() == 5) {
        check(events[0] == Event{0, 'W', 0x22d9ee, 0x00e0, 0x00ff,
                                  0x0057d16, 900, er, 0x80},
              "Payload snapshot lost lane, PC, time, registers or CCR");
        check(events[1] == Event{1, 'R', 0x234acc, 0x00e0, 0x00ff,
                                  0x0057d42, 1200, er, 0x05},
              "Source snapshot fields mismatch");
        check(events[4] == Event{4, 'R', 0x23f7fc, 0x0001, 0x00ff,
                                  0x0023f7fd, 1500, er, 0x01},
              "Control snapshot fields mismatch");
    }

    trace.observe('X', 0x234acc, 0, 0xffff, 0, 0, er, 0, emit);
    trace.observe('R', 0x22d9f0, 0, 0xffff, 0, 0, er, 0, emit);
    trace.observe('W', 0x234acd, 0, 0xffff, 0, 0, er, 0, emit);
    check(trace.events() == 5, "Invalid op or unselected word address was recorded");
    check(Trace::tracks(0x234ace) && !Trace::tracks(0x234acd),
          "Word-aligned capture boundary changed");

    uint64_t emitted = 0;
    Trace bounded;
    for (uint64_t i = 0; i < Trace::capacity + 9; ++i)
        bounded.observe('W', 0x22d9ee, 0, 0xffff, 0, i, er, 0,
                        [&](auto...) { ++emitted; });
    check(emitted == Trace::capacity && bounded.events() == Trace::capacity + 9 &&
          bounded.dropped() == 9,
          "Capacity must be bounded and overflow counted without extra emission");

    std::printf("PKS source CPU-context trace: %s\n", failures ? "FAIL" : "PASS");
    return failures ? 1 : 0;
}
