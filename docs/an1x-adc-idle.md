# Idle ADC configuration subset

Primary reference:[H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),section14.2.2,printed pages484–486.
Pinned original:MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
src/devices/cpu/h8/h8_adc.cpp adcsr_w/device_reset and H8_ADC_3337 mode_update,
plus h83002.cpp map. Inspected through the verified private archive snapshot.

ADCSR byteFFFFE8 starts at00. With ADST and ADIE clear,the low five bits retain
channel,scan and clock selection. ADF cannot be software-set. Accept raw00–1F
and80–9F (64 values);reject192 conversion-start/IRQ-enable encodings before
mutation or outward effects. Configuring SCAN does not start conversion.
Supported peripheral reset clears this register without rewinding time/RAM.
No analog sample,result register,conversion completion,trigger or ADC IRQ is
synthesized. ADCR atFFFFE9 and resultsFFFFE0–E7 remain unsupported. Word access
across ADCSR/ADCR rejects. External ADC trigger state is not connected.

Independent tests enumerate256 writes,reset/alias/RAM/time retention,no effects,
unknown results/trigger registers,word rejection and129 synthetic CPU partitions.
This is bounded original-source/manual evidence,not an active ADC runtime
comparison or hardware-conversion acceptance. Active conversion needs analog
input ownership,timing,result/TEMP semantics and IRQ/continuation validation.

Unchanged local1.04 SHA256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Firmware writes03 (channel3,idle). Explicit private swap16 probe with100000 cycles,
slice64,callback limit100000 next fails at byte writeFFFF61,PC0xA56,
peripheral cycle239,after192 completed slice cycles/55 callbacks,9 GPIO outputs.
The pinned map identifies FFFF61 as ITU TSYR;its behavior needs separate research.
No full boot/audio/plugin acceptance or physical analog voltage is claimed.

## Slow-scan restart time guard — 2026-11-10

The later slow-scan component stores its last accepted start, restart or service
time. Restart before that point rejects without changing the pending sample,
result lanes, bank, flags or deadline. Equal timestamps remain valid. Validation
and deadline-overflow failures leave the time watermark unchanged. Explicit
component reset clears that watermark for a new epoch; the enclosing bus still
owns its monotonic absolute clock. The bus already supplied monotonic times.
Tests cover 1044 stale restart cases across both banks, continued sample delivery,
same-time restart, invalid sample/overflow recovery and zero-time use after reset.
No firmware or hardware behavior is inferred from these synthetic guards.
