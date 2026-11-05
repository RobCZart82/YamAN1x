"""Compare all supported TCR encodings with locked unchanged H8H tcr_update."""
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tempfile
import argparse
from h8_reset_contract import verified_archive

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive', required=True, type=Path)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    lock = json.loads((root/'research/mame-source-lock.json').read_text())
    contract = json.loads((root/'research/h83002-active-itu-contract.json').read_text())
    with verified_archive(args.archive, lock) as archive:
        name = next(n for n in archive.namelist() if n.endswith('/src/devices/cpu/h8/h8_timer16.cpp'))
        raw = archive.read(name)
    identity = next(f for f in contract['files'] if f['path'].endswith('/h8_timer16.cpp'))
    if hashlib.sha256(raw).hexdigest() != identity['sha256']:
        raise ValueError('Original timer source identity mismatch')
    method = re.findall(r'^void h8h_timer16_channel_device::tcr_update\(\)\n\{.*?^\}', raw.decode(), re.M|re.S)
    if len(method) != 1:
        raise ValueError('Expected one original TCR method')
    expected_method = 'd4d2a472c2f0067e2e6138243e44cb2d3ddcd36614c7bb8652f7479157677f8a'
    if hashlib.sha256(method[0].encode()).hexdigest() != expected_method:
        raise ValueError('Original TCR method identity mismatch')
    prefix = r'''
#include "h8_itu_control_decode.h"
#include <cstdio>
#define LOG(...) do {} while(false)
struct h8h_timer16_channel_device {
 enum {CHAIN,INPUT_A,INPUT_B,INPUT_C,INPUT_D,DIV_1,DIV_2};
 enum {TGR_CLEAR_NONE=-1,TGR_CLEAR_EXT=-2};
 unsigned m_tcr=0,m_clock_type=0,m_clock_divider=0,m_phase=0;
 int m_tgr_clearing=0;unsigned m_tgr[2]{};
 void tcr_update();
};
'''
    suffix = r'''
int main(){unsigned count=0;
 for(unsigned value=0;value<256;++value){
  h8h_timer16_channel_device original;original.m_tcr=value;original.tcr_update();
  const auto decoded=yamanix::decode_itu_control(uint8_t(value));
  const bool supported=original.m_clock_type==original.DIV_1&&original.m_tgr_clearing!=-2;
  if(bool(decoded)!=supported)throw std::runtime_error("Original support mismatch");
  if(!decoded)continue;
  const int clear=decoded->clear==yamanix::H8ItuAutoClear::ClearMode::free_running?-1:int(decoded->clear);
  if(decoded->shift!=original.m_clock_divider||decoded->phase!=original.m_phase||clear!=original.m_tgr_clearing)
   throw std::runtime_error("Original TCR decode mismatch");
  ++count;
 }
 std::printf("256 original TCR profiles,including %u supported internal profiles PASS\n",count);
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-tcr-') as directory:
        source = Path(directory)/'check.cpp'
        binary = Path(directory)/'check'
        source.write_text(prefix+method[0]+suffix)
        subprocess.run(['c++','-std=c++20','-fsanitize=address,undefined','-fno-sanitize-recover=all','-I'+str(root/'src'),str(source),'-o',str(binary)],check=True)
        subprocess.run([str(binary)],check=True)

if __name__ == '__main__':
    main()
