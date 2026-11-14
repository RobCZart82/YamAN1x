"""Synthetic caller-window checks; no firmware or private traces."""
import sys
from pathlib import Path
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from pks_caller_input_audit import audit_caller_input_windows


def source_event(seq, time_ns, address, mask, data, ccr):
    return dict(seq=seq, op="W", pc=0x0039AC, time_ns=time_ns,
                address=address, mask=mask, data=data, ccr=ccr)


def operand_event(seq, time_ns, address, pc, mask, data, ccr):
    return dict(seq=seq, op="R", pc=pc, time_ns=time_ns,
                address=address, mask=mask, data=data, ccr=ccr)


def fixture():
    operands = [
        operand_event(0, 1_000, 0x2349EA, 0x002A08, 0xFF00, 0x4000, 0x84),
        operand_event(1, 90_000, 0x237204, 0x001FD8, 0x00FF, 0xFF00, 0x80),
    ]
    writers = [
        source_event(0, 14_000, 0x234ACC, 0xFF00, 0xE000, 0xA9),
        source_event(1, 21_500, 0x234ACC, 0x00FF, 0x0000, 0xA5),
        source_event(2, 29_000, 0x234ACE, 0xFF00, 0x2000, 0xA1),
        source_event(3, 100_000, 0x234ACE, 0x00FF, 0x00E0, 0xA9),
        source_event(4, 107_500, 0x234AD0, 0xFF00, 0x0000, 0xA5),
        source_event(5, 115_000, 0x234AD0, 0x00FF, 0x0000, 0xA5),
    ]
    return writers, operands


class PksCallerInputAuditTests(unittest.TestCase):
    def test_value_address_lane_flags_timing_and_continuation_are_retained(self):
        writers, operands = fixture()
        result = audit_caller_input_windows(writers, operands)
        self.assertFalse(result["hardware_validated"])
        first, second = result["windows"]
        self.assertEqual((first["static_input_word_base"], first["input_byte_value"],
                          first["input_mask"], first["input_ccr"]),
                         (0x2349EA, 0x40, 0xFF00, 0x84))
        self.assertEqual([event["writer_value"] for event in first["observations"]],
                         [0xE0, 0x00, 0x20])
        self.assertEqual([event["writer_byte_address"] for event in first["observations"]],
                         [0x234ACC, 0x234ACD, 0x234ACE])
        self.assertEqual([event["elapsed_ns"] for event in first["observations"]],
                         [13_000, 20_500, 28_000])
        self.assertEqual([event["writer_ccr"] for event in first["observations"]],
                         [0xA9, 0xA5, 0xA1])
        self.assertEqual((second["static_input_word_base"], second["input_byte_value"],
                          second["input_mask"], second["input_ccr"]),
                         (0x237204, 0x00, 0x00FF, 0x80))
        self.assertEqual([event["writer_value"] for event in second["observations"]],
                         [0xE0, 0x00, 0x00])

    def test_missing_address_or_wrong_lane_is_rejected(self):
        writers, operands = fixture()
        operands[0]["address"] = 0x2349E8
        with self.assertRaisesRegex(ValueError, "no matching static-input read"):
            audit_caller_input_windows(writers, operands)
        writers, operands = fixture()
        operands[0]["mask"] = 0x00FF
        with self.assertRaisesRegex(ValueError, "no matching static-input read"):
            audit_caller_input_windows(writers, operands)

    def test_stale_input_and_broken_writer_continuation_are_rejected(self):
        writers, operands = fixture()
        operands[0]["time_ns"] = -40_001
        with self.assertRaisesRegex(ValueError, "no matching static-input read"):
            audit_caller_input_windows(writers, operands)
        writers, operands = fixture()
        writers[2]["time_ns"] = writers[1]["time_ns"]
        with self.assertRaisesRegex(ValueError, "continuation times"):
            audit_caller_input_windows(writers, operands)
        writers, operands = fixture()
        writers[3]["address"] = 0x234ACC
        with self.assertRaisesRegex(ValueError, "address continuation"):
            audit_caller_input_windows(writers, operands)

    def test_later_read_inside_window_is_rejected_as_ambiguous(self):
        writers, operands = fixture()
        operands.append(operand_event(2, 17_000, 0x2349EA, 0x002A08,
                                      0xFF00, 0x4100, 0x85))
        with self.assertRaisesRegex(ValueError, "later matching input read"):
            audit_caller_input_windows(writers, operands)


if __name__ == "__main__":
    unittest.main()
