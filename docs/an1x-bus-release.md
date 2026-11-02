# Retained BRCR profile

Primary source:[Hitachi H8/3002 HD6413002 Hardware Manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),
section6.2.5,printed pages110–111,checked2026-11-02. BRCR is at lowFFF3;
upper writable bits select address-pin functions,bit0 enables bus release and
reserved bits4..1 read one. Its documented reset value isFE.

## Bounded implementation

Only the unchanged FE profile is supported. Writes whose writable bits matchE0
normalize toFE;this accepts16 raw encodings and rejects240 before output effects.
Changes to address-pin functions or bus-release enable remain fail-closed.
No physical BREQ/BACK,pin multiplexing,address output waveform or arbitration
is synthesized. The existing abstract24-bit logical address map is unchanged;
that must not be confused with proof of real external pin configuration.

Readback is fixedFE,including after the supported peripheral reset.24-bit masked
aliases work. Paired SYSCR/BRCR word access remains deliberately unsupported and
rejects before a partial SYSCR mutation. Unknown neighboring MMIO still fails.
This is project-owned register-profile handling,not copied MAME code or a new
original-runtime bus-release differential. Reference remains pinned
MAME706dbb73281320c54d7c2f16262c1c3786f2be01.

## Checks and local observation

Independent tests cover256 writes,exact16/240 acceptance partition,reserved-bit
readback,no output effects,reset/memory/time retention,aliases,paired-word rejection
and129 CPU partitions of a synthetic FE write.

Unchanged local1.04 SHA256 before/after:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
The firmware writesFE. Probe swap16,100000 requested cycles/slice64/callback
limit100000 now completes64 slice cycles/19 callbacks before the failing slice.
It then stops at byte write0xFFFFAC,PC0x000A18,peripheral cycle116,with five GPIO
output notifications counted. Those counts are a subset research observation,
not attached-device emulation,hardware timing or firmware boot acceptance.
Next identify0xFFFFAC from pinned source/manual before adding its bounded contract.
