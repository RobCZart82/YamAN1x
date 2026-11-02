# Selected WCR/WCER wait profile

Primary source:[Hitachi H8/3002 HD6413002 Hardware Manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),
sections6.2.3–6.2.4 and table6-3,printed pages108–109/113,
checked2026-11-02. WCR atFFEE has read-one upper bits,mode selection and a0–3
wait count. WCER atFFEF enables the controller per external area. Two-state
areas bypass wait insertion;WCER-disabled three-state areas use pin-wait0.

## Supported,explicit research conditions

WCR modes00(programmable) and01(no controller waits) are supported. Mode10/11
writes reject before latching;external pin-wait1/auto-wait is not implemented.
The explicit profile holds WAIT inactive for WCER-disabled pin-wait0 areas.
That is a modeling condition,not an observed AN1x electrical pin state.
Programmed waits apply only to enabled mapped external three-state areas:
ROM/SRAM cost3+count,up to6. Otherwise ASTCR selects2/3 and internal accesses
retain their two-cycle research cost. Other external areas remain unmapped;
8-bit mapped ROM/SRAM remains prohibited by ABWCR.

WCR byte writes ignore upper reserved bits and read them as ones;WCER latches all
bits. Paired WCR/WCER word access stays deliberately unsupported and fail-closed.
The existing research cold/supported-reset profile is ASTCR00,WCRF4,WCERFF,
not the physical manual defaults ASTCRFF/WCRF3/WCERFF. No physical reset,WAIT
waveform,pin multiplexing or board wait-state fidelity is claimed.

The checked H8hBusAccess cost query now permits2..6. Unsupported costs reject
before charge/data;each valid cost has checked signed underflow and the same
committed bus-overrun/debt semantics. Hosts lacking a query still use exactly2.
This is project-owned integration from the manual,not copied MAME code or a new
original-runtime wait-state differential. Pinned original reference remains
MAME706dbb73281320c54d7c2f16262c1c3786f2be01;adaptation provenance is updated.

## Checks and local observation

Tests enumerate256 raw WCR values (128 supported/128 pin-mode rejections),all256
WCER masks,reserved readback,ASTCR gating,independent area enables,internal timing,
reset/alias/paired-word behavior,literal five-helper times/counts at each2..6
cost,signed underflow and exact UINT64_MAX/overflow completion edges.257 CPU partitions exercise a synthetic six-cycle
programmed-wait loop. Existing cost-query reentry/invalid-cost checks remain.

Unchanged local1.04 input SHA256 before/after:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
Probe swap16,100000 requested cycles/slice64/callback limit100000 now stops at
byte write0xFFFFF3 (BRCR),PC0x0009FC,peripheral cycle60. No full64-cycle slice
returned. This is subset progress,not a hardware cycle measurement or boot.
Next inspect a bounded BRCR profile without pretending to release a physical bus.

## Subsequent retained BRCR candidate

The unchanged FE-profile candidate passes BRCR and next stops at0xFFFFAC/PC0xA18/
cycle116 after one complete64-cycle slice. See [BRCR scope](an1x-bus-release.md).
