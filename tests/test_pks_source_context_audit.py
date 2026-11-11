"""Synthetic source-context logs; no firmware or private traces."""
import subprocess
import json
import tempfile
import sys
from pathlib import Path
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from pks_source_context_audit import (ADDRESSES, BEGIN, parse_pks_source_context,
                                      validate_local_rom_identity,
                                      verify_source_to_payload_transfers)
from rom_inspect import SHA1
from test_pks_operand_context_audit import manifest as complete_manifest


def event(seq, time_ns, address="234ace", op="R", data="1234", mask="00ff"):
    regs = " ".join(f"er{i}={i + seq:08x}" for i in range(8))
    return (f"YAMANIX PKS_SOURCE_CONTEXT seq={seq} op={op} address={address} data={data} "
            f"mask={mask} pc=057d42 time_ns={time_ns} ccr=80 {regs}")


def capture(events):
    return [f"YAMANIX {BEGIN}", *events,
            f"YAMANIX PKS_SOURCE_CONTEXT_END events={len(events)} dropped=0 hardware_validated=0"]


def manifest():
    return complete_manifest()


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
        source_write = event(0, 50, address="234acc", op="W", data="e000", mask="ff00")
        source = event(1, 100, address="234acc", data="e000", mask="ff00")
        payload = event(2, 350, address="22d9ee", op="W", data="00e0", mask="00ff")
        source = source.replace("pc=057d42", "pc=00fa92").replace("er0=00000001", "er0=00000000").replace(
            "er1=00000002", "er1=000000e0").replace(
            "er4=00000005", "er4=0022d9ef").replace("ccr=80", "ccr=25")
        payload = payload.replace("pc=057d42", "pc=00fa94").replace("er0=00000002", "er0=000000e0").replace(
            "er4=00000006", "er4=0022d9ef").replace("ccr=80", "ccr=29")
        parsed = parse_pks_source_context(capture([source_write, source, payload]))
        transfer, = verify_source_to_payload_transfers(parsed["events"])
        self.assertEqual((transfer["value"], transfer["source_address"],
                          transfer["source_ccr"], transfer["payload_ccr"],
                          transfer["source_byte_address"], transfer["source_write_seq"],
                          transfer["source_write_time_ns"], transfer["time_delta_ns"]),
                         (0xe0, 0x234acc, 0x25, 0x29, 0x234acc, 0, 50, 250))
        self.assertEqual(transfer["source_write_ccr"], 0x80)
        self.assertEqual(transfer["source_write_er"], list(range(8)))
        # Report the writer snapshot,not the later read/write register values.
        self.assertNotEqual(transfer["source_write_er"], parsed["events"][1]["er"])
        parsed["events"][0]["er"][0] = 0xffffffff
        self.assertEqual(transfer["source_write_er"][0], 0)
        broken_value = parsed["events"][:2] + [dict(parsed["events"][2], data=0x00e1)]
        with self.assertRaises(ValueError):
            verify_source_to_payload_transfers(broken_value)
        wrong_r0l = list(parsed["events"][2]["er"])
        wrong_r0l[0] = 0
        broken_register = parsed["events"][:2] + [dict(parsed["events"][2], er=wrong_r0l)]
        with self.assertRaises(ValueError):
            verify_source_to_payload_transfers(broken_register)
        broken_time = parsed["events"][:2] + [dict(parsed["events"][2], time_ns=100)]
        with self.assertRaises(ValueError):
            verify_source_to_payload_transfers(broken_time)

    def test_source_read_must_match_the_latest_prior_write_to_its_byte_lane(self):
        first = event(0, 10, address="234acc", op="W", data="e000", mask="ff00")
        latest = event(1, 20, address="234acc", op="W", data="e100", mask="ff00")
        source = event(2, 30, address="234acc", data="e000", mask="ff00")
        source = source.replace("pc=057d42", "pc=00fa92").replace("er0=00000002", "er0=00000000")
        payload = event(3, 40, address="22d9ee", op="W", data="0000", mask="00ff")
        payload = payload.replace("pc=057d42", "pc=00fa94").replace("er0=00000003", "er0=00000000")
        with self.assertRaisesRegex(ValueError, "most recent preceding lane write"):
            verify_source_to_payload_transfers(parse_pks_source_context(
                capture([first, latest, source, payload]))["events"])

    def test_word_write_tracks_the_odd_source_byte_lane(self):
        word_write = event(0, 10, address="234acc", op="W", data="e0e1", mask="ffff")
        source = event(1, 20, address="234acc", data="00e1", mask="00ff")
        source = source.replace("pc=057d42", "pc=00fa92").replace("er4=00000005", "er4=0022d9ef")
        payload = event(2, 40, address="22d9ee", op="W", data="00e1", mask="00ff")
        payload = payload.replace("pc=057d42", "pc=00fa94").replace("er0=00000002", "er0=000000e1").replace(
            "er4=00000006", "er4=0022d9ef")
        parsed = parse_pks_source_context(capture([word_write, source, payload]))
        transfer, = parsed["source_to_payload_transfers"]
        self.assertEqual((transfer["source_byte_address"], transfer["source_write_seq"],
                          transfer["source_write_mask"], transfer["value"]),
                         (0x234ACD, 0, 0xFFFF, 0xE1))

    def test_source_read_requires_prior_write_and_matching_lane(self):
        source = event(0, 20, address="234acc", data="00e0", mask="00ff")
        source = source.replace("pc=057d42", "pc=00fa92").replace("er0=00000000", "er0=00000001")
        payload = event(1, 30, address="22d9ee", op="W", data="00e0", mask="00ff")
        payload = payload.replace("pc=057d42", "pc=00fa94").replace("er0=00000001", "er0=000000e0")
        with self.assertRaisesRegex(ValueError, "no preceding write"):
            verify_source_to_payload_transfers(parse_pks_source_context(
                capture([source, payload]))["events"])

        wrong_lane = event(0, 10, address="234acc", op="W", data="e000", mask="ff00")
        wrong_read = event(1, 20, address="234acc", data="00e0", mask="00ff")
        wrong_read = wrong_read.replace("pc=057d42", "pc=00fa92").replace("er0=00000001", "er0=00000000")
        wrong_payload = event(2, 30, address="22d9ee", op="W", data="00e0", mask="00ff")
        wrong_payload = wrong_payload.replace("pc=057d42", "pc=00fa94").replace("er0=00000002", "er0=000000e0")
        with self.assertRaisesRegex(ValueError, "lane does not match"):
            parse_pks_source_context(capture([wrong_lane, wrong_read, wrong_payload]))

    def test_source_index_selects_the_observed_aligned_word(self):
        write = event(0, 10, address="234ace", op="W", data="00e0", mask="00ff")
        source = event(1, 20, address="234ace", data="00e0", mask="00ff")
        source = source.replace("pc=057d42", "pc=00fa92").replace("er0=00000001", "er0=00000000")
        source = source.replace("mask=00ff", "mask=ff00").replace("data=00e0", "data=e000")
        with self.assertRaisesRegex(ValueError, "index does not select"):
            verify_source_to_payload_transfers(
                parse_pks_source_context(capture([write, source]))["events"])

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

    def test_source_cli_rejects_failed_or_partial_run_without_baseline(self):
        tool = Path(__file__).resolve().parents[1] / "tools/pks_source_context_audit.py"
        write = event(0,10,address="234acc",op="W",data="e000",mask="ff00")
        read = event(1,20,address="234acc",data="e000",mask="ff00")
        read = read.replace("pc=057d42","pc=00fa92").replace("er0=00000001","er0=00000000").replace("er4=00000005","er4=0022d9ef")
        payload = event(2,30,address="22d9ee",op="W",data="00e0",mask="00ff")
        payload = payload.replace("pc=057d42","pc=00fa94").replace("er0=00000002","er0=000000e0").replace("er4=00000006","er4=0022d9ef")
        with tempfile.TemporaryDirectory() as temporary:
            run = Path(temporary)
            (run / "trace.log").write_text("\n".join(capture([write,read,payload])),encoding="utf-8")
            failed = manifest();failed["returncode"] = 1
            partial = {"rom_identity": manifest()["rom_identity"]}
            for record in (partial,failed,[],{},None,manifest()):
                (run / "run.json").write_text(json.dumps(record),encoding="utf-8")
                result = subprocess.run([sys.executable,str(tool),str(run)],capture_output=True,text=True,timeout=30)
                if record == manifest():
                    self.assertEqual(result.returncode,0,result.stderr)
                    self.assertEqual(len(json.loads(result.stdout)["source_to_payload_transfers"]),1)
                else:
                    self.assertNotEqual(result.returncode,0,result.stdout)
                    self.assertNotIn('"complete": true',result.stdout)

    def test_rom_manifest_identity_is_checked_without_opening_rom(self):
        with tempfile.TemporaryDirectory() as temporary:
            run = Path(temporary)
            (run / "run.json").write_text(json.dumps(manifest()), encoding="utf-8")
            self.assertEqual(validate_local_rom_identity(run)["rom_identity"]["sha1"], SHA1)
            bad = manifest()
            bad["rom_identity"]["sha256"] = "unknown"
            (run / "run.json").write_text(json.dumps(bad), encoding="utf-8")
            with self.assertRaises(ValueError):
                validate_local_rom_identity(run)


if __name__ == "__main__":
    unittest.main()
