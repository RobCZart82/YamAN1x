#pragma once
#include "an1x_meg_research_sink.h"
#include "vop3_native_backend.h"
#include "vop3_host_word_adapter.h"
namespace yamanix {
// Explicit register-only research composition. No sample advancement/audio.
class An1xVop3ResearchSink:public An1xMegResearchSink {
public:
 using Adapter=Vop3HostWordAdapter<Vop3NativeBackend>;
 explicit An1xVop3ResearchSink(uint64_t scale,
   Adapter::ControlPolicy control=Adapter::ControlPolicy::reject,
   MegSourceStatus::Policy status=MegSourceStatus::Policy::reject)
  :An1xMegResearchSink(scale,status),policy_(control),vop3_(std::make_unique<State>(control)){}
 bool vop3_word_enabled()const{return true;}
 void vop3_write_word(uint32_t address,uint64_t cycle,uint16_t value){
  access(cycle,[&]{vop3_->adapter.write_word(address,value);});
 }
 uint16_t vop3_read_word(uint32_t address,uint64_t cycle){
  return access(cycle,[&]{return vop3_->adapter.read_word(address);});
 }
 Vop3NativeBackend::Snapshot vop3_snapshot(){return vop3_->backend.snapshot();}
 yamanix_vop3_metrics vop3_metrics(){return vop3_->backend.metrics();}
 // External sink reset is explicit while machine stopped;subset bus reset retains it.
 void cold_reset(){auto next=std::make_unique<State>(policy_);An1xMegResearchSink::cold_reset();vop3_.swap(next);}
private:
 struct State {Vop3NativeBackend backend;Adapter adapter;
  explicit State(Adapter::ControlPolicy policy):adapter(backend,policy){}
 };
 Adapter::ControlPolicy policy_;std::unique_ptr<State> vop3_;
};
}
