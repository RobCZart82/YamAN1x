#include "h8_itu_channel_scheduler.h"
#include "h8_itu_free_running.h"
#include "h8_itu_auto_clear.h"
#include <algorithm>
#include <cstdio>
#include <utility>
#include <vector>

using Scheduler = yamanix::H8ItuChannelScheduler<yamanix::H8ItuFreeRunning>;
using Events = std::vector<std::pair<unsigned, uint64_t>>;
void check(bool value) {
    if (!value) throw std::runtime_error("ITU channel scheduler contract failed");
}
void setup(Scheduler& scheduler) {
    for (unsigned channel = 0; channel < 5; ++channel) {
        scheduler.write_compare(channel, 0, uint16_t(9 - channel));
        scheduler.write_compare(channel, 1, uint16_t(19 - channel));
        scheduler.write_enables(channel, 3);
        scheduler.set_enabled(channel, true);
    }
}
int main() {
    Events expected;
    for (unsigned channel = 0; channel < 5; ++channel) {
        expected.emplace_back(24 + 4 * channel, 10 - channel);
        expected.emplace_back(25 + 4 * channel, 20 - channel);
    }
    std::sort(expected.begin(), expected.end(), [](auto a, auto b) { return a.second < b.second; });
    for (unsigned cut = 0; cut <= 32; ++cut) {
        Scheduler scheduler;
        setup(scheduler);
        Events events;
        auto record = [&](unsigned vector, uint64_t cycle) { check(scheduler.time() == cycle); events.emplace_back(vector, cycle); };
        scheduler.run_until(cut, 20, record);
        scheduler.run_until(32, 20, record);
        check(events == expected && scheduler.time() == 32);
        for (unsigned channel = 0; channel < 5; ++channel)
            check(scheduler.state(channel).counter.count == 32);
    }
    {
        Scheduler scheduler;
        for (unsigned channel = 0; channel < 5; ++channel) {
            scheduler.write_count(channel, 65535);
            scheduler.write_enables(channel, 7);
            scheduler.set_enabled(channel, true);
        }
        Events events;
        check(scheduler.run_until(1, 1, [&](unsigned v, uint64_t t) { events.emplace_back(v, t); }) == 1);
        Events simultaneous;
        for (unsigned channel = 0; channel < 5; ++channel)
            for (unsigned event = 0; event < 3; ++event)
                simultaneous.emplace_back(24 + 4 * channel + event, 1);
        check(events == simultaneous);
    }
    {
        Scheduler scheduler;
        setup(scheduler);
        unsigned calls = 0;
        bool rejected = false;
        try { scheduler.run_until(32, 1, [&](unsigned, uint64_t) { ++calls; }); }
        catch (const std::runtime_error&) { rejected = true; }
        check(rejected && scheduler.failed() && calls == 1 && scheduler.time() == 6);
        try { scheduler.run_until(33, 20, [&](unsigned, uint64_t) { ++calls; }); }
        catch (const std::logic_error&) {}
        check(calls == 1);
    }
    {
        Scheduler scheduler;
        setup(scheduler);
        bool rejected = false;
        try {
            scheduler.run_until(32, 20, [&](unsigned, uint64_t) {
                try { scheduler.write_count(0, 0); } catch (const std::logic_error&) {}
            });
        } catch (const std::logic_error&) { rejected = true; }
        check(rejected && scheduler.failed());
    }
    {
        Scheduler scheduler;
        setup(scheduler);
        scheduler.run_until(5, 20, [](unsigned, uint64_t) {});
        scheduler.set_enabled(4, false);
        scheduler.write_compare(0, 0, 5);
        Events events;
        scheduler.run_until(10, 20, [&](unsigned v, uint64_t t) { events.emplace_back(v, t); });
        check(events == Events{{24, 6}, {36, 7}, {32, 8}, {28, 9}});
        check(scheduler.state(4).counter.count == 5);
        bool rejected = false;
        try { scheduler.write_count(5, 0); } catch (const std::out_of_range&) { rejected = true; }
        check(rejected && !scheduler.failed() && scheduler.time() == 10);
        rejected = false;
        try { scheduler.run_until(9, 20, [](unsigned, uint64_t) {}); }
        catch (const std::invalid_argument&) { rejected = true; }
        check(rejected && !scheduler.failed() && scheduler.time() == 10);
    }
    {
        yamanix::H8ItuChannelScheduler<yamanix::H8ItuAutoClear> scheduler;
        scheduler.write_compare(0, 0, 2);
        scheduler.write_enables(0, 1);
        scheduler.set_enabled(0, true);
        Events events;
        scheduler.run_until(10, 3, [&](unsigned v, uint64_t t) { events.emplace_back(v, t); });
        check(events == Events{{24, 3}, {24, 6}, {24, 9}});
        check(scheduler.state(0).counter.count == 1);
    }
    {
        Scheduler scheduler;
        setup(scheduler);
        bool rejected = false;
        try { scheduler.run_until(10, 0, [](unsigned, uint64_t) {}); }
        catch (const std::invalid_argument&) { rejected = true; }
        check(rejected && !scheduler.failed() && scheduler.time() == 0);
        try { scheduler.run_until(10, 20, [](unsigned, uint64_t) { throw std::runtime_error("sink"); }); }
        catch (const std::runtime_error&) {}
        check(scheduler.failed() && scheduler.time() == 6);
    }
    std::puts("ITU five-channel chronology,33 splits,15 simultaneous vectors,budget/reentry/reconfigure PASS");
}
