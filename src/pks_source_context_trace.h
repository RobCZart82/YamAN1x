#ifndef YAMANIX_PKS_SOURCE_CONTEXT_TRACE_H
#define YAMANIX_PKS_SOURCE_CONTEXT_TRACE_H

#include <array>
#include <cstdint>

namespace yamanix {

// Bounded, read-only snapshots at previously observed PKS source/control
// words. The observer records model state only and never changes bus data.
class PksSourceContextTrace {
public:
    static constexpr uint64_t capacity = 4096;
    using Registers = std::array<uint32_t, 8>;

    uint64_t events() const { return m_events; }
    uint64_t dropped() const { return m_dropped; }

    template<class Emit>
    void observe(char op, uint32_t address, uint16_t data, uint16_t mask,
                 uint32_t pc, uint64_t time_ns, const Registers &er,
                 uint8_t ccr, Emit emit) {
        if ((op != 'R' && op != 'W') || !tracks(address))
            return;

        const uint64_t seq = m_events++;
        if (seq >= capacity) {
            ++m_dropped;
            return;
        }
        emit(seq, op, address, data, mask, pc, time_ns, er, ccr);
    }

    static bool tracks(uint32_t address) {
        switch (address) {
        case 0x22d9ee: // byte payload at 22d9ef
        case 0x234acc: // indexed source byte words
        case 0x234ace:
        case 0x234ad0:
        case 0x23f7fc: // byte control read at 23f7fd
            return true;
        default:
            return false;
        }
    }

private:
    uint64_t m_events = 0;
    uint64_t m_dropped = 0;
};

}

#endif
