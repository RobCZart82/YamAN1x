# Bounded PKS payload path observations

## PKS operand CPU context — local candidate, not merged

A bounded, read-only MAME observer now captures the post-fetch PC, virtual time, CCR and ER0–ER7 when firmware reads/writes the three previously observed aligned RAM word bases `237202`, `237204` and `2349E8`. The observer has a 4,096-event cap, reports overflow, filters all other addresses and does not change bus values. It was built against the pinned MAME source `706dbb73281320c54d7c2f16262c1c3786f2be01`; instrumented binary SHA-256 `0a7115cb99760c3644affe2a8f22a4491a180a824ab21b1d2b18d700174c1768`. The matching uninstrumented build hash is `6269f609a41a7e9e7202d2be9d3edc03f8744b48219fac0c9ebdfe1fea0746ff`.

Two nine-second passive/default-input runs produced byte-identical context traces (1,928 events; SHA-256 `8acf5f8c23ce9a912be8fa0a25323be342ab3f37084d3fbed012a18b5133172e` each). Per run, `2349E8` had 480 reads/483 writes, `237202` had 478 reads/4 writes, and `237204` had 478 reads/5 writes. All events fit within the cap without drops. The baseline and instrumented `run.json` matched on all 26 non-trace fields; only command path, binary hash and trace-line count were excluded. ROM identity remained AN1x 1.04 SHA-256 `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; no ROM contents were changed or added to the repository.

This is limited repeatability evidence for one nine-second passive profile. The callback PC remains the CPU-reported post-fetch PC, not an instruction-start PC. The run does not establish operand meaning, ADC arithmetic, PKS behavior, complete boot, MIDI acceptance, hardware agreement or sound. VOP3 samples remained zero. Next: use these bounded snapshots to follow the preceding value writes and their instruction context, retaining independent value/address/flag/timing and continuation checks.


## PKS control and ramp RAM reads — merged in #262, 2026-11-09

A local MAME binary with read-only taps was built using the explicit macOS OSD target (`OSD=mac`); SDL3 was not required. Two identical nine-second passive/default-input runs at the locked source commit `706dbb73281320c54d7c2f16262c1c3786f2be01` produced byte-identical traces, each 65,370 lines. The unchanged AN1x 1.04 ROM SHA-256 was `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; the diagnostic binary SHA-256 was `bd762ab222095721ea0843e7a765b8c844bcf47cc13bc182fcaffcf7d050aa88`.

The taps observed 956 reads across aligned word bases `237202` and `237204` (478 each), nine writes across those bases, and 480 reads plus 483 writes at `2349E8`. Reads at `2349E8` returned zero in this profile. At base `237204`, the low-byte read lane (`mask=00ff`) is consistent with the static disassembly `mov.b @h'237205,r2l` at `001fd2`, with the callback reporting `pc=001fd8`. Callback PC is the H8 CPU's reported post-fetch PC, not an instruction-start address. These are RAM address/lane observations only; they do not establish what the bytes mean or whether they are ADC/PKS inputs.

Both run metadata objects matched. The profile used passive DSP, no DMS board, no MIDI input and no DSP sample execution. VOP3 samples stayed at zero and complete boot remained false. The tap callbacks log address/data/mask/PC and do not modify read data. This is limited evidence from one profile and horizon; no alternate-input or hardware comparison was run. It does not establish ADC arithmetic, PKS behavior, hardware agreement or audio. Next: capture bounded CPU context or instruction-fetch history around these operand accesses and their preceding writes to follow value provenance.

PR [#262](https://github.com/RobCZart82/yaman1x/pull/262) merged at `1cdeb8f7aa629aa969beae9cce55c9292502522a` from head `53c91d5855aded332c4bd457acd766e143eb800f` on 2026-11-09T06:17:57Z. All four PR-head checks passed (ROM-free CI `42520377872`, initial-decode MSan `42520377977`); all four required main checks passed on the merge commit (ROM-free CI `42572182568`, MSan `42572182585`). The active ruleset required zero approvals and resolved review threads. No reviews, comments or threads were recorded; no automatic-review outcome or quota evidence is claimed. Squash merge used the verified head without bypass.

## PKS writer caller sites — merged in #261

PR [#261](https://github.com/RobCZart82/yaman1x/pull/261) merged at `9c89f291a630c86b959c3e173cfabbae1b54164d` from head `f3ce99d10fe34f33e668cfad3c2a62d3c03cba84`. All four PR-head checks passed (ROM-free CI `42512461512`, MSan `42512461563`); all four required main checks passed on the merge SHA (ROM-free CI `42515701366`, MSan `42515701585`). The active ruleset required zero approvals and resolved review threads. No reviews, comments or threads were recorded; no automatic-review outcome or quota evidence is claimed. Squash merge used the checked head without bypass.

The earlier candidate observation reproduced six writes from R1L to `234ACC–234AD1` through the routine at `0039A4`, associated with calls at `002A66` and `001FF2`. This narrowed the operand reads to inspect but did not identify their meaning. The historical report said the follow-up taps were not built due to SDL3; the explicit `OSD=mac` build resolved that local issue and the results above supersede the limitation.

## Register-fed writer source RAM — merged in #260, 2026-11-08

One additional read-only run of the locked MAME reference used the unchanged
AN1x 1.04 image for nine emulated seconds with the passive DSP profile, DMS
board absent, no MIDI file, and DSP sample execution disabled. The ROM SHA-256
remained `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
The temporary MAME executable SHA-256 was
`182d16627ad317db929cac7de9a1f174abeadee3a355c6295f399a4a34eeb265`; the
source remained at the locked MAME commit below, with observational taps only.

An aligned word-bus tap (`22D9EE–22D9EF`) observed six low-byte writes to the
odd byte `22D9EF` and six later reads at that byte. The values in order were
`E0 00 20 E0 00 00`; each read matched its preceding write. At the static
instruction beginning FA92, firmware loads `R0L` from `234ACC + ER0` and
stores it through `ER4`; the run observed indices 0–5, `ER4=22D9EF`, and the
same six byte values in `R0L`. A tap of the backing RAM showed the matching
source-buffer bytes at `234ACC–234AD1`, written from `R1L` by the routine at
last fetch 39A4 and read by the FA92 routine. These observations identify the
immediate RAM source and copy path for this default-input run; they do not
identify how those source bytes were generated, connect them to ADC arithmetic,
or establish a PKS response.

Static MAME disassembly identifies the FA92 instruction and the 39A4 byte
store. Callback `pc` fields are the CPU's reported post-fetch PC, not
instruction-start addresses. The new input taps and CPU snapshots left all
non-trace run fields unchanged compared with the preceding nine-second run;
trace line counts and executable hashes differ because the diagnostic taps
changed. A first attempt with an odd-address tap was rejected by MAME's
16-bit address-space alignment check before CPU execution; that failed run is
excluded. The aligned tap was rebuilt and the successful run above is the only
dynamic observation counted here.

Limits: one passive/default-input profile and one horizon; no slice-repeat
comparison, no injected MIDI, no hardware comparison, no validated full boot,
and no audio. The writer's caller and the source buffer's upstream data path
remain open. Next: trace the input to the 39A4 write and its callers.

Original MAME source remains locked at
`706dbb73281320c54d7c2f16262c1c3786f2be01`. VOP3 samples remain zero; boot,
MIDI and audio remain unaccepted.

Merged in #252; original observation base 0643ac56075ca0c518b8ba2f4f7c93d42036b322.

Four read-only local 1.04 probes extended the AN2-only and AN6-only high
scan profiles (initial single AN3=1023), with the same 130-million-cycle
horizon and slices 4,096/1,000,000. A temporary observer retained the last
128 instruction-word fetch addresses and emitted them at observed buffer
byte writes. All baseline JSON bytes remained identical and same-input
trace logs matched across slices. ROM hash before/after remained
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.

Immediately before entering the common FAB0 fetch sequence ending at FAC6,
the observed predecessor fetches were:

| Profile / output bytes | Predecessor fetches |
| --- | --- |
| AN2 D0 7F | 59C1A, 59C26 |
| AN2 E0 00 20, then E0 00 00 | 59A06, 59A12, 59A1E, repeated |
| AN6 E0 3E 5F, then E0 7E 7F | 59A06, 59A12, 59A1E, repeated |

This separates the observed D0 and E0 execution paths before their common
buffer writer. It does not establish call opcodes, instruction-start PCs,
command meanings, complete stack traces or a payload formula. Fetches may
include operand words. Only addresses and derived observations are recorded;
no ROM contents are included.

Previously verified stages: ADC shared result FFFFE4, input-dependent RAM
words, byte writes starting 2360CC at last fetch FAC6, buffer reads at 18B0,
and matching TDR1 writes five cycles later. These stages do not yet prove
every dependency between ADC values and payload encoding.

Next: observe input data reads around 59Cxx and 59Axx and connect them to
the ADC intermediates. No behavioral PKS response is justified yet.
Original MAME reference remains locked at
706dbb73281320c54d7c2f16262c1c3786f2be01. No hardware comparison was run.
VOP3 samples remain zero; full boot, MIDI and audio are unaccepted.

## Staging byte reads and writes — merged in #253, 2026-11-08

Eight further read-only probes used the same AN2/AN6 profiles and two slice
sizes. Four observed producer data reads in last-fetch range 59900–59CFF;
four added byte-write observation at 23FB0A–23FB0C. Both observer stages
preserved every previous JSON byte. Same-input logs matched across slices.
ROM hash remained unchanged at the value above.

D0-path reads were 23FB0A at last fetch 59C12 and 23FB0C at 59C22.
E0-path reads were 23FB0A/0B/0C at 599FE/59A0E/59A1A. All eight AN2
and six AN6 staging reads matched the preceding observed byte write.
The staging bytes reproduce the already documented outgoing sequences.

Initial staging writes used last fetches 5A2A0, 5A2AA, 5A2B4 for offsets
0,1,2. In the AN2 D0 case, D0 7F 00 was subsequently rewritten to
D0 00 7F at last fetches 5A2CE, 5A2D6, 5A2E0 before the D0-path reads.
This is observed staging behavior; no general byte-reordering rule or
protocol interpretation is accepted from this case alone.

The observed chain now reaches staging writes, producer reads, queue-byte
writes, queue reads and TDR1 writes. The ADC-to-staging calculation remains
unproven. Next: observe data inputs to 5A2xx and connect them to ADC RAM
intermediates, without injecting replies. Boot, MIDI and audio remain open.

## Earlier staging source — merged in #254, 2026-11-08

Eight read-only probes added data-read observation for last fetches
5A200–5A2FF, then byte-write observation at 23FAEC–23FAEE and word-read
observation for 59400–598FF. Both observer stages preserved baseline JSON
byte for byte; same-input logs matched across the two slice sizes.
The AN2/AN6 profiles, initial AN3 value and cycle horizon were unchanged.
ROM hash stayed unchanged at the value recorded above.

Last fetches 5A29A, 5A2A4 and 5A2AE read 23FAEC, 23FAED and 23FAEE
before the initial writes to 23FB0A–0C. The AN2 D0 path additionally read
23FAEC at 5A2C8 and 23FAED at 5A2DA before the observed rewrite.
A zero third byte in that first D0 copy is not evidence of a complete
three-byte input message: the preceding copy at 5A1C4 only wrote two bytes.

Byte writes at last fetch 5A1C4 supplied the observed D0/E0 source bytes.
Later writes at 5975E, 59768 and 59772 wrote the post-processing bytes back
to the same source region. Thus the source is mutable within this path;
it must not be treated as an immutable ADC snapshot. The bounded upstream
word-read observer did not establish a read from the documented ADC RAM
intermediates. No ADC-to-payload formula is claimed.

Next: inspect data reads at the 5A1C4 copy path and its callers. Preserve
that distinction from the already observed output chain. Original MAME
reference is unchanged; no ROM contents are included. Boot, MIDI and audio
remain unaccepted and VOP3 sample counters remain zero.

## Source-write register observation — merged in #255, 2026-11-08

Eight read-only probes added byte/word-read observation for last fetches
5A100–5A1FF, then a read-only CPU snapshot at each observed source byte
write with last fetch 5A1C4. Both stages preserved all previous JSON bytes;
same-input logs matched across slices. Profiles, initial AN3 and horizon
were unchanged; ROM hash remains the value recorded above.

All eight AN2 and six AN6 source writes matched the same snapshot facts:
ER0 equaled the destination byte address and R4's low byte equaled the
written byte. Snapshot PPC was 5A1C2 and PC was 5A1C6. These measured
fields identify the current CPU context at these writes; earlier last-fetch
addresses are still not retroactively claimed as instruction-start PCs.
ER0 was reconstructed from snapshot registers 0 and 8 according to the
project CPU accessor; this is a model-state observation, not a hardware
register capture. The snapshot callback performed no CPU run or bus access.

The bounded data-read observer did not identify the ADC source for these
register values. Calling this site a copy does not prove a direct RAM-to-RAM
copy from ADC intermediates. Next: trace R4's low-byte value into this write
context and its callers, then establish the arithmetic independently.
Boot, MIDI and audio remain unaccepted; VOP3 sample counters remain zero.

## Register value transition — merged in #256, 2026-11-08

Four read-only probes added R0/R4 snapshots before instruction-word fetches
in 59400–5A1FF and byte-read observation in 59400–598FF. All previous
JSON bytes remained identical; same-input logs matched across slices.
Profiles, initial AN3, fixed horizon and ROM hash were unchanged.

For every observed source write, R4's low byte changed between the fetch
observations at 5A196 and 5A198 to the byte subsequently written at the
previously verified source-write context. AN2 values were
D0 7F E0 00 20 E0 00 00; AN6 values were E0 3E 5F E0 7E 7F.
R0 was 0001 at both fetch observations in every case. This narrows the
value-setting interval; it does not identify an opcode or source operand
from the snapshots alone. The additional byte-read window showed accesses
to staging and control state, not a proven ADC-to-value calculation.

Next: identify the source register/operand for this interval and track it
back to the ADC intermediates. A direct arithmetic or protocol claim is
still premature. No replies were injected. VOP3 samples remain zero;
boot, MIDI and audio remain unaccepted.

## Source operand and entry value — merged in #257, 2026-11-08

Four read-only probes extended fetch snapshots to all model register words
in 59000–5A1FF. Baseline JSON remained byte-identical; same-input logs
matched across slices. At all eight AN2 and six AN6 observations, R1's low
byte before fetch 5A196 matched R4's low byte before fetch 5A198.
Local read-only instruction classification at 5A196, checked against the
project CPU decoder/accessors, identifies a byte register transfer from
R1L to R4L. No raw instruction bytes are included.

The same byte sequence was already present in R1L at observed entry fetch
5A080: AN2 D0 7F E0 00 20 E0 00 00; AN6 E0 3E 5F E0 7E 7F.
This identifies an earlier argument/value boundary for the source writes,
not the ADC computation. It does not prove an ABI or every caller's role.
Next: trace callers that set R1L before 5A080 and establish their data
inputs. Profiles, fixed horizon, slice sizes and ROM hash are unchanged.
The original MAME reference remains locked. No reply was injected; boot,
MIDI and audio remain unaccepted, with zero VOP3 sample counters.

## Earlier entry predecessor — merged in #258, 2026-11-08

Eight read-only probes added the preceding 128 fetch addresses at 5A080,
then register snapshots and byte/word-read observers for 57C00–57D4F.
Both stages preserved baseline JSON byte for byte; same-input logs matched
across slices. The same AN2/AN6 profiles and fixed cycle horizon were used.
ROM hash stayed unchanged at the value recorded above.

Every observed 5A080 entry immediately followed fetch 57D48. At earlier
fetch 57D42, R1L already held the same eight AN2 or six AN6 bytes as at
5A080. All fourteen paired values matched. These are fetch-boundary and
register observations, not an asserted complete call stack or ABI.

The bounded caller window observed a byte read of control location
23F7FD at last fetch 57D16 on each path; it did not expose an ADC-data read
or a new word-data source. Thus this path forwards an already available
value; the ADC-to-payload computation remains unproven. Next: follow R1L
before the 57Cxx/57Dxx path, using the observed predecessor history.
No PKS reply was supplied. Boot, MIDI and audio remain unaccepted, and
VOP3 sample counters remain zero. Original MAME reference is unchanged.

## Caller source read/write chain — candidate, 2026-11-08

Twelve read-only probes combined three observer stages: expanded caller
reads/registers in 57000–57D4F; byte writes to 22D9EF and overlapping word
writes at 22D9EE; CPU snapshots and fetch history at those byte writes.
Each stage used AN2-only and AN6-only high scan, initial single AN3=1023,
130 million requested cycles and 4,096/1,000,000-cycle slices. Every stage
preserved baseline JSON byte for byte; same-input logs matched across
slices. ROM SHA-256 remained
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.

At last fetch 57AEA, reads from 22D9EF reproduced all eight AN2 and six
AN6 outgoing bytes. Every read matched the latest preceding byte write
to that address, whose last fetch was FA94. Overlapping startup word
writes were tracked separately and were not mistaken for these byte writes.

At all fourteen byte writes, the CPU snapshot had PPC=FA92 and PC=FA96.
R0's low byte equaled the written value; ER4 equaled 22D9EF. ER4 was
reconstructed with the project's register accessor convention. These are
model snapshots during the host callback, not physical register captures.
The snapshot callback performed no CPU run or bus access. The predecessor
history reached a common FAxx sequence; higher callers remain to be traced.

This extends the observed path from the earlier caller's memory read back
to a register-fed writer. It does not prove the ADC arithmetic, stack/ABI
semantics, or a required PKS reply. Next: trace R0L before FA92 and its
callers, combining that investigation before another publication step.
Original MAME remains locked at 706dbb73281320c54d7c2f16262c1c3786f2be01.
No ROM contents or runtime changes are included. Boot, MIDI and audio
remain unaccepted; VOP3 sample counters remain zero.

## Run manifest validation — 2026-11-10 audit fix

The context-audit comparison now requires complete run_h8_probe metadata with
correct field types, known AN1x ROM identity, a zero exit code and observed H8
progress. Profiles, optional MIDI identity and bridge/clock evidence are checked
for consistency. Empty, incomplete, failed or mistyped records reject before
comparison; field absence differs from explicit null for optional extra fields.
Only command/binary-hash/trace-line deltas retain the existing comparison exception.
This validates metadata, not authenticity or firmware acceptance, and never opens
the referenced ROM, MIDI file or bridge. Trace-only parsing without --baseline
remains available and does not claim a manifest comparison.
Synthetic helper and CLI tests cover rejection on both sides, complete passive
and bridge/MIDI profiles, metadata presence, and accepted trace-only deltas.
Historical complete run observations are retained; they were not rerun here.

For an enabled DSP sample clock, metadata must include two positive integer
sample-pass counters. Missing/zero/negative/mistyped counters reject, including
failed probes that wrote run.json before the producer performed its final check.
