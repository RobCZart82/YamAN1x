"""Compare storage-only MEG writes with locked original methods;no device/audio proof."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import tempfile
from h8_reset_contract import verified_archive


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--compiler', default='c++')
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    lock = json.loads((root/'research/mame-source-lock.json').read_text())
    banks = [(4,'4'),(5,'5'),(8,'8'),(9,'9'),(14,'e'),(15,'f'),(20,'14'),(22,'16'),(23,'17'),(24,'18')]
    controls = [(1,'s1_w'),(2,'s2_w'),(3,'s3_w'),(7,'s7_w'),(10,'sa_w'),(21,'s15_w')]
    names = ['select_w']+['s'+name+'_w' for _,name in banks]+['consth_w','constl_w','offseth_w','offsetl_w']+[name for _,name in controls]
    with verified_archive(args.archive, lock) as archive:
        members = [n for n in archive.namelist() if n.endswith('/src/devices/sound/meg.cpp')]
        if len(members) != 1:
            raise ValueError('Expected one original MEG source')
        raw = archive.read(members[0]).decode()
    methods = []
    for name in names:
        found = re.findall(r'^void meg_device::'+name+r'\(u8 data\)\n\{.*?^\}',raw,re.M|re.S)
        if len(found) != 1:
            raise ValueError('Expected one original storage method: '+name)
        methods.append(found[0])
    prefix = '#include "meg_register_storage.h"\n#include "meg_host_write_adapter.h"\n#include <cstring>\n#include <cstdio>\nusing u8=uint8_t;\n#define logerror(...) ((void)0)\nstruct meg_device {\n'
    prefix += 'u8 m_reg=0;'+''.join('u8 m_r'+name+'[256]{};' for _,name in banks)
    prefix += 'std::array<int16_t,256>m_const{},m_offset{};'
    prefix += ''.join('void '+name+'(u8);' for name in names)+'};\n'
    checks = '\n'.join('case '+str(offset)+':ref.s'+name+'_w(uint8_t(value));break;' for offset,name in banks)
    arrays = ','.join('ref.m_r'+name for _,name in banks)
    saved_arrays = ','.join('saved.m_r'+name for _,name in banks)
    control_calls = '\n'.join('case '+str(offset)+':ref.'+name+'(uint8_t(value));break;' for offset,name in controls)
    suffix = r'''
int main(){meg_device ref;yamanix::MegRegisterStorage model;unsigned comparisons=0;
 const unsigned offsets[]={4,5,8,9,14,15,20,22,23,24};
 for(unsigned slot=0;slot<256;++slot){ref.select_w(uint8_t(slot));model.write(0,uint8_t(slot));
  for(unsigned bank=0;bank<10;++bank)for(unsigned value=0;value<256;++value){
   switch(offsets[bank]){CHECKS}
   model.write(offsets[bank],uint8_t(value));uint8_t*original[]={ARRAYS};
   if(model.state().bytes[bank][slot]!=original[bank][slot]||model.state().selected!=ref.m_reg)
    throw std::runtime_error("Original MEG bank mismatch");++comparisons;
  }
 }
 ref.select_w(137);model.write(0,137);
 for(unsigned value=0;value<65536;++value){
  ref.consth_w(uint8_t(value>>8));ref.constl_w(uint8_t(value));
  ref.offsetl_w(uint8_t(value));ref.offseth_w(uint8_t(value>>8));
  model.write(12,uint8_t(value>>8));model.write(13,uint8_t(value));
  model.write(19,uint8_t(value));model.write(18,uint8_t(value>>8));
  if(model.state().constants[137]!=uint16_t(ref.m_const[137])||model.state().offsets[137]!=uint16_t(ref.m_offset[137]))
   throw std::runtime_error("Original MEG word lane mismatch");++comparisons;
 }
 yamanix::MegHostWriteAdapter host(yamanix::MegHostWriteAdapter::ControlPolicy::observe_original_stubs);
 for(unsigned offset:{1u,2u,3u,7u,10u,21u})for(unsigned value=0;value<256;++value){
  auto saved=ref;auto model_before=host.state();
  switch(offset){CONTROL_CALLS}
  host.write_byte(0x400001+2*offset,uint8_t(value));
  uint8_t*original[]={ARRAYS};uint8_t*prior[]={SAVED_ARRAYS};
  for(unsigned bank=0;bank<10;++bank)if(std::memcmp(original[bank],prior[bank],256))throw std::runtime_error("Original control changed bank");
  if(ref.m_reg!=saved.m_reg||ref.m_const!=saved.m_const||ref.m_offset!=saved.m_offset||host.state()!=model_before)
   throw std::runtime_error("Original control storage mismatch");
 }
 if(host.observation().count!=1536)throw std::runtime_error("Missing explicit control observation");
 std::puts("1536 locked original logging-only control comparisons PASS;no command effects proven");
 std::printf("%u locked original MEG storage comparisons PASS;explicit matching initial state,no status/audio proof\n",comparisons);
}
'''.replace('CHECKS',checks).replace('SAVED_ARRAYS',saved_arrays).replace('CONTROL_CALLS',control_calls).replace('ARRAYS',arrays)
    with tempfile.TemporaryDirectory(prefix='yamanix-meg-reference-') as temp:
        source = Path(temp)/'check.cpp'
        binary = Path(temp)/'check'
        source.write_text(prefix+'\n'.join(methods)+suffix)
        subprocess.run([args.compiler,'-std=c++20','-O1','-fsanitize=address,undefined',
                        '-fno-sanitize-recover=all','-I'+str(root/'src'),str(source),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)


if __name__ == '__main__':
    main()
