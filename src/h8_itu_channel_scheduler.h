#pragma once
#include "h8_itu_bounded_dispatch.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>

namespace yamanix {
// One shared epoch for the five channels. Equal timestamps use channel order,
// then A/B/overflow order. The budget counts shared event timestamps.
template<class Channel>
class H8ItuChannelScheduler {
public:
    auto state(unsigned channel) const { return channels_.at(channel).state(); }
    bool failed() const { return failed_; }
    uint64_t time() const { return time_; }

    std::optional<uint64_t> next_event() const {
        std::optional<uint64_t> earliest;
        for (const auto& channel : channels_) {
            try {
                const auto event = next_itu_event(channel.state());
                if (event && (!earliest || event->time < *earliest))
                    earliest = event->time;
            } catch (const std::overflow_error&) {
                // Future events outside uint64 cannot precede a valid target.
            }
        }
        return earliest;
    }

    void configure(unsigned channel, unsigned shift, unsigned phase) {
        mutate(channel, [&](auto& owner) { owner.configure(shift, phase); });
    }
    void set_enabled(unsigned channel, bool enabled) {
        mutate(channel, [&](auto& owner) { owner.set_enabled(enabled); });
    }
    void write_count(unsigned channel, uint16_t value) {
        mutate(channel, [&](auto& owner) { owner.write_count(value); });
    }
    void write_compare(unsigned channel, unsigned index, uint16_t value) {
        mutate(channel, [&](auto& owner) { owner.write_compare(index, value); });
    }
    void write_enables(unsigned channel, uint8_t value) {
        mutate(channel, [&](auto& owner) { owner.write_enables(value); });
    }
    void write_status(unsigned channel, uint8_t value) {
        mutate(channel, [&](auto& owner) { owner.write_status(value); });
    }

    template<class Sink>
    std::size_t run_until(uint64_t target, std::size_t budget, Sink&& sink) {
        require_live();
        if (target < time_ || budget == 0)
            throw std::invalid_argument("Invalid ITU scheduler request");
        Guard guard(*this);
        std::size_t used = 0;
        try {
            auto forward = [&](unsigned vector, uint64_t cycle) {
                sink(vector, cycle);
                if (failed_)
                    throw std::logic_error("Reentered ITU scheduler");
            };
            while (time_ < target) {
                const auto event = next_event();
                if (!event || *event > target)
                    break;
                if (used == budget)
                    throw std::runtime_error("ITU scheduler event limit");
                time_ = *event;
                // Each channel can have at most one event timestamp at this edge.
                for (auto& channel : channels_)
                    channel.run_until(*event, 1, forward);
                ++used;
            }
            for (auto& channel : channels_)
                channel.run_until(target, 1, forward);
            time_ = target;
            return used;
        } catch (...) {
            failed_ = true;
            throw;
        }
    }

private:
    void require_live() {
        if (busy_) {
            failed_ = true;
            throw std::logic_error("ITU scheduler reentry");
        }
        if (failed_)
            throw std::logic_error("Failed ITU scheduler");
    }
    struct Guard {
        H8ItuChannelScheduler& owner;
        explicit Guard(H8ItuChannelScheduler& value) : owner(value) { owner.busy_ = true; }
        ~Guard() { owner.busy_ = false; }
    };
    template<class Operation>
    void mutate(unsigned channel, Operation operation) {
        require_live();
        auto& owner = channels_.at(channel);
        Guard guard(*this);
        operation(owner);
    }
    std::array<H8ItuBoundedDispatch<Channel>, 5> channels_ = {
        H8ItuBoundedDispatch<Channel>(0), H8ItuBoundedDispatch<Channel>(1),
        H8ItuBoundedDispatch<Channel>(2), H8ItuBoundedDispatch<Channel>(3),
        H8ItuBoundedDispatch<Channel>(4)
    };
    uint64_t time_ = 0;
    bool failed_ = false;
    bool busy_ = false;
};
}
