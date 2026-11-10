<img src="docs/images/yaman1x-logo.svg" alt="YamAN1x" width="520"><br>
Firmware-based Yamaha AN1x emulator research, preparing a future **VST3/AU**
plugin. Repository name: **yaman1x**.

![Original Yamaha AN1x hardware photographed for the YamAN1x project](docs/images/an1x-hardware.png)

*Original Yamaha AN1x hardware — illustrative photograph supplied by the project
owner. This is a photograph of the physical synthesizer, not a VST3/AU plugin
or a screenshot of YamAN1x.*

First public research snapshot: **2026-10-26** — [v0.1.0-research](https://github.com/RobCZart82/yaman1x/releases/tag/v0.1.0-research). This is a development foundation. It does not yet produce AN1x audio,
complete a validated hardware-equivalent boot, or provide a plugin or GUI.

## Build and test — no ROM required

Requirements: CMake 3.20+, a C++20 compiler, Python 3.9+, Git and platform build
tools. The reviewed VOP3 and AsmJit source snapshot is included, with original
licenses. Configuration does not download dependencies or Yamaha data.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

The source-only build defines **138 CTest groups and 125 Python cases**. Tests generate synthetic
programs/banks in temporary directories. The two upstream tests that require
Yamaha images are not part of this suite. ROM-free GitHub Actions checks this suite on Linux x64, macOS Apple Silicon and
Windows x64; see the current status below for tested revisions.
This checks the source build, not AN1x hardware fidelity or plugin-host support.

See [engine architecture decision](docs/ADR-0001-engine-boundary.md) and
[build baseline](docs/BUILD_BASELINE.md) for the M0 decisions and remaining gates.

## PKS forrás-RAM íráselőzmény — jelölt — 2026-11-10

A következő, még be nem olvadt auditorváltozás a rögzített PKS-forrásolvasások
értékét a legutóbbi, ugyanarra az igazított RAM-szóra és bájtlane-re eső korábbi
írással is összeveti. Az ER0 indexének ki kell választania a megfigyelt szót és
a bájtlane-et; a byte-write és full-word write eseteket külön kezeli. Az
ellenőrzés a meglévő FA92 olvasás → FA94 célírás cím/érték/R0L/ER4/idő kapcsolat
előtt fut. Ez megfigyelt írás-olvasás eredetlánc, nem PKS/ADC-szemantika.

A 12 célzott forráskontextus-teszt és a teljes Python-tesztkészlet (125/125)
helyben sikerült. A 138 CTest-csoportot ez a Python-only változás nem módosítja;
teljes CTest, MAME-fordítás és új firmware-futtatás ehhez a jelölthöz még nincs
rögzítve. ROM-ot ez a lépés nem olvas, nem módosít és nem tesz a repóba. A
távoli ellenőrzések és a pontos PR-head review a jelölt PR kapui; ez a rész még
nincs beolvasztva, hardver/boot/MIDI/hang-elfogadást nem jelent.

PR [#269](https://github.com/RobCZart82/YamAN1x/pull/269) az eredeti
headről (`0afd0e86fd4845514c2e969d809d25880b9d7d64`) nyílt, a
`947d67950345c7e03362d40f25174fcbe3b1f268` alapra. Ezen a pontos headen
mindhárom kötelező platformellenőrzés sikeres (Linux `130053106191`, macOS
`130053106085`, Windows `130053105515`); az MSan is sikeres
(`130053107228`). GitHub review-objektum, review-szál és PR-komment egyaránt
0; kvótahibára nincs bizonyíték. A szabályzat 0 jóváhagyást ír elő, a review-
szálak rendezését követeli. A naplófrissítés új PR-headet készít, ezért az új
pontos head ellenőrzései még függőben vannak. A PR még nincs beolvasztva.
## PKS source-byte CPU context — merged in #267 — 2026-11-10

PR [#267](https://github.com/RobCZart82/YamAN1x/pull/267) merged by squash as
`dae0d64db4d463d119ee7be1c452c90a8487513d` from verified PR head
`411524cd9797db6e0aa7b9f3d0b14340eb50b52c`, based on
`b04ff5e8ed1d8537c51b8a9ad0d03cf11494d36a`. The bounded, read-only MAME
observer, independent parser/auditor, tests and evidence are now on main.

Two nine-second passive/default-input runs produced identical 43-event source
context traces (SHA-256
`012b91ce66f3b3fdef937ef374f405d6004a53fc7703059d1e75c3bcecbe2d76`) with
no drops. The prior 1,928-line PKS context trace also matched. Six observed
source-read/payload-write pairs matched byte value, destination, R0L/ER4 and
250 ns of MAME virtual time. CCR is recorded without assigning flag meaning.
The local AN1x 1.04 ROM SHA-256 remains
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`;
it was not modified or added to the repository.

All five checks passed on the exact PR head: Linux, macOS, Windows, initial-decode
MSan and MAME research (check-run IDs `129967758579`, `129967758860`,
`129967758798`, `129967758081`, `129967758518`). Codex review completed
without findings on that exact head (comment `6772060805`). GitHub recorded
zero review objects and zero review threads; no quota issue was reported. The
active ruleset requires zero approvals and resolved threads. All five checks
also passed on the merge commit (IDs `129989103239`, `129989103935`,
`129989103739`, `129989105616`, `129989104714`). Squash merge used the
verified head without bypass.

This evidence is limited to one passive profile and does not establish PKS or
ADC semantics, hardware agreement, complete boot, MIDI acceptance or audio.
Next: trace the preceding writes and instruction context around the bounded
source/payload observations, retaining independent address/value/flag/time and
continuation checks. Do not infer a protocol or add HLE replies from observation
alone. Full reset, firmware/MIDI, DSP fidelity, audio, VST3/AU and REAPER
acceptance remain open. M0 is partial; M1 is prepared but unaccepted.

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

## Firmware diagnostic evidence — 2026-11-09

## PKS operand CPU context — merged in #264

A bounded, read-only MAME observer now captures the post-fetch PC, virtual time, CCR and ER0–ER7 when firmware reads/writes the three previously observed aligned RAM word bases `237202`, `237204` and `2349E8`. The observer has a 4,096-event cap, reports overflow, filters all other addresses and does not change bus values. It was built against the pinned MAME source `706dbb73281320c54d7c2f16262c1c3786f2be01`; instrumented binary SHA-256 `0a7115cb99760c3644affe2a8f22a4491a180a824ab21b1d2b18d700174c1768`. The matching uninstrumented build hash is `6269f609a41a7e9e7202d2be9d3edc03f8744b48219fac0c9ebdfe1fea0746ff`.

Two nine-second passive/default-input runs produced byte-identical context traces (1,928 events; SHA-256 `8acf5f8c23ce9a912be8fa0a25323be342ab3f37084d3fbed012a18b5133172e` each). Per run, `2349E8` had 480 reads/483 writes, `237202` had 478 reads/4 writes, and `237204` had 478 reads/5 writes. All events fit within the cap without drops. The baseline and instrumented `run.json` matched on all 26 non-trace fields; only command path, binary hash and trace-line count were excluded. ROM identity remained AN1x 1.04 SHA-256 `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; no ROM contents were changed or added to the repository.

This is limited repeatability evidence for one nine-second passive profile. The callback PC remains the CPU-reported post-fetch PC, not an instruction-start PC. The run does not establish operand meaning, ADC arithmetic, PKS behavior, complete boot, MIDI acceptance, hardware agreement or sound. VOP3 samples remained zero. Next: use these bounded snapshots to follow the preceding value writes and their instruction context, retaining independent value/address/flag/timing and continuation checks.

PR [#264](https://github.com/RobCZart82/yaman1x/pull/264) merged by squash at `a2f52d020ebc6d70e7ade57d9b78a920a9e2af4c` on 2026-11-09T17:35:18Z from verified head `1092df38cb5278e3db50ef29ac6c99fa621c85ce`. PR-head checks passed (ROM-free CI `42652447163`, MSan `42652447223`, MAME research `42652447044`); merge-commit checks passed (ROM-free CI `42666087454`, MSan `42666087385`, MAME research `42666087403`). Codex's review summary completed without findings on the exact head; no GitHub review objects or threads were present, and no quota issue was reported. The active ruleset required zero approvals and resolved review threads. Two macOS hosted-runner acquisition attempts failed with explicit hosted-capacity annotations; after each workflow completed, only the failed job was retried, and the third attempt passed. Squash merge used the verified head without bypass.

The prior bounded RAM observations were merged in [#262](https://github.com/RobCZart82/yaman1x/pull/262). This merged context trace extends that work without asserting data semantics. See [current status](docs/STATUS.md) and [bounded observations](docs/pks-payload-path.md).

## Verified research baseline — 2026-11-09

Verified research-code baseline: `a2f52d020ebc6d70e7ade57d9b78a920a9e2af4c` (#264); #265 reconciled its records. #264 records the bounded PKS operand CPU-context observer and its limited passive-profile evidence. PR-head ROM-free CI `42652447163`, MSan `42652447223` and MAME research `42652447044` passed; merge-commit checks `42666087454`, `42666087385` and `42666087403` passed. Codex’s review summary completed without findings on the exact PR head; no GitHub review objects or threads were present and no quota issue was reported. The ruleset requires zero approvals and resolved review threads. Squash merge used the checked head without bypass.

The suite defines **137 CTest groups and 109 Python cases**.

Historical research-sink reset fix: [#213](https://github.com/RobCZart82/yaman1x/pull/213),
`d60e6eb9907d0a9c89938e1677d68e54b319da74`, merged 2026-11-05T09:09:33Z (10:09:33 Europe/Budapest).
Tested PR head: `a421a261a6647e759dac7d5132567d82f2de32fc`.

The #213 suite defined **131 CTest groups and 103 Python cases**.
[Linux/macOS/Windows and Linux sanitizers PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686134),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686447),
[MAME research PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686368).
These results belong to #213's final PR head. Automated review completed without
findings on source commit `95b7edb0feaa97be23d1105fe59b6f259596e640`; only PR-link
documentation changed afterwards (three files). No final-head review is claimed. The actual macOS runner is15; the required check retains its old name.

Bounded shared SYSCR modes, absolute machine time, supported subset reset,
LCD/MEG/native register-only VOP3 research adapters, and standalone ITU counter,
flags, deadlines and exact-time vector dispatch are implemented. The full
hardware RESET line and boot remain unaccepted. ITU scheduler/MMIO/CPU IRQ
integration and subsequent firmware diagnostics were completed later in #215–222;
the FFFF60 boundary describes the historical #213 stage.
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

Merged #158 reconciles the audit documentation. #159–164 pin reset input,child
configuration,queued RESET and scheduler time contracts from the fixed MAME
reference. #165 adds a tested standalone absolute/reset-relative time policy.
Merged #166 connects the absolute clock to research-machine execution and cold
reconstruction,with split-run/input/wire tests and ten ASan/UBSan CI groups.
Merged #167 adds a CPU-only deferred reset request between run calls:
completed bus-cycle debt is retained and repaid before reset vector fetches.
This project-owned policy is not a physical RESET line or machine reset API.

CPU/bus cold reconstruction and component reset tests exist;whole-machine warm
reset and complete firmware boot are **not accepted**. The machine clock stays
monotonic within each cold epoch;CPU debt and queued input remain owned by their
existing components. Hardware reset ordering and assertion/release integration
remain open. Firmware MIDI/PKS acceptance,DSP/table/EG/audio routing,offline
reference sound,realtime rendering,VST3 and REAPER acceptance remain open.
No AN1x audio,VST3,AU or GUI is currently delivered. No project-wide release
license has been selected. ROMs remain local and unchanged.

Merged #168 resets supported bus peripherals without rewinding absolute time or
pending RX. #169 coordinates CPU/bus subset reset at debt-free stopped boundaries,
with separate reset count/relative time and fail-stop reentry handling.
Merged #170 adds boot diagnostics;#171–173 add constrained ABWCR,ASTCR and
programmed WCR/WCER profiles with checked2..6-cycle access costs. Physical
reset/WAIT/pin-mode limitations remain documented. Merged #186 introduced disabled ITU interrupt enables and empty status flags
on five stopped channels;active timer events/IRQs remain unsupported.
The separate source-stub candidate profile reaches VOP3 word write440002 at
cycle10832302. Existing observational/default profiles remain strict on MEG reads.
No physical MEG readiness,complete boot,audio or effects acceptance follows.
Next inspect VOP3 host integration;MEG status needs independent specification/capture.
See [source-stub experiment](docs/meg-source-status-experiment.md).
One PR at a time;candidate validation and CI/review are separate gates.

See the [development plan through the first sound in REAPER (Hungarian)](docs/FIRST_REAPER_SOUND_PLAN.md)
for the planned firmware, audio-engine and VST3 milestones and acceptance criteria.

- Read-only AN1x 1.04 ROM identification, byte view and reset-vector inspection.
- AN1x bulk-SysEx structure/checksum validation and synthetic upload adapter.
- Two VOP3 instances, C ABI 2, register replay and independent upload-state audit.
- LCD bus model and a separate instrumented MAME research patch chain.
- Two standalone H83002 SCI channels with byte/word MMIO and DMA-qualified
  interrupt routing to the H8H controller; synthetic CPU/scheduler RX/TX and
  receive-error integration is tested, while full AN1x machine integration remains open. See [SCI MMIO/IRQ evidence](docs/h83002-sci-mmio-irq.md).
- Opt-in experimental upload-control and selected EG-level readback profiles.
- Integer sample clock; synthetic tests run 2400 samples per chip with matching
  results under different call partitions.
- Standalone H8 GPIO, internal/external IRQ helpers and a shared asynchronous
  SCI TX/RX channel, with synthetic timing, status and isolation checks. The
  bounded CPU/SCI/GPIO/IRQ integration is implemented; complete machine
  coverage, reset and firmware acceptance remain open.

The local firmware research run loads both complete DSP programs under the
experimental status profile. Its audited upload fields match Python
reconstruction and native replay. The H8 harness is register-only by default;
an explicit `assumed-48k` profile advances both DSP models before MMIO. The
complete eleven-patch research target also builds in separate macOS CI without
Yamaha data. A known upstream EG target-hold discrepancy, the
AN1x clock and table-ROM scaling remain unresolved. These are recorded in
[development status](docs/STATUS.md) and the [roadmap](docs/ROADMAP.md).

The [development ledger](docs/DEVELOPMENT_LOG.md) records merged changes and
maintenance rules. The [dated repository audit](docs/REPOSITORY_AUDIT_2026-10-28.md)
preserves the October28 snapshot;the current status and ledger supersede it. Contribution requirements
are in [CONTRIBUTING.md](CONTRIBUTING.md).

## Local firmware research

No Yamaha firmware, program/table-ROM image, bank, service manual, raw capture,
compiled emulator or plugin is included. Keep local inputs and captures outside
the checkout or in ignored `roms/` and `private_runs/` folders. These inputs stay local and are not published with this repository.

```sh
python3 tools/rom_inspect.py /path/to/local-firmware.zip
python3 tools/bank_inspect.py /path/to/local-bank.syx
python3 tools/an1x_table_rom_audit.py /path/to/local-firmware.bin /path/to/VOP3-main.zip \
  --table-rom-member VOP3-main/docs/vop3_an1x/maskrom.bin
```

These tools do not change their input files. The table-ROM audit compares known
table regions with the CPU-ROM copy; it does not verify DSP address scaling or
hardware fidelity. The separate macOS MAME research build needs a compatible
local MAME source snapshot and separately obtained local firmware. See
[research instructions](research/README.md). It is not linked into this core,
and is not part of the default build.

## Source provenance and licensing

See [THIRD_PARTY.md](THIRD_PARTY.md), [licensing status](LICENSE) and the
per-file snapshot hashes in `third_party/provenance.json`. No project-wide
open-source license for the project's own code has been selected yet.
Upstream source files retain their licenses and copyright notices.

`tools/repository_audit.py` checks candidate Git files, disallowed file types,
binary/secret/path markers and every vendored source hash. The original hardware
photo and project logo are permitted only at their exact approved paths/hashes;
see [asset provenance](docs/images/README.md). It is an upload
check, not a guarantee of firmware authenticity or hardware fidelity.

Product spelling is **YamAN1x**. Existing internal `yamanix` identifiers and the
`YAMANIX` trace protocol are retained for reproducibility. Older local reports
use the previous working name YamANIx; historical context is explained in
[research history](docs/HISTORY.md).

The default branch uses an [active PR-based protection ruleset](docs/BRANCH_PROTECTION.md).

## Historical component increments

The following notes describe each increment at its original scope. Their old
coverage and pending-task statements are historical; current integration, counts
and remaining work are recorded above.

The bounded [integer-cycle dispatcher](docs/cycle-dispatcher.md) is a tested scheduling primitive; CPU/peripheral integration remains pending.

The [two-channel SCI timeline](docs/sci-cycle-timeline.md) now combines timestamped RX input with bounded UART clock dispatch. Its tie order is a software convention; CPU integration remains open.

The [H8 host-services boundary](docs/h8-host-services-boundary.md) records source-derived bus/cycle requirements and the remaining work before embedded CPU compilation.

The [H8H bus-access helpers](docs/h8h-bus-access.md) preserve source-defined word alignment, instruction/data routing and pre-access cycle charging. Full CPU execution remains open; a first generated subset is described below.

The [H8 execution budget](docs/h8-execution-budget.md) preserves inter-slice cycle debt and peripheral-boundary arithmetic; the first generated handler subset is now connected, while complete instruction coverage remains open.

The [first generated H8 instruction profile](docs/h8-first-instructions.md) compiles and executes reset, NOP, MOV.B immediate and BRA rel8 with per-instance register/prefetch/substate storage. Original H8H decoder/handler comparison agrees across 200,000 synthetic slices. Other opcodes fail explicitly; this is not a complete CPU, firmware boot or audio result.

The [H8H memory/branch extension](docs/h8-memory-branches.md) adds MOV.L immediate32, indirect byte data reads/writes and all 16 short branch conditions. The isolated original-decoder comparison agrees over 200,000 mixed slices; full CPU, IRQ/SCI integration and firmware acceptance remain open.

The [word/long memory extension](docs/h8-word-long-memory.md) adds aligned word data callbacks and original two-stage prefix decoding for long transfers. Mid-transfer continuations, aliases and wrap cases are tested against the original H8H execution path; complete CPU/firmware acceptance remains open.

The [H8H arithmetic extension](docs/h8-arithmetic-flags.md) adds selected ADD/SUB/CMP forms at 8/16/32 bits, verified against independent mathematical flags and the original H8H path. Full CPU, IRQ/SCI and firmware/audio acceptance remain open.

The [carry/register/logic extension](docs/h8-carry-register-logic.md) adds byte carry-input/sticky-Z arithmetic, wide register arithmetic, logical forms and CCR storage. Multi-byte arithmetic is tested against independent full-width results; IRQ entry and full firmware/audio acceptance remain open.

Selected CPU extension: [H8 word/long register logic](docs/h8-wide-register-logic.md).

Selected CPU extension: [H8 single-bit shifts and rotates](docs/h8-shift-rotate.md).

Selected CPU extension: [H8 register bit operations](docs/h8-register-bit-operations.md).

Selected CPU extension: [H8 indirect memory bit operations](docs/h8-memory-bit-operations.md).

Selected CPU extension: [H8 absolute memory bit operations](docs/h8-absolute-bit-operations.md).

Selected CPU extension: [H8 stack and control flow](docs/h8-stack-control-flow.md).

Merged extension (historical candidate): [coverage and CCR family](docs/h8-coverage-and-ccr.md).

Merged extension [register-to-register transfers](docs/h8-register-transfers.md) extend MOV at all three widths with lane/alias/flag checks.

Merged extension [small arithmetic](docs/h8-small-arithmetic.md) adds INC/DEC and ADDS/SUBS with explicit flag-preservation checks.

Merged extension [unary/extension family](docs/h8-unary-extension.md) adds NOT/NEG/EXTU/EXTS with independent value/flag expectations.

Merged extension [fixed-profile TRAPA entry](docs/h8-trapa-entry.md) adds synchronous trap stack/vector handling; external IRQ/NMI remains separate.

Historical extension: [signed16 displacement memory moves](docs/h8-displacement16-memory.md).
