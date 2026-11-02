"""Pin framework reset identities; lexical evidence,not a whole-machine reset trace."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile
import re
from h8_reset_contract import verified_archive, method_identity

METHODS={
 'mconfig.cpp':[],
 'schedule.cpp':[],
 'device.cpp':['device_t::reset'],
 'diexec.cpp':['device_execute_interface::interface_pre_reset','device_execute_interface::interface_post_reset','device_execute_interface::device_input::reset'],
}

def queued_reset_identity(raw):
    found=re.findall(r'^TIMER_CALLBACK_MEMBER\(device_execute_interface::device_input::empty_event_queue\)\n\{.*?^\}',raw.decode(),re.M|re.S)
    if len(found)!=1:raise ValueError('Expected one queued input callback')
    body=found[0].encode()
    return dict(name='device_input::empty_event_queue',bytes=len(body),sha256=hashlib.sha256(body).hexdigest())

TIME_SIGNATURES={
 'diexec.cpp':[
  'attotime device_execute_interface::local_time() const noexcept',
  'u64 device_execute_interface::total_cycles() const noexcept',
  'void device_execute_interface::abort_timeslice() noexcept'],
 'schedule.cpp':['void device_scheduler::synchronize(timer_expired_delegate callback, s32 param)'],
}
def time_identities(raw,signatures):
    result=[]
    for signature in signatures:
        found=re.findall('^'+re.escape(signature)+r'\n\{.*?^\}',raw.decode(),re.M|re.S)
        if len(found)!=1:raise ValueError('Expected unique time method: '+signature)
        body=found[0].encode();result.append(dict(signature=signature,bytes=len(body),sha256=hashlib.sha256(body).hexdigest()))
    return result

def report(archive,lock):
    # Verify and process one immutable private archive snapshot.
    files=[]
    with verified_archive(archive,lock) as z:
        for leaf,names in METHODS.items():
            path='src/emu/'+leaf
            members=[n for n in z.namelist() if n.endswith('/'+path)]
            if len(members)!=1:raise ValueError('Expected unique framework file: '+path)
            raw=z.read(members[0])
            identities=[method_identity(raw,n) for n in names]
            if leaf=='diexec.cpp':identities.append(queued_reset_identity(raw))
            identities.extend(time_identities(raw,TIME_SIGNATURES.get(leaf,[])))
            files.append(dict(path=path,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),
                              methods=identities))
    return dict(schema_version=1,upstream_commit=lock['commit'],files=files,
                scope='Framework method identities only;not child insertion order,input reset semantics or global time proof')

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',type=Path,required=True)
    p.add_argument('--check',action='store_true')
    a=p.parse_args();root=Path(__file__).resolve().parents[1]
    result=report(a.archive,json.loads((root/'research/mame-source-lock.json').read_text()))
    if a.check:
        if result!=json.loads((root/'research/h8-reset-traversal.json').read_text()):
            raise ValueError('Framework reset identity differs')
        print('Four framework files/reset-input and timing methods match;global reset unverified')
    else:print(json.dumps(result,indent=2))
if __name__=='__main__':main()
