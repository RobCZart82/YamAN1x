"""Validate bounded CPU snapshots at observed PKS operand RAM word bases."""
import argparse
import json
import re
from pathlib import Path

BEGIN = "PKS_CONTEXT_BEGIN version=1 capacity=4096 hardware_validated=0"
EVENT = re.compile(
    r"PKS_CONTEXT seq=(\d+) op=([WR]) address=([0-9a-f]{6}) "
    r"data=([0-9a-f]{4}) mask=([0-9a-f]{4}) pc=([0-9a-f]{6}) "
    r"time_ns=(\d+) ccr=([0-9a-f]{2}) "
    r"er0=([0-9a-f]{8}) er1=([0-9a-f]{8}) er2=([0-9a-f]{8}) er3=([0-9a-f]{8}) "
    r"er4=([0-9a-f]{8}) er5=([0-9a-f]{8}) er6=([0-9a-f]{8}) er7=([0-9a-f]{8})$")
END = re.compile(r"PKS_CONTEXT_END events=(\d+) dropped=(\d+) hardware_validated=0$")
ADDRESSES = (0x2349E8, 0x237202, 0x237204)


def parse_pks_operand_context(lines):
    """Parse a complete, lossless observer section; reject malformed captures."""
    begun, ended, events = False, False, []
    for line in lines:
        if "YAMANIX " not in line:
            continue
        body = line.split("YAMANIX ", 1)[1].strip()
        if not body.startswith("PKS_CONTEXT"):
            continue
        if ended:
            raise ValueError("PKS context record after completion")
        if body == BEGIN:
            if begun:
                raise ValueError("Repeated PKS context header")
            begun = True
            continue
        if not begun:
            raise ValueError("PKS context header missing or malformed")
        match = END.fullmatch(body)
        if match:
            total, dropped = map(int, match.groups())
            if dropped or total != len(events):
                raise ValueError("PKS context capture is lossy or incomplete")
            ended = True
            continue
        match = EVENT.fullmatch(body)
        if not match:
            raise ValueError("Malformed PKS context record")
        seq = int(match.group(1))
        op = match.group(2)
        address, data, mask, pc = (int(value, 16) for value in match.groups()[2:6])
        time_ns = int(match.group(7))
        ccr = int(match.group(8), 16)
        er = [int(value, 16) for value in match.groups()[8:]]
        if seq != len(events) or seq >= 4096:
            raise ValueError("Missing, repeated or excessive PKS context event")
        if address not in ADDRESSES or not mask:
            raise ValueError("PKS context address or mask outside observer contract")
        if events and time_ns < events[-1]["time_ns"]:
            raise ValueError("PKS context time moved backwards")
        events.append(dict(seq=seq, op=op, address=address, data=data, mask=mask,
                           pc=pc, time_ns=time_ns, ccr=ccr, er=er))
    if not begun:
        return None
    if not ended:
        raise ValueError("PKS context completion footer missing")
    counts = {f"{address:06x}": {op: 0 for op in "RW"} for address in ADDRESSES}
    for event in events:
        counts[f"{event['address']:06x}"][event["op"]] += 1
    return dict(events=events, counts=counts, complete=True, hardware_validated=False,
                note="Bounded CPU register/CCR snapshots at three observed RAM word bases; no data meaning or hardware agreement established.")


def compare_run_manifests(baseline, observed):
    """Compare all non-trace probe fields while allowing binary/path/line deltas."""
    excluded = {"command", "mame_sha256", "trace_lines"}
    keys = (set(baseline) | set(observed)) - excluded
    return sorted(key for key in keys if baseline.get(key) != observed.get(key))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("run", type=Path, help="Local MAME run directory containing trace.log")
    parser.add_argument("--baseline", type=Path,
                        help="Optional uninstrumented run directory for non-trace comparison")
    args = parser.parse_args()
    lines = (args.run / "trace.log").read_text(errors="replace").splitlines()
    result = parse_pks_operand_context(lines)
    if result is None:
        parser.error("run has no PKS context observer section")
    if not result["events"]:
        parser.error("run has no access to the selected operand word bases")
    comparison = None
    if args.baseline:
        baseline = json.loads((args.baseline / "run.json").read_text())
        observed = json.loads((args.run / "run.json").read_text())
        differences = compare_run_manifests(baseline, observed)
        if differences:
            parser.error("non-trace run fields changed: " + ", ".join(differences))
        comparison = {"same_nontrace_observations": True,
                      "compared_fields": len((set(baseline) | set(observed)) -
                                             {"command", "mame_sha256", "trace_lines"})}
    print(json.dumps({"events": len(result["events"]), "counts": result["counts"],
                      "complete": result["complete"],
                      "hardware_validated": result["hardware_validated"],
                      "manifest_comparison": comparison,
                      "note": result["note"]}, indent=2))


if __name__ == "__main__":
    main()
