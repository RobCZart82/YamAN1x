# Bounded firmware diagnostics beyond LCD/P9

The CLI's finite limits are250000000 requested cycles,1000000 slice cycles and
50000000 callbacks. The callback cap was raised to let the bounded scan profile
reach the cycle limit without making the probe unbounded. Existing fail-stop semantics
remain:callback exhaustion poisons CPU and discards the process state;never
resume that owner. There is no retry/cancellation of hosted CI in this change.

Valid execution JSON adds stop_reason and callback_limit_requested. stop_reason
is cycle_limit on a completed budget,callback_limit on the runner's exact guard
exception text,and execution_error otherwise. status remains cycle_limit/stopped
and exit codes remain0/2;input errors still return1. fault_access remains null
for callback/decode failures and identifies actual failed bus operations only.
Completed-slice counters still omit any work committed within a failed slice.
No actual total callback count is fabricated from the completed-slice count.

Reference:existing pinned H8 runner ordering contracts. Original source remains
MAME706dbb73281320c54d7c2f16262c1c3786f2be01. Project-owned runner guard text
is explicitly inspected in src/h8_execution_runner.h;CPU/bus behavior is unchanged.
The callback cap is diagnostic host policy,not a MAME hardware timing claim.
Synthetic CLI controls cover tiny exhaustion,maximum accepted finite guard,
above-limit/zero rejection,normal cycle completion and GPIO error classification.
Existing runner fail-stop and partition tests remain part of the full suite.

Read-only local1.04 ROM SHA256:
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
With100000000 requested cycles,20000000 callback limit,swap16,lcd-16mhz:
4096 and1000000 cycle slices both stop on byte data_writeFFFF94 (TIER4),
PC0x40A62,peripheral cycle8798299,instruction_state1863680/substate0,
5269 GPIO notifications,0 TX/clock edges,error Unsupported ITU interrupt enable.
Completed slice cycles8798208/8000000;callbacks1494954/1353465 respectively.
This is a real unsupported IRQ-enable boundary after LCD/P9,not the old callback
limit. Input hash identical before/after,no ROM bytes exported. No boot or audio
acceptance;next inspect active ITU flag/IRQ requirements before extending support.
