"""Pin active H83002 ITU source identities;not timer execution/hardware proof."""
import argparse
import hashlib
import json
from pathlib import Path
import re
from h8_reset_contract import verified_archive

METHODS = [
    'h8_timer16_device::tstr_r','h8_timer16_device::tstr_w',
    'h8_timer16_device::device_reset_after_children',
    'h8_timer16_channel_device::set_enable','h8_timer16_channel_device::device_reset',
    'h8_timer16_channel_device::tcr_w','h8_timer16_channel_device::tier_w',
    'h8_timer16_channel_device::tsr_r','h8_timer16_channel_device::tsr_w',
    'h8_timer16_channel_device::tcnt_r','h8_timer16_channel_device::tcnt_w',
    'h8_timer16_channel_device::tgr_w','h8_timer16_channel_device::internal_update',
    'h8_timer16_channel_device::update_counter','h8_timer16_channel_device::recalc_event',
    'h8h_timer16_channel_device::tier_update','h8h_timer16_channel_device::isr_update',
    'h8h_timer16_channel_device::isr_to_sr','h8h_timer16_channel_device::tcr_update',
]

def identity(raw,name):
    text=raw.decode('utf-8')
    matches=re.findall(r'^\w+ '+re.escape(name)+r'\([^\n]*\)(?: const)?\n\{.*?^\}',text,re.M|re.S)
    if len(matches)!=1:
        raise ValueError('Expected exactly one method: '+name)
    data=matches[0].encode('utf-8')
    return {'name':name,'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()}

def report(archive,lock):
    files=[]
    with verified_archive(archive,lock) as source:
        for leaf in ('h8_timer16.cpp','h8_timer16.h','h83002.cpp'):
            path='src/devices/cpu/h8/'+leaf
            names=[n for n in source.namelist() if n.endswith('/'+path)]
            if len(names)!=1:
                raise ValueError('Expected one archive member: '+path)
            raw=source.read(names[0])
            files.append({'path':path,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),
                          'methods':[identity(raw,n) for n in METHODS] if leaf=='h8_timer16.cpp' else []})
    return {'schema_version':1,'upstream_commit':lock['commit'], 'files':files,
            'scope':'Three original file and19 method identities only;not active timer execution,CPU integration or hardware acceptance.'}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive',required=True,type=Path)
    args=parser.parse_args()
    root=Path(__file__).resolve().parents[1]
    try:
        actual=report(args.archive,json.loads((root/'research/mame-source-lock.json').read_text()))
        expected=json.loads((root/'research/h83002-active-itu-contract.json').read_text())
        if actual!=expected:
            raise ValueError('Active ITU source identity mismatch')
        print(json.dumps({'files_verified':3,'methods_verified':len(METHODS),'source_identity_passed':True,'execution_verified':False}))
    except (OSError,ValueError,KeyError) as error:
        parser.exit(2,str(error)+'\n')

if __name__=='__main__':
    main()
