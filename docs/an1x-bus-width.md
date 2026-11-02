# ABWCR mapped-area profile

Primary source:[Hitachi H8/3002 HD6413002 Hardware Manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),
section6.1.4/table6-2 and6.2.1,printed pages105–106 (PDF pages122–123),
checked2026-11-02. ABWCR uses low addressFFEC and selects external-area access
width by bit;zero selects16-bit,one8-bit. Reset values depend on mode pins;
modes2/4 use00. Internal memories/register widths are independent of ABWCR.

## Bounded implementation

The project selects00 as a research reset profile,not verified AN1x mode pins.
The current external map implements ROM in area0 and SRAM in area1. Both must
remain16-bit:ABWCR writes with either low bit set fail before changing the latch.
Other bits are retained/read back but their external areas remain unmapped;
this does not implement8-bit transfers there. Thus64 values are accepted and192
rejected. This is not a permissive MMIO fallback or full bus-controller model.

Byte access at24-bit0xFFFFEC,including masked high-address aliases,is supported.
Incomplete ABWCR/ASTCR words still reject before lane effects. The supported
peripheral reset restores00;no new memory data or event timing is synthesized.
Existing two-cycle bus access timing is still a research approximation;wait
states,pin multiplexing and physical8-bit transfer timing are not validated.
The pinned MAME706dbb73281320c54d7c2f16262c1c3786f2be01 H83002 map inspection
supplied no explicit ABWCR mapping;this behavior is project-owned from the manual,
not copied upstream code or a new original-MAME differential.

## Checks and local observation

Independent tests enumerate all256 writes,accepted readback/reset,24-bit aliases,
word/neighbor rejection and129 CPU partitions of a synthetic write08 program.
An initial zero-only policy rejected the real firmware's08 configuration;it was
expanded only after identifying that area3 is unmapped while ROM/SRAM stay16-bit.

Read-only local1.04 input SHA256 remains
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe` before/after.
With explicit swap16,100000 requested cycles/slice64/callback limit100000,the
probe passes the ABWCR write08 and next stops on a byte write to0xFFFFED (ASTCR),
PC0x0009F0,peripheral cycle36. No full64-cycle slice returned. This replaces the
previous first-stop result at0xFFFFEC/cycle30 for the updated candidate only.
It does not establish firmware boot or hardware-cycle accuracy.
Next:ASTCR contract and a deliberately supported timing profile,with fail-closed
handling for modes the execution engine cannot implement.

## Subsequent ASTCR candidate

The selected no-wait ASTCR extension now passes firmware0F and next stops at
WCER0xFFFFEF/cycle44. See [access-state scope](an1x-access-state.md). Paired word
access remains unsupported;the prior ASTCR/cycle36 observation is historical.
