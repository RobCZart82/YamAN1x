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
            require(report["tstr_observations"] is None, "TSTR observer is opt-in")
        # Internal ASTCR write selects three-cycle ROM reads. Initial
        # instruction fetches cost two cycles in the default boot profile.
        image([0xf8ff,0x38ed,0x40fe])
        prefix_reference = None
        for slice_size in (1,8,64):
            code, report = run([2048,slice_size,10000], extra=("--observe-startup-prefix",))
            code_off, report_off = run([2048,slice_size,10000])
            prefix = report.pop("startup_prefix")
            require(code == code_off == 0 and report_off.pop("startup_prefix") is None and
                    report == report_off, "Startup prefix observer leaves execution unchanged")
            require(len(prefix) == 128 and prefix[:4] == [
                {"pc":0x100,"opcode":0xf8ff,"raw_ccr":0x80,"bus_cycle":6},
                {"pc":0x102,"opcode":0x38ed,"raw_ccr":0x88,"bus_cycle":8},
                {"pc":0x104,"opcode":0x40fe,"raw_ccr":0x88,"bus_cycle":12},
                {"pc":0x104,"opcode":0x40fe,"raw_ccr":0x88,"bus_cycle":18},
            ], "Startup prefix retains the ASTCR write boundary and raw CCR")
            if prefix_reference is None:
                prefix_reference = prefix
            require(prefix == prefix_reference, "Startup prefix is invariant across slices")
        for slice_size in (1, 8, 64):
            code, report = run([2048,slice_size,10000], extra=("--observe-startup-prefix=8:12",))
            require(code == 0 and report["startup_prefix"] == prefix_reference[1:3] and
                    report["startup_prefix_window"] == {
                        "first_cycle":8,"last_cycle":12,"capacity":128,"dropped":0,
                    }, "Dispatch window retains both endpoints and preserves exact records")
        code, report = run([2048,8,10000], extra=("--observe-startup-prefix=0:2048",))
        require(code == 0 and report["startup_prefix"] == prefix_reference and
                report["startup_prefix_window"]["dropped"] > 0,
                "Dispatch window reports eligible records beyond its retained capacity")
        for options in (("--observe-startup-prefix=12:8",),
                        ("--observe-startup-prefix=:8",),
                        ("--observe-startup-prefix","--observe-startup-prefix=8:12")):
            code, report = run([64,8,10000], extra=options)
            require(code == 1 and report["status"] == "input_error", "Reject invalid dispatch window")
        # Two committed TSTR byte writes. The observer must retain the exact
        # MMIO value/cycle/raw CCR and the first completed CPU continuation,
        # independent of where the runner's cycle slices fall.
        image([0x7a00,0xff,0xff60,0xf3e0,0x6883,0xf3e1,0x6883,0x40fe])
        tstr_reference = None
        for slice_size in (1,8,64):
            code, report = run([2048,slice_size,10000], extra=("--observe-tstr",))
            require(code == 0 and report["tstr_observations"]["capacity"] == 64 and
                    report["tstr_observations"]["dropped"] == 0, "TSTR observer is bounded and did not overflow")
            writes = report["tstr_observations"]["writes"]
            require(len(writes) == 2 and [event["address"] for event in writes] == [0xffff60,0xffff60] and
                    [event["width"] for event in writes] == [1,1] and
                    [event["value"] for event in writes] == [0xe0,0xe1],
                    "TSTR observer reports exact successful byte-write addresses and values")
            require([event["cycle"] for event in writes] == [18,24] and
                    [event["raw_ccr"] for event in writes] == [0x88,0x88],
                    "TSTR event cycles and raw CPU flags are retained")
            require([event["before"]["pc"] for event in writes] == [0x10a,0x10e] and
                    [event["before"]["ppc"] for event in writes] == [0x108,0x10c] and
                    [event["continuation"]["pc"] for event in writes] == [0x10c,0x110] and
                    [event["continuation"]["ppc"] for event in writes] == [0x108,0x10c] and
                    [event["continuation"]["instruction_state"] for event in writes] == [0xf3e1,0x40fe] and
                    all(event["continuation"] is not None and event["continuation"]["instruction_substate"] == 0 and
                        event["continuation"]["raw_ccr"] == event["raw_ccr"] for event in writes),
                    "TSTR records carry the post-instruction continuation without flag rewriting")
            observation = report["tstr_observations"]
            if tstr_reference is None:
                tstr_reference = observation
            require(observation == tstr_reference, "TSTR address/value/flags/cycle/continuation are slice invariant")
            code_off, report_off = run([2048,slice_size,10000])
            compared = ("status","stop_reason","completed_slice_cycles","peripheral_cycle","pc",
                        "instruction_state","instruction_substate","cpu_failed","fault_access",
                        "tx_edges","clock_edges","gpio_writes")
            require(code_off == code and all(report_off[key] == report[key] for key in compared),
                    "Opt-in TSTR observation leaves execution and peripheral outputs unchanged")
        # ITU0 compare A raises vector 24 exactly 100 cycles after TSTR.
        image([0x7a07,0xff,0xfefe,0x7a00,0xff,0xff6a,0xf300,0x6883,
               0x0b00,0xf363,0x6883,0x7a00,0xff,0xff66,0xf301,0x6883,
               0x7a00,0xff,0xff60,0x6883,0x0700,0x0180,0x40fe])
        program = bytearray(path.read_bytes())
        program[24*4:24*4+4] = (0x180).to_bytes(4, "big")
        program[0x180:0x182] = bytes.fromhex("40 fe")
        path.write_bytes(program)
        irq_reference = None
        for slice_size in (1, 8, 64):
            code, report = run([400,slice_size,10000], extra=("--observe-tstr","--observe-irq"))
            code_off, report_off = run([400,slice_size,10000], extra=("--observe-tstr",))
            irq = report.pop("irq_observations")
            require(code == code_off == 0 and report_off.pop("irq_observations") is None and
                    report == report_off, "IRQ observation is opt-in and does not change execution")
            events = irq["events"]
            tstr_cycle = report["tstr_observations"]["writes"][0]["cycle"]
            require(irq["dropped"] == 0 and len(events) == 2 and events[0] ==
                    {"vector":24,"bus_cycle":tstr_cycle+100,"kind":"request"},
                    f"ITU compare A request: {irq}")
            accepted = events[1]
            require(accepted["kind"] == "accepted" and accepted["vector"] == 24 and
                    accepted["handler_pc"] == 0x180 and accepted["raw_ccr"] == 0x80 and
                    accepted["bus_cycle"] > events[0]["bus_cycle"],
                    f"Hardware IRQ entered handler with post-entry CCR: {irq}")
            if irq_reference is None:
                irq_reference = irq
            require(irq == irq_reference, "IRQ observations are slice invariant")
        # SCI output observation is opt-in and does not alter the bounded run.
        image([0x7a00,0xff,0xffb1,0xfa00,0x688a,0x0b00,0xfa20,0x688a,
               0x0b00,0xfaa5,0x688a,0x0b00,0x680b,0xeb7f,0x688b,0x40fe])
        code_off, report_off = run([1000,8,10000])
        require(report_off["sci_tx_observations"] is None, "SCI TX observer is opt-in")
        expected_tx_events = [
                    {"channel":0,"cycle":48,"level":False,"first_observation":True},
                    {"channel":0,"cycle":80,"level":True,"first_observation":False},
                    {"channel":0,"cycle":112,"level":False,"first_observation":False},
                    {"channel":0,"cycle":144,"level":True,"first_observation":False},
                    {"channel":0,"cycle":176,"level":False,"first_observation":False},
                    {"channel":0,"cycle":240,"level":True,"first_observation":False},
                    {"channel":0,"cycle":272,"level":False,"first_observation":False},
                    {"channel":0,"cycle":304,"level":True,"first_observation":False},
                ]
        trace_reference = None
        for slice_size in (1, 8, 64):
            code_on, report_on = run([1000,slice_size,10000], extra=("--observe-sci-tx",))
            require(code_on == code_off and report_on["stop_reason"] == report_off["stop_reason"] and
                    report_on["pc"] == report_off["pc"] and report_on["peripheral_cycle"] == report_off["peripheral_cycle"] and
                    report_on["tx_edges"] == report_off["tx_edges"] and
                    report_on["sci_tx_observations"]["capacity"] == 4096 and
                    report_on["sci_tx_observations"]["dropped"] == 0,
                    "SCI TX observer leaves execution and legacy output counters unchanged")
            events = report_on["sci_tx_observations"]["events"]
            require(events == expected_tx_events and
                    all(a["cycle"] <= b["cycle"] for a,b in zip(events,events[1:])),
                    "SCI TX trace records exact channel, serial levels and transition cycles")
            if trace_reference is None:
                trace_reference = report_on["sci_tx_observations"]
            require(report_on["sci_tx_observations"] == trace_reference,
                    "SCI TX trace is invariant under CPU slice partitioning")
        code_dup, report_dup = run([64,8,10000], extra=("--observe-sci-tx","--observe-sci-tx"))
        require(code_dup == 1 and report_dup["status"] == "input_error", "Reject duplicate SCI TX observer option")
        require(report_off["sci_write_observations"] is None, "SCI write observer is opt-in")
        for size in (1, 8, 64):
            code, traced = run([1000,size,10000], extra=("--observe-sci-writes",))
            writes = traced["sci_write_observations"]
            events = writes["events"]
            require(code == code_off and writes["dropped"] == 0 and
                    [(e["address"], e["value"]) for e in events[:3]] ==
                    [(0xffffb1, 0), (0xffffb2, 0x20), (0xffffb3, 0xa5)] and
                    len(events) == 4 and events[-1]["address"] == 0xffffb4,
                    "Capture the four CPU writes to the SCI byte registers")
            require(all(a["cycle"] < b["cycle"] for a,b in zip(events,events[1:])),
                    "SCI writes are recorded in cycle order")
            if size == 1:
                write_reference = writes
            require(writes == write_reference, "SCI write trace is invariant across slices")
            if size == 8:
                traced.pop("sci_write_observations")
                require(traced == {k:v for k,v in report_off.items() if k != "sci_write_observations"},
                        "SCI write observation leaves the rest of the report unchanged")
        first, last = write_reference["events"][1:3]
        option = f'--observe-sci-writes={first["cycle"]}:{last["cycle"]}'
        for size in (1, 8, 64):
            code, report = run([1000, size, 10000], extra=(option,))
            require(code == code_off and report["sci_write_observations"]["events"] == [first, last]
                    and report["sci_write_observations"]["dropped"] == 0,
                    "An inclusive cycle window retains exactly the selected SCI writes")
        for options in (("--observe-sci-writes=5:4",), ("--observe-sci-writes=0:",),
                        ("--observe-sci-writes", option), ("--observe-sci-writes=0:600000001",)):
            code, report = run([1000, 8, 10000], extra=options)
            require(code == 1 and report["status"] == "input_error", "Reject invalid SCI cycle windows")
        for size in (8, 64):
            code, report = run([10000, size, 10000], extra=("--midi-byte=1024:165",))
            require(code == 0 and report["midi_input_events_consumed"] == 10,
                    "Consume all ten scheduled MIDI line levels")
        code, report = run([10000, 8, 10000], extra=("--midi-byte=9000:165",))
        require(code == 1 and report["status"] == "input_error",
                "Reject a MIDI frame that extends beyond the cycle limit")
        require(report_off["sci0_rx_read_observations"] is None, "RX read observer is opt-in")
        rx_reference = None
        for size in (1, 8, 64):
            code, report = run([1000, size, 10000], extra=("--observe-sci0-rx-reads",))
            trace = report.pop("sci0_rx_read_observations")
            events = trace["events"]
            require(code == code_off and trace["dropped"] == 0 and len(events) == 1
                    and events[0]["address"] == 0xffffb4 and events[0]["value"] & 0x80,
                    "Capture the CPU status read with the transmit-empty flag")
            if rx_reference is None:
                rx_reference = trace
            require(trace == rx_reference, "SCI0 read trace is slice invariant")
            if size == 8:
                require(report == {k:v for k,v in report_off.items() if k != "sci0_rx_read_observations"},
                        "Observe SCI0 reads without altering execution")
        image([0x7a00, 0xff, 0xffb4, 0x6903, 0x40fe])
        code, report = run([1000, 8, 10000], extra=("--observe-sci0-rx-reads",))
        events = report["sci0_rx_read_observations"]["events"]
        require(code == 0 and len(events) == 1 and events[0]["address"] == 0xffffb4
                and events[0]["width"] == 2 and events[0]["value"] & 0x8000,
                "SCI0 word read preserves the high status lane")
        image([0x7a00, 0xff, 0xffbc, 0x6903, 0x680a, 0x40fe])
        for size in (8, 64):
            code, report = run([1000, size, 10000],
                               extra=("--observe-sci0-rx-reads", "--observe-sci1-rx-reads"))
            reads = report["sci1_rx_read_observations"]["events"]
            require(code == 0 and report["sci0_rx_read_observations"]["events"] == []
                    and [(e["address"], e["width"]) for e in reads] ==
                    [(0xffffbc, 2), (0xffffbc, 1)]
                    and reads[0]["value"] >> 8 == reads[1]["value"]
                    and reads[1]["value"] & 0x80 and reads[0]["cycle"] < reads[1]["cycle"],
                    "Observe SCI1 SSR byte and SSR/RDR word without including them in SCI0")
            if size == 8:
                sci1_reference = reads
            require(reads == sci1_reference, "SCI1 receive read trace is slice invariant")
        # Equal consecutive opcodes are still distinct completed instructions;
        # completion must not be inferred from a changed instruction-state word.
        image([0x7a00,0xff,0xff60,0xf3e0,0x6883,0x6883,0x40fe])
        code, report = run([2048,8,10000], extra=("--observe-tstr",))
        writes = report["tstr_observations"]["writes"]
        require(code == 0 and len(writes) == 2, "Observe both adjacent identical TSTR stores")
        require([event["continuation"] for event in writes] == [
                    {"pc":0x10c,"ppc":0x108,"instruction_state":0x6883,
                     "instruction_substate":0,"raw_ccr":writes[0]["raw_ccr"]},
                    {"pc":0x10e,"ppc":0x10a,"instruction_state":0x40fe,
                     "instruction_substate":0,"raw_ccr":writes[1]["raw_ccr"]}],
                "Each identical store gets its own completed continuation")
        code, report = run([2048,8,10000], extra=("--observe-tstr","--observe-tstr"))
        require(code == 1 and report["status"] == "input_error", "Reject duplicate TSTR observer option")
        words = [0x7a00,0xff,0xff60]
        words.extend(word for i in range(65) for word in (0xf3e0 + (i % 32),0x6883))
        words.append(0x40fe)
        image(words)
        code, report = run([20000,64,100000], extra=("--observe-tstr",))
        require(code == 0 and len(report["tstr_observations"]["writes"]) == 64 and
                report["tstr_observations"]["capacity"] == 64 and report["tstr_observations"]["dropped"] == 1,
                "TSTR event storage stops at its explicit capacity and reports overflow")
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
            require(report["fault_access"] == {"kind": "data_write", "address": address, "width": 1, "value": 0x5a}, "Exact failed byte-write address and value")
            require(report["completed_slice_cycles"] < 64, "Failed slice is not reported completed")
        image([0x7a00, 0, 0, 0x7901, 0x1234, 0x6981])
        code, report = run([64, 8, 10000])
        require(code == 2 and report["fault_access"] == {"kind": "data_write", "address": 0, "width": 2, "value": 0x1234},
                "Exact failed word-write address and value")
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
        # remains fail-closed. MOV.L loads ER0, then MOV.B writes immediate 0xA5 via R3L.
        image([0x7a00,0xff,0xffe8,0xf3a5,0x6883,0x40fe])
        code, report = run([2048,1,10000])
        require(code == 2 and report["fault_access"] == {"kind":"data_write","address":0xffffe8,"width":1,"value":0xa5}
                and report["error"] == "Unsupported ADC control mode"
                and report["adc_an3_input_profile"] == "disabled" and report["adc_an3_samples"] == 0,
                "AN3 remains strict without explicit input and reports the attempted write value")
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
        scan_values = (0, 1, 257, 1023, 42, 512, 768, 1000)
        scan_option = "--adc-scan=" + ",".join(map(str, scan_values))
        image([0x7a00,0xff,0xffe8,0xf323,0x6883,0x40fe])
        code, report = run([2048,8,10000], extra=(scan_option,))
        require(code == 0 and report["adc_an3_input_profile"] == "explicit-constant-from-eight-channel-scan" and
                report["adc_an3_value"] == scan_values[3] and report["adc_an3_samples"] == 1 and
                report["adc_scan_sample_counts"] == [0,0,0,0,0,0,0,0],
                "The explicit scan profile also supplies the preceding single-AN3 conversion")
        image([0x7a00,0xff,0xffe8,0xf333,0x6883,0x40fe])
        scan_reference = None
        for slice_size in (1, 17, 256):
            code, report = run([4096,slice_size,10000], extra=(scan_option,))
            require(code == 0 and report["status"] == "cycle_limit" and
                    report["fault_access"] is None and not report["cpu_failed"],
                    "Explicit eight-channel scan continues in the bounded probe")
            require(report["adc_scan_input_profile"] == "explicit-constant-eight-channel" and
                    report["adc_scan_values"] == list(scan_values) and
                    report["adc_scan_sample_counts"] == [4,4,4,4,0,0,0,0] and
                    report["adc_scan_first_sample_cycles"] == [18,284,540,796,None,None,None,None],
                    "Scan reports independent channel values, callback counts and first deadlines")
            snapshot = (report["pc"], report["peripheral_cycle"], report["adc_scan_sample_counts"],
                        report["adc_scan_first_sample_cycles"], report["fault_access"])
            if scan_reference is None:
                scan_reference = snapshot
            require(snapshot == scan_reference,
                    "Scan input delivery is independent of CPU slice partition")
        for option in ("--adc-scan=1,2,3", "--adc-scan=1,2,3,4,5,6,7,1024",
                       "--adc-scan=1,2,3,4,5,6,7,8,9", "--adc-scan=1,,3,4,5,6,7,8",
                       "--adc-scan=1,2,3,4,5,6,7,8,9,10"):
            code, report = run([64,8,10000], extra=(option,))
            require(code == 1 and report["status"] == "input_error",
                    "Reject malformed, incomplete, out-of-range or extra scan channels")
        code, report = run([64,8,10000], extra=(scan_option, "--adc-scan=0,0,0,0,0,0,0,0"))
        require(code == 1 and report["status"] == "input_error", "Reject duplicate ADC scan inputs")
        code, report = run([64,8,10000], extra=(scan_option, "--adc-an3=1"))
        require(code == 1 and report["status"] == "input_error", "Reject ambiguous single-channel and scan inputs")
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
            expected = {"kind":kind,"address":address,"width":width}
            if kind == "data_write":
                expected["value"] = 0
            require(code == 2 and report["fault_access"] == expected,
                    "Panel profile maps only the observed byte read")
        service = "lcd-meg-vop3-t6-panel-experiment-16mhz"
        # Drive P6 selector 5, then read the upper panel lane into R0L.
        # Branch around the invalid address only if the active-low value matches.
        for pins, selector, expected in (
            (0, 0, 0xff), (1, 4, 0xff), (2, 2, 0xff), (3, 6, 0x77),
            (4, 1, 0xff), (5, 5, 0xf7), (6, 3, 0xff), (7, 7, 0xff),
        ):
            image([0xf807, 0x38c9, 0xf800 | pins, 0x38cb,
                   0x7a00, 0x68, 0, 0x680a, 0xaa00 | expected, 0x4702, 0x681a, 0x40fe])
            for slice_size in (1, 8, 64):
                code, report = run([128, slice_size, 10000], profile=service)
                require(code == 0 and report["panel_counts"][selector] == 1
                        and report["panel_pressed_reads"] == int(expected != 255),
                        "Service panel selector and active-low byte")
                code, observed = run([128, slice_size, 10000], profile=service,
                                     extra=("--observe-panel=0:128",))
                events = observed["panel_read_observations"]["events"]
                require(code == 0 and observed["panel_read_observations"]["dropped"] == 0
                        and len(events) == 1 and events[0]["selector"] == selector
                        and events[0]["value"] == expected and events[0]["cycle"] == 28
                        and events[0]["pc"] == 0x110 and events[0]["ppc"] == 0x10e
                        and events[0]["instruction_state"] == 0x680a
                        and events[0]["raw_ccr"] == 0x80,
                        "Panel observer records selector, value, cycle and CPU context")
        image([0xf807, 0x38c9, 0xf805, 0x38cb,
               0x7a00, 0x68, 0, 0x680a, 0xaa77, 0x4702, 0x681a, 0x40fe])
        for slice_size in (1, 8, 64):
            code, report = run([128, slice_size, 10000], profile=service,
                               extra=("--panel-rows=136:8", "--observe-panel=0:128"))
            events = report["panel_read_observations"]["events"]
            require(code == 0 and len(events) == 1 and events[0]["value"] == 0x77
                    and report["panel_pressed_reads"] == 1,
                    "Panel pressed-row masks select the expected active-low value")
            code, report = run([128, slice_size, 10000], profile=service,
                               extra=("--panel-rows=0:0", "--panel-window=29:128",
                                      "--observe-panel=0:128"))
            events = report["panel_read_observations"]["events"]
            require(code == 0 and len(events) == 1 and events[0]["value"] == 0xff
                    and report["panel_pressed_reads"] == 0,
                    "Panel window releases inputs and accepts an empty pressed mask")
            code, report = run([128, slice_size, 10000], profile=service,
                               extra=("--observe-panel=29:128",))
            require(code == 0 and report["panel_read_observations"]["events"] == []
                    and report["panel_pressed_reads"] == 1,
                    "Panel observer window filters without changing the input")
        image([0xf807, 0x38c9, 0xf805, 0x38cb,
               0x7a00, 0x68, 0, 0x680a, 0xaaff, 0x4702, 0x681a, 0x40fe])
        for slice_size in (1, 8, 64):
            code, report = run([128, slice_size, 10000], profile=service,
                               extra=("--panel-chord-end=1",))
            require(code == 0 and report["panel_pressed_reads"] == 0,
                    "Released chord returns idle byte")
        for options, profile_arg in ((("--panel-rows=1:2", "--panel-rows=3:4"), service),
                                     (("--panel-rows=1:2",), "lcd-meg-vop3-idle-panel-experiment-16mhz"),
                                     (("--observe-panel=9:8",), service),
                                     (("--panel-rows=1:256",), service)):
            code, report = run([128, 8, 10000], profile=profile_arg, extra=options)
            require(code == 1 and report["status"] == "input_error",
                    "Invalid or duplicate panel observer input is rejected")
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
        code, report = run([64,8,125000000])
        require(code == 0 and report["stop_reason"] == "cycle_limit" and report["callback_limit_requested"] == 125000000, "Larger finite callback guard is explicit")
        for args in ([0,8,100], [-1,8,100], [600000001,8,100], [64,0,100], [64,1000001,100], [64,8,125000001], [64,8,0], ["NaN",8,100], ["18446744073709551616",8,100]):
            code, report = run(args)
            require(code == 1 and report["status"] == "input_error", "Strict finite CLI limits")
        for size in (1, 1048577):
            path.write_bytes(bytes(size))
            code, report = run([64,8,100])
            require(code == 1 and report["status"] == "input_error", "Reject incorrect ROM size")
    print("Bounded boot probe: synthetic progress/fault/limits/read-only checks passed")


if __name__ == "__main__":
    main(sys.argv[1])
