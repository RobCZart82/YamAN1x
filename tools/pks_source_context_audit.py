"""Validate bounded CPU snapshots at previously observed PKS source words."""
import argparse
import json
import re
from pathlib import Path
from rom_inspect import SIZE, CRC32, SHA1
from pks_operand_context_audit import compare_run_manifests

BEGIN = "PKS_SOURCE_CONTEXT_BEGIN version=1 capacity=4096 hardware_validated=0"
EVENT = re.compile(
    r"PKS_SOURCE_CONTEXT seq=(\d+) op=([WR]) address=([0-9a-f]{6}) "
    r"data=([0-9a-f]{4}) mask=([0-9a-f]{4}) pc=([0-9a-f]{6}) "
    r"time_ns=(\d+) ccr=([0-9a-f]{2}) "
    r"er0=([0-9a-f]{8}) er1=([0-9a-f]{8}) er2=([0-9a-f]{8}) er3=([0-9a-f]{8}) "
    r"er4=([0-9a-f]{8}) er5=([0-9a-f]{8}) er6=([0-9a-f]{8}) er7=([0-9a-f]{8})$")
END = re.compile(r"PKS_SOURCE_CONTEXT_END events=(\d+) dropped=(\d+) hardware_validated=0$")
ADDRESSES = (0x22D9EE, 0x234ACC, 0x234ACE, 0x234AD0, 0x23F7FC)
SOURCE_READ_PC = 0x00FA92
PAYLOAD_WRITE_PC = 0x00FA94
PAYLOAD_BYTE_ADDRESS = 0x22D9EE
PAYLOAD_REGISTER_ADDRESS = 0x22D9EF


def verify_source_to_payload_transfers(events):
    """Independently match observed FA92 byte reads to the following FA94 byte write."""
    transfers = []
    for index, event in enumerate(events):
        if (event["op"] != "R" or event["pc"] != SOURCE_READ_PC or
                event["address"] not in (0x234ACC, 0x234ACE, 0x234AD0)):
            continue
        if event["mask"] == 0xFF00:
            value = event["data"] >> 8
        elif event["mask"] == 0x00FF:
            value = event["data"] & 0xFF
        else:
            raise ValueError("PKS source read is not a single-byte lane access")
        if index + 1 >= len(events):
            raise ValueError("PKS source read has no following payload event")
        payload = events[index + 1]
        if (payload["seq"] != event["seq"] + 1 or payload["op"] != "W" or
                payload["address"] != PAYLOAD_BYTE_ADDRESS or
                payload["mask"] != 0x00FF or payload["pc"] != PAYLOAD_WRITE_PC):
            raise ValueError("PKS source read is not followed by the observed payload-byte write")
        # At the read callback the low register still contains the observed
        # index; the post-fetch PC does not imply that the load retired yet.
        # The immediately following write snapshot is the value-setting point.
        if payload["er"][0] & 0xFF != value:
            raise ValueError("PKS source byte does not match R0L at the observed payload write")
        if payload["data"] & 0xFF != value:
            raise ValueError("PKS payload write data differs from the observed source byte")
        if event["er"][4] != PAYLOAD_REGISTER_ADDRESS or payload["er"][4] != PAYLOAD_REGISTER_ADDRESS:
            raise ValueError("PKS payload destination register differs from the observed byte address")
        delta = payload["time_ns"] - event["time_ns"]
        if delta <= 0:
            raise ValueError("PKS source-to-payload event time did not advance")
        transfers.append(dict(seq=event["seq"], source_address=event["address"],
                              source_index=event["er"][0] & 0xFF,
                              value=value, source_data=event["data"], source_mask=event["mask"],
                              source_ccr=event["ccr"], payload_seq=payload["seq"],
                              payload_ccr=payload["ccr"], time_delta_ns=delta))
    return transfers


def parse_pks_source_context(lines):
    """Parse one complete, ordered and lossless observer section."""
    begun, ended, events = False, False, []
    for line in lines:
        if "YAMANIX " not in line:
            continue
        body = line.split("YAMANIX ", 1)[1].strip()
        if not body.startswith("PKS_SOURCE_CONTEXT"):
            continue
        if ended:
            raise ValueError("PKS source context record after completion")
        if body == BEGIN:
            if begun:
                raise ValueError("Repeated PKS source context header")
            begun = True
            continue
        if not begun:
            raise ValueError("PKS source context header missing or malformed")
        match = END.fullmatch(body)
        if match:
            total, dropped = map(int, match.groups())
            if dropped or total != len(events):
                raise ValueError("PKS source context capture is lossy or incomplete")
            ended = True
            continue
        match = EVENT.fullmatch(body)
        if not match:
            raise ValueError("Malformed PKS source context record")
        seq, op = int(match.group(1)), match.group(2)
        address, data, mask, pc = (int(value, 16) for value in match.groups()[2:6])
        time_ns, ccr = int(match.group(7)), int(match.group(8), 16)
        er = [int(value, 16) for value in match.groups()[8:]]
        if seq != len(events) or seq >= 4096:
            raise ValueError("Missing, repeated or excessive PKS source context event")
        if address not in ADDRESSES or not mask:
            raise ValueError("PKS source context address or mask outside observer contract")
        if events and time_ns < events[-1]["time_ns"]:
            raise ValueError("PKS source context time moved backwards")
        events.append(dict(seq=seq, op=op, address=address, data=data, mask=mask,
                           pc=pc, time_ns=time_ns, ccr=ccr, er=er))
    if not begun:
        return None
    if not ended:
        raise ValueError("PKS source context completion footer missing")
    counts = {f"{address:06x}": {op: 0 for op in "RW"} for address in ADDRESSES}
    for event in events:
        counts[f"{event['address']:06x}"][event["op"]] += 1
    transfers = verify_source_to_payload_transfers(events)
    return dict(events=events, counts=counts, complete=True, hardware_validated=False,
                source_to_payload_transfers=transfers,
                note=("Bounded CPU snapshots at observed aligned source/control RAM words; "
                      "no operand meaning or hardware agreement established."))


def validate_local_rom_identity(run):
    """Check manifest identity fields without opening any ROM or archive."""
    manifest = json.loads((run / "run.json").read_text())
    identity = manifest.get("rom_identity")
    if (type(identity) is not dict or identity.get("bytes") != SIZE or
            identity.get("crc32") != CRC32 or identity.get("sha1") != SHA1 or
            identity.get("matches_known_an1x_104") is not True or
            type(identity.get("sha256")) is not str or
            not re.fullmatch(r"[0-9a-f]{64}", identity["sha256"])):
        raise ValueError("Probe manifest does not identify the known AN1x 1.04 image")
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("run", type=Path, help="Local MAME run directory containing trace.log and run.json")
    parser.add_argument("--baseline", type=Path,
                        help="Optional uninstrumented run directory for non-trace comparison")
    args = parser.parse_args()
    try:
        result = parse_pks_source_context((args.run / "trace.log").read_text(errors="replace").splitlines())
        if result is None:
            parser.error("run has no PKS source context observer section")
        if not result["events"]:
            parser.error("run has no access to the selected source/control words")
        if not result["source_to_payload_transfers"]:
            parser.error("run has no independently matched source-to-payload transfers")
        manifest_comparison = None
        validate_local_rom_identity(args.run)
        if args.baseline:
            baseline = validate_local_rom_identity(args.baseline)
            observed = validate_local_rom_identity(args.run)
            differences = compare_run_manifests(baseline, observed)
            if differences:
                parser.error("non-trace run fields changed: " + ", ".join(differences))
            manifest_comparison = {"same_nontrace_observations": True,
                                   "compared_fields": len((set(baseline) | set(observed)) -
                                                          {"command", "mame_sha256", "trace_lines"})}
        print(json.dumps({"events": len(result["events"]), "counts": result["counts"],
                          "source_to_payload_transfers": result["source_to_payload_transfers"],
                          "complete": result["complete"], "hardware_validated": False,
                          "manifest_comparison": manifest_comparison, "note": result["note"]}, indent=2))
    except (OSError, ValueError, json.JSONDecodeError) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
