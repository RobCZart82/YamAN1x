# Active bus refresh clock without IRQ enable

The standalone [pinned timer policy](h8-refresh-timer.md) now belongs to An1xSciBus.
Every successful absolute bus advance settles the timer to the completed bus
horizon. Byte MMIO forwards control,counter and compare;all seven clocks are
supported. CMIE writes remain rejected before mutation. No timer deadline is
scheduled because no timer outward IRQ/callback is enabled;CMF is settled lazily
at each bus advance and observed by subsequent reads. SCI/input callback reentry
remains fail-stop;failed owners are not resumable. No physical refresh pin or
DRAM mode is added. Paired refresh word accesses still reject.

Supported peripheral reset resets the timer at the existing absolute cycle.
Counter/compare/control cold values00/FF/07 remain. CPU slice debt never rewinds
the bus horizon;absolute divider phase survives normal slicing. This retains
the pinned original clear-on-zero CMF policy,not a hardware read-history gate.
Active IRQ integration remains a separate event-ordering/CPU acceptance task.

Independent bus tests cover seven per-cycle divider oracles,CMF,reset/restart,
bus completion visibility,nonrewinding debt and257 CPU partitions. The existing
raw-control test now allows128 encodings with CMIE clear and rejects128 with
CMIE set. The unchanged standalone original-handler tool passes524288 snapshots
under ASan/UBSan against locked MAME706dbb73281320c54d7c2f16262c1c3786f2be01.

Local1.04 image SHA256 remains
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Explicit in-memory swap16,100000 requested cycles,slice64,callback limit100000:
192 completed slice cycles/55 callbacks,next failure byte writeFFFFE8,
PC0xA44,peripheral cycle204,9 GPIO output notifications,no TX/clock edges.
FFFFE8 is the ADC control/status address in the pinned H83002 map;its complete
contract still needs investigation. These subset diagnostics do not establish
full firmware boot,hardware timing,audio or plugin acceptance. No ROM exported.
