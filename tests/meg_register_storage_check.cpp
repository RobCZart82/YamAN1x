#include "meg_register_storage.h"
#include <cstdio>
#include <initializer_list>
using M=yamanix::MegRegisterStorage;
void check(bool v){if(!v)throw std::runtime_error("MEG storage contract failed");}
int main(){M m;const unsigned banks[]={4,5,8,9,14,15,20,22,23,24};unsigned cases=0;
 for(unsigned slot=0;slot<256;++slot){m.write(0,uint8_t(slot));check(m.state().selected==slot);
  for(unsigned b=0;b<10;++b)for(unsigned v=0;v<256;++v){m.write(banks[b],uint8_t(v));check(m.state().bytes[b][slot]==v);++cases;}}
 for(unsigned slot=0;slot<256;++slot){m.write(0,uint8_t(slot));
  for(unsigned v=0;v<256;++v){m.write(12,uint8_t(v));m.write(13,uint8_t(255-v));m.write(18,uint8_t(255-v));m.write(19,uint8_t(v));
   check(m.state().constants[slot]==v*256+255-v&&m.state().offsets[slot]==(255-v)*256+v);++cases;}}
 // Exhaustive word patterns and lane retention on a selected nonzero slot.
 m.write(0,137);for(unsigned word=0;word<65536;++word){
  m.write(13,uint8_t(word));m.write(12,uint8_t(word>>8));check(m.state().constants[137]==word);
  m.write(18,uint8_t(word>>8));m.write(19,uint8_t(word));check(m.state().offsets[137]==word);}
 for(unsigned b=0;b<10;++b)for(unsigned slot=0;slot<256;++slot){m.write(0,uint8_t(slot));m.write(banks[b],uint8_t(slot+b));}
 for(unsigned b=0;b<10;++b)for(unsigned slot=0;slot<256;++slot)check(m.state().bytes[b][slot]==uint8_t(slot+b));
 auto before=m.state();m.reset_retaining_state();check(m.state()==before);
 for(unsigned offset:{1u,2u,3u,6u,7u,10u,11u,16u,17u,21u,25u,256u,0x400007u}){
  bool bad=false;try{m.write(offset,0x5a);}catch(const std::out_of_range&){bad=true;}check(bad&&m.state()==before);}
 check(M().state().constants[137]==0&&M().state().bytes[9][255]==0);
 std::printf("MEG storage:%u selected bank/lane cases,65536 word patterns,isolation/strict/reset PASS\n",cases);
}
