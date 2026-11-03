# Stopped ITU interrupt-enable configuration

Five TIER bytes FFFF66/70/7A/84/94 now store low three enable bits separately;
readback isF8|value,resetF8. TSR remainsF8 with clear-only writes because no
supported counter start,capture or event can set flags. TSTR remains unmapped
and fails closed,including after enabling every TIER bit. Enabling alone neither
sets a status flag nor delivers an interrupt. Existing paired TIER/TSR words
remain unsupported. Supported reset restores all enables while retaining time/RAM.
This supersedes the earlier disabled-only write restriction in an1x-itu-irq-idle.md;
its historical observations remain true for that earlier revision.

Pinned original MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
h8_timer16.cpp:tier_w updates the counter,stores data,calls tier_update and
recalculates events. H8H tier_update ORsF8 and maps low bits to IRQ_A/B/V;
isr_update only clears flags;isr_to_sr returnsF8 plus current flags. Source read
through the verified locked archive snapshot. Here counters are stopped with
empty flags,so only configuration storage is reachable. Active event generation,
flag assertion,pending IRQ when enabling against a nonempty flag,and controller
vector delivery remain unimplemented and must precede any counter start support.
This is not a shortcut accepting a running timer or complete hardware fidelity.

Independent checks sweep1280 channel/raw values using arithmetic readback oracle,
verify channel isolation,empty status,no callbacks/deadline,reset/RAM/time and
129 CPU partitions writing enabled TIER4. Negative TSTR control still fails.
Existing idle test retains status/software-set,reset,word and disabled profile
checks while expanding TIER acceptance. Strict ASan/UBSan passed.
`tools/check_itu_tier_reference.py --archive /absolute/local/mame-master.zip`
extracts three unchanged H8H tier_update/isr_update/isr_to_sr methods into a
temporary ASan/UBSan harness;1280 stored-TIER/empty-TSR comparisons pass.
It excludes the original active counter/update/event scheduler and does not
prove active flag or interrupt behavior. No original source copied into repo.

Read-only1.04 ROM SHA256:
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
100000000 cycles,20000000 callbacks,swap16,lcd-16mhz,slices4096/1000000:
identical next stop data_write400007,width1,PC0xD9F2,peripheral9555129,
instruction_state26844/substate0,6316 GPIO notifications,0 TX/clock edges.
Completed slice cycles9551872/9000000,callbacks1620022/1521596;failed slice
work is excluded. Original hash unchanged before/after,no image data exported.
No boot/audio acceptance. Next inspect400007 in the pinned machine map.
