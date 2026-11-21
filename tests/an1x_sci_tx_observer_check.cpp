#define YAMANIX_AN1X_BOOT_PROBE_OBSERVER_TEST
#include "../src/an1x_boot_probe.cpp"

#include <cstdio>

static unsigned failures;
static void check(bool ok, const char* message) {
    if (!ok && ++failures < 12) std::fprintf(stderr, "%s\n", message);
}

int main() {
    using Observer = An1xSciTxObserver;
    Observer observer;
    observer.tx(0, 0, true);
    observer.tx(0, 4, true);
    observer.tx(0, 9, false);
    observer.tx(1, 11, false);
    observer.tx(1, 12, true);
    const auto events = observer.retained();
    check(events.size() == 4 && observer.dropped() == 0,
          "Observer retains per-channel first levels and only transitions");
    if (events.size() == 4) {
        check(events[0] == Observer::Event{0, 0, true, true} &&
              events[1] == Observer::Event{0, 9, false, false} &&
              events[2] == Observer::Event{1, 11, false, true} &&
              events[3] == Observer::Event{1, 12, true, false},
              "Observer preserves exact channel, cycle, level and first-observation marker");
    }

    Observer bounded;
    for (std::size_t i = 0; i < Observer::capacity + 3; ++i)
        bounded.tx(0, i, (i & 1) != 0);
    check(bounded.retained().size() == Observer::capacity && bounded.dropped() == 3,
          "Observer stops at its fixed capacity and reports every dropped transition");
    check(bounded.retained().back() ==
              Observer::Event{0, Observer::capacity - 1,
                              ((Observer::capacity - 1) & 1) != 0, false},
          "Observer retains the last event within its capacity");
    bool invalid_channel_rejected = false;
    try { bounded.tx(2, 0, false); }
    catch (const std::out_of_range&) { invalid_channel_rejected = true; }
    check(invalid_channel_rejected && bounded.dropped() == 3,
          "Invalid channel fails closed without changing overflow accounting");

    An1xSciWriteObserver writes;
    writes.write(0xffffaf, 0xaa, 1);
    writes.write(0xffffb6, 0xbb, 2);
    writes.write(0xffffb7, 0xcc, 3);
    writes.write(0xffffbe, 0xdd, 4);
    writes.write(0x12ffffb0, 0x80, 19);
    writes.write(0xffffbd, 0x23, 21);
    check(writes.retained().size()==2 && writes.dropped()==0,
          "SCI writes exclude neighboring and gap addresses");
    if(writes.retained().size()==2)
      check(writes.retained()[0]==An1xSciWriteObserver::Event{0xffffb0,0x80,19} &&
            writes.retained()[1]==An1xSciWriteObserver::Event{0xffffbd,0x23,21},
            "SCI writes preserve normalized address, value and exact cycle");
    for(unsigned i=0;i<257;++i)writes.write(0xffffb3,0x55,30+i);
    check(writes.retained().size()==256 && writes.dropped()==3,
          "SCI write storage is bounded and counts every lost record");
    std::printf("AN1x SCI TX observer: exact line changes, per-channel baseline and %zu-event bound: %u failures\n",
                Observer::capacity, failures);
    return failures ? 1 : 0;
}
