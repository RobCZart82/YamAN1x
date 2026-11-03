#pragma once
#include "lcd_bus.h"
#include <stdexcept>

namespace yamanix {
// Wiring adapter only. Controller owns busy state, memory and cycle conversion.
// GPIO values are resolved output/pull-up levels from H8DigitalPort callbacks.
template<class Controller> class An1xLcdGpio {
public:
    explicit An1xLcdGpio(Controller& controller): controller_(controller) {}
    bool write(unsigned port, uint64_t cycle, uint8_t value) {
        if(port==10) bus_.pa=value;
        else if(port==11) bus_.pb=value;
        else if(port==9) {
            switch(bus_.control(value)) {
            case LcdBus::Event::status_read: bus_.read_latch=controller_.status_read(cycle); break;
            case LcdBus::Event::data_read: bus_.read_latch=controller_.data_read(cycle); break;
            case LcdBus::Event::instruction_write: controller_.instruction_write(cycle,bus_.output_data()); break;
            case LcdBus::Event::data_write: controller_.data_write(cycle,bus_.output_data()); break;
            case LcdBus::Event::none: break;
            }
        } else return false;
        return true;
    }
    uint8_t read(unsigned port) const {
        if(port==10) return bus_.input_pa();
        if(port==11) return bus_.input_pb();
        throw std::logic_error("Unmodeled non-LCD GPIO input");
    }
private:
    Controller& controller_;
    LcdBus bus_;
};
}
