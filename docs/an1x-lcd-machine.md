# LCD research sink and bounded machine integration

An1xLcdResearchSink owns the controller and its GPIO adapter in a heap state,
with controller lifetime preceding adapter lifetime. Copy is disabled to prevent
references into another owner. It accepts an explicit integral cycles/us scale.
GPIO10/11/9 outputs route to LCD;other output ports are ignored in this diagnostic
sink,not simulated as peripherals. Only PA/PB input is modeled. Other input
ports fail closed. TX/clock/dma methods supply no audio or device replies.

The controller and bus latch remain external state during supported machine/bus
reset. A cold machine epoch requires explicit sink.cold_reset while stopped
before machine.cold_restart;no automatic coupling is claimed. Reconstruction
allocates before swapping;the old sink remains intact on construction failure.
Time rewinds and controller/input errors poison the sink epoch;subsequent GPIO
callbacks reject until explicit reconstruction. Bus/CPU own their existing
fail-stop logic. No outward callback can reenter from this sink.

Probe optional argument:`lcd-16mhz` after raw|swap16. Omission retains the old
strict no-LCD profile;JSON adds lcd_profile. Unknown profiles reject. This named
research profile uses16 cycles/us;locked original yman1x.cpp configures H83002
at16_MHz_XTAL (HD6413002FP16). This ties the profile to the source clock,not a
physical timing measurement or proof of complete scheduler/peripheral fidelity.
The probe lifetime constructs sink before bus/CPU;it does not expose reset.

ROM-free tests use real bus DDR/pin composition to check reset busy,latched status
until a new E edge,ready at deadline,split command data,external LCD retention on
subset reset,unknown GPIO poisoning,reconstruction,time reversal,and129 CPU
execution partitions. CLI tests verify LCD-enabled Port B reads,strict unknown
GPIO,default disabled behavior,unknown profile rejection,and immutable inputs.
Initial fixture failure exposed a write edge when dropping E and RW together;
corrected to drop E while reading before switching to write mode.

Local unchanged1.04 ROM SHA256:
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
10000000 requested cycles,1000000 callback limit,swap16,lcd-16mhz:
slices1/64/257 stop at FFFFD2 data_read,width1,PC0x3F66A,peripheral482170,
instruction_state1863680/substate0. Returned slice cycles482168/482112/482132,
callbacks739456/116288/104976,10 GPIO outputs,0 TX/clock edges. Same fault/time
across partitions;completed-slice counters exclude failed slice work. Input hash
identical before/after. No ROM bytes exported. firmware_boot_accepted stays false.
Next establish P9 external-input wiring;no blanket ready/pull-up fallback.
