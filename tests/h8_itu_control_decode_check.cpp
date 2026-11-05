#include "h8_itu_control_decode.h"
#include <cstdio>
#include <stdexcept>
int main() {
    unsigned accepted = 0;
    for (unsigned value = 0; value < 256; ++value) {
        const auto profile = yamanix::decode_itu_control(uint8_t(value));
        const unsigned clock = value % 8;
        const unsigned clear = (value / 32) % 4;
        const bool supported = clock < 4 && clear < 3;
        if (bool(profile) != supported)
            throw std::runtime_error("ITU control support mismatch");
        if (!profile) continue;
        ++accepted;
        const unsigned modes[] = {2,0,1};
        if (profile->shift != clock || profile->phase != 0 ||
            unsigned(profile->clear) != modes[clear])
            throw std::runtime_error("ITU control value mismatch");
        yamanix::H8ItuAutoClear channel;
        channel.configure(0, profile->shift, profile->phase);
        channel.write_compare(0, 0, 6);
        channel.write_compare(0, 1, 10);
        channel.set_clear_mode(0, profile->clear);
        channel.set_enabled(0, true);
        channel.advance(128);
        const unsigned cycles[] = {65536,7,11};
        if (channel.state().counter.count != (128u / (1u << clock)) % cycles[clear])
            throw std::runtime_error("ITU decoded counter mismatch");
    }
    if (accepted != 96) throw std::runtime_error("ITU support count mismatch");
    std::puts("ITU TCR:256 encodings,96 supported source profiles and counter probes PASS");
}
