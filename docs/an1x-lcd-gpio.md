# LCD GPIO controller boundary

`An1xLcdGpio<Controller>` connects the existing `LcdBus` wiring to an explicitly
supplied controller. It is not a controller implementation or a machine sink.
The caller routes resolved GPIO output levels (including H8DigitalPort pull-ups)
for ports10(A),11(B),9. Direction mixing remains in the existing H8 port model.
Unknown output ports return false for caller routing;unknown input ports throw.
PA/PB input reads expose only the latched LCD bus reply and pull-up bits.

Controller methods:status_read(cycle),data_read(cycle),instruction_write(cycle,
byte),data_write(cycle,byte). Only E rising reads and E falling writes invoke
them,with the exact supplied absolute cycle. Input polling does not call the
controller again. The caller owns monotonic time,reset/lifetime and fail-stop
handling;discard the adapter after any controller exception. No reset/save-state
or reentry contract is added. Controller reference must outlive the adapter.

Wiring evidence is the earlier service-schematic research in
[lcd-milestone.md](lcd-milestone.md) and the established MAME research patch
`research/mame-lcd-controller.patch`. Split data:PA0/2/3/4/5,PB1/6/7;RS=PB2,
RW=P90,E=P95. Existing H8DigitalPort already resolves input/output direction.
This adapter preserves that wiring and passes time rather than inventing replies.

Pinned original MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
`src/devices/video/lc7985.cpp` inspected through the verified locked archive:
ir_w at124 rejects while busy,status_r at184 returns address|busy80,
dr_w at189 and dr_r at209 own controller memory/address behavior.
The adapter does not reproduce those methods or claim their timing acceptance.
No always-ready response is supplied. No new original runtime differential.

Independent tests assemble each data bit,cover512 byte/RS cases,read/write edge
direction,duplicate levels,read-latch stability,cycle propagation,unknown-port
routing and idle pull-ups. Synthetic controller counters are test-only evidence.
Real firmware still stops on the unmodeled Port B input in the existing probe.
Next implement/test controller state and cycle/busy semantics separately before
connecting to the machine. No ROM data,GUI or plugin change.
