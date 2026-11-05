#pragma once
#include "h8_itu_auto_clear.h"
#include <optional>

namespace yamanix {
struct H8ItuControlProfile {
    unsigned shift;
    unsigned phase;
    H8ItuAutoClear::ClearMode clear;
    bool operator==(const H8ItuControlProfile&) const = default;
};

// Bounded H8H source profile. The locked source compares count_type (0..3)
// against enum DIV_2 (6), so every internal clock selects phase zero.
inline std::optional<H8ItuControlProfile> decode_itu_control(uint8_t value) {
    const unsigned clock = value & 7;
    const unsigned clear = (value >> 5) & 3;
    if (clock >= 4 || clear == 3)
        return std::nullopt;
    using Mode = H8ItuAutoClear::ClearMode;
    const auto mode = clear == 0 ? Mode::free_running :
                      clear == 1 ? Mode::compare_a : Mode::compare_b;
    return H8ItuControlProfile{clock, 0, mode};
}
}
