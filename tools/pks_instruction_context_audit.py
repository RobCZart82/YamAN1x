"""Audit bounded H8 instruction snapshots around two observed PKS input reads."""
import argparse
import hashlib
import json
import re
from pathlib import Path

from pks_operand_context_audit import compare_run_manifests, validate_run_manifest
from pks_source_context_audit import parse_pks_source_context
from pks_operand_context_audit import parse_pks_operand_context
from pks_caller_input_audit import audit_caller_input_windows

BEGIN = re.compile(r"PKS_INSTRUCTION_CONTEXT_BEGIN version=1 capacity=(\d+) window=(\d+) hardware_validated=0$")
WINDOWS = re.compile(r"PKS_INSTRUCTION_CONTEXT_WINDOWS count=(\d+) events=(\d+) dropped=(\d+) finished=([01]) hardware_validated=0$")
WINDOW = re.compile(
    r"PKS_INSTRUCTION_WINDOW id=(\d+) trigger_seq=(\d+) address=([0-9a-f]{6}) "
    r"pc=([0-9a-f]{6}) data=([0-9a-f]{4}) mask=([0-9a-f]{4}) ccr=([0-9a-f]{2}) "
    r"time_ns=(\d+) first=(\d+) count=(\d+) complete=([01])$")
INSTRUCTION = re.compile(
    r"PKS_INSTRUCTION seq=(\d+) window=(\d+) pc=([0-9a-f]{6}) opcode=([0-9a-f]{4}) "
    r"time_ns=(\d+) ccr=([0-9a-f]{2}) " + " ".join(f"r{i}=([0-9a-f]{{4}})" for i in range(8)) + " " +
    " ".join(f"er{i}=([0-9a-f]{{8}})" for i in range(8)) + r"$")
DISASSEMBLY = re.compile(r"DASM pc=([0-9a-f]{6}) (.+)$")
SOURCE_WRITER_PC = 0x0039AC
SITES = {
    0x2349EA: dict(pc=0x002A08, mask=0xFF00, data_register=1,
                   path=(0x002A08, 0x002A0E, 0x002A10, 0x002A12, 0x002A36,
                         0x002A3C, 0x002A42, 0x002A52, 0x002A54, 0x002A56,
                         0x002A62, 0x002A64, 0x002A66, 0x003AA4, 0x0039A4, 0x0039AC)),
    0x237204: dict(pc=0x001FD8, mask=0x00FF, data_register=2,
                   path=(0x001FD8, 0x001FDE, 0x001FE0, 0x001FE2, 0x001FEE,
                         0x001FF0, 0x001FF2, 0x003AA4, 0x0039A4, 0x0039AC)),
}
STATIC_INSTRUCTIONS = {
    0x001FD2: "mov.b   @h'237205, r2l",
    0x001FDE: "mov.b   r2l, r1l",
    0x001FEE: "mov.b   #h'00, r1l",
    0x001FF0: "mov.b   #h'e0, r0l",
    0x001FF2: "jsr     h'003aa4",
    0x002A02: "mov.b   @h'2349ea, r1l",
    0x002A52: "mov.b   r2l, r1l",
    0x002A5A: "shll.b  r1l",
    0x002A5C: "and.b   #h'7f, r1l",
    0x002A64: "mov.b   #h'e0, r0l",
    0x002A66: "jsr     h'003aa4",
    0x0039A4: "mov.b   r1l, @(h'234acc, er2)",
    0x0039AC: "inc.w   #1, r2",
}


def _one_byte(data, mask):
    if mask == 0xFF00:
        return data >> 8
    if mask == 0x00FF:
        return data & 0xFF
    raise ValueError("instruction-window input is not a single-byte lane")


def parse_instruction_context(lines):
    """Parse one complete, lossless fixed-capacity instruction-context section."""
    begun = ended = False
    summary = None
    capacity = window_length = None
    windows, events = [], []
    for line in lines:
        if "YAMANIX " not in line:
            continue
        body = line.split("YAMANIX ", 1)[1].strip()
        if body.startswith("PKS_SOURCE_CONTEXT_END") and summary is not None:
            ended = True
        if (not body.startswith("PKS_INSTRUCTION_CONTEXT") and
                not body.startswith("PKS_INSTRUCTION_WINDOW ") and
                not body.startswith("PKS_INSTRUCTION ")):
            continue
        if ended:
            raise ValueError("instruction-context record after completion")
        match = BEGIN.fullmatch(body)
        if match:
            if begun:
                raise ValueError("repeated instruction-context header")
            begun = True
            capacity, window_length = map(int, match.groups())
            if capacity != 8192 or window_length != 2048:
                raise ValueError("instruction-context bounds differ from the pinned profile")
            continue
        if not begun:
            raise ValueError("instruction-context header missing or malformed")
        match = WINDOWS.fullmatch(body)
        if match:
            if summary is not None:
                raise ValueError("repeated instruction-context summary")
            summary = tuple(map(int, match.groups()))
            continue
        match = WINDOW.fullmatch(body)
        if match:
            values = match.groups()
            window = dict(id=int(values[0]), trigger_seq=int(values[1]), address=int(values[2], 16),
                          pc=int(values[3], 16), data=int(values[4], 16), mask=int(values[5], 16),
                          ccr=int(values[6], 16), time_ns=int(values[7]), first=int(values[8]),
                          count=int(values[9]), complete=bool(int(values[10])))
            expected_id = len(windows)
            if (window["id"] != expected_id or window["trigger_seq"] != expected_id or
                    window["first"] != expected_id * window_length or
                    window["count"] != window_length or not window["complete"] or
                    window["address"] not in SITES):
                raise ValueError("instruction-context window bounds, order or caller site are invalid")
            site = SITES[window["address"]]
            if window["pc"] != site["pc"] or window["mask"] != site["mask"]:
                raise ValueError("instruction-context trigger does not match the static caller contract")
            windows.append(window)
            continue
        match = INSTRUCTION.fullmatch(body)
        if match:
            values = match.groups()
            seq, window_id, time_ns = int(values[0]), int(values[1]), int(values[4])
            pc, opcode, ccr = (int(value, 16) for value in (values[2], values[3], values[5]))
            regs = [int(value, 16) for value in values[6:14]]
            ers = [int(value, 16) for value in values[14:22]]
            if seq != len(events) or seq >= capacity or window_id >= 4:
                raise ValueError("instruction-context event sequence or window is invalid")
            if seq // window_length != window_id:
                raise ValueError("instruction-context event is outside its declared window")
            if any((ers[index] & 0xFFFF) != regs[index] for index in range(8)):
                raise ValueError("instruction-context ER low word differs from captured R register")
            if events and time_ns < events[-1]["time_ns"]:
                raise ValueError("instruction-context time moved backwards")
            events.append(dict(seq=seq, window=window_id, pc=pc, opcode=opcode, time_ns=time_ns,
                               ccr=ccr, r=regs, er=ers))
            continue
        raise ValueError("malformed instruction-context record")
    if not begun or summary is None or not ended:
        raise ValueError("instruction-context section is absent or incomplete")
    count, total, dropped, finished = summary
    if dropped or not finished or count != len(windows) or total != len(events):
        raise ValueError("instruction-context footer reports loss or incomplete capture "
                         f"(summary={summary}, parsed_windows={len(windows)}, parsed_events={len(events)})")
    if len(windows) != 4 or len(events) != 8192:
        raise ValueError("instruction-context capture does not contain four full windows")
    for window in windows:
        section = events[window["first"]:window["first"] + window["count"]]
        if len(section) != window_length or any(event["window"] != window["id"] for event in section):
            raise ValueError("instruction-context window event range is not contiguous")
        if section[0]["time_ns"] < window["time_ns"]:
            raise ValueError("instruction-context begins before its trigger read")
    return dict(windows=windows, events=events, complete=True, hardware_validated=False)


def parse_static_disassembly(lines):
    observed = {}
    for line in lines:
        if "YAMANIX DASM " not in line:
            continue
        body = line.split("YAMANIX ", 1)[1].strip()
        match = DISASSEMBLY.fullmatch(body)
        if not match:
            raise ValueError("malformed PKS static disassembly record")
        address, instruction = int(match.group(1), 16), match.group(2)
        if address in observed and observed[address] != instruction:
            raise ValueError("conflicting static disassembly at one address")
        observed[address] = instruction
    for address, instruction in STATIC_INSTRUCTIONS.items():
        if observed.get(address) != instruction:
            raise ValueError(f"static instruction mismatch at {address:06x}")
    return observed


def audit_instruction_path(instruction, source, operand, lines):
    static = parse_static_disassembly(lines)
    by_window = {index: instruction["events"][index * 2048:(index + 1) * 2048]
                 for index in range(4)}
    operand_events = operand["events"]
    source_writes = [event for event in source["events"]
                     if event["op"] == "W" and event["pc"] == SOURCE_WRITER_PC and
                     event["address"] in (0x234ACC, 0x234ACE, 0x234AD0)]
    checked_windows = []
    for win in range(4):
        header = instruction["windows"][win]
        site = SITES[header["address"]]
        matching_reads = [event for event in operand_events
                          if event["op"] == "R" and event["address"] == header["address"] and
                          event["pc"] == header["pc"] and event["mask"] == header["mask"] and
                          event["data"] == header["data"] and event["ccr"] == header["ccr"] and
                          event["time_ns"] == header["time_ns"]]
        if not matching_reads:
            raise ValueError("instruction window has no matching independently observed input read")
        selected = _one_byte(header["data"], header["mask"])
        events = by_window[win]
        first = events[0]
        if (first["pc"] != header["pc"] or first["time_ns"] != header["time_ns"] or
                (first["r"][site["data_register"]] & 0xFF) != selected):
            raise ValueError("first post-read instruction snapshot differs from selected bus byte")
        if win >= 2:
            continue
        pcs = [event["pc"] for event in events]
        cursor = -1
        for expected_pc in site["path"]:
            try:
                cursor = pcs.index(expected_pc, cursor + 1)
            except ValueError as error:
                raise ValueError(f"caller instruction window misses ordered path PC {expected_pc:06x}") from error
        # Compare raw CPU context at the common writer with the independent bus observer.
        group = [event for event in source_writes if event["time_ns"] >= header["time_ns"]][:3]
        if len(group) != 3:
            raise ValueError("caller window has no complete three-byte source-writer group")
        writer_contexts = [event for event in events if event["pc"] == SOURCE_WRITER_PC]
        if len(writer_contexts) < 3:
            raise ValueError("instruction window misses source-writer instruction snapshots")
        writer_contexts = writer_contexts[:3]
        for bus, cpu in zip(group, writer_contexts):
            if (cpu["time_ns"] != bus["time_ns"] or cpu["ccr"] != bus["ccr"] or
                    cpu["er"] != bus["er"]):
                raise ValueError("CPU and bus snapshots disagree at the source-writer instruction")
            byte = bus["data"] >> 8 if bus["mask"] == 0xFF00 else bus["data"] & 0xFF
            if (bus["mask"] not in (0xFF00, 0x00FF) or cpu["er"][1] & 0xFF != byte):
                raise ValueError("writer R1L differs from the independently observed byte lane")
        checked_windows.append(dict(window=win, input_address=header["address"],
                                   selected_input_byte=selected,
                                   input_ccr=header["ccr"], writer_count=len(writer_contexts),
                                   writer_ccrs=[event["ccr"] for event in writer_contexts],
                                   raw_ccr_interpreted=False))
    checkpoints = (
        (0, 0x002A54, 1, 0x20), (0, 0x002A64, 1, 0x00),
        (0, 0x002A66, 0, 0xE0), (1, 0x001FDE, 2, 0x00),
        (1, 0x001FF2, 0, 0xE0),
    )
    for window_id, pc, register, expected in checkpoints:
        matches = [event for event in by_window[window_id] if event["pc"] == pc]
        if not matches or (matches[0]["r"][register] & 0xFF) != expected:
            raise ValueError(f"runtime register checkpoint differs at window {window_id}, PC {pc:06x}")
    return dict(windows=checked_windows, checked_static_instructions=len(static),
                trace_sha256=hashlib.sha256(("\n".join(
                    f"{event['seq']}:{event['window']}:{event['pc']:06x}:{event['opcode']:04x}:"
                    f"{event['time_ns']}:{event['ccr']:02x}:" + ",".join(f"{value:08x}" for value in event['er'])
                    for event in instruction["events"]) + "\n").encode()).hexdigest(),
                hardware_validated=False,
                note="Bounded runtime instruction/register snapshots cross-checked against static disassembly and bus records; no causal or semantic claim.")


def audit_run(run, baseline=None):
    run = Path(run)
    lines = (run / "trace.log").read_text(errors="replace").splitlines()
    instruction = parse_instruction_context(lines)
    source = parse_pks_source_context(lines)
    operand = parse_pks_operand_context(lines)
    if source is None or operand is None:
        raise ValueError("run must contain complete source and operand context records")
    manifest = json.loads((run / "run.json").read_text())
    validate_run_manifest(manifest)
    if baseline is not None:
        reference = json.loads((Path(baseline) / "run.json").read_text())
        differences = compare_run_manifests(reference, manifest)
        if differences:
            raise ValueError("non-trace run fields changed: " + ", ".join(differences))
        manifest_comparison = dict(same_nontrace_observations=True,
                                   compared_fields=len((set(reference) | set(manifest)) -
                                                       {"command", "mame_sha256", "trace_lines"}))
        reference_lines = (Path(baseline) / "trace.log").read_text(errors="replace").splitlines()
        reference_instruction = parse_instruction_context(reference_lines)
        current_rows = [event for event in instruction["events"]]
        reference_rows = [event for event in reference_instruction["events"]]
        if current_rows != reference_rows:
            raise ValueError("repeat instruction-context events differ")
        manifest_comparison["identical_instruction_events"] = len(current_rows)
    else:
        manifest_comparison = None
    result = audit_instruction_path(instruction, source, operand, lines)
    timing = audit_caller_input_windows(source["events"], operand["events"])
    result.update(rom_sha256=manifest["rom_identity"]["sha256"], mame_sha256=manifest["mame_sha256"],
                  manifest_comparison=manifest_comparison, independent_timing_audit=timing)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("run", type=Path)
    parser.add_argument("--baseline", type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(audit_run(args.run, args.baseline), indent=2))
    except (OSError, ValueError, json.JSONDecodeError) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
