"""Verify lexical H83002 configuration order,not runtime reset traversal."""
import argparse
import hashlib
import json
from pathlib import Path
import re
from h8_reset_contract import verified_archive

def extract(raw):
    matches=re.findall(r'^void h83002_device::device_add_mconfig\(machine_config &config\)\n\{(.*?)^\}',raw.decode(),re.M|re.S)
    if len(matches)!=1:raise ValueError('Expected one H83002 configuration method')
    body=matches[0]
    calls=re.findall(r'^\s*(H8[A-Z0-9_]*)\(config,\s*(m_[a-z0-9_]+(?:\[[01]\])?),',body,re.M)
    if not calls or len({member for _,member in calls})!=len(calls):raise ValueError('Missing or duplicate configured member')
    return dict(source_sha256=hashlib.sha256(raw).hexdigest(),method_body_sha256=hashlib.sha256(body.encode()).hexdigest(),
                declarations=[dict(type=t,member=m) for t,m in calls],scope='Lexical declaration order only;not actual hierarchy or reset execution proof')
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--archive',type=Path,required=True);p.add_argument('--check',action='store_true');a=p.parse_args()
    root=Path(__file__).resolve().parents[1];lock=json.loads((root/'research/mame-source-lock.json').read_text())
    with verified_archive(a.archive,lock) as z:
        names=[n for n in z.namelist() if n.endswith('/src/devices/cpu/h8/h83002.cpp')]
        if len(names)!=1:raise ValueError('Expected unique H83002 source')
        result=extract(z.read(names[0]));result['upstream_commit']=lock['commit']
    if a.check:
        if result!=json.loads((root/'research/h83002-child-order.json').read_text()):raise ValueError('Configuration identity/order differs')
        print('H83002 configuration order matches;runtime reset unverified')
    else:print(json.dumps(result,indent=2))
if __name__=='__main__':main()
