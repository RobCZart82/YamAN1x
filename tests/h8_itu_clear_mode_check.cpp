#include "h8_itu_auto_clear.h"
#include "h8_itu_channel_scheduler.h"
#include <cstdio>
#include <vector>
#include <utility>
using Channel = yamanix::H8ItuAutoClear;
using Mode = Channel::ClearMode;
void check(bool value) {
    if (!value) throw std::runtime_error("ITU clear-mode contract failed");
}
int main() {
    unsigned cases = 0;
    for (unsigned initial = 0; initial < 65536; ++initial) {
        Channel channel;
        channel.set_clear_mode(0, Mode::free_running);
        channel.write_compare(0, 0, 6);
        channel.write_compare(0, 1, 16);
        channel.write_count(0, uint16_t(initial));
        channel.set_enabled(0, true);
        unsigned count = initial;
        unsigned flags = 0;
        for (unsigned tick = 1; tick <= 32; ++tick) {
            count = (count + 1) % 65536;
            if (count == 7) flags |= 1;
            if (count == 17) flags |= 2;
            if (count == 0) flags |= 4;
        }
        channel.advance(32);
        check(channel.state().counter.count == count && (channel.state().status & 7) == flags);
        ++cases;
    }
    for (unsigned cut = 0; cut <= 64; ++cut) {
        Channel whole, split;
        for (auto* channel : {&whole, &split}) {
            channel->write_compare(0, 0, 6);
            channel->write_compare(0, 1, 10);
            channel->set_enabled(0, true);
            channel->advance(7);
            check(channel->state().counter.count == 0);
            channel->set_clear_mode(7, Mode::free_running);
        }
        whole.advance(71);
        split.advance(7 + cut);
        split.advance(71);
        check(whole.state() == split.state());
        whole.set_clear_mode(71, Mode::compare_b);
        check(whole.state().counter.count == 64 && whole.state().counter.cycle == 11);
        whole.write_count(71, 0);
        whole.advance(82);
        check(whole.state().counter.count == 0);
        const auto before = whole.state();
        bool rejected = false;
        try { whole.set_clear_mode(83, static_cast<Mode>(3)); }
        catch (const std::out_of_range&) { rejected = true; }
        check(rejected && whole.state() == before);
    }
    for (unsigned before = 0; before < 3; ++before) {
        for (unsigned after = 0; after < 3; ++after) {
            for (unsigned sample = 0; sample < 256; ++sample) {
                Channel channel;
                channel.write_compare(0, 0, 6);
                channel.write_compare(0, 1, 10);
                channel.write_enables(0, 7);
                channel.set_clear_mode(0, static_cast<Mode>(before));
                channel.write_count(0, uint16_t(sample * 257));
                channel.set_enabled(0, true);
                unsigned count = sample * 257;
                unsigned flags = 0;
                for (unsigned tick = 1; tick <= 32; ++tick) {
                    const unsigned mode = tick <= 16 ? before : after;
                    const unsigned cycle = mode == 0 ? 7 : mode == 1 ? 11 : 65536;
                    const unsigned raw = count + 1;
                    count = raw == cycle || raw == 65536 ? 0 : raw;
                    if (count == 7 || (raw == 7 && raw == cycle)) flags |= 1;
                    if (count == 11 || (raw == 11 && raw == cycle)) flags |= 2;
                    if (raw == 65536) flags |= 4;
                }
                channel.set_clear_mode(16, static_cast<Mode>(after));
                channel.advance(32);
                check(channel.state().counter.count == count);
                check((channel.state().status & 7) == flags && channel.pending() == flags);
            }
        }
    }
    using Scheduler = yamanix::H8ItuChannelScheduler<Channel>;
    Scheduler scheduler;
    scheduler.set_clear_mode(0, Mode::free_running);
    scheduler.set_clear_mode(1, Mode::compare_a);
    scheduler.set_clear_mode(2, Mode::compare_b);
    for (unsigned channel = 0; channel < 3; ++channel) {
        scheduler.write_compare(channel, 0, 2);
        scheduler.write_compare(channel, 1, 4);
        scheduler.write_enables(channel, 3);
        scheduler.set_enabled(channel, true);
    }
    std::vector<std::pair<unsigned, uint64_t>> events;
    scheduler.run_until(10, 10, [&](unsigned vector, uint64_t cycle) { events.emplace_back(vector, cycle); });
    const std::vector<std::pair<unsigned, uint64_t>> expected = {
        {24,3},{28,3},{32,3},{25,5},{33,5},{28,6},{32,8},{28,9},{33,10}
    };
    check(events == expected);
    check(scheduler.state(0).counter.count == 10);
    check(scheduler.state(1).counter.count == 1);
    check(scheduler.state(2).counter.count == 0);
    std::printf("ITU clear modes:%u tick oracles,65 switch/split cases,mixed bank PASS\n",cases);
}
