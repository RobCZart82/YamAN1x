#include "h8_adc_slow_scan.h"
#include <array>

using yamanix::H8AdcSlowScan;

static void check(bool condition) {
    if (!condition)
        throw std::runtime_error("Slow ADC scan contract failed");
}

int main() {
    constexpr std::array<uint64_t, 8> edges{306, 562, 818, 1074, 1330, 1586, 1842, 2098};
    constexpr std::array<unsigned, 8> channels{0, 1, 2, 3, 0, 1, 2, 3};
    constexpr std::array<uint16_t, 9> samples{1, 257, 513, 1023, 5, 21, 42, 128, 0};
    for (uint64_t cut = 40; cut <= edges.back(); ++cut) {
        H8AdcSlowScan adc;
        adc.start(40, samples[0]);
        adc.write_adf(true);
        check(adc.control() == 0x33);
        std::array<uint16_t, 4> expected{};
        unsigned event = 0;
        for (uint64_t horizon : {cut, edges.back()}) {
            while (adc.deadline() && *adc.deadline() <= horizon) {
                check(event < edges.size() && adc.deadline() == edges[event]);
                check(adc.next_channel() == (event + 1) % 4);
                adc.service(edges[event], samples[event + 1]);
                expected[channels[event]] = uint16_t(samples[event] * 64);
                for (unsigned channel = 0; channel < expected.size(); ++channel)
                    check(adc.result_word(channel) == expected[channel]);
                check(adc.control() == (event == 3 || event == 7 ? 0xb3 : 0x33));
                if (event == 3 || event == 7) {
                    adc.write_adf(true);
                    check(adc.control() == 0xb3);
                    adc.write_adf(false);
                    check(adc.control() == 0x33);
                }
                ++event;
            }
        }
        check(event == edges.size() && adc.deadline() == 2354);
        adc.reset();
        check(!adc.deadline() && adc.control() == 0 && adc.result_word(3) == 0);
    }
    for (uint64_t cut = 0; cut <= 1034; ++cut) {
        H8AdcSlowScan scan;
        scan.start(0, 42);
        scan.service(266, 43);
        scan.clear_start(false);
        scan.restart(300, 4, 100, false);
        check(scan.control() == 0x37 && scan.deadline() == 566 &&
              scan.result_word(0) == 42 * 64 && scan.next_channel() == 5);
        unsigned event = 0;
        constexpr std::array<uint64_t,4> upper_edges{566,822,1078,1334};
        for (uint64_t horizon : {300 + cut, uint64_t(1334)}) {
            while (*scan.deadline() <= horizon) {
                check(event < 4 && scan.deadline() == upper_edges[event]);
                check(scan.next_channel() == (event == 3 ? 4 : event + 5));
                scan.service(upper_edges[event], uint16_t(101 + event));
                check(scan.result_word(event) == (100 + event) * 64);
                check(scan.control() == (event == 3 ? 0xb7 : 0x37));
                ++event;
            }
        }
        check(event == 4 && scan.deadline() == 1590);
        scan.clear_start(true);
        scan.restart(1400, 0, 7, true);
        check(scan.control() == 0xb3 && scan.deadline() == 1666 &&
              scan.result_word(3) == 103 * 64 && scan.next_channel() == 1);
    }
    // A failed stale restart must preserve the pending sample,flags,bank and time.
    for (unsigned bank : {0u, 4u}) for (uint64_t stale = 0; stale < 522; ++stale) {
        H8AdcSlowScan scan;
        scan.start(0, 1);scan.service(266, 2);scan.service(522, 3);
        scan.clear_start(false);
        bool rejected = false;
        try { scan.restart(stale, bank, 99, true); }
        catch (const std::invalid_argument&) { rejected = true; }
        check(rejected && scan.deadline() == 778 && scan.control() == 0x13 &&
              scan.next_channel() == 3 && scan.result_word(0) == 64 &&
              scan.result_word(1) == 128 && scan.result_word(2) == 0);
        scan.service(778, 4);
        check(scan.result_word(2) == 192 && scan.deadline() == 1034);
        scan.restart(778, bank, 7, false); // Equal settled timestamp remains valid.
        check(scan.deadline() == 1044 && scan.control() == (0x33 | bank));
        scan.clear_start(false);
        rejected = false;
        try { scan.restart(777, bank, 8, false); }
        catch (const std::invalid_argument&) { rejected = true; }
        check(rejected && scan.deadline() == 1044);
        scan.reset();scan.start(0, 5); // Explicit reset permits a fresh zero-time epoch.
        check(scan.deadline() == 266 && scan.control() == 0x33);
    }
    // Invalid sample/overflow must not move the accepted-time watermark.
    {
        H8AdcSlowScan scan;scan.start(0,1);scan.service(266,2);scan.clear_start(false);
        bool bad=false;try {scan.restart(1000,4,1024,false);}
        catch(const std::invalid_argument&) {bad=true;}check(bad);
        bad=false;try {scan.restart(std::numeric_limits<uint64_t>::max(),4,3,false);}
        catch(const std::overflow_error&) {bad=true;}check(bad);
        scan.restart(266,4,3,false);check(scan.deadline()==532);
    }
    H8AdcSlowScan adc;
    adc.start(40, 1);
    for (auto cycle : {305, 307}) {
        bool rejected = false;
        try { adc.service(cycle, 2); } catch (const std::invalid_argument&) { rejected = true; }
        check(rejected && adc.deadline() == 306 && adc.result_word(0) == 0);
    }
    bool rejected = false;
    try { adc.service(306, 1024); } catch (const std::invalid_argument&) { rejected = true; }
    check(rejected && adc.deadline() == 306 && adc.result_word(0) == 0);
}
