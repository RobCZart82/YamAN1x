"""Validate bounded CPU snapshots at observed PKS operand RAM word bases."""
import argparse
import json
import re
from pathlib import Path
from rom_inspect import SIZE, CRC32, SHA1

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


MANIFEST_TYPES = {
    "rom_identity": dict, "command": list, "returncode": int,
    "dsp_profile": str, "dsp_hardware_validated": bool, "dms_board": str,
    "dsp_sample_execution": bool, "dsp_sample_clock": str, "eg_readback": str,
    "live_bridge_completed": bool, "mame_sha256": str, "trace_lines": int,
    "complete_an1x_boot": bool, "note": str, "pc_samples": int,
    "unique_sampled_pcs": list, "h8_cpu_progress_observed": bool,
    "read_addresses_observed": int, "write_addresses_observed": int,
    "dsp_mmio_addresses_observed": list,
}
NULLABLE_FIELDS = {"midi_input", "vop3_bridge", "bridge_sha256", "dsp_sample_rate_hz"}
SHA256 = re.compile(r"[0-9a-f]{64}\Z")


def validate_run_manifest(manifest):
    """Require a successful run_h8_probe record, without opening local inputs."""
    def require(condition, message):
        if not condition:
            raise ValueError("Invalid probe manifest: " + message)

    require(type(manifest) is dict, "expected an object")
    require((MANIFEST_TYPES.keys() | NULLABLE_FIELDS) <= manifest.keys(),
            "required probe fields missing")
    for key, kind in MANIFEST_TYPES.items():
        require(type(manifest[key]) is kind, "wrong type for " + key)
    require(manifest["returncode"] == 0, "run did not exit successfully")
    require(manifest["h8_cpu_progress_observed"], "no H8 progress observed")
    identity = manifest["rom_identity"]
    require(type(identity.get("bytes")) is int and identity["bytes"] == SIZE and
            identity.get("crc32") == CRC32 and identity.get("sha1") == SHA1 and
            identity.get("matches_known_an1x_104") is True and
            type(identity.get("sha256")) is str and SHA256.fullmatch(identity["sha256"]),
            "missing or unknown ROM identity")
    require(manifest["command"] and all(type(arg) is str and arg for arg in manifest["command"]),
            "empty or malformed command")
    require(SHA256.fullmatch(manifest["mame_sha256"]), "malformed binary hash")
    for key in ("trace_lines", "pc_samples", "read_addresses_observed", "write_addresses_observed"):
        require(manifest[key] >= 0, "negative count for " + key)
    require(manifest["pc_samples"] > 0, "no PC samples")
    for key in ("unique_sampled_pcs", "dsp_mmio_addresses_observed"):
        require(all(type(address) is int and 0 <= address <= 0xffffff for address in manifest[key]),
                "malformed addresses for " + key)
    require(len(set(manifest["unique_sampled_pcs"]) - {0}) > 1,
            "PC samples do not establish H8 progress")
    require(manifest["dsp_profile"] in ("passive", "firmware-contract") and
            manifest["dms_board"] in ("absent", "installed") and
            manifest["eg_readback"] in ("zero", "an1x-level-experiment"), "unknown profile")
    require(manifest["dsp_hardware_validated"] is False, "unverified hardware claim")
    clock = manifest["dsp_sample_clock"]
    require(clock in ("disabled", "assumed-48k"), "unknown DSP clock")
    require(manifest["dsp_sample_execution"] == (clock != "disabled"), "DSP clock/execution mismatch")
    rate = manifest["dsp_sample_rate_hz"]
    require(rate is None if clock == "disabled" else type(rate) is int and rate == 48000,
            "DSP clock/rate mismatch")
    if clock != "disabled":
        passes = manifest.get("dsp_sample_passes")
        require(type(passes) is list and len(passes) == 2 and
                all(type(count) is int and count > 0 for count in passes),
                "DSP samples did not advance on both chips")
    bridge = manifest["vop3_bridge"]
    if bridge is None:
        require(manifest["bridge_sha256"] is None and not manifest["live_bridge_completed"] and
                clock == "disabled" and manifest["eg_readback"] == "zero", "bridge evidence missing")
    else:
        require(type(bridge) is str and bool(bridge) and
                type(manifest["bridge_sha256"]) is str and SHA256.fullmatch(manifest["bridge_sha256"]) and
                manifest["live_bridge_completed"] and manifest["dsp_profile"] == "firmware-contract",
                "incomplete bridge run")
    midi = manifest["midi_input"]
    if midi is not None:
        require(type(midi) is dict and type(midi.get("filename")) is str and bool(midi["filename"]) and
                type(midi.get("bytes")) is int and midi["bytes"] >= 0 and
                type(midi.get("sha256")) is str and SHA256.fullmatch(midi["sha256"]),
                "malformed MIDI identity")


def compare_run_manifests(baseline, observed):
    """Validate both runs, then compare every non-trace field including presence."""
    validate_run_manifest(baseline)
    validate_run_manifest(observed)
    excluded = {"command", "mame_sha256", "trace_lines"}
    keys = (set(baseline) | set(observed)) - excluded
    missing = object()
    return sorted(key for key in keys if baseline.get(key, missing) != observed.get(key, missing))


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
        try:
            differences = compare_run_manifests(baseline, observed)
        except ValueError as error:
            parser.error(str(error))
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
