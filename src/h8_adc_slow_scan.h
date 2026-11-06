#pragma once
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>

namespace yamanix {

// Standalone H8_ADC_3337 subset for ADCSR 0x33. The caller supplies
// a sample at each scheduled edge and owns the event budget.
class H8AdcSlowScan {
public:
    std::optional<uint64_t> deadline() const { return deadline_; }
    unsigned next_channel() const { return (channel_ + 1) % results_.size(); }
    uint16_t result_word(unsigned channel) const { return uint16_t(results_.at(channel) << 6); }
    uint8_t control() const { return uint8_t((deadline_ ? 0x33 : 0) | (finished_ ? 0x80 : 0)); }

    void start(uint64_t cycle, uint16_t an0) {
        if (deadline_)
            throw std::logic_error("ADC scan already active");
        deadline_ = sample_deadline(cycle, 266, an0);
        pending_sample_ = an0;
    }

    void service(uint64_t cycle, uint16_t next_sample) {
        if (!deadline_ || cycle != *deadline_)
            throw std::invalid_argument("ADC scan completion outside deadline");
        const auto next = sample_deadline(cycle, 256, next_sample);
        results_[channel_] = pending_sample_;
        finished_ |= channel_ == 3;
        channel_ = next_channel();
        pending_sample_ = next_sample;
        deadline_ = next;
    }

    void write_adf(bool keep) { finished_ &= keep; }
    void reset() { *this = {}; }

private:
    static uint64_t sample_deadline(uint64_t cycle, unsigned delay, uint16_t sample) {
        if (sample > 1023)
            throw std::invalid_argument("ADC scan sample exceeds ten bits");
        if (cycle > std::numeric_limits<uint64_t>::max() - delay)
            throw std::overflow_error("ADC scan deadline exceeds timeline");
        return cycle + delay;
    }

    std::array<uint16_t, 4> results_{};
    std::optional<uint64_t> deadline_;
    uint16_t pending_sample_ = 0;
    unsigned channel_ = 0;
    bool finished_ = false;
};

}
