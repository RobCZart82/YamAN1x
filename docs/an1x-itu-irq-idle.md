# ITU disabled interrupts and empty status flags

Primary: [H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),sections10.2.12–13,printed pages308–312.
TIER/TSR byte pairs:FFFF66/67,FFFF70/71,FFFF7A/7B,FFFF84/85,FFFF94/95.
Both reset/readF8. TIER accepts only low three enable bits clear (32 raw
values);224 enabling writes reject before effects. TSR supports all256 writes
because software cannot set its clear-only flags and no supported event can
set them. Reserved bits read one. No mutable IRQ/status state is necessary
within this profile. TSTR/counter start remains unsupported.

Pinned original MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
src/devices/cpu/h8/h8_timer16.cpp,inspected through the locked verified private
archive: H8H tier_update forcesF8 and maps low bits to IRQ_A/B/V;
isr_update clears corresponding flags on written zero;isr_to_sr returnsF8
plus those flags. This is source evidence,not an active-event differential.
Hardware read-before-clear history,sticky nonempty flags,counter events and
interrupt delivery remain outside this idle profile and require further work.
Paired TIER/TSR word accesses remain unsupported.

Independent tests cover2560 channel/value cases,160 accepted TIER writes,
1120 rejected enables,1280 TSR writes,reset/aliases/RAM/time/no-effect checks,
word rejection and129 CPU execution partitions. Existing negative counter
controls remain strict after mapping the two previously unknown bytes.

Read-only local1.04 ROM SHA256:
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
Private swap16 probe,100000 requested cycles,slice64,callback limit100000:
cycle_limit,100000 completed slice cycles,25339 callbacks,peripheral cycle100002,
PC634348,instruction state2949/substate2,9 GPIO notifications,0 TX/clock edges,
no CPU fault. firmware_boot_accepted remains false. This bounded observation
proves neither meaningful boot progress nor audio. No ROM bytes are exported.
Next establish a boot checkpoint/longer bounded diagnostic and required input
or missing-device evidence before extending active peripherals.

Follow-up stopped TIER storage supersedes the disabled-only restriction:
[an1x-itu-tier-stopped.md](an1x-itu-tier-stopped.md). TSR remains empty and
TSTR/counter start remains unsupported;active IRQ acceptance is still open.
