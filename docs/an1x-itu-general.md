# Stopped ITU general-register storage

Primary:[H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),sections10.2.8 and10.3.1,printed pages302–303 and313–314.
Ten GRA/GRB words map channels0–4 atFFFF6A/6C,FFFF74/76,FFFF7E/80,
FFFF88/8A andFFFF98/9A. Cold/reset valueFFFF. Aligned word access and direct
big-endian high/low byte access are supported;byte writes preserve the other
lane. Raw aliases are masked to24 bits. Odd words reject before any write.

Only storage while ITU channels remain stopped is implemented. TSTR,TCR,TIOR,
TIER,TSR,TCNT and buffer registers remain unsupported;there is no reachable
counter-start or capture/output configuration. Consequently these writes do not
schedule compare events,update flags,transfer buffers,drive pins or request IRQs.
Supported peripheral reset restores all tenFFFF values without clearing RAM or
rewinding the absolute cycle. Instruction fetch does not gain generic MMIO access.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01 h8_timer16.cpp tgr_r/w
reads storage and combines masked writes after update_counter,before recalc_event.
Inspected through verified_archive with the locked private snapshot. This
project-owned stopped subset preserves storage/lane behavior;active counter,
compare/capture and event recalculation are not claimed as original runtime
differential or hardware acceptance.

Tests cover655360 full word values,independent byte-lane equations,ten-address
isolation,24-bit aliases,cold/reset/time/RAM,no-events,odd-word and unsupported
counter/buffer guards,and129 CPU partitions of a synthetic GRA0 word write.
Unchanged local1.04 SHA256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Private explicit swap16 probe100000 cycles/slice64/callback limit100000 next
stops at byte writeFFFF64 (TCR0),PC0xA76,peripheral cycle299,after256 completed
slice cycles/69 callbacks and9 GPIO outputs. Next inspect stopped control
configuration and clock/clear semantics before supporting TCR. No full boot,
audio or physical timing acceptance;no ROM export/modification.
