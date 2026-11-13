# An1X P9 Input

This English document no longer mixes Hungarian passages into its text. The original Hungarian-language passages are preserved in the [Hungarian notes archive](HUNGARIAN_NOTES_HUN.md).

# Scoped P9 input levels in the LCD research profile

Evidence:established research/mame-lcd-controller.patch explicitly connects
read_port9 toFF,with outputs merged by H8. Locked original MAME
706dbb73281320c54d7c2f16262c1c3786f2be01 src/devices/cpu/h8/h8.cpp
h8_device::port_default_r returnsFF for unhooked ports. h8_port.cpp port_r
mixes callback bits only where DDR allows inputs;H83002 P9 reserved mask isC0.
Both original files inspected through the locked hash-verified archive snapshot.
This is an original/emulator research profile,not measured AN1x physical wiring.

An1xLcdResearchSink now suppliesFF only for P9 input callbacks. Actual MMIO
readback still combines DDR-driven data and reserved bits in H8DigitalPort;
it does not force output pins high. PA/PB remain LCD latch inputs and every
other unknown input still fails/poisons. Time validation and epoch recovery
are unchanged. The default no-LCD CLI remains strict;only explicit lcd-16mhz
uses this headless P9 profile. No invented PKS,DSP or MIDI responses.

Independent per-bit oracle sweeps all65536 DDR/data pairs through real bus MMIO,
including24-bit alias reads. Separate checks cover time reversal,cold recovery
and strict rejection of ports4/6/7/8. CLI synthetic P9 input passes only with
explicit LCD profile. This does not prove active timer/pin mux behavior,hardware
pull-ups,full firmware boot or audio. No new original runtime differential;
existing pinned port differentials remain component evidence.
