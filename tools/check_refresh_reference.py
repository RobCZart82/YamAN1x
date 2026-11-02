#!/usr/bin/env python3
"""Compile unchanged selected refresh handlers from the locked private archive.
No source extraction or generated binaries are retained in the repository.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import tempfile
from h8_reset_contract import verified_archive


def function(source, name):
    start = source.index('void h8_refresh_device::' + name + '(')
    brace = source.index('{', start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('archive', type=Path)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    lock = json.loads((root / 'research/mame-source-lock.json').read_text())
    with verified_archive(args.archive, lock) as archive:
        paths = [p for p in archive.namelist() if p.endswith('/src/devices/cpu/h8/h8_refresh.cpp')]
        if len(paths) != 1:
            raise ValueError('Expected exactly one original refresh source')
        source = archive.read(paths[0]).decode()
    constants = re.search(r'namespace \{(.*?)\} // anonymous namespace', source, re.S).group(1)
    handlers = '\n'.join(function(source, n) for n in ['rtcnt_update', 'rtcnt_w', 'rtcor_w', 'rtmcsr_w'])
    harness = r'''
#include "h8_refresh_timer.h"
#include <cstdint>
#include <cstdio>
using u8=uint8_t;using u32=uint32_t;using u64=uint64_t;
struct CPU{u64 at=0;u64 total_cycles(){return at;}void internal_update(){}};
struct IRQ{void internal_interrupt(int){}};
class h8_refresh_device{public:
 CPU cpu;IRQ irq;CPU*m_cpu=&cpu;IRQ*m_intc=&irq;int m_irq=21;
 u8 m_rtcnt=0,m_rtcor=255,m_rtmcsr=0;u64 m_cycle_base=0;
 void rtcnt_update(u64 cur_time=0);void rtcnt_w(u8);void rtcor_w(u8);void rtmcsr_w(u8);
};
'''+constants+'\n'+handlers+r'''
int main(){unsigned cases=0;
 for(unsigned mode=0;mode<8;++mode)for(unsigned compare:{0u,1u,7u,255u}){
 h8_refresh_device original;yamanix::H8RefreshTimer selected;
 original.rtcor_w(compare);selected.write_compare(compare);
 original.rtmcsr_w((mode<<3)|64);selected.write_control((mode<<3)|64);
 for(unsigned at=1;at<=16384;++at){original.cpu.at=at;original.rtcnt_update(at);selected.advance_to(at);
 if(at%101==0){original.rtmcsr_w((mode<<3)|64);selected.write_control((mode<<3)|64);}
 if(original.m_rtcnt!=selected.count()||uint8_t(original.m_rtmcsr|7)!=selected.control())return 1;++cases;}}
 std::printf("Pinned original refresh handlers:%u comparisons PASS\n",cases);
}
'''
    with tempfile.TemporaryDirectory(prefix='yamanix-refresh-reference-') as directory:
        cpp = Path(directory) / 'reference.cpp'
        binary = Path(directory) / 'reference'
        cpp.write_text(harness)
        subprocess.run(['c++', '-std=c++20', '-O1', '-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-I'+str(root/'src'), str(cpp), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    main()
