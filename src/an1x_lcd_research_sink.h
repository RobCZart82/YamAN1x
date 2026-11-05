#pragma once
#include "an1x_lcd_gpio.h"
#include "lc7985_controller.h"
#include <memory>

namespace yamanix {
// Headless research sink. No PKS,MIDI acceptance,DSP responses or audio.
// External LCD survives machine subset reset. Cold machine epochs require
// explicit cold_reset() of this sink while the machine is stopped.
class An1xLcdResearchSink {
public:
    explicit An1xLcdResearchSink(uint64_t cycles_per_us):scale_(cycles_per_us),state_(std::make_unique<State>(scale_)){}
    An1xLcdResearchSink(const An1xLcdResearchSink&)=delete;
    An1xLcdResearchSink& operator=(const An1xLcdResearchSink&)=delete;
    virtual ~An1xLcdResearchSink()=default;
    virtual void cold_reset(){auto next=std::make_unique<State>(scale_);state_.swap(next);}
    bool failed() const {return state_->failed;}
    const Lc7985Controller::State& lcd_state() const {return state_->controller.state();}
    uint8_t gpio_read(unsigned port,uint64_t cycle){
        // P9 only:the established headless MAME LCD profile leaves inputs high.
        // H8DigitalPort still merges driven output bits;other inputs stay strict.
        return access(cycle,[&]{return port==9?uint8_t{0xff}:state_->gpio.read(port);});
    }
    void gpio_write(unsigned port,uint64_t cycle,uint8_t value,uint8_t){
        access(cycle,[&]{(void)state_->gpio.write(port,cycle,value);});
    }
    void tx(unsigned,uint64_t,bool){}
    void clock(unsigned,uint64_t,bool){}
    bool dma_request(unsigned,uint64_t){return false;}
private:
    struct State {
        Lc7985Controller controller;
        An1xLcdGpio<Lc7985Controller> gpio;
        uint64_t cycle=0;bool failed=false;
        explicit State(uint64_t scale):controller(scale),gpio(controller){}
    };
protected:
    template<class F>auto access(uint64_t cycle,F operation) -> decltype(operation()){
        if(state_->failed)throw std::logic_error("Failed LCD sink epoch");
        try{
            if(cycle<state_->cycle)throw std::invalid_argument("LCD sink time cannot rewind");
            state_->cycle=cycle;return operation();
        }catch(...){state_->failed=true;throw;}
    }
private:
    uint64_t scale_;
    std::unique_ptr<State> state_;
};
}
