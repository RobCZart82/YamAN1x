# An1X Boot Probe

## Optional SCI0 receive register observation

Use `--observe-sci0-rx-reads` to record the address, returned byte and bus
cycle and width of successful CPU reads from SCI0 SSR and RDR (`0xFFFFB4` and
`0xFFFFB5`). The first 256 reads are retained and later reads are counted
as dropped. See [SCI MMIO and IRQ](h83002-sci-mmio-irq.md) for the pinned
MAME register reference. A word read at `0xFFFFB4` retains both SSR and RDR in one event.

## Optional SCI register write observation

Use `--observe-sci-writes=FIRST:LAST` to restrict recording to an inclusive
range of bus cycles. Each endpoint is an integer from 0 through 600,000,000.
Writes outside the selected range use no capacity and do not count as drops.
Supply only one SCI write observation option per run.

Enable `--observe-sci-writes` to record up to 256 successful CPU byte writes
to the two SCI register ranges `0xFFFFB0–0xFFFFB5` and `0xFFFFB8–0xFFFFBD`.
The report records address, value and bus cycle and counts discarded records
after the limit. With the option disabled, `sci_write_observations` is null.
Word writes are outside this observation. The register map reference is the
pinned MAME source described in [SCI MMIO and IRQ](h83002-sci-mmio-irq.md).
The trace supports comparison with SCI output levels under a bounded profile.

## Bounded SCI transmit-level observer — PR #314 candidate

The opt-in `--observe-sci-tx` probe option retains at most 4,096 SCI transmit
line-level observations across both channels. It records the first observed
level per channel and subsequent level changes with the channel and peripheral
cycle; repeated same-level callbacks are omitted. Once full, it retains no more
records and increments a dropped counter. The default report remains `null`,
and observing the line does not change execution. Synthetic checks cover both
channels, exact values/cycles, repeated-level suppression, the storage bound,
overflow accounting, a CPU-driven SCI frame and observer-on/off invariance.
This is an output diagnostic, not MIDI decoding or audio evidence. No physical
UART or board behavior is inferred.

This English document no longer mixes Hungarian passages into its text. The original Hungarian-language passages are preserved in the [Hungarian notes archive](HUNGARIAN_NOTES_HUN.md).

# Bounded in-process boot probe

The project-owned `yamanix_an1x_boot_probe` runs the current H8H/bus subset,
not instrumented MAME and not a complete AN1x. Build with the normal CMake build.
It reads exactly1MiB from a caller-supplied file,never opens it for writing,and
requires an explicit `raw` or `swap16` view. The latter exchanges adjacent bytes
only in the private in-memory copy. It never rewrites or exports the image.
Odd reset targets reject before execution to catch common view mistakes.

```
yamanix_an1x_boot_probe /absolute/path/to/local-ROM.bin 100000 64 100000 swap16
```

Arguments are total requested cycles,slice cycles,global callback limit,and view.
Strict positive integer caps:600000000/1000000/125000000. No implicit GPIO input,
MIDI,PKS,LCD or DSP response is invented:an external GPIO read fails closed.
Output notifications are only counted;they do not emulate attached hardware.
An optional `--midi-byte=CYCLE:BYTE` schedules one SCI0 (RXD0) frame at
31,250 bits/s on the 16 MHz project timeline. Use decimal values and allow
the complete frame to fit within the cycle budget. The consumed input count
reports scheduler delivery. The [MIDI input reference](midi-input-milestone.md)
describes the RXD0 route used by the separate MAME research target.

## Optional FFFF60 TSTR write observer

Append `--observe-tstr` to opt into a fixed-capacity, read-only trace of
successful byte writes to `0xFFFF60`. The default remains off and reports
`tstr_observations: null`. When enabled, JSON reports the 64-entry capacity,
the number of dropped events, and each retained write's 24-bit address, width,
byte value, bus cycle, raw CCR, pre-dispatch PC/PPC/instruction state, and the
first completed dispatch continuation. If the bounded buffer fills, later
events are not retained and the dropped count makes the trace incomplete.
Writes are recorded only after the existing bus write returns successfully.
The observer does not change timer, IRQ, bus, or scheduling behavior; the ROM
remains read-only. It records raw flags without interpreting them.

The source comparison is limited to the pinned MAME commit in
`research/mame-source-lock.json` and the existing
`h8_timer16_device::tstr_w` identity in
`research/h83002-active-itu-contract.json`. That source method stores the TSTR
byte and calls each timer channel's `set_enable` from the corresponding bit.
This comparison does not prove the project CPU's timer execution, IRQ
causality, hardware equivalence, full firmware boot, MIDI, DSP, audio, or
REAPER acceptance. Synthetic ROM tests independently check address, width,
value, cycle, raw flags, instruction continuation, slice invariance,
opt-in behavior and bounded overflow.

One JSON report is written to stdout. Exit0 means the requested bounded cycle
budget completed;exit2 means execution stopped;exit1 means invalid arguments or
input. None means boot acceptance:firmware_boot_accepted is always false.
Valid execution reports add `stop_reason` (cycle_limit/callback_limit/execution_error)
and `callback_limit_requested`. The callback-limit classification matches the
runner's explicit guard error;it does not infer a device fault from CPU failure.
`completed_slice_cycles` and `completed_slice_callbacks` count only successfully
returned slices;work within a failed slice remains committed and can advance
`peripheral_cycle` beyond that count. CPU PC/state/substate and exact failed
memory access kind/address/width are diagnostic metadata,not a resumable checkpoint.
Decoder/callback stops have no fabricated memory fault. Reports omit ROM bytes,
register dumps and user file paths. The failed CPU is discarded on process exit.

## Exact-main TSTR observer rerun — 2026-11-17

Two read-only invocations used source tree `99ce57491916bde11ba81ed8ab7d962a9a704470`
and probe binary SHA-256 `527e9a182bc70b3d0cc47e684f7c86cc5ecd2e974aaa5bac5e2158691938f14b`.
Both requested 600,000,000 cycles, 1,000,000-cycle slices, a 100,000,000 callback
limit, `swap16`, `lcd-meg-vop3-idle-panel-experiment-16mhz`, no ADC input and
`--observe-tstr`. The stdout JSON reports were byte-identical, SHA-256
`d50bdf1985fe165d0a1a745d534f34c7f1597b5e71da8e6b33294fe0130be1ab`; local
reports remain in ignored `work/firmware-itustart-20261115a/`. The external ROM
SHA-256 was unchanged before and after at
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.

Both runs stopped at peripheral cycle 95,706,795 / post-fetch PC 0x1B64 on the
existing rejected byte write 0x23 to 0xFFFFE8. The observer retained exactly
three one-byte writes to 0xFFFF60, with zero dropped records: 0xFF at cycle
11,925,102 (raw CCR 0x29), 0xFB at 92,381,419 (raw CCR 0x25), then 0xFF at
92,384,136 (raw CCR 0x00). Records include pre-write CPU state and first
completed dispatch continuation; raw CCR remains uninterpreted. There were no
TX/clock edges or VOP3 sample passes. This bounded project-profile diagnostic
reproduces the TSTR trace and proceeds beyond FFFF60 to the later ADC boundary;
it does not establish timer/IRQ causality, physical hardware agreement, full
boot, MIDI, DSP, audio or REAPER acceptance.

## Exact-main synthetic ADC-scan diagnostic — 2026-11-17

The exact-main probe and run parameters, four report hashes, scan counts,
first-sample cycles, TSTR trace and explicit limits are recorded in
[the current verified main summary](STATUS.md#fresh-synthetic-adc-scan-firmware-diagnostic--2026-11-17).
The four invocations cover two eight-channel constant synthetic profiles
(0 and 1023) at two slice sizes (1,000,000 and 4,096 cycles). For each profile,
all trace and output fields stayed invariant across slice sizes except the
completed callback count, which follows batching. Each run reached its
600,000,000-cycle limit without a CPU failure or reported fault. All four
retained the same three successful TSTR writes, and the external ROM hash was
unchanged.

These synthetic callback values are diagnostic inputs, not hardware samples.
The result is limited to this project profile. It does not establish ADC
semantics, timer/IRQ causality, full hardware acceptance, a completed firmware
boot, MIDI/DSP behavior, audio or REAPER acceptance. The local reports remain
outside Git.

## 500-million-cycle bounded observation — candidate, 2026-11-13

The current local candidate raises the total-cycle and callback caps to
500,000,000 and 100,000,000. Four fresh read-only runs covered AN3=0/1023 and
4,096/1,000,000-cycle slices. All reached the cycle cap without access faults;
peripheral-cycle accounting ended at 500,000,000 or 500,000,002 because a
slice may be in flight. AN0–AN3 scan deadlines began at 95,707,121 and then
spaced by 256 cycles; AN4–AN7 began at 127,793,965 and likewise advanced every
256 cycles. These deadlines matched across slice sizes per input. Final PC was
103956 for AN3=0 and 103566 for AN3=1023. Every run had VOP3 sample passes
[0,0] and audio_accepted=false. Callback totals vary with slice partition and
are not an invariant. This is finite synthetic execution only, not completed
boot, hardware agreement, MIDI, audio or REAPER acceptance. ROM SHA-256
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe remained
unchanged and outside Git; no new MAME build/run was made.

## Local read-only observation — 2026-11-02

Input SHA256:`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Explicit swap16 view:reset target0x0009D8. At requested100000 cycles/slice64/
callback limit100000,the first stop is a data_write,width1,address0xFFFFEC,
PC0x0009EC,peripheral cycle30. Zero complete64-cycle slices returned before
that stop;this does not mean zero instructions or bus work occurred.
The original file SHA256 was identical before and after execution.

An initial raw-view experiment was discarded as a byte-order mistake;it is not
valid firmware-behavior evidence. Its odd reset vector motivated the new guard.
Next inspect the pinned original map and hardware documentation for0xFFFFEC,
then implement/test its supported contract without a permissive MMIO fallback.
This is a local subset observation,not a new original-MAME differential or hardware
trace. Reference source remains MAME706dbb73281320c54d7c2f16262c1c3786f2be01.

## ROM-free checks

`tests/check_an1x_boot_probe.py` creates only synthetic temporary images. It checks
bounded loop progress over three slice sizes,ROM and unmapped writes,unmodeled
GPIO reads,invalid EEPMOV extension,callback stop,strict numeric/view/size errors,
explicit swap16 and odd-vector rejection. Every supplied temporary file is hashed
before and after. The first decoder-control test mistakenly usedFFFF,which is a
valid immediate MOV opcode;the control now uses a known invalid EEPMOV extension.
The normal CTest target runs this contract;Ubuntu sanitizer CI includes it.

## Subsequent ABWCR candidate observation

The mapped-area ABWCR candidate accepts firmware write08 while preserving16-bit
ROM/SRAM. The same unchanged input now stops at ASTCR0xFFFFED/PC0x9F0/cycle36.
See [ABWCR contract](an1x-bus-width.md);the earlier cycle30 result remains historical.

## Subsequent ASTCR candidate observation

The selected2/3-cycle ASTCR profile now stops at WCER0xFFFFEF/PC0x9F4/cycle44,
with unchanged input. See [access-state contract](an1x-access-state.md).

## Subsequent wait-profile candidate observation

The selected WCR/WCER extension now stops at BRCR0xFFFFF3/PC0x9FC/cycle60,with
unchanged input. See [wait profile](an1x-wait-profile.md).

## Subsequent retained BRCR candidate observation

The unchanged BRCRFE profile now reaches0xFFFFAC/PC0xA18/cycle116,with64 completed
slice cycles and unchanged input. See [BRCR contract](an1x-bus-release.md).

## Hosszabb boot-diagnosztika — 2026-11-03

## Optional LCD profile — 2026-11-03

Append `lcd-16mhz` to explicitly connect the headless LCD at16 cycles/us.
JSON reports lcd_profile;default is disabled. This remains a bounded diagnostic,
not hardware boot acceptance. New stop and ownership/reset limits are documented
in [LCD machine integration](an1x-lcd-machine.md).

## Explicit MEG write observation — 2026-11-03

Profile lcd-meg-observe-16mhz opts into canonical MEG byte writes,including
explicitly observed original logging-only controls. JSON adds meg_observed_controls.
No status reads are supplied. Default and lcd-16mhz remain strict for MEG.
See [ownership,tests and next stop](an1x-meg-research.md).

## Separate original MEG status-stub experiment — 2026-11-03

lcd-meg-source-stubs-16mhz additionally opts into the original MAME zero-read
stubs at MEG offsets2/10/11. It is not physical status/ready behavior. JSON adds
meg_source_status_reads. Existing profiles stay strict. See
[scope and evidence](meg-source-status-experiment.md).
