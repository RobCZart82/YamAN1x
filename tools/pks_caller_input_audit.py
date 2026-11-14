"""Audit bounded timing windows between two static PKS input reads and writer stores."""
import argparse
import json
from pathlib import Path

from pks_operand_context_audit import compare_run_manifests, parse_pks_operand_context
from pks_source_context_audit import (
    SOURCE_WRITER_POSTFETCH_PC,
    parse_pks_source_context,
    validate_local_rom_identity,
)

MAX_INPUT_AGE_NS = 50_000
WRITER_LANES = ((0xFF00, 0x00FF, 0xFF00), (0x00FF, 0xFF00, 0x00FF))
WRITER_ADDRESSES = (0x234ACC, 0x234ACC, 0x234ACE)
CALLER_WINDOWS = (
    dict(first_writer=0, input_address=0x2349EA, input_pc=0x002A08, input_mask=0xFF00),
    dict(first_writer=3, input_address=0x237204, input_pc=0x001FD8, input_mask=0x00FF),
)


def _lane_value(event):
    if event["mask"] == 0xFF00:
        return event["data"] >> 8
    if event["mask"] == 0x00FF:
        return event["data"] & 0xFF
    raise ValueError("caller-input event is not a single-byte lane read")


def audit_caller_input_windows(source_events, operand_events):
    """Pair six observed writer bytes with their nearest bounded static-read windows.

    This reports temporal proximity only. It does not assert that either read
    caused, computed or semantically represents a writer byte.
    """
    writers = [event for event in source_events
               if event["op"] == "W" and event["pc"] == SOURCE_WRITER_POSTFETCH_PC
               and event["address"] in {0x234ACC, 0x234ACE, 0x234AD0}]
    if len(writers) != 6:
        raise ValueError("expected the six ordered source-writer byte events")
    if any(event["time_ns"] >= following["time_ns"]
           for event, following in zip(writers, writers[1:])):
        raise ValueError("source-writer continuation times are not strictly increasing")

    windows = []
    for caller in CALLER_WINDOWS:
        start = caller["first_writer"]
        group = writers[start:start + 3]
        if len(group) != 3:
            raise ValueError("caller window does not contain three writer events")
        expected_addresses = WRITER_ADDRESSES if start == 0 else (0x234ACE, 0x234AD0, 0x234AD0)
        if tuple(event["address"] for event in group) != expected_addresses:
            raise ValueError("source-writer address continuation changed")
        if tuple(event["mask"] for event in group) != WRITER_LANES[start // 3]:
            raise ValueError("source-writer byte-lane continuation changed")

        candidates = [event for event in operand_events
                      if event["op"] == "R"
                      and event["address"] == caller["input_address"]
                      and event["pc"] == caller["input_pc"]
                      and event["mask"] == caller["input_mask"]
                      and event["time_ns"] < group[0]["time_ns"]
                      and group[0]["time_ns"] - event["time_ns"] <= MAX_INPUT_AGE_NS]
        if not candidates:
            raise ValueError("no matching static-input read precedes the writer window")
        read = max(candidates, key=lambda event: event["time_ns"])
        observations = []
        for writer in group:
            elapsed = writer["time_ns"] - read["time_ns"]
            if elapsed <= 0 or elapsed > MAX_INPUT_AGE_NS:
                raise ValueError("static-input read falls outside the bounded writer window")
            if any(event["op"] == "R"
                   and event["address"] == caller["input_address"]
                   and event["pc"] == caller["input_pc"]
                   and read["time_ns"] < event["time_ns"] <= writer["time_ns"]
                   for event in operand_events):
                raise ValueError("a later matching input read makes the window ambiguous")
            value = _lane_value(writer)
            if writer["mask"] == 0xFF00:
                byte_address = writer["address"]
            else:
                byte_address = writer["address"] + 1
            observations.append(dict(
                writer_seq=writer["seq"], writer_byte_address=byte_address,
                writer_value=value, writer_ccr=writer["ccr"],
                writer_time_ns=writer["time_ns"], elapsed_ns=elapsed))
        windows.append(dict(
            static_input_word_base=caller["input_address"],
            static_input_pc=caller["input_pc"], input_read_seq=read["seq"],
            input_data=read["data"], input_mask=read["mask"],
            input_byte_value=_lane_value(read), input_ccr=read["ccr"],
            input_time_ns=read["time_ns"], observations=observations))
    return dict(windows=windows, complete=True, hardware_validated=False,
                note="Static-read/write timing windows only; no data-flow causality, meaning or hardware agreement established.")


def audit_run(run, baseline=None):
    run = Path(run)
    lines = (run / "trace.log").read_text(errors="replace").splitlines()
    source = parse_pks_source_context(lines)
    operand = parse_pks_operand_context(lines)
    if source is None or operand is None:
        raise ValueError("run must contain complete PKS source and operand context sections")
    if not source["events"] or not source["source_to_payload_transfers"]:
        raise ValueError("run has no independently matched source-to-payload transfers")
    manifest = validate_local_rom_identity(run)
    comparison = None
    if baseline is not None:
        reference = Path(baseline)
        baseline_manifest = validate_local_rom_identity(reference)
        differences = compare_run_manifests(baseline_manifest, manifest)
        if differences:
            raise ValueError("non-trace run fields changed: " + ", ".join(differences))
        comparison = dict(
            same_nontrace_observations=True,
            compared_fields=len((set(baseline_manifest) | set(manifest)) -
                                {"command", "mame_sha256", "trace_lines"}))
    result = audit_caller_input_windows(source["events"], operand["events"])
    result.update(mame_sha256=manifest["mame_sha256"],
                  rom_sha256=manifest["rom_identity"]["sha256"],
                  manifest_comparison=comparison)
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
