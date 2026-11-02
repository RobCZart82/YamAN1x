# ITU output-disable subset

Primary:[H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),section10.2.5,printed pages297–299.
TOER atFFFF90 resets toFF:permission bits are set but output still depends on
TIOR/TMDR/TFCR configuration. No active ITU pin function is currently supported.
Only all-output-disable writes are accepted:raw00/40/80/C0 normalize toC0.
All252 writes requesting any output permission reject before mutation/effects.
The cold/resetFF readback does not mean writingFF is supported;explicit enabling
needs the missing physical pin-function contract. Supported reset restoresFF.
No output callback,pin level,counter/PWM or IRQ is synthesized. TOCR and paired
TOER/TOCR word accesses remain unsupported.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01 h8_timer16.cpp toer_r/w
is read00/write-ignored. Verified through the private locked archive snapshot.
That stub is not active hardware evidence;the bounded manual-based profile
explicitly uses FF cold and C0 disabled readback instead. No original active
output differential or full ITU acceptance is claimed.

Tests cover256 encodings,4/252 acceptance,cold/readback/reset distinction,
alias/RAM/time/no-effects,word rejection and129 CPU partitions.
Unchanged local1.04 SHA256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Firmware writesC0;private explicit swap16 probe100000 cycles/slice64/callback
limit100000 next stops at word writeFFFF6A (channel0 GRA),PC0xA6A,
peripheral cycle277,after256 completed slice cycles/69 callbacks,9 GPIO outputs.
Next inspect stopped-channel general-register word semantics and reset/byte
ordering before supporting them. No full firmware boot,physical timing,audio
or plugin acceptance. ROM bytes stay local and unchanged.
