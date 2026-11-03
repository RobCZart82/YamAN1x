"""ROM-free CLI contract checks; temporary synthetic programs only."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile


def require(ok, message):
    if not ok:
        raise AssertionError(message)


def main(binary):
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / "synthetic.bin"
        def image(words):
            data = bytearray(1048576)
            data[2] = 1
            for offset, word in enumerate(words):
                data[256 + 2*offset:258 + 2*offset] = word.to_bytes(2, "big")
            path.write_bytes(data)
        def run(args, view="raw", profile=None):
            before = hashlib.sha256(path.read_bytes()).hexdigest()
            result = subprocess.run([binary, str(path), *map(str, args), view, *([] if profile is None else [profile])], capture_output=True, text=True, timeout=20)
            require(hashlib.sha256(path.read_bytes()).hexdigest() == before, "Input must remain byte-identical")
            report = json.loads(result.stdout)
            require(report["firmware_boot_accepted"] is False, "Bounded progress is never boot acceptance")
            return result.returncode, report
        image([0x40fe])
        for slice_size in (1, 8, 64):
            code, report = run([64, slice_size, 10000])
            require(code == 0 and report["status"] == "cycle_limit" and report["completed_slice_cycles"] == 64, "Bounded synthetic loop")
            require(report["fault_access"] is None and not report["cpu_failed"], "No fabricated fault")
        raw = path.read_bytes()
        swapped = bytearray(raw)
        swapped[0::2], swapped[1::2] = raw[1::2], raw[0::2]
        path.write_bytes(swapped)
        code, report = run([64,8,10000], "swap16")
        require(code == 0 and report["rom_view"] == "swap16" and report["reset_target"] == 256, "Explicit in-memory word-byte view")
        code, report = run([64,8,10000], "guess")
        require(code == 1, "Never guess ROM view")
        image([0x40fe])
        odd = bytearray(path.read_bytes()); odd[3] = 1; path.write_bytes(odd)
        code, report = run([64,8,10000])
        require(code == 1 and report["status"] == "input_error", "Odd reset vector rejects byte-order mistakes")
        for address in (0, 0x440000):
            image([0x7a00, address >> 16, address & 65535, 0xfa5a, 0x688a])
            code, report = run([64, 8, 10000])
            require(code == 2 and report["status"] == "stopped" and report["cpu_failed"], "Fail closed on unmapped/ROM writes")
            require(report["fault_access"] == {"kind": "data_write", "address": address, "width": 1}, "Exact failed access context")
            require(report["completed_slice_cycles"] < 64, "Failed slice is not reported completed")
        image([0x7a00, 0xff, 0xffc7, 0x680a])
        code, report = run([64,8,10000])
        require(code == 2 and report["fault_access"] == {"kind": "data_read", "address": 0xffffc7, "width": 1}, "Unknown GPIO input fails closed")
        require("GPIO input" in report["error"], "No invented pin values")
        code, report = run([64,8,10000], profile="lcd-16mhz")
        require(code == 2 and report["fault_access"]["address"] == 0xffffc7, "LCD profile does not invent other inputs")
        image([0x7a00, 0xff, 0xffd6, 0x680a, 0x40fe])
        code, report = run([64,8,10000], profile="lcd-16mhz")
        require(code == 0 and report["lcd_profile"] == "lcd-16mhz", "Explicit LCD profile supports Port B inputs")
        code, report = run([64,8,10000])
        require(code == 2 and report["lcd_profile"] == "disabled", "Default keeps strict no-LCD profile")
        code, report = run([64,8,10000], profile="always-ready")
        require(code == 1, "Unknown LCD profiles reject")
        image([0x7b5c, 0])
        code, report = run([64, 8, 10000])
        require(code == 2 and report["fault_access"] is None, "Decoder stop is not a bus fault")
        image([0x40fe])
        code, report = run([64, 8, 1])
        require(code == 2 and "limit" in report["error"], "Global callback bound stops execution")
        for args in ([0,8,100], [-1,8,100], [100000001,8,100], [64,0,100], [64,1000001,100], [64,8,1000001], ["NaN",8,100], ["18446744073709551616",8,100]):
            code, report = run(args)
            require(code == 1 and report["status"] == "input_error", "Strict finite CLI limits")
        for size in (1, 1048577):
            path.write_bytes(bytes(size))
            code, report = run([64,8,100])
            require(code == 1 and report["status"] == "input_error", "Reject incorrect ROM size")
    print("Bounded boot probe: synthetic progress/fault/limits/read-only checks passed")


if __name__ == "__main__":
    main(sys.argv[1])
