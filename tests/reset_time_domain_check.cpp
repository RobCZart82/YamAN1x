#include "reset_time_domain.h"
#include <cstdio>
#include <initializer_list>
#include <limits>
int main(){
 unsigned failures=0;auto check=[&](bool ok){if(!ok)++failures;};
 using Clock=yamanix::ResetTimeDomain;
 for(uint64_t base:{uint64_t{0},uint64_t{1000},uint64_t{1}<<48}){
  Clock c(base);check(c.absolute()==base&&c.since_reset()==0&&c.generation()==0);
  for(uint64_t epoch=1;epoch<=100;++epoch){
   c.advance_to(base+epoch*17);check(c.since_reset()==17);
   c.reset_origin();check(c.absolute()==base+epoch*17&&c.since_reset()==0&&c.generation()==epoch);
   check(c.absolute_from_reset(9)==base+epoch*17+9);
   try{c.advance_to(base+epoch*17-1);++failures;}catch(const std::invalid_argument&){}
   check(c.absolute()==base+epoch*17&&c.since_reset()==0);
  }
 }
 const auto max=std::numeric_limits<uint64_t>::max();Clock high(max-2);
 check(high.absolute_from_reset(2)==max);
 try{high.absolute_from_reset(3);++failures;}catch(const std::overflow_error&){}
 high.advance_to(max);check(high.since_reset()==2);high.reset_origin();check(high.absolute()==max&&high.since_reset()==0);
 try{high.absolute_from_reset(1);++failures;}catch(const std::overflow_error&){}
 std::printf("Reset time domains:%u failures\n",failures);return failures?1:0;
}
