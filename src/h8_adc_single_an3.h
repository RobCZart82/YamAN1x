#pragma once
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>

namespace yamanix {

// Standalone H8_ADC_3337 subset for the observed slow AN3 conversion.
class H8AdcSingleAn3 {
public:
    uint8_t control() const { return control_; }
    uint8_t result_high() const { return uint8_t(result_ >> 2); }
    uint8_t result_low() const { return uint8_t(result_ << 6); }
    std::optional<uint64_t> deadline() const { return deadline_; }

    void advance_to(uint64_t cycle) {
        if (cycle < cycle_)
            throw std::invalid_argument("ADC time moves backwards");
        if (deadline_ && *deadline_ <= cycle) {
            result_ = pending_sample_;
            control_ = uint8_t((control_ & ~0x20) | 0x80);
            deadline_.reset();
        }
        cycle_ = cycle;
    }

    void write_control(uint8_t value, std::optional<uint16_t> sample = std::nullopt) {
        const uint8_t mode = value & 0x7f;
        const bool start = mode == 0x23 && !deadline_;
        if ((deadline_ && mode != 0x23) || ((mode & 0x60) && mode != 0x23))
            throw std::invalid_argument("Unsupported ADC control mode");
        if (start) {
            if (!sample || *sample > 1023)
                throw std::invalid_argument("ADC requires a 10-bit AN3 sample");
            if (cycle_ > std::numeric_limits<uint64_t>::max() - 266)
                throw std::overflow_error("ADC deadline exceeds timeline");
            pending_sample_ = *sample;
            deadline_ = cycle_ + 266;
        }
        control_ = uint8_t(mode | (control_ & value & 0x80));
    }

    void reset() {
        deadline_.reset();
        control_ = 0;
        result_ = 0;
    }

private:
    uint64_t cycle_ = 0;
    std::optional<uint64_t> deadline_;
    uint16_t pending_sample_ = 0;
    uint16_t result_ = 0;
    uint8_t control_ = 0;
};

}
