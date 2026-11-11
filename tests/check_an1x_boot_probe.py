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
        def run(args, view="raw", profile=None, extra=()):
            before = hashlib.sha256(path.read_bytes()).hexdigest()
            result = subprocess.run([binary, str(path), *map(str, args), view, *([] if profile is None else [profile]), *extra], capture_output=True, text=True, timeout=20)
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
        require("GPIO input" in report["error"] and report["stop_reason"] == "execution_error", "No invented pin values or callback-limit misclassification")
        code, report = run([64,8,10000], profile="lcd-16mhz")
        require(code == 2 and report["fault_access"]["address"] == 0xffffc7, "LCD profile does not invent other inputs")
        image([0x7a00, 0xff, 0xffd6, 0x680a, 0x40fe])
        code, report = run([64,8,10000], profile="lcd-16mhz")
        require(code == 0 and report["lcd_profile"] == "lcd-16mhz", "Explicit LCD profile supports Port B inputs")
        code, report = run([64,8,10000])
        require(code == 2 and report["lcd_profile"] == "disabled", "Default keeps strict no-LCD profile")
        image([0x7a00, 0xff, 0xffd2, 0x680a, 0x40fe])
        code, report = run([64,8,10000], profile="lcd-16mhz")
        require(code == 0, "Explicit LCD research profile supplies P9 pull-ups")
        code, report = run([64,8,10000])
        require(code == 2 and report["fault_access"]["address"] == 0xffffd2, "Default does not assume P9 input levels")
        code, report = run([64,8,10000], profile="always-ready")
        require(code == 1, "Unknown LCD profiles reject")
        # The input is opt-in and constant for this bounded probe; the default
        # remains fail-closed. MOV.L loads ER0, then MOV.B writes immediate 0x23 via R3L.
        image([0x7a00,0xff,0xffe8,0xf323,0x6883,0x40fe])
        code, report = run([2048,1,10000])
        require(code == 2 and report["fault_access"] == {"kind":"data_write","address":0xffffe8,"width":1}
                and report["error"] == "Unsupported ADC conversion or IRQ enable"
                and report["adc_an3_input_profile"] == "disabled" and report["adc_an3_samples"] == 0,
                "AN3 remains strict without explicit input")
        for address, opcode, width in ((0xffffe6, 0x6901, 2), (0xffffe6, 0x680a, 1), (0xffffe7, 0x680a, 1)):
            image([0x7a00, address >> 16, address & 0xffff, opcode, 0x40fe])
            code, report = run([64,8,10000])
            require(code == 2 and report["fault_access"] == {"kind":"data_read","address":address,"width":width},
                    "AN3 result lanes remain unmapped without explicit input")
        for sample in (0, 1023):
            image([0x7a00,0xff,0xffe8,0xf323,0x6883,0x40fe])
            first_cycle = None
            for slice_size in (1, 8, 64):
                code, report = run([2048,slice_size,10000], extra=(f"--adc-an3={sample}",))
                require(code == 0 and report["status"] == "cycle_limit" and report["fault_access"] is None,
                        "Explicit AN3 input continues the bounded synthetic program")
                require(report["adc_an3_input_profile"] == "explicit-constant" and
                        report["adc_an3_value"] == sample and report["adc_an3_samples"] == 1 and
                        report["adc_an3_first_sample_cycle"] is not None,
                        "Probe reports the selected input and exact sample callback")
                if first_cycle is None:
                    first_cycle = report["adc_an3_first_sample_cycle"]
                require(report["adc_an3_first_sample_cycle"] == first_cycle,
                        "AN3 callback timing is independent of CPU slice partition")
        image([0x7a00,0xff,0xffe8,0xf323,0x6883,
               0x7a00,0xff,0xffe6,0x6901,0x40fe])
        code, report = run([2048,8,10000], extra=("--adc-an3=1023",))
        require(code == 0 and report["fault_access"] is None and report["adc_an3_samples"] == 1,
                "Explicit AN3 input keeps the result word readable after conversion")
        for option in ("--adc-an3=-1", "--adc-an3=1024", "--adc-an3=NaN", "--adc-an3=18446744073709551616"):
            code, report = run([64,8,10000], extra=(option,))
            require(code == 1 and report["status"] == "input_error", "Reject out-of-range or malformed AN3 values")
        code, report = run([64,8,10000], extra=("--adc-an3=1", "--adc-an3=2"))
        require(code == 1 and report["status"] == "input_error", "Reject duplicate AN3 inputs")
        code, report = run([64,8,10000], profile="lcd-16mhz", extra=("lcd-meg-observe-16mhz",))
        require(code == 1 and report["status"] == "input_error", "Reject duplicate LCD profiles")
        code, report = run([64,8,10000], extra=("disabled",))
        require(code == 1 and report["status"] == "input_error", "Reject unknown profile words")
        image([0x7a00, 0x40, 7, 0xfa5a, 0x688a, 0x40fe])
        code, report = run([64,8,10000], profile="lcd-meg-observe-16mhz")
        require(code == 0 and report["meg_observed_controls"] == 1, "Explicit MEG profile observes one control without fake response")
        code, report = run([64,8,10000], profile="lcd-16mhz")
        require(code == 2, "LCD-only profile keeps MEG writes strict")
        image([0x7a00, 0x40, 5, 0x680a])
        code, report = run([64,8,10000], profile="lcd-meg-observe-16mhz")
        require(code == 2 and report["fault_access"]["kind"] == "data_read", "MEG profile never invents status")
        code, report = run([64,8,10000], profile="lcd-meg-source-stubs-16mhz")
        require(code == 0 and report["meg_source_status_reads"] == 1, "Explicit original-source stub experiment is separately counted")
        panel = "lcd-meg-vop3-idle-panel-experiment-16mhz"
        # Read the panel byte, compare it to FF and touch an unmapped address on mismatch.
        image([0x7a00, 0x1268, 0, 0x7a01, 0x68, 1, 0x680a, 0xaaff, 0x4702, 0x681a, 0x40fe])
        for slice_size in (1, 8, 64):
            code, report = run([128, slice_size, 10000], profile=panel)
            require(code == 0 and report["lcd_profile"] == panel, "Panel byte alias reads FF across slices")
        for profile in (None, "lcd-meg-vop3-level-experiment-16mhz"):
            code, report = run([128, 8, 10000], profile=profile)
            require(code == 2 and report["fault_access"] == {"kind":"data_read","address":0x680000,"width":1},
                    "Existing profiles keep panel reads strict")
        for address, op, kind, width in ((0x680001, 0x680a, "data_read", 1),
                                          (0x680002, 0x680a, "data_read", 1),
                                          (0x680000, 0x6901, "data_read", 2),
                                          (0x680000, 0x688a, "data_write", 1)):
            image([0x7a00, address >> 16, address & 65535, op, 0x40fe])
            code, report = run([128, 8, 10000], profile=panel)
            require(code == 2 and report["fault_access"] == {"kind":kind,"address":address,"width":width},
                    "Panel profile maps only the observed byte read")
        native = "lcd-meg-vop3-register-experiment-16mhz"
        # Two distinct chip coefficient writes,then the explicit upload command.
        image([0x7a00,0x44,0x16,0x7901,0x1234,0x6981,
               0x7a00,0x48,0x16,0x6981,0x7a00,0x44,2,0x7901,1,0x6981,0x40fe])
        for slice_size in (1,8,64):
            code, report = run([128,slice_size,10000], profile=native)
            require(code == 0 and report["vop3_writes"] == [2,1] and report["vop3_metrics_available"], "Native two-chip writes and explicit upload control")
            require(report["vop3_sample_passes"] == [0,0] and report["audio_accepted"] is False, "Register-only experiment never claims samples/audio")
            require(report["vop3_profile"] == "native-registers-experimental-upload" and report["lcd_profile"] == native, "Explicit assumptions in report")
        code, report = run([128,8,10000], profile="lcd-meg-source-stubs-16mhz")
        require(code == 2 and report["vop3_profile"] == "disabled" and report["vop3_writes"] == [0,0], "Old profile remains strict")
        for address,width,write in ((0x440044,2,False),(0x440000,1,True)):
            image([0x7a00,address>>16,address&65535,0xfa5a,0x688a if write else 0x6901,0x40fe])
            code, report = run([64,8,10000], profile=native)
            require(code == 2 and report["fault_access"]["address"] == address and report["fault_access"]["width"] == width, "Native experiment keeps unsupported read/byte strict")
        level = "lcd-meg-vop3-level-experiment-16mhz"
        image([0x7a00,0x44,0,0x7901,3,0x6981,0x7a00,0x44,0x58,0x7901,0xff00,0x6981,
               0x7a00,0x44,0x44,0x6901,0x40fe])
        for slice_size in (1,8,64):
            code, report = run([128,slice_size,10000], profile=level)
            require(code == 0 and report["vop3_reads"] == [1,0] and report["vop3_writes"] == [2,0], "Explicit level profile routes actual selected channel read")
            require(report["vop3_profile"] == "native-registers-experimental-upload-and-level" and report["vop3_sample_passes"] == [0,0], "Level experiment remains named and clockless")
        code, report = run([128,8,10000], profile=native)
        require(code == 2 and report["fault_access"]["address"] == 0x440044, "Register-only experiment remains strict at22")
        image([0x7a00,0x44,0,0x7901,16,0x6981,0x7a00,0x44,0x44,0x6901])
        code, report = run([128,8,10000], profile=level)
        require(code == 2 and report["vop3_reads"] == [0,0] and report["fault_access"]["address"] == 0x440044, "Invalid selected index never fabricates a value")
        image([0x7b5c, 0])
        code, report = run([64, 8, 10000])
        require(code == 2 and report["fault_access"] is None, "Decoder stop is not a bus fault")
        image([0x40fe])
        code, report = run([64, 8, 1])
        require(code == 2 and "limit" in report["error"] and report["stop_reason"] == "callback_limit"
                and report["callback_limit_requested"] == 1, "Global callback bound is explicitly classified")
        code, report = run([64,8,20000000])
        require(code == 0 and report["stop_reason"] == "cycle_limit" and report["callback_limit_requested"] == 20000000, "Larger finite callback guard is explicit")
        for args in ([0,8,100], [-1,8,100], [100000001,8,100], [64,0,100], [64,1000001,100], [64,8,20000001], [64,8,0], ["NaN",8,100], ["18446744073709551616",8,100]):
            code, report = run(args)
            require(code == 1 and report["status"] == "input_error", "Strict finite CLI limits")
        for size in (1, 1048577):
            path.write_bytes(bytes(size))
            code, report = run([64,8,100])
            require(code == 1 and report["status"] == "input_error", "Reject incorrect ROM size")
    print("Bounded boot probe: synthetic progress/fault/limits/read-only checks passed")


if __name__ == "__main__":
    main(sys.argv[1])
