# Retained ITU independent-channel profile

Primary reference:[H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),section10.2.2,printed pages290–291.
The manual names addressFFFF61 TSNC;the pinned MAME map calls it TSYR.
The five low bits select channel synchronization;upper three bits read one.
Only the unchanged E0 profile is supported:8 raw encodings with low five bits
zero normalize toE0. All248 synchronization-enable encodings reject before
outward effects. Supported reset still readsE0. No ITU counter,start,event,IRQ,
synchronous preset/clear or pin waveform is simulated. TSTR and adjacent TMDR
remain unsupported;paired TSTR/TSNC words reject before any partial operation.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01 h8_timer16.cpp has a stub
TSYR reader returning00 and an ignored writer. Inspected via verified_archive
against research/mame-source-lock.json. This is not hardware evidence and was
not used to silently accept unsupported synchronization. Readback follows the
manufacturer's E0 retained profile,explicitly differing from that stub. No
original active synchronization differential or full ITU acceptance is claimed.

Independent tests cover256 encodings,8/248 acceptance,aliases,readback,
reset/memory/time,word rejection and129 synthetic CPU partitions.
Unchanged local1.04 SHA256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Firmware writesE0. Explicit private swap16 probe100000 cycles/slice64/callback
limit100000 next stops at byte writeFFFF62 (TMDR),PC0xA5A,peripheral cycle247,
after192 completed slice cycles/55 callbacks and9 GPIO output notifications.
This is not complete boot,audio or hardware timing. Next inspect TMDR before
implementing a bounded profile;ROM remains local and unchanged.
