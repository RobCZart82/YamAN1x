# Retained refresh-control profile

Primary reference: [H8/3002 hardware manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual), section7.2.1, printed pages142–144, checked2026-11-02.
RFSHCR is the byte register at0xFFFFAC;reset reads02 and bit1 is read-only one.

Only the unchanged02 profile is accepted:raw writes00 and02 normalize to02.
All254 other values reject before external effects. DRAM/PSRAM enable,address
multiplexing,refresh pins,refresh cycles and self-refresh are not implemented.
The interval timer is also not implemented:RTMCSR/RTCNT/RTCOR access remains
unsupported,including paired word access beginning atFFFFAC. No scheduled
refresh event,interrupt or external timing is invented. Supported reset keeps02.

Pinned MAME commit706dbb73281320c54d7c2f16262c1c3786f2be01 has no RFSHCR mapping
in src/devices/cpu/h8/h83002.cpp;it maps refresh timer registers atFFAD–FFAF.
The archive was verified through the locked private snapshot helper. This new
bounded profile is project-owned manual-based handling,not an original-runtime
differential or full refresh-controller integration.

Tests enumerate256 raw writes,independently accept only00/02,check readback,
24-bit aliases,reset/RAM/time retention,word rejection,unmapped RTMCSR and129
synthetic CPU cut points. CI adds this group to the existing sanitizer selection.

The unchanged local1.04 image SHA256 is
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Explicit in-memory swap16 probe:100000 cycle budget,slices64,callback limit100000.
Firmware writes02;the candidate stops next at byte writeFFFFAD,PC0xA1C,
peripheral cycle124,after64 completed slice cycles/19 callbacks,with5 GPIO output
notifications. This is not full firmware boot,hardware timing or audio acceptance.
Next establish a bounded refresh-timer contract before implementing RTMCSR.
