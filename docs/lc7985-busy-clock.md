# LC7985 bounded busy timing

Standalone `Lc7985BusyClock` owns absolute cycle/deadline only. It requires an
explicit positive integral cycles-per-microsecond scale;fractional conversion
and actual machine-frequency selection remain caller responsibilities. This
component is not connected to the machine or probe and fabricates no status
address,busy input,display data or boot acceptance.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01:
`src/devices/video/lc7985.cpp`,SHA256
29370236ef63d52d5c4ba87873af401c84cf1766fa56d023d531aa06b280a8f0.
Read through the hash-verified private archive snapshot. Original device_reset
sets10ms busy;busy() sets the flag and adjusts the clear timer;clear_busy_flag
clears it. ir_w first returns when busy,then highest-bit command decoding selects
40us for04..FF,16400us for01..03,and no operation/busy for00. In particular03
uses home precedence and is still16400us. status_r ORs address with busy80.
Original dr_r/dr_w have no busy() call and are not gated by the instruction
busy guard. Those data semantics must not be inferred from the instruction API.
This is pinned source evidence,not a new original runtime differential or a
physical timing measurement. Original16400us is preserved without correction.

API:reset(cycle) starts10000us;advance(cycle) settles time;instruction(cycle,
byte) returns false while busy so a future controller must ignore all command
state changes. Accepted commands return true. Exactly at deadline busy is false.
Repeated status polling cannot extend the deadline. Successful busy-rejected
instructions still advance the component time. No-op00 leaves the past deadline
unchanged. Snapshot has cycle/deadline;no external scheduler is required yet.
All backwards times and overflowing reset/command deadlines reject before state
mutation. Invalid scale rejects construction. Scale multiplication is bounded.
Reset retention/state is a project-owned component policy,not whole-machine reset.

Tests:all256 commands at1/16/24 cycles/us,independent bit-based delay oracle,
last-busy/first-ready instant,ignored command deadline retention,reset,reversed
time,no-op,invalid scale,addition overflow and10001 split-time partitions.
Strict ASan/UBSan passed. Next implement/test controller address,mode,DDRAM and
CGRAM behavior before connecting this timing to the LCD GPIO adapter. Rendering,
font ROM,GUI,hardware-equivalent boot and audio remain outside this step.
