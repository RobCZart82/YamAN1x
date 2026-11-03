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
Strict positive integer caps:100000000/1000000/20000000. No implicit GPIO input,
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

A #186 utáni main mind5 CI-ellenőrzése sikeres. Változatlan1.04 ROM,privát
swap16,10000000 kért ciklus,1000000 callback-limit mellett a próba482160.
perifériaciklusnál megáll:byte data_readFFFFD6 (Port B),PC0x3F666,
instruction_state1864224/substate0. Szeletméret1/64/257 esetén azonos fault,
PC és perifériaidő;teljes visszatért szeletciklus rendre482158/482112/482132,
callback739440/116288/104976.9 GPIO,0 TX/clock él. A hibás szelet munkája nem
szerepel a completed_slice számlálóban,ezért ezek eltérése önmagában nem hiba.
Ez diagnosztikai részprofil,nem firmware-boot-,hardver- vagy hangelfogadás.

Rögzített eredeti MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
h83002.cpp132. sor:FFFFD6 portb port_r/dr_w;159. sor utáni konfiguráció
PORT_B,resetDDR00,mask00. Ellenőrzött privát archívumpillanatképből olvasva.
A chipcímtér nem bizonyítja az AN1x külső huzalozását vagy bemeneti értékét.
Következő:Port B külső eszköz/protokoll szerződés vizsgálata meglévő kutatási
forrásokból,majd explicit bemeneti modell és független tesztek. A hibajelzést
nem oldjuk fel bizonyítatlan fix bemenettel. ROM SHA előtte/utána azonos:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.

## Optional LCD profile — 2026-11-03

Append `lcd-16mhz` to explicitly connect the headless LCD at16 cycles/us.
JSON reports lcd_profile;default is disabled. This remains a bounded diagnostic,
not hardware boot acceptance. New stop and ownership/reset limits are documented
in [LCD machine integration](an1x-lcd-machine.md).
