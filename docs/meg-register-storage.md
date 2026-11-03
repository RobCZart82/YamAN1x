# Storage-only MEG host component

Context:local firmware stops on byte write400007 at cycle9555129. Earlier
[bus-map.md](bus-map.md) schematic work identifies IC6 MEGCS for the400000
region,with mirrored address/chip-enable details still open. Locked original
AN1x yman1x.cpp maps only ROM and NVRAM;it configures a MEG device but does not
connect its host map. No unsupported400007 write is allowed by this change.

Pinned original MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
src/devices/sound/meg.cpp and meg.h,inspected via verified archive snapshot:
select_w stores8-bit index. Indexed byte banks4/5/8/9/E/F/14/16/17/18 store
256 entries each. C/D write constant high/low,12/13 write offset high/low.
Original words are signed16;this storage model preserves raw uint16 bit patterns
without claiming signed DSP arithmetic. Selector changes do not modify data.
Original device_start/device_reset are empty. Project cold storage is explicitly
zeroed;this is reproducible initialization,not a hardware reset-value claim.
reset_retaining_state mirrors empty original reset behavior.

MegRegisterStorage accepts only those storage offsets and selector0. Logging-only
controls1/2/3/7/A/15,holes/read-only offsets,unknown offsets and physical CPU
addresses reject before mutation. It exposes no fake read/status method. Original
s2_r/s10_r/s11_r return zero stubs;these cannot justify host readiness or completed
operations. The original bank methods log only changes;this model stores values
without logging or audio. It has no clocks,scheduler,CPU MMIO,reset coupling or
MEG effects execution. It does not advance firmware beyond its current stop.

Independent tests:655360 indexed byte writes,65536 selected word-lane pairs,
all65536 word patterns for both word banks,slot/bank isolation,invalid-offset
strong state retention,retaining reset and explicit cold zero policy.
Local reference checker:
`python3 tools/check_meg_storage_reference.py --archive /absolute/local/mame-master.zip`
extracts15 unchanged original storage handlers into a temporary ASan/UBSan
harness.720896 comparisons pass from explicitly matched zero initial state.
This excludes uninitialized original start state,host control/status behavior,
CPU byte-lane mapping,mirrors,timing and audio. No original implementation copied
into the repository and no Yamaha data exported. Next establish exact host
mapping and separately model or reject unknown controls before machine integration.
