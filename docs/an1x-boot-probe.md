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
Strict positive integer caps:100000000/1000000/1000000. No implicit GPIO input,
MIDI,PKS,LCD or DSP response is invented:an external GPIO read fails closed.
Output notifications are only counted;they do not emulate attached hardware.
No RX input schedule is supplied by this initial CLI.

One JSON report is written to stdout. Exit0 means the requested bounded cycle
budget completed;exit2 means execution stopped;exit1 means invalid arguments or
input. None means boot acceptance:firmware_boot_accepted is always false.
`completed_slice_cycles` and `completed_slice_callbacks` count only successfully
returned slices;work within a failed slice remains committed and can advance
`peripheral_cycle` beyond that count. CPU PC/state/substate and exact failed
memory access kind/address/width are diagnostic metadata,not a resumable checkpoint.
Decoder/callback stops have no fabricated memory fault. Reports omit ROM bytes,
register dumps and user file paths. The failed CPU is discarded on process exit.

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
