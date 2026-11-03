# Native VOP3 register backend

A noncopyable/nonmovable RAII owner connects the existing ABI2 bridge to the
standalone full-word host adapter. Both chips belong to one private handle;
no raw handle escapes. Internal bridge error -3 destroys the handle and later
calls fail closed. Invalid arguments/masks preserve native state. Construction
checks ABI2 and allocation. Backend must outlive its referencing host adapter.
1024 independent coefficient/address/value cases check fixed BE snapshot offsets,
exact write/read counters,strict read rejection,two-chip and owner isolation;
32 reconstruction epochs check fresh state and unrelated-owner preservation.
A separate fault-injected ABI fixture verifies ABI/allocation rejection,four
internal-error paths,exact destruction and no callbacks after failure. It does
not simulate a real DSP fault. No samples are advanced,so sample counters remain zero. No clock,CPU bus,
firmware boot,MIDI,table ROM or audio acceptance is added. Control status remains
only the explicitly selected firmware-contract experiment from #199.
The pinned original MAME reference in research/mame-source-lock.json is unchanged;
this integration reuses the existing project bridge,not a new original-MAME
method implementation or physical YSS236 proof. Next: explicit CPU word-bus/sink
routing and bounded firmware diagnostics. Local/remote validation recorded in
DEVELOPMENT_LOG; own PR CI and automatic review pending.

