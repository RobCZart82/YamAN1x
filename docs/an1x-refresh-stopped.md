# Stopped refresh timer subset

Primary reference: [H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual), sections7.2.2–7.2.4, printed pages145–147.
Pinned original reference: MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
src/devices/cpu/h8/h8_refresh.cpp and h83002.cpp, read through verified_archive
using research/mame-source-lock.json. Original rtcnt_update immediately returns
for clock selection0;reset count00/compareFF;RTMCSR reads reserved low bits as1.
The original map places control/counter/compare atFFFFAD/AE/AF.

Only CKS=0 and CMIE=0 are supported. Sixteen raw control values are accepted:
00–07 and80–87. Software cannot set CMF,which remains clear without any clock;
reserved low bits read111. Other240 controls reject before mutation/effects.
RTCNT and RTCOR byte writes/readback are supported while halted,including masked
aliases;updates do not advance the counter,raise IRQs or schedule events. Supported
peripheral reset restores00/FF without rewinding time or clearing RAM. All paired
word accesses across the refresh block remain unsupported. No running timer,
CMF read-before-clear sequence,interrupt scheduling or DRAM refresh is claimed.

Tests cover256 control bytes,65536 independent count/compare pairs across elapsed
cycles,reset,aliases,word failures and129 synthetic CPU partitions. This is
manual/pinned-source bounded evidence,not an active original-runtime timer
comparison. Running-clock integration needs independent prescaler/compare/IRQ
and continuation checks before enabling any CKS value.

Local unchanged firmware writes0F to RTMCSR (CKS=1,/2);the candidate intentionally
still rejects that request atFFFFAD,PC0xA1C,peripheral cycle124. No boot progress
is claimed by this preparatory subset. ROM bytes stay local and unchanged.
