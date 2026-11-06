#pragma once
#include "an1x_memory_map.h"
#include "h8_refresh_timer.h"
#include "h8_adc_single_an3.h"
#include "h8_itu_channel_scheduler.h"
#include "h8_itu_control_decode.h"
#include "h83002_syscr_modes.h"
#include "h8_digital_port.h"
#include "sci_cycle_timeline.h"
#include <utility>
#include <array>
#include <type_traits>

namespace yamanix {
// Project-owned bounded composition;docs/an1x-sci-bus.md.
// Sink must outlive this nonmovable host;callbacks must not reenter or mutate it.
// Sink:tx(channel,cycle,level),clock(channel,cycle,level),dma_request(vector,cycle).
// GPIO access requires gpio_read(port,cycle)/gpio_write(port,cycle,data,direction).
// Optional adc_an3_sample(cycle) enables the bounded AN3 converter.
// Optional exception_hook(vector). No real-time threading/reentry support.
// MEG writes require both pure meg_write_enabled() and meg_write_byte(address,cycle,value).
// MEG has only explicit byte routing. VOP3 requires pure vop3_word_enabled()
// plus vop3_read_word/write_word(address,cycle[,value]);never byte/instruction fallback.
template<class Sink>class An1xSciBus {
public:
    using RxEvent=SciCycleTimeline::RxEvent;
    An1xSciBus(std::span<const uint8_t> rom,std::span<const RxEvent> input,Sink& sink,
              std::size_t event_limit=65536)
        :memory_(rom),sci_(irq_),input_(input.begin(),input.end()),timeline_(sci_,input_),sink_(sink),limit_(event_limit) {
        reset_stopped_itu_control();
    }
    An1xSciBus(const An1xSciBus&)=delete;
    An1xSciBus& operator=(const An1xSciBus&)=delete;
    H8hIrqController& irq(){return irq_;}
    uint8_t syscr_value()const{return syscr_;}
    std::optional<H8ItuControlProfile> stopped_itu_profile(unsigned channel) const {
        if (!decode_itu_control(itu_control_.at(channel)))
            return std::nullopt;
        const auto state = itu_.state(channel);
        return H8ItuControlProfile{state.counter.shift, state.counter.phase,
                                  static_cast<H8ItuAutoClear::ClearMode>(state.clear_index)};
    }
    void sync_cpu_ccr(uint8_t ccr){cpu_ccr_=ccr;irq_.set_filter(H83002SyscrModes::irq_filter(syscr_,ccr),-1);}
    bool failed()const{return failed_;}
    // Machine owner propagates fail-stop after rejected callback reentry.
    // Does not recover or otherwise mutate peripheral state.
    void poison_from_owner(){failed_=true;}
    uint64_t cycle()const{return timeline_.cycle();}
    std::size_t consumed_input()const{return timeline_.consumed_input();}
    std::vector<uint8_t> nvram_image()const{return memory_.nvram_image();}
    void load_nvram(std::span<const uint8_t> image){access([&]{memory_.load_nvram(image);});}
    // Stopped-owner policy for the supported subset only. No CPU reset or time advance.
    // Keep memory,physical input levels and pending RX schedule at their absolute times.
    void reset_supported_peripherals(){access([&]{
        write_itu_start(0xe0);
        reset_stopped_itu_control();
        reset_stopped_itu_registers();toer_=0xff;tmdr_=0x80;adc_.reset();refresh_.reset();abwcr_=astcr_=0;wcr_=0xf4;wcer_=0xff;syscr_=H83002SyscrModes::cold_value;irq_.reset();sync_cpu_ccr(cpu_ccr_);
        reset_port(4,port4_);reset_port(6,port6_);reset_port(7,port7_);
        reset_port(8,port8_);reset_port(9,port9_);reset_port(10,porta_);reset_port(11,portb_);
        sci_.reset(*this);
    });}
    uint8_t peek_sci(uint32_t a){return access([&]{return sci_.read8(a&0xffffff,false);});}
    uint64_t update(uint64_t cycle){return access([&]{advance(cycle);return deadline();});}
    void bus_cycle(uint64_t cycle){access([&]{advance(cycle);now_=timeline_.cycle();});}
    uint64_t next_bus_event()const{return deadline();}
    // Pure selected profile;only mapped external ROM/SRAM use ASTCR/WCR/WCER.
    int access_cycles(uint32_t raw,unsigned width)const{
        const auto region=An1xMemoryMap::region(raw);
        if(region==An1xMemoryMap::Region::rom)return external_access_cycles(0);
        if(region==An1xMemoryMap::Region::nvram)return external_access_cycles(1);
        if((raw&0xffffff)>=0x400000&&(raw&0xffffff)<=0x40003f){
            if constexpr(requires(const Sink& s){s.meg_write_enabled();})
                if(std::as_const(sink_).meg_write_enabled())return external_access_cycles(2);
        }
        if(width==2&&vop3_window(raw&0xffffff)){
            if constexpr(requires(const Sink& s){s.vop3_word_enabled();})
                if(std::as_const(sink_).vop3_word_enabled())return external_access_cycles(2);
        }
        return 2;
    }
    uint16_t read_instruction_word(uint32_t a){return access([&]{return memory_.read_instruction_word(a);});}
    // RFSHCR stays02;refresh pins/memory mux are not modeled.Timer IRQ enable rejects.
    // BRCR accepts only the retained FE profile;physical bus release/pin mux is not modeled.
    uint8_t read_byte(uint32_t raw){return access([&]{const auto a=raw&0xffffff;return a==0xffffec?abwcr_:a==0xffffed?astcr_:a==0xffffee?wcr_:a==0xffffef?wcer_:a==0xfffff2?syscr_:a==0xfffff3?uint8_t{0xfe}:a==0xffffac?uint8_t{2}:a==0xffffad?refresh_.control():a==0xffffae?refresh_.count():a==0xffffaf?refresh_.compare():a==0xffffe8?adc_.control():a==0xffff60?itu_start_:a==0xffff61?uint8_t{0xe0}:a==0xffff62?tmdr_:a==0xffff63?uint8_t{0xc0}:a==0xffff90?toer_:itu_tier_index(a)>=0?itu_.state(itu_tier_index(a)).enables:itu_tier_index(a-1)>=0?itu_.state(itu_tier_index(a-1)).status:itu_io_mapped(a)?uint8_t{0x88}:itu_control_index(a)>=0?itu_control_[itu_control_index(a)]:itu_counter_index(a&~1u)>=0?itu_counter_byte(a):itu_general_index(a&~1u)>=0?itu_general_byte(a):H83002SciMmio::mapped(a)?sci_.read8(a):irq_mapped(a)?read_irq(a):gpio_mapped(a)?read_gpio(a):read_external_byte(a);});}
    void write_byte(uint32_t raw,uint8_t v){access([&]{const auto a=raw&0xffffff;if(a==0xffffec){if(v&3)throw std::logic_error("Unsupported ABWCR 8-bit ROM/SRAM area");abwcr_=v;}else if(a==0xffffed){astcr_=v;}else if(a==0xffffee){if(v&8)throw std::logic_error("Unsupported external WAIT pin mode");wcr_=uint8_t(v|0xf0);}else if(a==0xffffef){wcer_=v;}else if(a==0xfffff2){if(v!=1&&v!=9)throw std::logic_error("Unsupported SYSCR mode bits");syscr_=v;sync_cpu_ccr(cpu_ccr_);}else if(a==0xfffff3){if((v&0xe1)!=0xe0)throw std::logic_error("Unsupported BRCR bus release or address pin mux");}else if(a==0xffffac){if((v&0xfd)!=0)throw std::logic_error("Unsupported refresh control mode");}else if(a==0xffffad){if(v&0x40)throw std::logic_error("Unsupported refresh timer IRQ enable");refresh_.write_control(v);}else if(a==0xffffae){refresh_.write_count(v);}else if(a==0xffffaf){refresh_.write_compare(v);}else if(a==0xffffe8){write_adc_control(v);}else if(a==0xffff60){write_itu_start(v);}else if(a==0xffff61){if(v&0x1f)throw std::logic_error("Unsupported ITU channel synchronization");}else if(a==0xffff62){if(v&0x5f)throw std::logic_error("Unsupported ITU PWM/phase mode");tmdr_=uint8_t(v|0x80);}else if(a==0xffff63){if(v&0x3f)throw std::logic_error("Unsupported ITU PWM/buffer function");}else if(a==0xffff90){if(v&0x3f)throw std::logic_error("Unsupported ITU output enable");toer_=0xc0;}else if(itu_tier_index(a)>=0){write_itu_enables(itu_tier_index(a),v);}else if(itu_tier_index(a-1)>=0){settle_itu();itu_.write_status(itu_tier_index(a-1),v);}else if(itu_io_mapped(a)){if(v&0x77)throw std::logic_error("Unsupported ITU output/capture function");}else if(itu_control_index(a)>=0){write_itu_control(itu_control_index(a), v);}else if(itu_counter_index(a&~1u)>=0){write_itu_counter_byte(a,v);}else if(itu_general_index(a&~1u)>=0){write_itu_general_byte(a,v);}else if(H83002SciMmio::mapped(a))sci_.write8(a,v,now_,*this);else if(irq_mapped(a))write_irq(a,v);else if(gpio_mapped(a))write_gpio(a,v);else if(a>=0x400000&&a<=0x40003f){
            if constexpr(requires{sink_.meg_write_byte(a,now_,v);std::as_const(sink_).meg_write_enabled();}){
                if(std::as_const(sink_).meg_write_enabled())sink_.meg_write_byte(a,now_,v);else memory_.write_byte(a,v);
            }
            else memory_.write_byte(a,v);
        }else memory_.write_byte(a,v);});}
    uint16_t read_word(uint32_t raw){return access([&]{const auto a=raw&0xffffff;
        if constexpr (requires { sink_.adc_an3_sample(now_); }) {
            if (a == 0xffffe6)
                return uint16_t((uint16_t(adc_.result_high()) << 8) | adc_.result_low());
        }
        if (const auto channel = itu_counter_index(a & ~1u); channel >= 0) {
            if (a & 1) throw std::invalid_argument("Unaligned ITU counter word");
            return itu_.state(channel).counter.count;
        }
        if(itu_general_index(a&~1u)>=0){if(a&1)throw std::invalid_argument("Unaligned ITU general word");return itu_general_word(itu_general_index(a));}if(H83002SciMmio::mapped(a))return sci_.read16(a);
        if(irq_mapped(a)||irq_mapped((a+1)&0xffffff)){
            require_irq_word(a);const auto high=read_irq(a);const auto low=read_irq(a+1);
            return uint16_t((uint16_t(high)<<8)|low);
        }
        if(gpio_mapped(a)||gpio_mapped((a+1)&0xffffff)){
            require_gpio_word(a,false);const auto high=read_gpio(a);
            if(failed_)throw std::logic_error("Reentered GPIO word read");
            const auto low=read_gpio(a+1);return uint16_t((uint16_t(high)<<8)|low);
        }return read_external_word(a);});}
    void write_word(uint32_t raw,uint16_t v){access([&]{const auto a=raw&0xffffff;
        if (const auto channel = itu_counter_index(a & ~1u); channel >= 0) {
            if (a & 1) throw std::invalid_argument("Unaligned ITU counter word");
            settle_itu();
            itu_.write_count(channel, v);
            return;
        }
        if(itu_general_index(a&~1u)>=0){if(a&1)throw std::invalid_argument("Unaligned ITU general word");write_itu_general_word(itu_general_index(a),v);}else if(H83002SciMmio::mapped(a))sci_.write16(a,v,0xffff,now_,*this);else if(irq_mapped(a)||irq_mapped((a+1)&0xffffff)){
            require_irq_word(a);write_irq(a,uint8_t(v>>8));write_irq(a+1,uint8_t(v));
        }else if(gpio_mapped(a)||gpio_mapped((a+1)&0xffffff)){
            require_gpio_word(a,true);write_gpio(a,uint8_t(v>>8));
            if(failed_)throw std::logic_error("Reentered GPIO word write");
            write_gpio(a+1,uint8_t(v));
        }else write_external_word(a,v);});}
    void exception_hook(int v){access([&]{if constexpr(requires{sink_.exception_hook(v);})sink_.exception_hook(v);});}
    // Timed timeline sink and untimed MMIO sink,sharing one outward clock.
    void tx(unsigned c,uint64_t at,bool v){sink_.tx(c,at,v);require_callback_live();}
    void clock(unsigned c,uint64_t at,bool v){sink_.clock(c,at,v);require_callback_live();}
    bool dma_request(unsigned v,uint64_t at){return sink_.dma_request(v,at);}
    void tx(unsigned c,bool v){tx(c,now_,v);}
    void clock(unsigned c,bool v){clock(c,now_,v);}
    bool dma_request(unsigned v){return dma_request(v,now_);}
private:
    friend class SciCycleTimeline;
    // SCI wins ties, followed by ITU and then ADC.
    std::optional<uint64_t> next_itu_event() const {
        return itu_start_ & 0x1f ? itu_.next_event() : std::nullopt;
    }
    std::optional<uint64_t> next_peer_event() const {
        const auto timer = next_itu_event();
        const auto adc = adc_.deadline();
        return adc && (!timer || *adc < *timer) ? adc : timer;
    }
    void service_peer(uint64_t cycle) {
        if (next_itu_event() == cycle)
            settle_itu_at(cycle);
        else
            adc_.advance_to(cycle);
    }
    void write_adc_control(uint8_t value) {
        adc_.advance_to(now_);
        if constexpr (requires { sink_.adc_an3_sample(now_); }) {
            if ((value & 0x7f) == 0x23 && !adc_.deadline()) {
                const auto sample = sink_.adc_an3_sample(now_);
                require_callback_live();
                adc_.write_control(value, sample);
                return;
            }
        } else if (value & 0x60) {
            throw std::logic_error("Unsupported ADC conversion or IRQ enable");
        }
        adc_.write_control(value);
    }
    static bool vop3_window(uint32_t a){return (a>=0x440000&&a<0x440100)||(a>=0x480000&&a<0x480100);}
    uint16_t read_external_word(uint32_t a){
        if(vop3_window(a)){
            if constexpr(requires{std::as_const(sink_).vop3_word_enabled();sink_.vop3_read_word(a,now_);})
                if(std::as_const(sink_).vop3_word_enabled())return sink_.vop3_read_word(a,now_);
        }
        return memory_.read_word(a);
    }
    void write_external_word(uint32_t a,uint16_t value){
        if(vop3_window(a)){
            if constexpr(requires{std::as_const(sink_).vop3_word_enabled();sink_.vop3_write_word(a,now_,value);}){
                if(std::as_const(sink_).vop3_word_enabled()){sink_.vop3_write_word(a,now_,value);return;}
            }
        }
        memory_.write_word(a,value);
    }
    uint8_t read_external_byte(uint32_t a){
        if constexpr (requires { sink_.adc_an3_sample(now_); }) {
            if (a == 0xffffe6) return adc_.result_high();
            if (a == 0xffffe7) return adc_.result_low();
        }
        if (a == 0x680000) {
            if constexpr (requires { std::as_const(sink_).idle_panel_read_enabled(); })
                if (std::as_const(sink_).idle_panel_read_enabled()) return 0xff;
        }
        if(a>=0x400000&&a<=0x40003f){
            if constexpr(requires{std::as_const(sink_).meg_read_enabled();sink_.meg_read_byte(a,now_);})
                if(std::as_const(sink_).meg_read_enabled())return sink_.meg_read_byte(a,now_);
        }
        return memory_.read_byte(a);
    }
    // TIER and TSR use scheduler state. Running channels publish enabled timer IRQs.
    static bool itu_io_mapped(uint32_t a){return a==0xffff65||a==0xffff6f||a==0xffff79||a==0xffff83||a==0xffff93;}
    inline static constexpr std::array<uint32_t,5> itu_tier_addresses_={0xffff66,0xffff70,0xffff7a,0xffff84,0xffff94};
    static int itu_tier_index(uint32_t a){for(unsigned i=0;i<itu_tier_addresses_.size();++i)if(itu_tier_addresses_[i]==a)return int(i);return -1;}
    inline static constexpr std::array<uint32_t,5> itu_control_addresses_={0xffff64,0xffff6e,0xffff78,0xffff82,0xffff92};
    static int itu_control_index(uint32_t a){for(unsigned i=0;i<itu_control_addresses_.size();++i)if(itu_control_addresses_[i]==a)return int(i);return -1;}
    // The AN1x diagnostic supplies no PKSCLK edges to channel 3 input B.
    // Keep TSTR active while excluding the channel from the internal clock.
    static bool itu_no_edge_clock(unsigned channel, uint8_t control) {
        return channel == 3 && (control & 0x67) == 5;
    }
    void write_itu_control(unsigned channel, uint8_t value) {
        if (const auto profile = decode_itu_control(value)) {
            settle_itu();
            itu_.configure(channel, profile->shift, profile->phase);
            itu_.set_clear_mode(channel, profile->clear);
            if (itu_no_edge_clock(channel, itu_control_[channel]))
                itu_.set_enabled(channel, (itu_start_ & (1u << channel)) != 0);
        } else if (itu_no_edge_clock(channel, value)) {
            settle_itu();
            itu_.set_enabled(channel, false);
        } else if (itu_start_ & (1u << channel)) {
            throw std::logic_error("Unsupported active ITU clock or clear mode");
        }
        // Retain legacy stopped readback even for profiles that cannot run.
        itu_control_[channel] = uint8_t(value | 0x80);
    }
    void reset_stopped_itu_control() {
        for (unsigned channel = 0; channel < itu_control_.size(); ++channel)
            write_itu_control(channel, 0x80);
    }
    std::array<uint8_t, 5> itu_control_{};
    H8ItuChannelScheduler<H8ItuAutoClear> itu_;
    inline static constexpr std::array<uint32_t,10> itu_general_addresses_={0xffff6a,0xffff6c,0xffff74,0xffff76,0xffff7e,0xffff80,0xffff88,0xffff8a,0xffff98,0xffff9a};
    static int itu_general_index(uint32_t a){for(unsigned i=0;i<itu_general_addresses_.size();++i)if(itu_general_addresses_[i]==a)return int(i);return -1;}
    void write_itu_start(uint8_t value) {
        for (unsigned channel = 0; channel < itu_control_.size(); ++channel) {
            if (!(value & (1u << channel))) continue;
            if (!decode_itu_control(itu_control_[channel]) && !itu_no_edge_clock(channel, itu_control_[channel]))
                throw std::logic_error("Unsupported active ITU clock or clear mode");
        }
        settle_itu();
        for (unsigned channel = 0; channel < itu_control_.size(); ++channel)
            itu_.set_enabled(channel, (value & (1u << channel)) != 0 &&
                                      !itu_no_edge_clock(channel, itu_control_[channel]));
        itu_start_ = value;
    }
    void write_itu_enables(unsigned channel, uint8_t value) {
        settle_itu();
        itu_.write_enables(channel, value);
    }
    uint8_t itu_start_ = 0xe0;
    void settle_itu() { settle_itu_at(now_); }
    void settle_itu_at(uint64_t cycle) {
        if (itu_.time() == cycle) return;
        itu_.run_until(cycle, 1, [&](unsigned vector, uint64_t at) {
            const bool consumed = dma_request(vector, at);
            require_callback_live();
            irq_.internal_irq(vector, consumed);
        });
    }
    static int itu_counter_index(uint32_t address) {
        constexpr std::array<uint32_t, 5> addresses = {
            0xffff68, 0xffff72, 0xffff7c, 0xffff86, 0xffff96
        };
        for (unsigned channel = 0; channel < addresses.size(); ++channel)
            if (addresses[channel] == address) return int(channel);
        return -1;
    }
    uint8_t itu_counter_byte(uint32_t address) const {
        const auto count = itu_.state(itu_counter_index(address & ~1u)).counter.count;
        return uint8_t((address & 1) ? count : count >> 8);
    }
    void write_itu_counter_byte(uint32_t address, uint8_t value) {
        settle_itu();
        const auto channel = itu_counter_index(address & ~1u);
        const auto count = itu_.state(channel).counter.count;
        const auto merged = (address & 1) ? uint16_t((count & 0xff00) | value)
                                          : uint16_t((count & 0x00ff) | (uint16_t(value) << 8));
        itu_.write_count(channel, merged);
    }
    uint16_t itu_general_word(unsigned index) const {
        return itu_.state(index / 2).compare[index % 2];
    }
    uint8_t itu_general_byte(uint32_t address) const {
        const auto word = itu_general_word(itu_general_index(address & ~1u));
        return uint8_t((address & 1) ? word : word >> 8);
    }
    void write_itu_general_word(unsigned index, uint16_t value) {
        settle_itu();
        itu_.write_compare(index / 2, index % 2, value);
    }
    void write_itu_general_byte(uint32_t address, uint8_t value) {
        const auto index = itu_general_index(address & ~1u);
        const auto word = itu_general_word(index);
        const auto merged = (address & 1) ? uint16_t((word & 0xff00) | value)
                                          : uint16_t((word & 0x00ff) | (uint16_t(value) << 8));
        write_itu_general_word(index, merged);
    }
    void reset_stopped_itu_registers() {
        settle_itu();
        for (unsigned channel = 0; channel < itu_tier_addresses_.size(); ++channel) {
            itu_.write_enables(channel, 0);
            itu_.write_status(channel, 0);
            itu_.write_count(channel, 0);
            itu_.write_compare(channel, 0, 0xffff);
            itu_.write_compare(channel, 1, 0xffff);
        }
    }

    int external_access_cycles(unsigned area)const{
        const auto bit=uint8_t(1u<<area);
        if(!(astcr_&bit))return 2;
        // WCER0 selects pin wait0;this explicit research profile holds WAIT inactive.
        if(!(wcer_&bit)||(wcr_&4))return 3;
        return 3+int(wcr_&3);
    }
    void require_callback_live()const{if(failed_)throw std::logic_error("Reentered AN1x callback");}
    void reset_port(unsigned id,H8DigitalPort& port){port.reset([&](uint8_t data,uint8_t direction){
        if constexpr(requires{sink_.gpio_write(id,now_,data,direction);})sink_.gpio_write(id,now_,data,direction);
        else throw std::logic_error("GPIO output sink unavailable");
        require_callback_live();
    });}
    template<class F>decltype(auto) access(F&& f){
        if(busy_||failed_){failed_=true;throw std::logic_error("Unavailable AN1x SCI bus");}
        busy_=true;
        try{
            if constexpr(std::is_void_v<std::invoke_result_t<F>>){std::forward<F>(f)();if(failed_)throw std::logic_error("Reentered AN1x SCI bus");busy_=false;return;}
            else {auto value=std::forward<F>(f)();if(failed_)throw std::logic_error("Reentered AN1x SCI bus");busy_=false;return value;}
        }catch(...){failed_=true;busy_=false;throw;}
    }
    static bool irq_mapped(uint32_t a){return a==0xfffff4||a==0xfffff5||a==0xfffff6||a==0xfffff8||a==0xfffff9;}
    static void require_irq_word(uint32_t a){
        if(a&1)throw std::invalid_argument("Unaligned IRQ word");
        if(!irq_mapped(a)||!irq_mapped(a+1))throw std::out_of_range("Incomplete IRQ word");
    }
    uint8_t read_irq(uint32_t a)const{
        if(a==0xfffff4)return irq_.iscr();
        if(a==0xfffff5)return irq_.ier();
        if(a==0xfffff6)return irq_.isr();
        return irq_.icr(unsigned(a-0xfffff8));
    }
    void write_irq(uint32_t a,uint8_t value){
        if(a==0xfffff4)irq_.write_iscr(value);
        else if(a==0xfffff5)irq_.write_ier(value);
        else if(a==0xfffff6)irq_.write_isr(value);
        else irq_.write_icr(unsigned(a-0xfffff8),value);
    }
    static bool gpio_mapped(uint32_t a){return a==0xffffc5||a==0xffffc7||a==0xffffda||a==0xffffc9||a==0xffffcb||a==0xffffcd||a==0xffffcf||a==0xffffce||a==0xffffd0||a==0xffffd2||a==0xffffd1||a==0xffffd3||a==0xffffd4||a==0xffffd6;}
    static void require_gpio_word(uint32_t a,bool write){
        if(a&1)throw std::invalid_argument("Unaligned GPIO word");
        if(!gpio_mapped(a)||!gpio_mapped(a+1))throw std::out_of_range("Incomplete GPIO word");
        if(write&&(a==0xffffce||a+1==0xffffce))throw std::logic_error("Read-only GPIO word lane");
    }
    uint8_t read_gpio(uint32_t a){
        if(a==0xffffc5||a==0xffffc9||a==0xffffcd||a==0xffffd0||a==0xffffd1||a==0xffffd4)return 0xff; // H83002 DDR map uses ff_r.
        if(a==0xffffda)return port4_.pcr();
        const unsigned id=a==0xffffd6?11u:a==0xffffd3?10u:a==0xffffd2?9u:a==0xffffce?7u:a==0xffffcf?8u:a==0xffffcb?6u:4u;
        auto& port=id==11?portb_:id==10?porta_:id==9?port9_:id==7?port7_:id==8?port8_:id==6?port6_:port4_;
        return port.read_pins([&]() -> uint8_t {
            if constexpr(requires{sink_.gpio_read(id,now_);})return sink_.gpio_read(id,now_);
            else throw std::logic_error("GPIO input sink unavailable");
        });
    }
    void write_gpio(uint32_t a,uint8_t value){
        if(a==0xffffce)throw std::logic_error("Read-only AN1x port7");
        if(a==0xffffda){port4_.write_pcr(value);return;}
        const unsigned id=(a==0xffffd4||a==0xffffd6)?11u:(a==0xffffd1||a==0xffffd3)?10u:(a==0xffffd0||a==0xffffd2)?9u:(a==0xffffcd||a==0xffffcf)?8u:(a==0xffffc9||a==0xffffcb)?6u:4u;
        auto& port=id==11?portb_:id==10?porta_:id==9?port9_:id==8?port8_:id==6?port6_:port4_;
        auto output=[&](uint8_t data,uint8_t direction){
            if constexpr(requires{sink_.gpio_write(id,now_,data,direction);})sink_.gpio_write(id,now_,data,direction);
            else throw std::logic_error("GPIO output sink unavailable");
        };
        if(a==0xffffc5||a==0xffffc9||a==0xffffcd||a==0xffffd0||a==0xffffd1||a==0xffffd4)port.write_ddr(value,output);else port.write_data(value,output);
    }
    uint64_t deadline()const{
        auto next=sci_.next_event();const auto i=timeline_.consumed_input();
        if(i<input_.size()&&(!next||input_[i].cycle<next))next=input_[i].cycle;
        if (const auto peer = next_peer_event(); peer && (!next || *peer < next))
            next = *peer;
        return next;
    }
    void advance(uint64_t target){
        // A completed CPU bus access may overrun its slice. Its effects stay
        // committed while the next slice repays debt;do not rewind peripherals.
        target=std::max(target,timeline_.cycle());
        if(!timeline_.advance_to(target,limit_,*this).complete)
            throw std::runtime_error("AN1x peripheral event budget exhausted");
        now_=timeline_.cycle();
        refresh_.advance_to(now_);
        if (itu_start_ & 0x1f) settle_itu();
    }
    // ABWCR bits0/1 must stay16-bit for mapped ROM/SRAM;other areas remain unmapped.
    // Selected active refresh clock;IRQ enable remains unsupported.
    H8RefreshTimer refresh_;
    uint8_t toer_=0xff; // Cold permission bits only;no active ITU pin function exists.
    uint8_t tmdr_=0x80; // FDIR latch only. PWM and phase modes are unsupported.
    H8AdcSingleAn3 adc_;
    uint8_t wcr_=0xf4,wcer_=0xff; // No-wait research cold/reset profile,not hardware F3.
    uint8_t abwcr_=0,astcr_=0; // Explicit research reset profile,not verified board mode pins.
    uint8_t syscr_=H83002SyscrModes::cold_value,cpu_ccr_=0x80;
    An1xMemoryMap memory_;
    H8DigitalPort port4_;
    H8DigitalPort port6_{0x80,0x80};
    H8DigitalPort port8_{0xf0,0xe0};
    H8DigitalPort port7_;
    H8DigitalPort port9_{0x00,0xc0};
    H8DigitalPort porta_;
    H8DigitalPort portb_;
    H8hIrqController irq_;
    H83002SciMmio sci_;
    const std::vector<RxEvent> input_;
    SciCycleTimeline timeline_;
    Sink& sink_;
    std::size_t limit_;
    uint64_t now_=0;
    bool busy_=false,failed_=false;
};
} // namespace yamanix
