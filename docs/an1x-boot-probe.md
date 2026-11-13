# An1X Boot Probe

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
Strict positive integer caps:500000000/1000000/100000000. No implicit GPIO input,
MIDI,PKS,LCD or DSP response is invented:an external GPIO read fails closed.
Output notifications are only counted;they do not emulate attached hardware.
No RX input schedule is supplied by this initial CLI.

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
