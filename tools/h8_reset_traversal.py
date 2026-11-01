"""Pin framework reset identities; lexical evidence,not a whole-machine reset trace."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile
from h8_reset_contract import verified_archive, method_identity

METHODS={
 'device.cpp':['device_t::reset'],
 'diexec.cpp':['device_execute_interface::interface_pre_reset','device_execute_interface::interface_post_reset'],
}

def report(archive,lock):
    # Verify and process one immutable private archive snapshot.
    files=[]
    with verified_archive(archive,lock) as z:
        for leaf,names in METHODS.items():
            path='src/emu/'+leaf
            members=[n for n in z.namelist() if n.endswith('/'+path)]
            if len(members)!=1:raise ValueError('Expected unique framework file: '+path)
            raw=z.read(members[0])
            files.append(dict(path=path,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),
                              methods=[method_identity(raw,n) for n in names]))
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
        print('Two framework files/three reset methods match;global reset unverified')
    else:print(json.dumps(result,indent=2))
if __name__=='__main__':main()
