"""Synthetic source-context logs; no firmware or private traces."""
import json
import tempfile
import sys
from pathlib import Path
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from pks_source_context_audit import (ADDRESSES, BEGIN, parse_pks_source_context,
                                      validate_local_rom_identity,
                                      verify_source_to_payload_transfers)
from rom_inspect import SIZE, CRC32, SHA1


def event(seq, time_ns, address="234ace", op="R", data="1234", mask="00ff"):
    regs = " ".join(f"er{i}={i + seq:08x}" for i in range(8))
    return (f"YAMANIX PKS_SOURCE_CONTEXT seq={seq} op={op} address={address} data={data} "
            f"mask={mask} pc=057d42 time_ns={time_ns} ccr=80 {regs}")


def capture(events):
    return [f"YAMANIX {BEGIN}", *events,
            f"YAMANIX PKS_SOURCE_CONTEXT_END events={len(events)} dropped=0 hardware_validated=0"]


def manifest():
    return {"rom_identity": {"bytes": SIZE, "crc32": CRC32, "sha1": SHA1,
                             "sha256": "b" * 64, "matches_known_an1x_104": True}}


class PksSourceContextAuditTests(unittest.TestCase):
    def test_patch_embeds_the_tested_helper(self):
        root = Path(__file__).resolve().parents[1]
        patch = (root / "research/mame-pks-source-context.patch").read_text()
        section = patch.split(
            "+++ b/src/mame/yamaha/yamanix_pks_source_context_trace.h\n", 1)[1]
        embedded = "\n".join(line[1:] for line in section.splitlines()
                               if line.startswith("+")) + "\n"
        self.assertEqual(embedded, (root / "src/pks_source_context_trace.h").read_text())
        self.assertIn("const offs_t pks_source_words[] = {0x22d9ee, 0x23f7fc};", patch)
        self.assertIn("install_read_tap(pks_source_words[i], pks_source_words[i]", patch)
        self.assertIn("pks_source_context('R', address, data, mask)", patch)

    def test_addresses_are_the_exact_word_bases(self):
        self.assertEqual(ADDRESSES, (0x22D9EE, 0x234ACC, 0x234ACE, 0x234AD0, 0x23F7FC))

    def test_parses_ordered_cpu_context_and_counts(self):
        result = parse_pks_source_context(capture([
            event(0, 10, address="22d9ee", op="W", data="00e0"),
            event(1, 20, address="234acc", data="00e0"),
            event(2, 30, address="23f7fc", data="0001")]))
        self.assertEqual(result["counts"]["22d9ee"]["W"], 1)
        self.assertEqual(result["counts"]["234acc"]["R"], 1)
        self.assertEqual(result["events"][0]["er"], list(range(8)))
        self.assertFalse(result["hardware_validated"])

    def test_independent_source_value_register_flag_and_timing_link(self):
        source = event(0, 100, address="234acc", data="e000", mask="ff00")
        payload = event(1, 350, address="22d9ee", op="W", data="00e0", mask="00ff")
        source = source.replace("pc=057d42", "pc=00fa92").replace("er0=00000000", "er0=000000e0").replace(
            "er4=00000004", "er4=0022d9ef").replace("ccr=80", "ccr=25")
        payload = payload.replace("pc=057d42", "pc=00fa94").replace("er0=00000001", "er0=000000e0").replace(
            "er4=00000005", "er4=0022d9ef").replace("ccr=80", "ccr=29")
        parsed = parse_pks_source_context(capture([source, payload]))
        transfer, = verify_source_to_payload_transfers(parsed["events"])
        self.assertEqual((transfer["value"], transfer["source_address"],
                          transfer["source_ccr"], transfer["payload_ccr"],
                          transfer["time_delta_ns"]), (0xe0, 0x234acc, 0x25, 0x29, 250))
        broken_value = [parsed["events"][0], dict(parsed["events"][1], data=0x00e1)]
        with self.assertRaises(ValueError):
            verify_source_to_payload_transfers(broken_value)
        wrong_r0l = list(parsed["events"][1]["er"])
        wrong_r0l[0] = 0
        broken_register = [parsed["events"][0], dict(parsed["events"][1], er=wrong_r0l)]
        with self.assertRaises(ValueError):
            verify_source_to_payload_transfers(broken_register)
        broken_time = [parsed["events"][0], dict(parsed["events"][1], time_ns=100)]
        with self.assertRaises(ValueError):
            verify_source_to_payload_transfers(broken_time)

    def test_legacy_run_has_no_observer_section(self):
        self.assertIsNone(parse_pks_source_context(["YAMANIX PC tick=0 pc=000000"]))

    def test_rejects_missing_repeated_malformed_lossy_or_unordered_sections(self):
        valid = capture([event(0, 10), event(1, 20, address="234ad0")])
        broken = [valid[1:], valid[:-1], valid + [valid[-1]],
                  capture([event(1, 10)]),
                  capture([event(0, 20), event(1, 10)]),
                  capture([event(0, 10, address="234acd")]),
                  capture([event(0, 10, mask="0000")]),
                  [line.replace("er7=00000007", "er7=0000000") for line in valid],
                  [line.replace("events=2", "events=3") for line in valid],
                  [line.replace("dropped=0", "dropped=1") for line in valid],
                  [line.replace("version=1", "version=2") for line in valid]]
        for lines in broken:
            with self.subTest(lines=lines), self.assertRaises(ValueError):
                parse_pks_source_context(lines)

    def test_capacity_is_enforced(self):
        events = [event(seq, seq) for seq in range(4096)]
        self.assertEqual(len(parse_pks_source_context(capture(events))["events"]), 4096)
        with self.assertRaises(ValueError):
            parse_pks_source_context(capture(events + [event(4096, 4096)]))

    def test_rom_manifest_identity_is_checked_without_opening_rom(self):
        with tempfile.TemporaryDirectory() as temporary:
            run = Path(temporary)
            (run / "run.json").write_text(json.dumps({"rom_identity": manifest()["rom_identity"]}), encoding="utf-8")
            self.assertEqual(validate_local_rom_identity(run)["rom_identity"]["sha1"], SHA1)
            bad = manifest()
            bad["rom_identity"]["sha256"] = "unknown"
            (run / "run.json").write_text(json.dumps(bad), encoding="utf-8")
            with self.assertRaises(ValueError):
                validate_local_rom_identity(run)


if __name__ == "__main__":
    unittest.main()
