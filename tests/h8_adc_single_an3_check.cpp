#include "h8_adc_single_an3.h"
#include <limits>
#include <stdexcept>

using yamanix::H8AdcSingleAn3;

static void check(bool condition) {
    if (!condition)
        throw std::runtime_error("Single AN3 conversion contract failed");
}

template<class Error, class Operation>
static void rejects(Operation operation) {
    try {
        operation();
    } catch (const Error&) {
        return;
    }
    check(false);
}

int main() {
    for (uint16_t sample = 0; sample < 1024; ++sample) {
        H8AdcSingleAn3 adc;
        adc.advance_to(123);
        adc.write_control(0x23, sample);
        check(adc.deadline() == 389 && adc.control() == 0x23);
        adc.advance_to(388);
        check(adc.result_high() == 0 && adc.result_low() == 0 && adc.control() == 0x23);
        adc.write_control(0xa3, uint16_t(sample ^ 1023));
        check(adc.deadline() == 389 && adc.control() == 0x23);
        adc.advance_to(389);
        const unsigned packed = unsigned(sample) * 64;
        check(!adc.deadline() && adc.control() == 0x83 &&
              adc.result_high() == packed / 256 && adc.result_low() == packed % 256);
        adc.write_control(0x83);
        check(adc.control() == 0x83);
        adc.write_control(0x03);
        adc.write_control(0x83);
        check(adc.control() == 0x03);
    }

    for (uint64_t split = 0; split <= 267; ++split) {
        H8AdcSingleAn3 adc;
        adc.write_control(0x23, 0x2ab);
        adc.advance_to(split);
        adc.advance_to(267);
        check(adc.control() == 0x83 && adc.result_high() == 0xaa && adc.result_low() == 0xc0);
        adc.write_control(0xa3, 1);
        check(adc.control() == 0xa3 && adc.result_high() == 0xaa);
        adc.reset();
        adc.advance_to(533);
        check(!adc.deadline() && adc.control() == 0 && adc.result_high() == 0 && adc.result_low() == 0);
    }

    H8AdcSingleAn3 adc;
    rejects<std::invalid_argument>([&] { adc.write_control(0x23); });
    rejects<std::invalid_argument>([&] { adc.write_control(0x23, 1024); });
    for (uint8_t mode : {0x20, 0x2b, 0x33, 0x43, 0x63})
        rejects<std::invalid_argument>([&] { adc.write_control(mode, 0); });
    check(adc.control() == 0 && !adc.deadline());
    adc.write_control(0x23, 1);
    rejects<std::invalid_argument>([&] { adc.write_control(0x03); });
    check(adc.control() == 0x23 && adc.deadline() == 266);
    adc.reset();
    adc.advance_to(std::numeric_limits<uint64_t>::max());
    rejects<std::overflow_error>([&] { adc.write_control(0x23, 0); });
    rejects<std::invalid_argument>([&] { adc.advance_to(0); });
    check(adc.control() == 0 && !adc.deadline());
}
