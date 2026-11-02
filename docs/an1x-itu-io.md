# ITU no-output compare selection

Primary:[H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),section10.2.11,printed pages306–308.
Five TIOR bytes atFFFF65/6F/79/83/93 select GRA/GRB output compare or input
capture. Only reset-style compare selection without output is supported:
raw00/08/80/88 normalize to88 (reserved bits7/3 read one). Other252 values per
channel reject before effects. No pin waveform,input capture,edge sampling,
compare event or IRQ is synthesized. TSTR still rejects,so channels stay stopped.
Supported reset retains88;paired TCR/TIOR words remain deliberately unsupported.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01 h8_timer16.cpp base
tior_r returns00 and tior_w ignores data beyond logging. Inspected through the
verified locked private archive snapshot;no H8H implementation was found there.
This stub cannot justify silently accepting physical output/capture modes.
The selected profile follows manufacturer reserved-bit readback instead,without
claiming original active I/O differential or full ITU hardware acceptance.

Independent tests cover all1280 channel/value combinations,4/252 acceptance per
channel,cold/reset/aliases/RAM/time,no-effects,word rejection and129 CPU cuts.
The previous stopped-control negative test now uses unmapped counter-byte69
instead of the newly supported TIOR0 address.
Unchanged local1.04 SHA256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Firmware writes88;private explicit swap16 probe100000 cycles/slice64/callback
limit100000 next stops at byteFFFF66 (TIER0),PC0xA7E,peripheral cycle315,
after256 completed slice cycles/69 callbacks and9 GPIO output notifications.
Next inspect IRQ-enable semantics and stopped status flags before handling TIER.
No complete boot,audio or plugin acceptance;ROM unchanged and not exported.
