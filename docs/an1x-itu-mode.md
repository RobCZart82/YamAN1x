# Bounded ITU normal-mode configuration

Primary:[H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),sections10.2.3–10.2.4,printed pages292–296.
TMDR atFFFF62 selects PWM,phase counting and channel2 overflow direction.
Only normal non-PWM/non-phase mode is accepted. FDIR is retained as a configuration
bit;there is no counter or overflow flag implementation yet. Raw00/20/80/A0 read
80/A0;other252 values reject. Reset restores80. TFCR atFFFF63 accepts only its
unbuffered normal profile:raw00/40/80/C0 readC0;other252 values reject.
No complementary/reset-synchronized PWM,buffer transfer,counter start,IRQ or
pin waveform is synthesized. Paired TMDR/TFCR word access remains unsupported.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01 h8_timer16.cpp tmdr_r/w
and tfcr_r/w are read00/write-ignored stubs,inspected through verified_archive.
Those handlers cannot validate active modes. Project-owned readback uses the
manual's reserved bits and FDIR configuration instead;no original active-mode
differential or full ITU hardware acceptance is claimed.

Tests enumerate all256 values for each register,independent acceptance sets,
alias/reset/memory/time,word rejection and129 CPU partitions of an A0 write.
Unchanged local1.04 SHA256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Firmware writesA0 to TMDR;the paired bounded initialization now advances to byte
writeFFFF90 (ITU output master enable),PC0xA62,peripheral cycle263,after256
completed slice cycles/69 callbacks and9 GPIO output notifications.
Probe uses explicit private swap16,100000 cycles,slice64,callback limit100000.
Next inspect output-enable semantics before adding its profile. No full boot,
audio,physical timing or analog input acceptance;ROM remains unchanged/local.
