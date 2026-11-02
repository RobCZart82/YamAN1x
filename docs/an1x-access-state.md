# ASTCR selected no-wait access timing

Primary source:[Hitachi H8/3002 HD6413002 Hardware Manual](https://www.renesas.com/en/document/mah/microcomputer-h83002-hd6413002-hardware-manual),
section6.2.2,printed page107/PDF page124,checked2026-11-02. ASTCR at lowFFED
selects two/three-state external-area access by bit. Hardware reset initializesFF;
internal-memory/register timing does not follow these bits.

## Explicit research scope

The existing project uses an explicit **ASTCR00 no-wait research reset profile**,
not the manual's physical reset value. This preserves its established two-cycle
baseline before firmware configures timing. Physical reset timing,external WAIT waveforms and standby are not accepted. The WCR/WCER candidate
adds the selected programmable profile described in [wait scope](an1x-wait-profile.md). The supported peripheral
reset restores that same00 profile;it must not be presented as hardware reset.

All256 ASTCR values latch/read back. Only mapped external ROM/SRAM areas0/1 select
two or three bus cycles;internal memory/register accesses stay at the existing
two-cycle research cost. Other external areas remain unmapped and acquire no
invented behavior. ABWCR still disallows8-bit ROM/SRAM transfers. Any future new
external map must implement its width/state/timing contract before becoming live.
ABWCR/ASTCR paired word access remains deliberately unsupported;neighbor WCR/WCER
remain fail-closed. No general MMIO fallback is introduced.

H8hBusAccess accepts an optional pure access_cycles(address,width) query. The
query sees aligned word addresses;ASTCR alone selects2/3. The WCR/WCER extension
permits checked totals2..6. Reentry/invalid costs
poison before charge/data. Arithmetic underflow rejects without charging/data
or poisoning,as in the old helper. Cost is charged before time/data callbacks;
negative-counter completion remains committed and runner debt repays it before
continuation. Hosts without the query retain exactly the old two-cycle behavior.
The diagnostic host explicitly forwards the query so it observes the same model.
This is a project-owned extension,not copied MAME behavior or an original-runtime
three-state differential. Pinned original MAME reference remains
706dbb73281320c54d7c2f16262c1c3786f2be01;adaptation provenance is updated.

## Validation and observation

Tests cover256 register values,internal/external cost selection,reset/alias/word
rejection,literal time/counter/address results for all five bus helpers at2/3
cycles,underflow,invalid costs and swallowed cost-query reentry.193 CPU partitions
of a synthetic ASTCR0F write verify changed-cost debt and continuation.

The unchanged local1.04 image writesASTCR0F. With swap16 and probe arguments
100000/64/100000,the updated model passes that write and next stops at a byte
write to0xFFFFEF (WCER),PC0x0009F4,peripheral cycle44. SHA256 before/after remains
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
No full64-cycle slice completed. The count is a research-model observation,
not a measured hardware cycle count or successful firmware boot.
Next inspect WCER and supported wait-state profiles without bypassing limits.

## Subsequent wait-profile candidate

The selected programmable wait extension now reaches BRCR0xFFFFF3/PC0x9FC/cycle60
with unchanged input. The earlier WCER/cycle44 stop remains historical.
