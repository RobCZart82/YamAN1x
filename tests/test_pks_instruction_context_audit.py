"""Synthetic bounded instruction traces; no firmware, ROM or private logs."""
import copy
import re
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from pks_instruction_context_audit import (SITES, STATIC_INSTRUCTIONS,
                                           audit_instruction_path,
                                           audit_preceding_input_writes,
                                           parse_instruction_context,
                                           parse_static_disassembly)

TEST_PROFILE = dict(capacity=16384, post=2048, preroll=2048)


def _event(seq, window, pc, time_ns, r=None, er=None, ccr=0x80):
    r = list(r or [0] * 8)
    er = list(er or [value for value in r] + [0] * 8)
    return dict(seq=seq, window=window, pc=pc, opcode=0x1234, time_ns=time_ns,
                ccr=ccr, r=r, er=er)


def synthetic_capture():
    lines = ["YAMANIX PKS_INSTRUCTION_CONTEXT_BEGIN version=2 capacity=16384 post=2048 preroll=2048 hardware_validated=0",
             "YAMANIX PKS_INSTRUCTION_CONTEXT_WINDOWS count=4 events=16384 dropped=0 finished=1 hardware_validated=0"]
    all_events = []
    addresses = (0x2349EA, 0x237204, 0x2349EA, 0x237204)
    for win, address in enumerate(addresses):
        site = SITES[address]
        start = win * 4096
        tm = 100000 + win * 10000
        data = (0x4000 if win == 0 else 0xFF00 if win == 1 else 0 if win == 2 else 0xFFFF)
        mask = site["mask"]
        lines.append(f"YAMANIX PKS_INSTRUCTION_WINDOW id={win} trigger_seq={win} address={address:06x} "
                     f"pc={site['pc']:06x} data={data:04x} mask={mask:04x} ccr=80 time_ns={tm} "
                     f"first={start} preroll=2048 count=4096 complete=1")
        for i in range(2048):
            pc = 0x004000 + i * 2
            event = _event(start + i, win, pc, tm - 2048 + i)
            all_events.append(event)
            lines.append(
                f"YAMANIX PKS_INSTRUCTION seq={event['seq']} window={win} pc={pc:06x} opcode=1234 "
                f"time_ns={event['time_ns']} ccr={event['ccr']:02x} " +
                " ".join(f"r{n}={event['r'][n]:04x}" for n in range(8)) + " " +
                " ".join(f"er{n}={event['er'][n]:08x}" for n in range(8)))
        pcs = list(site["path"]) if win < 2 else [site["pc"]]
        for i in range(2048):
            pc = pcs[i] if i < len(pcs) else 0x006000 + i * 2
            regs = [0] * 8
            if i == 0:
                regs[site["data_register"]] = (data >> 8 if mask == 0xFF00 else data) & 0xFF
            if win == 0 and pc == 0x002A54:
                regs[1] = 0x20
            if win == 0 and pc == 0x002A64:
                regs[1] = 0
            if win == 0 and pc == 0x002A66:
                regs[0] = 0xE0
            if win == 1 and pc == 0x001FDE:
                regs[2] = 0
            if win == 1 and pc == 0x001FF2:
                regs[0] = 0xE0
            writer_index = (i - (len(pcs) - 1)) // 20
            if win < 2 and i >= len(pcs) - 1 and i <= len(pcs) - 1 + 40 and (i - (len(pcs) - 1)) % 20 == 0:
                # Three writer records align with the independent source bus callbacks.
                pc = 0x0039AC
                values = ((0xE0, 0x00, 0x20), (0xE0, 0x00, 0x00))[win]
                regs[1] = values[writer_index]
            event = _event(start + 2048 + i, win, pc, tm + i,
                           regs, [value for value in regs] + [0] * 8,
                           ccr=(0xA9, 0xA5, 0xA1)[writer_index] if win < 2 and
                           pc == 0x0039AC else 0x80)
            all_events.append(event)
            lines.append(
                f"YAMANIX PKS_INSTRUCTION seq={event['seq']} window={win} pc={pc:06x} opcode=1234 "
                f"time_ns={event['time_ns']} ccr={event['ccr']:02x} " +
                " ".join(f"r{i}={event['r'][i]:04x}" for i in range(8)) + " " +
                " ".join(f"er{i}={event['er'][i]:08x}" for i in range(8)))
    lines.append("YAMANIX PKS_SOURCE_CONTEXT_END events=0 dropped=0 hardware_validated=0")
    return lines, all_events


class PksInstructionContextAuditTests(unittest.TestCase):
    def test_parses_four_complete_ordered_windows(self):
        lines, _ = synthetic_capture()
        result = parse_instruction_context(lines, TEST_PROFILE)
        self.assertEqual((len(result["windows"]), len(result["events"])), (4, 16384))
        self.assertEqual(result["windows"][0]["pre_roll"], 2048)
        self.assertEqual(result["windows"][1]["address"], 0x237204)
        self.assertFalse(result["hardware_validated"])

    def test_default_parser_requires_wider_candidate_profile(self):
        lines, _ = synthetic_capture()
        with self.assertRaisesRegex(ValueError, "bounds differ from the pinned profile"):
            parse_instruction_context(lines)

    def test_pre_roll_must_be_full_and_straddle_the_input_read(self):
        lines, _ = synthetic_capture()
        lines[2] = lines[2].replace("preroll=2048", "preroll=2047")
        with self.assertRaisesRegex(ValueError, "bounds, order"):
            parse_instruction_context(lines, TEST_PROFILE)
        lines, _ = synthetic_capture()
        lines[2050] = lines[2050].replace("time_ns=99999", "time_ns=100001")
        lines[2051] = lines[2051].replace("time_ns=100000", "time_ns=100001")
        with self.assertRaisesRegex(ValueError, "pre/post timing"):
            parse_instruction_context(lines, TEST_PROFILE)

    def test_rejects_lossy_footer(self):
        lines, _ = synthetic_capture()
        lines[1] = lines[1].replace("dropped=0", "dropped=1")
        with self.assertRaisesRegex(ValueError, "loss or incomplete"):
            parse_instruction_context(lines, TEST_PROFILE)
        lines, _ = synthetic_capture()
        lines[2] = lines[2].replace("mask=ff00", "mask=00ff")
        with self.assertRaisesRegex(ValueError, "static caller contract"):
            parse_instruction_context(lines, TEST_PROFILE)

    def test_rejects_duplicate_sequence_and_wrong_register_composition(self):
        lines, _ = synthetic_capture()
        lines[4] = lines[4].replace("seq=1 ", "seq=0 ", 1)
        with self.assertRaisesRegex(ValueError, "sequence"):
            parse_instruction_context(lines, TEST_PROFILE)
        lines, _ = synthetic_capture()
        lines[3] = lines[3].replace("er0=00000000", "er0=00000001", 1)
        with self.assertRaisesRegex(ValueError, "ER low word"):
            parse_instruction_context(lines, TEST_PROFILE)

    def test_rejects_nonmonotonic_time_and_records_after_completion(self):
        lines, _ = synthetic_capture()
        lines[4] = lines[4].replace("time_ns=97953", "time_ns=97951")
        with self.assertRaisesRegex(ValueError, "time moved backwards within a window"):
            parse_instruction_context(lines, TEST_PROFILE)
        lines, _ = synthetic_capture()
        lines.append(lines[-2].replace("seq=16383", "seq=16384"))
        with self.assertRaisesRegex(ValueError, "record after completion"):
            parse_instruction_context(lines, TEST_PROFILE)

    def test_overlapping_capture_windows_remain_individually_ordered(self):
        lines, _ = synthetic_capture()
        for index, line in enumerate(lines):
            if ("PKS_INSTRUCTION_WINDOW id=1 " in line or
                    "PKS_INSTRUCTION seq=" in line and "window=1 " in line):
                lines[index] = re.sub(r"time_ns=(\d+)",
                                      lambda match: f"time_ns={int(match.group(1)) - 7000}",
                                      line)
        result = parse_instruction_context(lines, TEST_PROFILE)
        self.assertLess(result["events"][4096]["time_ns"],
                        result["events"][4095]["time_ns"])

    def test_static_instruction_inventory_is_exact(self):
        lines = [f"YAMANIX DASM pc={address:06x} {instruction}"
                 for address, instruction in STATIC_INSTRUCTIONS.items()]
        result = parse_static_disassembly(lines)
        self.assertEqual(len(result), len(STATIC_INSTRUCTIONS))
        lines[0] = lines[0].replace("mov.b", "mov.w")
        with self.assertRaisesRegex(ValueError, "static instruction mismatch"):
            parse_static_disassembly(lines)

    def test_dynamic_input_register_path_and_writer_snapshots(self):
        lines, events = synthetic_capture()
        windows = []
        for win, address in enumerate((0x2349EA, 0x237204, 0x2349EA, 0x237204)):
            site = SITES[address]
            start, tm = win * 4096, 100000 + win * 10000
            windows.append(dict(id=win, address=address, pc=site["pc"],
                                data=(0x4000 if win == 0 else 0xFF00 if win == 1 else
                                      0 if win == 2 else 0xFFFF),
                                mask=site["mask"], ccr=0x80, time_ns=tm,
                                first=start, pre_roll=2048, count=4096, complete=True))
        source = dict(events=[])
        values = ((0xE0, 0, 0x20), (0xE0, 0, 0))
        addresses = ((0x234ACC, 0x234ACC, 0x234ACE), (0x234ACE, 0x234AD0, 0x234AD0))
        masks = ((0xFF00, 0x00FF, 0xFF00), (0x00FF, 0xFF00, 0x00FF))
        for win in range(2):
            for i in range(3):
                index = (win * 4096) + 2048 + len(SITES[windows[win]["address"]]["path"]) - 1 + i * 20
                cpu = events[index]
                byte = values[win][i]
                data = byte << 8 if masks[win][i] == 0xFF00 else byte
                source["events"].append(dict(op="W", pc=0x0039AC, address=addresses[win][i],
                    data=data, mask=masks[win][i], time_ns=cpu["time_ns"], ccr=cpu["ccr"], er=cpu["er"][:]))
        prior_cpu = events[0]
        prior_write = dict(seq=0, op="W", address=0x2349EA, data=0x4000, mask=0xFF00,
                           pc=prior_cpu["pc"], time_ns=prior_cpu["time_ns"],
                           ccr=prior_cpu["ccr"], er=prior_cpu["er"][:])
        later_cpu = events[10239]
        later_write = dict(seq=1, op="W", address=0x2349EA, data=0, mask=0xFF00,
                           pc=later_cpu["pc"], time_ns=later_cpu["time_ns"],
                           ccr=later_cpu["ccr"], er=later_cpu["er"][:])
        operand = dict(events=[prior_write, later_write] + [
            dict(seq=i + 2, op="R", address=w["address"], pc=w["pc"], mask=w["mask"],
                 data=w["data"], ccr=w["ccr"], time_ns=w["time_ns"])
            for i, w in enumerate(windows)])
        instruction = dict(windows=windows, events=events)
        static = [f"YAMANIX DASM pc={address:06x} {value}"
                  for address, value in STATIC_INSTRUCTIONS.items()]
        result = audit_instruction_path(instruction, source, operand, static)
        self.assertEqual(len(result["pre_roll_windows"]), 4)
        self.assertEqual(result["pre_roll_windows"][0]["instruction_count"], 2048)
        self.assertEqual(result["pre_roll_windows"][0]["input_address"], 0x2349EA)
        self.assertFalse(result["pre_roll_windows"][0]["raw_ccr_interpreted"])
        self.assertEqual([item["writer_count"] for item in result["windows"]], [3, 3])
        self.assertEqual([item["preceding_input_write"]["status"] for item in result["windows"]],
                         ["last preceding write agrees", "no preceding write observed"])
        self.assertEqual(result["windows"][0]["preceding_instruction_snapshot"]["status"],
                         "CPU and operand-context snapshots agree")
        bad_context = copy.deepcopy(instruction)
        bad_context["events"][0]["er"][0] ^= 1
        bad_context["events"][0]["r"][0] ^= 1
        with self.assertRaisesRegex(ValueError, "preceding write CPU snapshot differs"):
            audit_instruction_path(bad_context, source, operand, static)
        self.assertFalse(result["windows"][0]["raw_ccr_interpreted"])
        bad_input = copy.deepcopy(instruction)
        bad_input["events"][2048]["r"][1] ^= 1
        with self.assertRaisesRegex(ValueError, "first post-read instruction"):
            audit_instruction_path(bad_input, source, operand, static)
        bad_path = copy.deepcopy(instruction)
        bad_path["events"][2049]["pc"] = 0x002A10
        with self.assertRaisesRegex(ValueError, "ordered path PC 002a0e"):
            audit_instruction_path(bad_path, source, operand, static)
        bad = copy.deepcopy(source)
        bad["events"][0]["ccr"] ^= 1
        with self.assertRaisesRegex(ValueError, "snapshots disagree"):
            audit_instruction_path(instruction, bad, operand, static)

    def test_last_preceding_word_write_checks_selected_high_lane(self):
        read = dict(seq=3, op="R", address=0x2349EA, data=0x4000,
                    mask=0xFF00, time_ns=30)
        writes = [
            dict(seq=0, op="W", address=0x2349EA, data=0x1000,
                 mask=0xFFFF, time_ns=10),
            dict(seq=1, op="W", address=0x2349EA, data=0x4000,
                 mask=0xFF00, time_ns=20),
            dict(seq=2, op="W", address=0x2349EA, data=0x00AA,
                 mask=0x00FF, time_ns=25),
        ]
        result = audit_preceding_input_writes(read, writes)
        self.assertEqual(result["status"], "last preceding write agrees")
        self.assertEqual(result["write_seq"], 1)
        self.assertEqual(result["selected_byte"], 0x40)

    def test_last_preceding_byte_write_checks_selected_low_lane(self):
        read = dict(seq=2, op="R", address=0x237204, data=0xFF00,
                    mask=0x00FF, time_ns=20)
        writes = [dict(seq=1, op="W", address=0x237204, data=0x0000,
                       mask=0x00FF, time_ns=10)]
        result = audit_preceding_input_writes(read, writes)
        self.assertTrue(result["checked"])
        self.assertEqual(result["selected_byte"], 0)

    def test_last_preceding_mismatched_write_rejects(self):
        read = dict(seq=2, op="R", address=0x2349EA, data=0x4000,
                    mask=0xFF00, time_ns=20)
        writes = [dict(seq=1, op="W", address=0x2349EA, data=0x4100,
                       mask=0xFF00, time_ns=10)]
        with self.assertRaisesRegex(ValueError, "differs from caller-read byte"):
            audit_preceding_input_writes(read, writes)

    def test_wrong_word_and_opposite_lane_do_not_replace_selected_byte(self):
        read = dict(seq=3, op="R", address=0x2349EA, data=0x4000,
                    mask=0xFF00, time_ns=30)
        writes = [
            dict(seq=0, op="W", address=0x2349E8, data=0x9900,
                 mask=0xFF00, time_ns=10),
            dict(seq=1, op="W", address=0x2349EA, data=0x0055,
                 mask=0x00FF, time_ns=20),
        ]
        result = audit_preceding_input_writes(read, writes)
        self.assertFalse(result["checked"])
        self.assertEqual(result["status"], "no preceding write observed")

    def test_write_after_read_is_not_a_preceding_producer(self):
        read = dict(seq=1, op="R", address=0x2349EA, data=0x4000,
                    mask=0xFF00, time_ns=20)
        later = dict(seq=2, op="W", address=0x2349EA, data=0x4000,
                     mask=0xFF00, time_ns=30)
        result = audit_preceding_input_writes(read, [later])
        self.assertEqual(result["status"], "no preceding write observed")


if __name__ == "__main__":
    unittest.main()
