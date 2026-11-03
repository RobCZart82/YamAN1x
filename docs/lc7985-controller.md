# Headless LC7985 controller state

`Lc7985Controller` composes the explicit-scale busy clock with80-byte DDRAM,
64-byte CGRAM,address counters,selection,function,display,entry and shift state.
It implements the four methods consumed by An1xLcdGpio. It has no renderer,
character ROM,GUI or machine/probe connection. Caller supplies cycles/us and
monotonic absolute cycles;no actual AN1x clock conversion is asserted yet.
State inspection is read-only,not a restore/save-state or hardware acceptance API.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01:
`src/devices/video/lc7985.cpp` SHA256
29370236ef63d52d5c4ba87873af401c84cf1766fa56d023d531aa06b280a8f0.
Header confirms DDRAM80/CGRAM64. Verified private archive snapshot supplies the
nine original reset/address/shift/instruction/status/data methods for the local
reference check. This implementation is independently written around that
behavior;original methods exist only in a temporary local compiled harness.

Reset fills DDRAM20/CGRAM00,zeroes counters/shift/modes,selects CGRAM and entry02,
then sets10ms busy. Instructions use highest-bit priority and the existing
busy clock. Home selects DDRAM;clear preserves prior selection and CGRAM address,
clears DDRAM/DDAC and sets entry increment. Function/display/entry retain raw
command bytes as original. Status returns selected address OR busy80.
DDRAM increment wraps79->0 in one-line mode;two-line39->64 and103->0.
Decrement reverses those boundaries. CGRAM wraps modulo64. Display shift wraps
modulo80;entry shift applies to DDRAM writes only,not reads. Data accesses remain
allowed during busy and neither start nor extend a busy interval,matching the
original handlers. Busy instruction writes change no controller registers.

Safety boundary:DDRAM data index is checked before time or memory mutation;
addresses whose original mapping lies outside80 bytes throw. Address commands
and status can still expose those addresses. Original potentially out-of-bounds
array behavior is not emulated. Backwards time and overflowing instruction/reset
deadlines reject before state changes. No unsupported status is silently zeroed.

Independent tests cover114688 byte/address/direction/line-mode memory cases,
all256 command busy controls,address/status/counter wrap,read advancement,
clear/home selection,reset and an end-to-end busy/ready split GPIO latch.
`tools/check_lcd_controller_reference.py --archive /absolute/local/mame-master.zip`
extracts nine unchanged original methods from the locked archive into a temporary
ASan/UBSan harness.50000 deterministic mixed command/data/read/reset/state
comparisons pass,excluding data indices outside the original arrays. This does
not establish physical LCD timing,full reset ordering or firmware boot.
Next wire an explicitly clocked controller into a bounded research-machine sink
with routing/reset/fail-stop tests before observing the unchanged local firmware.
