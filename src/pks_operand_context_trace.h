#ifndef YAMANIX_PKS_OPERAND_CONTEXT_TRACE_H
#define YAMANIX_PKS_OPERAND_CONTEXT_TRACE_H

#include <array>
#include <cstdint>

namespace yamanix {

// Bounded, read-only CPU snapshots around observed RAM word bases and one
// statically identified caller input word. This records observations only.
class PksOperandContextTrace {
public:
    static constexpr uint64_t capacity = 4096;
    using Registers = std::array<uint32_t, 8>;

    uint64_t events() const { return m_events; }
    uint64_t dropped() const { return m_dropped; }

    template<class Emit>
    void observe(char op, uint32_t address, uint16_t data, uint16_t mask,
                 uint32_t pc, uint64_t time_ns, const Registers &er,
                 uint8_t ccr, Emit emit) {
        if ((op != 'R' && op != 'W') ||
            (address != 0x237202 && address != 0x237204 &&
             address != 0x2349e8 && address != 0x2349ea))
            return;

        const uint64_t seq = m_events++;
        if (seq >= capacity) {
            ++m_dropped;
            return;
        }
        emit(seq, op, address, data, mask, pc, time_ns, er, ccr);
    }

private:
    uint64_t m_events = 0;
    uint64_t m_dropped = 0;
};

}

#endif
