# Development roadmap

## Current verified main — PR #287 — 2026-11-14

PR [#287](https://github.com/RobCZart82/YamAN1x/pull/287) merged by squash at
2026-11-14T06:39:15Z from exact head
`6ebf0262c056d2fecaf68216c419c3a0a2f3ee42` to main
`dd988f6c9aa2fc147b9f0022f188202f722aa844`. Linux, macOS, Windows and MSan
checks passed on the PR head (ROM-free CI run `43563617346`, MSan run
`43563617365`) and exact main (ROM-free CI run `43567393627`, MSan run
`43567393625`). MAME research was path-filtered for this documentation-only
change. Codex review completed without findings on the exact PR head (comment
`6872683772`); GitHub review submissions, inline review comments and threads
were zero. Active ruleset `25910947` required zero approvals and resolved
threads. No quota error was reported and no merge bypass was used.

PR #287 reconciled the #286 merge record and the current main. The source-only
build remains at 138 CTest groups and 127 Python cases. Full boot, hardware
agreement, MIDI, audio and REAPER acceptance remain open. The ROM remains
unchanged and outside Git. MAME remains pinned at
`706dbb73281320c54d7c2f16262c1c3786f2be01`.
## Current bounded PKS trace candidate — 2026-11-14

A read-only extension now observes the aligned word base `2349EA`, the second
static caller-input location already identified in the trace notes. Two fresh
nine-second passive/default-input runs on the pinned MAME source captured six
`39AC` source-buffer byte writes. In each run, the `2349EA` high-byte read at
post-fetch PC `002A08` preceded the first three writes by 12,625 ns; the selected
byte was `40` from bus word `4000/FF00`. The `237205` low-byte read (aligned
word base `237204`) at post-fetch PC `001FD8` preceded the next three writes
by 10,375 ns; the selected byte was `00` from `FF00/00FF`. The respective
writer-byte sequences were `E0 00 20` and `E0 00 00`, with monotonically
advancing timestamps. Both run manifests matched on 26 non-trace fields.

These are bounded temporal windows, not proof that either read caused or
computed a writer value. CCR is recorded without interpretation. The ROM hash
is unchanged and outside Git; no complete boot, hardware agreement, MIDI, DSP
sample or audio acceptance is claimed. See [PKS payload path notes](pks-payload-path.md).

Next: trace the instruction/value path feeding these observed caller windows,
then rerun a bounded profile with the independent address, value, flags, timing
and continuation audit. Keep all PKS semantics unclaimed until supported by
separate evidence.
## Verified runtime baseline — 2026-11-12

PR [#274](https://github.com/RobCZart82/YamAN1x/pull/274) merged on
2026-11-11 at 21:56:03 UTC (22:56:03 Europe/Budapest), from final head
`329a7bf626e251c39a3273eef69dad24ad3e161e` to main
`d9f9ffb04e5dee587efebf33b6cf6bfc17b95f94`. The final-head platform CI,
MSan and MAME research workflows passed (runs `43102583960`, `43102583923`,
`43102583907`). The exact-main runs also passed:
[Linux/macOS/Windows](https://github.com/RobCZart82/YamAN1x/actions/runs/43107390172),
[MSan](https://github.com/RobCZart82/YamAN1x/actions/runs/43107390248) and
[MAME research](https://github.com/RobCZart82/YamAN1x/actions/runs/43107390414).
[Codex review](https://github.com/RobCZart82/YamAN1x/pull/274#issuecomment-6806170735)
completed without findings on that final head; the earlier result-read finding
was addressed and its review thread is resolved.

Current inventory: **138 CTest groups, 127 Python cases, 14 targeted
source-context cases and 13 research patches**. The November 12 Windows audit
passed a fresh Release build and 137/137 runnable CTest groups (Python wrapper
checked separately); Python passed 126/127 with one WinError 1314 symlink
privilege error, and the 14 source-context cases passed under Python -O.
Source/notice audits passed. M0 remains partial; full boot, hardware, MIDI,
audio and REAPER acceptance remain open. Next: establish the unsupported ADC
control-mode semantics from source evidence before extending the bounded probe.
The older preparation and test records below retain their original revisions.

## Repository audit and source-context follow-up — 2026-11-11

Correction and merge status: [PR #270](https://github.com/RobCZart82/YamAN1x/pull/270).

Preparation baseline is merged #269, `97e341ef56ff81a511ae64b6f98ca8af01251f84`.
The preceding changes add bounded PKS source-context observations and correlate
FA92 reads with the latest earlier write to the selected byte lane. #269 final
PR-head platform/MSan runs 42885941250/42885941221 passed all four jobs.

The audit found that the source-context CLI checked complete successful run
metadata only when --baseline was supplied. Its standalone entry now uses the
same full validator, so failed/partial runs reject before reporting transfers.
A synthetic end-to-end CLI regression covers failed, identity-only, malformed
and complete records. The provenance report now carries the earlier writer's
CCR and ER0–ER7 snapshot, copied independently from later read/write snapshots.
These fields support tracing the register value that produced the source byte.

Historical #270 source inventory was **138 CTest groups and 126 Python cases**, with
13 research patches. Release build and137/137 runnable CTest pass (60.63s).
Python125/126 pass;one established WinError1314 symlink-privilege limit. All13
source tests pass. Source/notice audits pass (688 files,201 upstream identities,
231 covered files/five licence texts),as do UTF8/links/ledger checks. Local
sanitizers were not run;this change's remote checks are recorded in its PR.
Next research work follows the preceding writer's inputs and instruction path.
M0 remains partial and M1 prepared/unaccepted. Full boot/MIDI/audio/REAPER gates
remain open. Older candidate labels below describe their preparation dates.

## Audit corrections and next work — 2026-11-10

Recorded preparation baseline: [#265](https://github.com/RobCZart82/YamAN1x/pull/265),
`5421585951258f430ea13fd40207bdabd23ed654` (documentation reconciliation),
following the #264 read-only PKS-context research code. That baseline had
137 CTest groups/109 Python cases. This source tree adds four Python regressions:
**137 CTest groups/113 Python cases**. Exact merge/remote-check status of this
correction is tracked on [PR #266](https://github.com/RobCZart82/YamAN1x/pull/266); no self-referential merge SHA is predicted.

The corrections reject stale standalone ADC scan restarts without mutation,
validate complete successful PKS probe manifests before comparison, and reconcile
the development records and twelve-patch source inventory. ADC guard tests also
run in the Linux ASan/UBSan group. Release build and 136/136 locally runnable CTest groups pass (51.54s). Python:
112/113 pass; one established Windows symlink privilege case is blocked. All ten
PKS audit tests also pass under Python -O. Source/notice/link/ledger checks pass.
Local sanitizers were not run; this change adds the ADC target to Linux sanitizer CI.
Own remote checks and merge status are recorded on the correction PR.
No firmware/hardware/MIDI/audio acceptance is added by these synthetic checks.

Active internal-clock ITU scheduler/MMIO/CPU IRQ delivery was completed in
#215–222; bounded ADC conversion/scan/restart integration followed in #229–242.
These are supported source profiles, not complete peripheral/hardware coverage.
Next research step: follow the preceding writes/instruction context of the PKS
operand RAM snapshots from #264, with independent address/value/flag/time checks.
Do not infer the PKS protocol or introduce HLE replies from observations alone.
Full reset, firmware boot/MIDI, DSP clock/table/EG fidelity, audio, VST3/AU and
REAPER acceptance remain open. M0 is partial; M1 is prepared but unaccepted.
Older dated candidate/pending entries preserve their preparation-time observations;
they do not override this summary.

## Historical verified status — #213, 2026-11-05

Latest verified merged fix: [#213](https://github.com/RobCZart82/yaman1x/pull/213),
`d60e6eb9907d0a9c89938e1677d68e54b319da74`, merged 2026-11-05T09:09:33Z (10:09:33 Europe/Budapest).
Tested PR head: `a421a261a6647e759dac7d5132567d82f2de32fc`.
The suite defines **131 CTest groups and 103 Python cases**.
[Linux/macOS/Windows and Linux sanitizers PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686134),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686447),
[MAME research PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686368).
These results belong to #213's final PR head. Automated review completed without
findings on source commit `95b7edb0feaa97be23d1105fe59b6f259596e640`; only PR-link
documentation changed afterwards (three files). No final-head review is claimed. The actual macOS runner is15; the required check retains its old name.

Bounded shared SYSCR modes, absolute machine time, supported subset reset,
LCD/MEG/native register-only VOP3 research adapters, and standalone ITU counter,
flags, deadlines and exact-time vector dispatch are implemented. The full
hardware RESET line and boot remain unaccepted. Next: selected active ITU
scheduler/MMIO integration and actual CPU IRQ delivery, then fresh local
firmware diagnostics at the documented FFFF60 timer-start boundary.
PKS/MIDI, DSP clock/table/EG fidelity, audio rendering, VST3/AU and REAPER
acceptance remain open. M0 is partial; M1 is prepared but unaccepted.

Merged fix: complete external research-sink cold reset through LCD
or MEG base references, with safe base ownership/destruction. Local regression
reproduced retained MEG/VOP3 state before the fix and passes after it. Own
Release build and 130/130 locally runnable CTest groups PASS (53.85s). Python:
102/103 PASS, one symlink case blocked by Windows privilege (WinError1314).
Source/notice audits and document links PASS. Local sanitizers not run; own
remote platform/UBSan/ASan/UBSan, MSan and MAME checks passed. No active engine
candidate is recorded. This documentation reconciliation changes no runtime code.
Machine subset reset still retains external chips; explicit sink cold reset
must be called with the machine stopped. No Yamaha data is needed.

## Historical preparation and closure records

The following dated candidate/pending statements preserve the original observations.
The current summary above supersedes their former active-task wording.

## Historical verified status — 2026-11-01

Latest merged change:[#157](https://github.com/RobCZart82/yaman1x/pull/157),
`15217f7fe1ffa8e6894730b24357feac311c1c0d` (2026-11-01T19:52:16Z).
The source-only suite contains **90 CTest groups and 95 Python tests**.
The complete local build/90 CTest/95 Python validation belongs to #156;
#157 additionally passed the eight selected local ASan/UBSan groups.
CI and review evidence must be read for its exact revision;local results are
not a claim of hardware fidelity or a completed review of this candidate.

Merged #154 pins framework reset identities;#155 supplies the gold vector README
logo and preserves its bytes under Windows checkout. #156 fixes swallowed reentry
in dispatcher/runner/bus and processes reset archives from a verified private
snapshot. #157 adds automated ASan/UBSan coverage for eight reset/peripheral/guard
groups. This third audit-fix candidate reconciles documentation only.

CPU/bus cold reconstruction and component reset tests exist;whole-machine warm
reset and complete firmware boot are **not accepted**. Next inspect execution
input reset semantics,actual child reset order and scheduler time before adding
an in-place machine reset. Firmware MIDI/PKS acceptance,DSP/table/EG/audio routing,
offline reference sound,realtime rendering,VST3 and REAPER acceptance remain open.
No AN1x audio,VST3,AU or GUI is currently delivered. No project-wide release
license has been selected. ROMs remain local and unchanged.

One PR at a time. Current candidate:documentation reconciliation;not yet merged.

## Historical status snapshots

The following entries preserve their original dates and candidate counts.
The current summary above supersedes their pending/active wording.


Detailed execution plan, with milestone acceptance criteria through the first
audible firmware-driven VST3 voice in REAPER:
[First REAPER sound plan](FIRST_REAPER_SOUND_PLAN.md).

Changes/fixes are recorded in the [development ledger](DEVELOPMENT_LOG.md).
The [dated repository audit](REPOSITORY_AUDIT_2026-10-28.md) preserves that snapshot;
the current status below and the development ledger supersede its old open-task counts.
Merged research evidence:[#145](https://github.com/RobCZart82/yaman1x/pull/145),
`e470f406c2d65eccda70e0c6381db1f826b2810b`;five CI jobs PASS on head
`90f8aea04fdddce17de60fe706b690b3b5f62c3b`. No recorded automated review result.
[Pinned method contract](h83002-warm-reset-contract.md):five files/six methods;91 Python tests.
Merged #147 CPU reset tests:89 CTest/91 Python PASS locally,four CI jobs PASS.
Merged #148 GPIO retention tests;four exact-head CI jobs PASS.
Merged #149 SCI reset tests:four CI jobs and exact-head code review PASS.
Merged #150 IRQ reset tests:four CI jobs and exact-head code review PASS.
Merged #151 post-reset TX tests:four CI jobs and exact-head code review PASS.
Merged #152 post-reset RX tests at8b2930234de7b0b512f8ecdb9f04414782f14a53.
All four exact-head CI jobs PASS;automated review failed due to usage limit,not completed.
Merged #153 documentation and original hardware illustration.
Current candidate:framework reset traversal identities and ordering limits;94 Python tests PASS.
Targeted ASan/UBSan PASS;no whole-machine warm reset API added.
#146 documentation merged at41a1c1cea0b777653ede9e470ec5e6e7797b2ff2;all four CI jobs PASS.
Engine implementation baseline remains #143.

Verified engine baseline:[#143](https://github.com/RobCZart82/yaman1x/pull/143),
`301eec75c72028b8f377ee4e099fce0b1df4de5e`;tested head `d2da27c15c8ebf8c083f35bb4a2cef18eba4d8c4`.
88 CTest/87 Python;[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41127313949),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41127314006),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/41127313987):all five jobs PASS.
Actual macOS runner15;historical required check name macos-14 retained for ruleset compatibility.
No recorded automated review result at inspection;none claimed.
[Cold research lifecycle](an1x-research-lifecycle.md) owns stable CPU/bus epochs,immutable ROM and
explicit NVRAM preserve/clear.401 cut-point restarts match fresh CPU/TX histories;
full256KiB NVRAM retains across8 epochs;invalid replacement preserves old epoch.
Failed/reentrant recovery and epoch-relative copied RX schedules tested;targeted ASan/UBSan PASS.
Not hardware warm reset. Research candidate above verifies method identities only. Next:verify warm-reset
retention/publication matrix and reset ordering/time/input semantics before adding
an in-place reset API;test active SCI,IRQ pins,GPIO latches,RAM and pending events.
Generated CPU bodies unchanged:562 methods,264 selected/0 missing/135 excluded rows.
Complete machine,LCD/DSP/MEG,physical timing,firmware/MIDI/PKS/audio/REAPER remain open.

Next M0 work, in order:

1. Retain full eligible H8H handler selection (264 rows),fixed-profile SLEEP/wake and original reference controls;
   retain the reviewed [word-multiply widening and UBSan control](h8-word-multiply.md);
   retain fixed-profile IRQ/TRAPA limitations and pinned reference comparisons.
2. Connect CPU, address map/MMIO and existing peripherals to a common scheduler;
   verify event ordering, isolation, reset and partition-independent execution.
3. Close whole-machine dependency/distribution requirements. Source notice checks
   alone are not legal or binary release clearance.

M0 remains partial. M1 protocol research can proceed independently in the pinned
MAME harness. M1 firmware MIDI acceptance and M2–M6 audio/plugin work remain open.
The research items below are evidence requirements, not a second implementation order.

These tasks follow the first private source snapshot. No GUI work is planned
before the engine has a validated signal path.

1. **EG target/hold behavior.** The source/description discrepancy is
   reproduced synthetically. Determine the hardware-relevant arming/holding
   rule from an AN1x reference capture before changing the upstream model or an
   isolated adapter; keep the current characterization explicitly non-hardware
   validated.
2. **AN1x sample clock and table-ROM access.** A 128 KiB VOP3 mask-ROM image
   has been identified in an external research archive and the shipped-voice
   table regions match the CPU-ROM copy. The DMS image identity, VOP3 address
   index scale and actual sample rate remain unverified. Trace the clock path
   from the service schematic and primary chip evidence; preserve original
   input files.
3. **Timed H8–VOP3 execution.** An opt-in research patch now connects sample
   stepping to MAME time and orders due samples before DSP MMIO; a local MAME
   build and 1/5-second firmware runs advanced both chips. Its assumed 48 kHz
   rate is unverified for AN1x, and this is not a complete boot. Audit evolving
   firmware readbacks and establish the hardware clock before treating timing
   as device behavior. Keep experimental profiles explicit and opt-in.
4. **MIDI and one dry reference voice.** A separate MAME patch now connects
   MIDI IN through H8 RXD0 / SCI0; a local MIDI-file smoke run shows line
   transitions. Verify that firmware consumes note events and establish a
   documented dry output path, then compare with a reference instrument. Full
   PKS-ROM emulation, chip routing and MEG effects remain separate tasks.
   Direct SCI snapshots and keyboard-port traffic are now observable; both
   passive and DSP-experiment runs leave SCI0 disabled at sampled instants
   while SCI1 has 30 TDR writes and no RDR reads. Trace the PKS setup and MIDI
   enable control flow before implementing any assumed keyboard replies.
5. **VST3/AU host layer.** Wrap the validated engine, check real-time behavior,
   sample-rate conversion and state serialization, then begin product UI work.

Repository work: keep source/notice audits, dependency provenance, current
status and the ledger in step with each PR. Cross-platform ROM-free CI already
exists; retain it alongside MSan and the separately filtered MAME build. Plugin,
hardware and host support require their own evidence.

Historical #78 increment (merged): [signed16 displacement memory](h8-displacement16-memory.md), six original groups. See [STATUS](STATUS.md) for the current baseline.
