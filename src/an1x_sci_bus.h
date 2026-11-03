#pragma once
#include "an1x_memory_map.h"
#include "h8_refresh_timer.h"
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
// Optional exception_hook(vector). No real-time threading/reentry support.
// MEG writes require both pure meg_write_enabled() and meg_write_byte(address,cycle,value).
// Only canonical byte window is forwarded;no read/word/instruction fallback.
template<class Sink>class An1xSciBus {
public:
    using RxEvent=SciCycleTimeline::RxEvent;
    An1xSciBus(std::span<const uint8_t> rom,std::span<const RxEvent> input,Sink& sink,
              std::size_t event_limit=65536)
        :memory_(rom),sci_(irq_),input_(input.begin(),input.end()),timeline_(sci_,input_),sink_(sink),limit_(event_limit){}
    An1xSciBus(const An1xSciBus&)=delete;
    An1xSciBus& operator=(const An1xSciBus&)=delete;
    H8hIrqController& irq(){return irq_;}
    uint8_t syscr_value()const{return syscr_;}
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
        itu_tier_.fill(0xf8);itu_control_.fill(0x80);itu_general_.fill(0xffff);toer_=0xff;tmdr_=0x80;adcsr_=0;refresh_.reset();abwcr_=astcr_=0;wcr_=0xf4;wcer_=0xff;syscr_=H83002SyscrModes::cold_value;irq_.reset();sync_cpu_ccr(cpu_ccr_);
        reset_port(4,port4_);reset_port(6,port6_);reset_port(7,port7_);
        reset_port(8,port8_);reset_port(9,port9_);reset_port(10,porta_);reset_port(11,portb_);
        sci_.reset(*this);
    });}
    uint8_t peek_sci(uint32_t a){return access([&]{return sci_.read8(a&0xffffff,false);});}
    uint64_t update(uint64_t cycle){return access([&]{advance(cycle);return deadline();});}
    void bus_cycle(uint64_t cycle){access([&]{advance(cycle);now_=timeline_.cycle();});}
    uint64_t next_bus_event()const{return deadline();}
    // Pure selected profile;only mapped external ROM/SRAM use ASTCR/WCR/WCER.
    int access_cycles(uint32_t raw,unsigned)const{
        const auto region=An1xMemoryMap::region(raw);
        if(region==An1xMemoryMap::Region::rom)return external_access_cycles(0);
        if(region==An1xMemoryMap::Region::nvram)return external_access_cycles(1);
        if((raw&0xffffff)>=0x400000&&(raw&0xffffff)<=0x40003f){
            if constexpr(requires(const Sink& s){s.meg_write_enabled();})
                if(std::as_const(sink_).meg_write_enabled())return external_access_cycles(2);
        }
        return 2;
    }
    uint16_t read_instruction_word(uint32_t a){return access([&]{return memory_.read_instruction_word(a);});}
    // RFSHCR stays02;refresh pins/memory mux are not modeled.Timer IRQ enable rejects.
    // BRCR accepts only the retained FE profile;physical bus release/pin mux is not modeled.
    uint8_t read_byte(uint32_t raw){return access([&]{const auto a=raw&0xffffff;return a==0xffffec?abwcr_:a==0xffffed?astcr_:a==0xffffee?wcr_:a==0xffffef?wcer_:a==0xfffff2?syscr_:a==0xfffff3?uint8_t{0xfe}:a==0xffffac?uint8_t{2}:a==0xffffad?refresh_.control():a==0xffffae?refresh_.count():a==0xffffaf?refresh_.compare():a==0xffffe8?adcsr_:a==0xffff61?uint8_t{0xe0}:a==0xffff62?tmdr_:a==0xffff63?uint8_t{0xc0}:a==0xffff90?toer_:itu_tier_index(a)>=0?itu_tier_[itu_tier_index(a)]:itu_irq_register(a)==1?uint8_t{0xf8}:itu_io_mapped(a)?uint8_t{0x88}:itu_control_index(a)>=0?itu_control_[itu_control_index(a)]:itu_general_index(a&~1u)>=0?itu_general_byte(a):H83002SciMmio::mapped(a)?sci_.read8(a):irq_mapped(a)?read_irq(a):gpio_mapped(a)?read_gpio(a):read_external_byte(a);});}
    void write_byte(uint32_t raw,uint8_t v){access([&]{const auto a=raw&0xffffff;if(a==0xffffec){if(v&3)throw std::logic_error("Unsupported ABWCR 8-bit ROM/SRAM area");abwcr_=v;}else if(a==0xffffed){astcr_=v;}else if(a==0xffffee){if(v&8)throw std::logic_error("Unsupported external WAIT pin mode");wcr_=uint8_t(v|0xf0);}else if(a==0xffffef){wcer_=v;}else if(a==0xfffff2){if(v!=1&&v!=9)throw std::logic_error("Unsupported SYSCR mode bits");syscr_=v;sync_cpu_ccr(cpu_ccr_);}else if(a==0xfffff3){if((v&0xe1)!=0xe0)throw std::logic_error("Unsupported BRCR bus release or address pin mux");}else if(a==0xffffac){if((v&0xfd)!=0)throw std::logic_error("Unsupported refresh control mode");}else if(a==0xffffad){if(v&0x40)throw std::logic_error("Unsupported refresh timer IRQ enable");refresh_.write_control(v);}else if(a==0xffffae){refresh_.write_count(v);}else if(a==0xffffaf){refresh_.write_compare(v);}else if(a==0xffffe8){if(v&0x60)throw std::logic_error("Unsupported ADC conversion or IRQ enable");adcsr_=uint8_t(v&0x1f);}else if(a==0xffff61){if(v&0x1f)throw std::logic_error("Unsupported ITU channel synchronization");}else if(a==0xffff62){if(v&0x5f)throw std::logic_error("Unsupported ITU PWM/phase mode");tmdr_=uint8_t(v|0x80);}else if(a==0xffff63){if(v&0x3f)throw std::logic_error("Unsupported ITU PWM/buffer function");}else if(a==0xffff90){if(v&0x3f)throw std::logic_error("Unsupported ITU output enable");toer_=0xc0;}else if(itu_tier_index(a)>=0){itu_tier_[itu_tier_index(a)]=uint8_t(v|0xf8);}else if(itu_irq_register(a)==1){}else if(itu_io_mapped(a)){if(v&0x77)throw std::logic_error("Unsupported ITU output/capture function");}else if(itu_control_index(a)>=0){itu_control_[itu_control_index(a)]=uint8_t(v|0x80);}else if(itu_general_index(a&~1u)>=0){const auto i=itu_general_index(a&~1u);auto &word=itu_general_[i];word=(a&1)?uint16_t((word&0xff00)|v):uint16_t((word&0xff)|(uint16_t(v)<<8));}else if(H83002SciMmio::mapped(a))sci_.write8(a,v,now_,*this);else if(irq_mapped(a))write_irq(a,v);else if(gpio_mapped(a))write_gpio(a,v);else if(a>=0x400000&&a<=0x40003f){
            if constexpr(requires{sink_.meg_write_byte(a,now_,v);std::as_const(sink_).meg_write_enabled();}){
                if(std::as_const(sink_).meg_write_enabled())sink_.meg_write_byte(a,now_,v);else memory_.write_byte(a,v);
            }
            else memory_.write_byte(a,v);
        }else memory_.write_byte(a,v);});}
    uint16_t read_word(uint32_t raw){return access([&]{const auto a=raw&0xffffff;if(itu_general_index(a&~1u)>=0){if(a&1)throw std::invalid_argument("Unaligned ITU general word");return itu_general_[itu_general_index(a)];}if(H83002SciMmio::mapped(a))return sci_.read16(a);
        if(irq_mapped(a)||irq_mapped((a+1)&0xffffff)){
            require_irq_word(a);const auto high=read_irq(a);const auto low=read_irq(a+1);
            return uint16_t((uint16_t(high)<<8)|low);
        }
        if(gpio_mapped(a)||gpio_mapped((a+1)&0xffffff)){
            require_gpio_word(a,false);const auto high=read_gpio(a);
            if(failed_)throw std::logic_error("Reentered GPIO word read");
            const auto low=read_gpio(a+1);return uint16_t((uint16_t(high)<<8)|low);
        }return memory_.read_word(a);});}
    void write_word(uint32_t raw,uint16_t v){access([&]{const auto a=raw&0xffffff;if(itu_general_index(a&~1u)>=0){if(a&1)throw std::invalid_argument("Unaligned ITU general word");itu_general_[itu_general_index(a)]=v;}else if(H83002SciMmio::mapped(a))sci_.write16(a,v,0xffff,now_,*this);else if(irq_mapped(a)||irq_mapped((a+1)&0xffffff)){
            require_irq_word(a);write_irq(a,uint8_t(v>>8));write_irq(a+1,uint8_t(v));
        }else if(gpio_mapped(a)||gpio_mapped((a+1)&0xffffff)){
            require_gpio_word(a,true);write_gpio(a,uint8_t(v>>8));
            if(failed_)throw std::logic_error("Reentered GPIO word write");
            write_gpio(a+1,uint8_t(v));
        }else memory_.write_word(a,v);});}
    void exception_hook(int v){access([&]{if constexpr(requires{sink_.exception_hook(v);})sink_.exception_hook(v);});}
    // Timed timeline sink and untimed MMIO sink,sharing one outward clock.
    void tx(unsigned c,uint64_t at,bool v){sink_.tx(c,at,v);require_callback_live();}
    void clock(unsigned c,uint64_t at,bool v){sink_.clock(c,at,v);require_callback_live();}
    bool dma_request(unsigned v,uint64_t at){return sink_.dma_request(v,at);}
    void tx(unsigned c,bool v){tx(c,now_,v);}
    void clock(unsigned c,bool v){clock(c,now_,v);}
    bool dma_request(unsigned v){return dma_request(v,now_);}
private:
    uint8_t read_external_byte(uint32_t a){
        if(a>=0x400000&&a<=0x40003f){
            if constexpr(requires{std::as_const(sink_).meg_read_enabled();sink_.meg_read_byte(a,now_);})
                if(std::as_const(sink_).meg_read_enabled())return sink_.meg_read_byte(a,now_);
        }
        return memory_.read_byte(a);
    }
    // Stored TIER enables,clear-only stopped TSR;TSTR still rejects,so no flags/events.
    static int itu_irq_register(uint32_t a){if(a==0xffff66||a==0xffff70||a==0xffff7a||a==0xffff84||a==0xffff94)return 0;if(a==0xffff67||a==0xffff71||a==0xffff7b||a==0xffff85||a==0xffff95)return 1;return -1;}
    static bool itu_io_mapped(uint32_t a){return a==0xffff65||a==0xffff6f||a==0xffff79||a==0xffff83||a==0xffff93;}
    inline static constexpr std::array<uint32_t,5> itu_tier_addresses_={0xffff66,0xffff70,0xffff7a,0xffff84,0xffff94};
    static int itu_tier_index(uint32_t a){for(unsigned i=0;i<itu_tier_addresses_.size();++i)if(itu_tier_addresses_[i]==a)return int(i);return -1;}
    std::array<uint8_t,5> itu_tier_={0xf8,0xf8,0xf8,0xf8,0xf8};
    inline static constexpr std::array<uint32_t,5> itu_control_addresses_={0xffff64,0xffff6e,0xffff78,0xffff82,0xffff92};
    static int itu_control_index(uint32_t a){for(unsigned i=0;i<itu_control_addresses_.size();++i)if(itu_control_addresses_[i]==a)return int(i);return -1;}
    std::array<uint8_t,5> itu_control_={0x80,0x80,0x80,0x80,0x80}; // Config only;TSTR remains unsupported.
    inline static constexpr std::array<uint32_t,10> itu_general_addresses_={0xffff6a,0xffff6c,0xffff74,0xffff76,0xffff7e,0xffff80,0xffff88,0xffff8a,0xffff98,0xffff9a};
    static int itu_general_index(uint32_t a){for(unsigned i=0;i<itu_general_addresses_.size();++i)if(itu_general_addresses_[i]==a)return int(i);return -1;}
    uint8_t itu_general_byte(uint32_t a)const{auto word=itu_general_[itu_general_index(a&~1u)];return uint8_t((a&1)?word:word>>8);}
    std::array<uint16_t,10> itu_general_=[] {std::array<uint16_t,10> v;v.fill(0xffff);return v;}();

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
        return next;
    }
    void advance(uint64_t target){
        // A completed CPU bus access may overrun its slice. Its effects stay
        // committed while the next slice repays debt;do not rewind peripherals.
        target=std::max(target,timeline_.cycle());
        if(!timeline_.advance_to(target,limit_,*this).complete)
            throw std::runtime_error("AN1x SCI event budget exhausted");
        now_=timeline_.cycle();
        refresh_.advance_to(now_);
    }
    // ABWCR bits0/1 must stay16-bit for mapped ROM/SRAM;other areas remain unmapped.
    // Selected active refresh clock;IRQ enable remains unsupported.
    H8RefreshTimer refresh_;
    uint8_t toer_=0xff; // Cold permission bits only;no active ITU pin function exists.
    uint8_t tmdr_=0x80; // FDIR latch only;ITU counters/overflow flags remain unsupported.
    uint8_t adcsr_=0; // Idle ADC configuration only;no conversion or analog sample.
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
