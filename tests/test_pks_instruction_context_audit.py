"""Synthetic bounded instruction traces; no firmware, ROM or private logs."""
import copy
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from pks_instruction_context_audit import (SITES, STATIC_INSTRUCTIONS,
                                           audit_instruction_path,
                                           audit_preceding_input_writes,
                                           parse_instruction_context,
                                           parse_static_disassembly)


def _event(seq, window, pc, time_ns, r=None, er=None, ccr=0x80):
    r = list(r or [0] * 8)
    er = list(er or [value for value in r] + [0] * 8)
    return dict(seq=seq, window=window, pc=pc, opcode=0x1234, time_ns=time_ns,
                ccr=ccr, r=r, er=er)


def synthetic_capture():
    lines = ["YAMANIX PKS_INSTRUCTION_CONTEXT_BEGIN version=1 capacity=8192 window=2048 hardware_validated=0",
             "YAMANIX PKS_INSTRUCTION_CONTEXT_WINDOWS count=4 events=8192 dropped=0 finished=1 hardware_validated=0"]
    all_events = []
    addresses = (0x2349EA, 0x237204, 0x2349EA, 0x237204)
    for win, address in enumerate(addresses):
        site = SITES[address]
        start = win * 2048
        tm = 1000 + win * 3000
        data = (0x4000 if win == 0 else 0xFF00 if win == 1 else 0 if win == 2 else 0xFFFF)
        mask = site["mask"]
        lines.append(f"YAMANIX PKS_INSTRUCTION_WINDOW id={win} trigger_seq={win} address={address:06x} "
                     f"pc={site['pc']:06x} data={data:04x} mask={mask:04x} ccr=80 time_ns={tm} "
                     f"first={start} count=2048 complete=1")
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
            event = _event(start + i, win, pc, tm + i,
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
        result = parse_instruction_context(lines)
        self.assertEqual((len(result["windows"]), len(result["events"])), (4, 8192))
        self.assertEqual(result["windows"][1]["address"], 0x237204)
        self.assertFalse(result["hardware_validated"])

    def test_rejects_lossy_footer(self):
        lines, _ = synthetic_capture()
        lines[1] = lines[1].replace("dropped=0", "dropped=1")
        with self.assertRaisesRegex(ValueError, "loss or incomplete"):
            parse_instruction_context(lines)
        lines, _ = synthetic_capture()
        lines[2] = lines[2].replace("mask=ff00", "mask=00ff")
        with self.assertRaisesRegex(ValueError, "static caller contract"):
            parse_instruction_context(lines)

    def test_rejects_duplicate_sequence_and_wrong_register_composition(self):
        lines, _ = synthetic_capture()
        lines[4] = lines[4].replace("seq=1 ", "seq=0 ", 1)
        with self.assertRaisesRegex(ValueError, "sequence"):
            parse_instruction_context(lines)
        lines, _ = synthetic_capture()
        lines[3] = lines[3].replace("er0=00000000", "er0=00000001", 1)
        with self.assertRaisesRegex(ValueError, "ER low word"):
            parse_instruction_context(lines)

    def test_rejects_nonmonotonic_time_and_records_after_completion(self):
        lines, _ = synthetic_capture()
        lines[4] = lines[4].replace("time_ns=1001", "time_ns=999")
        with self.assertRaisesRegex(ValueError, "time moved backwards"):
            parse_instruction_context(lines)
        lines, _ = synthetic_capture()
        lines.append(lines[3].replace("seq=0", "seq=8192"))
        with self.assertRaisesRegex(ValueError, "record after completion"):
            parse_instruction_context(lines)

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
            start, tm = win * 2048, 1000 + win * 3000
            windows.append(dict(id=win, address=address, pc=site["pc"],
                                data=(0x4000 if win == 0 else 0xFF00 if win == 1 else
                                      0 if win == 2 else 0xFFFF),
                                mask=site["mask"], ccr=0x80, time_ns=tm,
                                first=start, count=2048, complete=True))
        source = dict(events=[])
        values = ((0xE0, 0, 0x20), (0xE0, 0, 0))
        addresses = ((0x234ACC, 0x234ACC, 0x234ACE), (0x234ACE, 0x234AD0, 0x234AD0))
        masks = ((0xFF00, 0x00FF, 0xFF00), (0x00FF, 0xFF00, 0x00FF))
        for win in range(2):
            for i in range(3):
                index = (win * 2048) + len(SITES[windows[win]["address"]]["path"]) - 1 + i * 20
                cpu = events[index]
                byte = values[win][i]
                data = byte << 8 if masks[win][i] == 0xFF00 else byte
                source["events"].append(dict(op="W", pc=0x0039AC, address=addresses[win][i],
                    data=data, mask=masks[win][i], time_ns=cpu["time_ns"], ccr=cpu["ccr"], er=cpu["er"][:]))
        operand = dict(events=[dict(seq=i, op="R", address=w["address"], pc=w["pc"], mask=w["mask"],
                                   data=w["data"], ccr=w["ccr"], time_ns=w["time_ns"])
                               for i, w in enumerate(windows)])
        instruction = dict(windows=windows, events=events)
        static = [f"YAMANIX DASM pc={address:06x} {value}"
                  for address, value in STATIC_INSTRUCTIONS.items()]
        result = audit_instruction_path(instruction, source, operand, static)
        self.assertEqual([item["writer_count"] for item in result["windows"]], [3, 3])
        self.assertEqual([item["preceding_input_write"]["status"] for item in result["windows"]],
                         ["no preceding write observed"] * 2)
        self.assertFalse(result["windows"][0]["raw_ccr_interpreted"])
        bad_input = copy.deepcopy(instruction)
        bad_input["events"][0]["r"][1] ^= 1
        with self.assertRaisesRegex(ValueError, "first post-read instruction"):
            audit_instruction_path(bad_input, source, operand, static)
        bad_path = copy.deepcopy(instruction)
        bad_path["events"][1]["pc"] = 0x002A10
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
