# VOP3 reg22 readback integration contract

Research reconciliation:2026-11-04. No runtime change in this step.

## Evidence and boundaries

The current standalone probe stops on word read440044 (reg22),cycle11260853,
CPU snapshot PC54678,after10236 chip0 writes. This snapshot PC is not the same
instrumentation field as the historical MAME bus trace's next PC54676;do not
claim an instruction-address mismatch or exact trace equivalence from those
numbers. Current native reads remain0 because the host adapter rejects reg22
before calling the backend. Both sample counters remain0.

The earlier [firmware analysis](eg-readback-milestone.md) identifies SELECT0..15
scans,16-bit RAM storage and signed comparisons with bounds-256..16384 (channel14
skips the lower bound). A separate channel14 comparison expectsC000. Another
routine compares a RAM level multiplied by64 with the readback. Historical
[trace audit](eg-readback-milestone/eg-audit.json) and
[verification](eg-readback-milestone/verification.json) retain that evidence.
These are firmware-consumer observations,not a measured physical YSS236
readback specification. The historical run uses installed DMS;the current
headless in-process sink leaves P9 inputs high. They are distinct profiles.

Existing ABI2 in src/vop3_bridge.cpp can explicitly select profile1:
selected index = SELECT &511,accept only0..15;model EG level multiplied by64,
truncated toward zero,saturated signed16. Nonfinite levels fail with-3;
invalid index returns-4 before output/counters change. ABI profile0 returns
unsupported zero for22. The strict host adapter currently exposes neither
profile0's zero fallback nor profile1's experiment. Reg21 stays separate.

The [existing timing test](../tests/check_vop3_timing.py) checks synthetic signed
levels,independent chips/channels,saturation,index rejection,independent
pre-crossing exponential evolution,split advancement and2400 actual model
passes/chip. It records target-vs-aim disagreement after crossing. Successful
transport tests do not settle physical EG phase,rounding,saturation or timing.

## Next bounded implementation

1. Add a separately selected host read policy,default reject. Allow22 only
   with an explicitly configured experimental native backend;do not alias21.
2. Keep native profile selection explicit and lifetime-scoped. Preserve invalid
   index/nonfinite fail-stop semantics and per-chip accounting. Add independent
   full signed input/index boundary tests plus strict-default regression.
3. Connect that policy through a separately named firmware probe. Keep sample
   passes0 and report the model assumption. Probe both slices and document
   whether theC000 condition remains unmet;never force the expected response.
4. Treat sample advancement as another step. Verify CPU-to-model time conversion,
   same-time ordering,upload inhibition and bounded advancement before enabling
   it. The model48kHz value is not physical AN1x clock evidence.

No hardware status,complete firmware boot,MIDI,audio or plugin acceptance.
Original MAME reference stays pinned in research/mame-source-lock.json;reg22's
experimental bridge conversion is project code,not an original MAME method.
No ROM bytes,disassembly or DSP program data are included here.
