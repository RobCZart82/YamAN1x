"""Synthetic CPU-context logs; no firmware or private traces."""
import sys
from pathlib import Path
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from pks_operand_context_audit import BEGIN, compare_run_manifests, parse_pks_operand_context


def event(seq, time_ns, address="237204", op="R", data="1234", mask="00ff"):
    regs = " ".join(f"er{i}={i + seq:08x}" for i in range(8))
    return (f"YAMANIX PKS_CONTEXT seq={seq} op={op} address={address} data={data} "
            f"mask={mask} pc=001fd8 time_ns={time_ns} ccr=80 {regs}")


def capture(events):
    return [f"YAMANIX {BEGIN}", *events,
            f"YAMANIX PKS_CONTEXT_END events={len(events)} dropped=0 hardware_validated=0"]


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
            *capture([event(0, 10), event(1, 10, address="2349e8", op="W")]),
            "YAMANIX DSP_END events=0 dropped=0"])
        self.assertEqual(result["counts"]["237204"]["R"], 1)
        self.assertEqual(result["counts"]["2349e8"]["W"], 1)
        self.assertEqual(result["events"][0]["er"], list(range(8)))
        self.assertFalse(result["hardware_validated"])

    def test_legacy_capture_has_no_observer_section(self):
        self.assertIsNone(parse_pks_operand_context(["YAMANIX PC tick=0 pc=000000"]))

    def test_run_comparison_allows_trace_only_deltas(self):
        baseline = {"command": ["base"], "mame_sha256": "a", "trace_lines": 10,
                    "rom_identity": {"sha256": "locked"}, "pc_samples": 5}
        observed = {"command": ["new"], "mame_sha256": "b", "trace_lines": 12,
                    "rom_identity": {"sha256": "locked"}, "pc_samples": 5}
        self.assertEqual(compare_run_manifests(baseline, observed), [])
        observed["pc_samples"] = 6
        self.assertEqual(compare_run_manifests(baseline, observed), ["pc_samples"])

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
