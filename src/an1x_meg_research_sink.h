#pragma once
#include "an1x_lcd_research_sink.h"
#include "meg_host_write_adapter.h"
#include "meg_source_status.h"
namespace yamanix {
// Explicit observational source profile,not a functioning effect processor.
class An1xMegResearchSink:public An1xLcdResearchSink {
public:
 explicit An1xMegResearchSink(uint64_t scale,MegSourceStatus::Policy status= MegSourceStatus::Policy::reject):An1xLcdResearchSink(scale),meg_(MegHostWriteAdapter::ControlPolicy::observe_original_stubs),status_(status){}
 bool meg_read_enabled()const{return status_.enabled();}
 uint64_t meg_status_reads()const{return status_.reads();}
 uint8_t meg_read_byte(uint32_t address,uint64_t cycle){return access(cycle,[&]{return status_.read_byte(address);});}
 bool meg_write_enabled()const{return true;}
 void meg_write_byte(uint32_t address,uint64_t cycle,uint8_t value){
  access(cycle,[&]{meg_.write_byte(address,value);});
 }
 const MegRegisterStorage::State& meg_state()const{return meg_.state();}
 MegHostWriteAdapter::Observation meg_observation()const{return meg_.observation();}
 void cold_reset() override{
  MegHostWriteAdapter next(MegHostWriteAdapter::ControlPolicy::observe_original_stubs);
  MegSourceStatus next_status(status_.enabled()?MegSourceStatus::Policy::original_zero_stubs:MegSourceStatus::Policy::reject);
  An1xLcdResearchSink::cold_reset();meg_=next;status_=next_status;
 }
private:
 MegHostWriteAdapter meg_;
 MegSourceStatus status_;
};
}
