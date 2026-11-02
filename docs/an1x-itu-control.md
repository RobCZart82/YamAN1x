# Stopped ITU channel-control configuration

Primary:[H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),section10.2.10,printed pages304–306.
Five TCR bytes atFFFF64/6E/78/82/92 select clock,edge and counter-clear settings.
All raw byte values retain their low seven configuration bits;reserved bit7
reads one. Cold/supported reset80. Channels remain stopped because TSTR and
counter-start paths are unsupported. Internal/external clock selection is only
configuration storage,not active clock sampling or counted edges. Clear-source
selection does not synthesize compare/capture/synchronous clearing.

Paired TCR/TIOR word access still rejects. TIOR,TIER,TSR and TCNT remain
unsupported. Supported reset resets control/general registers without rewinding
time or clearing RAM. No events,pins,flags,buffer transfers or IRQs are invented.

Pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01 h8_timer16.cpp tcr_w
updates the counter,stores control,decodes H8H clock/clear and recalculates events.
Inspected through the verified locked private archive snapshot. The selected
stopped subset retains configuration only;it does not claim original active
counter/event differential or physical clock acceptance. Reserved-bit readback
is explicitly normalized to the manufacturer's read-one contract.

Independent tests cover1280 channel/value cases,alias normalization,owner
isolation,reset/no-events,general-register reset,unsupported pairs and129 CPU
partitions. Firmware writes83 (internal /8 selection,no automatic clear).
Unchanged local1.04 SHA256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Private explicit swap16 probe100000 cycles/slice64/callback limit100000 next
stops at byteFFFF65 (TIOR0),PC0xA7A,peripheral cycle307,after256 completed slice
cycles/69 callbacks and9 GPIO outputs. Next inspect no-output compare/input
capture selection before allowing TIOR. No full boot,audio or plugin acceptance.
