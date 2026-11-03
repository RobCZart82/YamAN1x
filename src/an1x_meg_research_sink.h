#pragma once
#include "an1x_lcd_research_sink.h"
#include "meg_host_write_adapter.h"
namespace yamanix {
// Explicit observational source profile,not a functioning effect processor.
class An1xMegResearchSink:public An1xLcdResearchSink {
public:
 explicit An1xMegResearchSink(uint64_t scale):An1xLcdResearchSink(scale),meg_(MegHostWriteAdapter::ControlPolicy::observe_original_stubs){}
 bool meg_write_enabled()const{return true;}
 void meg_write_byte(uint32_t address,uint64_t cycle,uint8_t value){
  access(cycle,[&]{meg_.write_byte(address,value);});
 }
 const MegRegisterStorage::State& meg_state()const{return meg_.state();}
 MegHostWriteAdapter::Observation meg_observation()const{return meg_.observation();}
 void cold_reset(){
  MegHostWriteAdapter next(MegHostWriteAdapter::ControlPolicy::observe_original_stubs);
  An1xLcdResearchSink::cold_reset();meg_=next;
 }
private:
 MegHostWriteAdapter meg_;
};
}
