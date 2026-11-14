"""Synthetic CPU-context logs; no firmware or private traces."""
import copy
import json
import subprocess
import tempfile
import sys
from pathlib import Path
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from pks_operand_context_audit import (BEGIN, MANIFEST_TYPES, NULLABLE_FIELDS,
                                       compare_run_manifests, parse_pks_operand_context)
from rom_inspect import SIZE, CRC32, SHA1


def event(seq, time_ns, address="237204", op="R", data="1234", mask="00ff"):
    regs = " ".join(f"er{i}={i + seq:08x}" for i in range(8))
    return (f"YAMANIX PKS_CONTEXT seq={seq} op={op} address={address} data={data} "
            f"mask={mask} pc=001fd8 time_ns={time_ns} ccr=80 {regs}")


def capture(events):
    return [f"YAMANIX {BEGIN}", *events,
            f"YAMANIX PKS_CONTEXT_END events={len(events)} dropped=0 hardware_validated=0"]


def manifest():
    # Metadata only;no ROM,bank or private capture is constructed by these tests.
    return {"command": ["baseline"], "mame_sha256": "a" * 64, "trace_lines": 10,
            "rom_identity": {"bytes": SIZE, "crc32": CRC32, "sha1": SHA1,
                             "sha256": "b" * 64, "matches_known_an1x_104": True},
            "returncode": 0, "midi_input": None, "dsp_profile": "passive",
            "dsp_hardware_validated": False, "dms_board": "absent", "vop3_bridge": None,
            "dsp_sample_execution": False, "dsp_sample_clock": "disabled",
            "dsp_sample_rate_hz": None, "eg_readback": "zero", "live_bridge_completed": False,
            "bridge_sha256": None, "complete_an1x_boot": False, "note": "synthetic metadata",
            "pc_samples": 5, "unique_sampled_pcs": [256,258], "h8_cpu_progress_observed": True,
            "read_addresses_observed": 0, "write_addresses_observed": 0,
            "dsp_mmio_addresses_observed": []}


class PksOperandContextAuditTests(unittest.TestCase):
    def test_research_patch_installs_the_tested_helper_and_low_ram_taps(self):
        root = Path(__file__).resolve().parents[1]
        patch = (root / "research/mame-pks-operand-context.patch").read_text()
        section = patch.split(
            "+++ b/src/mame/yamaha/yamanix_pks_operand_context_trace.h\n", 1)[1]
        embedded = "\n".join(line[1:] for line in section.splitlines()
                               if line.startswith("+")) + "\n"
        self.assertEqual(embedded, (root / "src/pks_operand_context_trace.h").read_text())
        self.assertIn("install_read_tap(0x2349e8, 0x237205", patch)
        self.assertIn("install_write_tap(0x2349e8, 0x237205", patch)

    def test_parses_ordered_register_snapshots_and_per_address_counts(self):
        result = parse_pks_operand_context([
            *capture([event(0, 10), event(1, 10, address="2349ea", op="R"),
                      event(2, 10, address="2349e8", op="W")]),
            "YAMANIX DSP_END events=0 dropped=0"])
        self.assertEqual(result["counts"]["237204"]["R"], 1)
        self.assertEqual(result["counts"]["2349ea"]["R"], 1)
        self.assertEqual(result["counts"]["2349e8"]["W"], 1)
        self.assertEqual(result["events"][0]["er"], list(range(8)))
        self.assertFalse(result["hardware_validated"])

    def test_static_caller_input_word_is_selected_without_neighbor_alias(self):
        result = parse_pks_operand_context(capture([
            event(0, 10, address="2349ea", op="R", data="12ab", mask="00ff")]))
        self.assertEqual(result["events"][0]["address"], 0x2349EA)
        self.assertEqual(result["events"][0]["data"], 0x12AB)
        self.assertEqual(result["counts"]["2349ea"]["R"], 1)
        broken = capture([event(0, 10, address="2349e9")])
        with self.assertRaisesRegex(ValueError, "outside observer contract"):
            parse_pks_operand_context(broken)

    def test_legacy_capture_has_no_observer_section(self):
        self.assertIsNone(parse_pks_operand_context(["YAMANIX PC tick=0 pc=000000"]))

    def test_run_comparison_allows_trace_only_deltas(self):
        baseline = manifest()
        observed = copy.deepcopy(baseline)
        observed.update(command=["instrumented"], mame_sha256="c" * 64, trace_lines=12)
        self.assertEqual(compare_run_manifests(baseline, observed), [])
        observed["pc_samples"] = 6
        self.assertEqual(compare_run_manifests(baseline, observed), ["pc_samples"])

    def test_empty_incomplete_failed_and_mistyped_manifests_are_rejected(self):
        good = manifest()
        invalid = [{}, [], None, {"returncode": 0}]
        for key in MANIFEST_TYPES.keys() | NULLABLE_FIELDS:
            incomplete = copy.deepcopy(good)
            del incomplete[key]
            invalid.append(incomplete)
        for key, value in (("returncode", 1), ("returncode", False), ("returncode", "0"),
                           ("h8_cpu_progress_observed", False), ("pc_samples", 0),
                           ("pc_samples", True), ("rom_identity", {}),
                           ("mame_sha256", "short"), ("command", []),
                           ("trace_lines", -1), ("dsp_profile", "unknown"),
                           ("dsp_sample_execution", True), ("dsp_sample_rate_hz", 48000),
                           ("vop3_bridge", "missing-evidence"), ("midi_input", {})):
            bad = copy.deepcopy(good)
            bad[key] = value
            invalid.append(bad)
        for bad in invalid:
            with self.subTest(manifest=bad):
                for baseline, observed in ((bad, good), (good, bad), (bad, bad)):
                    with self.assertRaises(ValueError):
                        compare_run_manifests(baseline, observed)

    def test_optional_field_absence_is_distinct_from_null(self):
        baseline, observed = manifest(), manifest()
        baseline["extra_observation"] = None
        self.assertEqual(compare_run_manifests(baseline, observed), ["extra_observation"])
        observed["extra_observation"] = None
        self.assertEqual(compare_run_manifests(baseline, observed), [])

    def test_complete_experimental_bridge_and_midi_metadata_is_accepted(self):
        good = manifest()
        good.update(dsp_profile="firmware-contract", vop3_bridge="synthetic-bridge",
                    bridge_sha256="d"*64, live_bridge_completed=True,
                    dsp_sample_clock="assumed-48k", dsp_sample_execution=True,
                    dsp_sample_rate_hz=48000, dsp_sample_passes=[42,43], eg_readback="an1x-level-experiment",
                    midi_input={"filename":"synthetic.mid", "bytes":42, "sha256":"e"*64})
        self.assertEqual(compare_run_manifests(good, copy.deepcopy(good)), [])
        for passes in (None, [], [1], [1,2,3], [0,1], [1,0], [-1,2],
                       [True,1], [1,"2"], [1,1.5]):
            bad = copy.deepcopy(good)
            if passes is None:
                del bad["dsp_sample_passes"]
            else:
                bad["dsp_sample_passes"] = passes
            for baseline, observed in ((bad,good), (good,bad), (bad,bad)):
                with self.subTest(passes=passes), self.assertRaises(ValueError):
                    compare_run_manifests(baseline,observed)
        bad = copy.deepcopy(good)
        bad["live_bridge_completed"] = False
        with self.assertRaises(ValueError):
            compare_run_manifests(good, bad)

    def test_cli_rejects_incomplete_or_failed_metadata_and_accepts_complete_pair(self):
        tool = Path(__file__).resolve().parents[1] / "tools/pks_operand_context_audit.py"
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            baseline, observed = root / "baseline", root / "observed"
            baseline.mkdir();observed.mkdir()
            (observed / "trace.log").write_text("\n".join(capture([event(0,10)])),encoding="utf-8")
            command = [sys.executable, str(tool), str(observed), "--baseline", str(baseline)]
            failed = manifest();failed["returncode"] = 1
            incomplete = manifest();del incomplete["dms_board"]
            clocked = manifest()
            clocked.update(dsp_profile="firmware-contract",vop3_bridge="bridge",bridge_sha256="c"*64,
                           live_bridge_completed=True,dsp_sample_clock="assumed-48k",
                           dsp_sample_execution=True,dsp_sample_rate_hz=48000)
            zero_chip = copy.deepcopy(clocked);zero_chip["dsp_sample_passes"]=[1,0]
            for bad in ({}, failed, incomplete, [], clocked, zero_chip):
                for directory in (baseline, observed):
                    (directory / "run.json").write_text(json.dumps(bad),encoding="utf-8")
                result = subprocess.run(command,capture_output=True,text=True,timeout=30)
                self.assertNotEqual(result.returncode,0,result.stdout)
                self.assertNotIn('"same_nontrace_observations": true',result.stdout)
            for directory in (baseline, observed):
                (directory / "run.json").write_text(json.dumps(manifest()),encoding="utf-8")
            result = subprocess.run(command,capture_output=True,text=True,timeout=30)
            self.assertEqual(result.returncode,0,result.stderr)
            comparison = json.loads(result.stdout)["manifest_comparison"]
            self.assertTrue(comparison["same_nontrace_observations"])
            self.assertGreater(comparison["compared_fields"],0)

    def test_malformed_or_lossy_streams_are_rejected(self):
        valid = capture([event(0, 10), event(1, 20, address="237202", op="W")])
        broken = [
            valid[1:], valid[:-1], valid + [valid[-1]],
            capture([event(1, 10)]), capture([event(0, 20), event(1, 10)]),
            capture([event(0, 10, address="237206")]),
            capture([event(0, 10, mask="0000")]),
            [line.replace("er7=00000007", "er7=0000000") for line in valid],
            [line.replace("events=2", "events=3") for line in valid],
            [line.replace("dropped=0", "dropped=1") for line in valid],
            [line.replace("version=1", "version=2") for line in valid],
        ]
        for lines in broken:
            with self.subTest(lines=lines), self.assertRaises(ValueError):
                parse_pks_operand_context(lines)

    def test_capture_bound_is_enforced(self):
        events = [event(seq, seq) for seq in range(4096)]
        self.assertEqual(len(parse_pks_operand_context(capture(events))["events"]), 4096)
        with self.assertRaises(ValueError):
            parse_pks_operand_context(capture(events + [event(4096, 4096)]))


if __name__ == "__main__":
    unittest.main()
