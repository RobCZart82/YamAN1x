# Pinned reset-method contract — merged research evidence

Recorded:2026-10-30. Integrated engine baseline #143;no warm reset API added.
This preparation establishes local source identities and method-local effects,
not global reset ordering or physical AN1x behavior.

`research/h83002-reset-contract.json` pins five files and six device_reset method
bodies at original MAME706dbb73281320c54d7c2f16262c1c3786f2be01. Whole archive SHA256
is checked before ZIP content is read,using research/mame-source-lock.json.
No original body or ROM payload is copied into tracked code.

Reproduce:

```
python3 tools/h8_reset_contract.py --archive /absolute/path/to/mame-master.zip --check
```

Without --check,the tool prints the report;--output writes a new file exclusively.
It reads only fixed members into memory,never extracts or modifies the archive.
The method matcher is formatting-sensitive lexical evidence,not a C++ parser.
Full file hashes detect surrounding changes;method hashes isolate observed bodies.
`h8h.h` and h8h.cpp have no device_reset override in the inspected source;this is
an inspected inheritance boundary,not a new whole-framework closure claim.

## Method-local matrix

| Original method | Explicit effects | Not established by that body alone |
|---|---|---|
| h8_device::device_reset | Sets cycles_base from machine time;STATE_RESET/substate0;clears requested/taken IRQ,DMA/DTC and standby pending;publishes standby0 | Generated reset instruction effects/register values,global time continuity/framework counters |
| h83002_device::device_reset | Calls parent reset;sets SYSCR09 | When the new SYSCR filter becomes published relative to child resets |
| h8_intc_device::device_reset | Sets level modes/falling NMI;clears pending,ISCR,IER,ISR;then check_level_irqs(false) | Final ISR need not stay zero under held physical levels;whole CPU publication ordering |
| h8h_intc_device::device_reset | Calls base reset;clears ICR | Does not itself establish filter or last-publication reset policy |
| h8_sci_device::device_reset | RDR/SMR/SCR0,TDR/BRR/TSR FF,SSR84;clears shifter/phase/clock state;clock_update;emits clock high then TX high | External RX/read-latch retention across every owner reset;absolute event rebase/global pin order |
| h8_port_device::device_reset | Calls update_output only | It does not clear DDR/latch/PCR/ODR in this method;output notification may be suppressed if unchanged |

RAM/NVRAM/ROM reset retention is not proved by absence of assignments in these
methods. Bus maps,driver/framework memory lifecycle and battery semantics require
separate evidence. Cold reconstruction in #143 deliberately has a different
contract:zero internal state,new event epoch,explicit retain/clear NVRAM.

## Implementation gates before in-place reset

1. Verify generated CPU reset and runner/debt behavior,then define supported
   reset boundary:stopped at a valid bus/event point versus mid-instruction.
2. Review owner/device reset traversal order and original timestamp/frame-counter
   interactions;choose explicit event-time/input policy without calling it a
   measured hardware order. Never implicitly rewind an active event timeline.
3. Test SCI transmission/reception interrupted by reset,including high-output
   publication and held RX;test GPIO retained registers and notification suppression.
4. Test IRQ/NMI physical levels,relatched ISR,ICR/filter republishing,CPU CCR/SYSCR
   and pending requests. Source-method identity alone does not validate these.
5. Validate RAM/NVRAM persistence,instance isolation,repeated reset and callback
   reentry/failed-owner behavior. Keep cold recovery API distinct.

The existing component reset tests remain evidence for those components;this
candidate adds no replacement implementation. A whole-machine MAME reset trace
and physical hardware trace are NOT RUN. No new firmware/audio acceptance.

## Tool validation

Four independent Python controls check changed body identity,nested braces under
original indentation,missing/duplicate methods,non-reset substitution and rejection
of wrong archive hash before ZIP parsing. Real locked archive report matches all
six methods. Full Python suite91 tests PASS;engine evidence stays #143 (88 CTest/87 Python),
not rerun for this research-only increment. Final-head CI/review pending.

## CPU reset and runner boundary review — 2026-10-31 candidate

This is a review of the current local implementation, not an executed upstream
warm-reset comparison. PR #145 merged at 2026-10-30T22:25:47Z with all five CI
jobs passing. The engine implementation baseline remains #143.

`src/h8_first_handlers.h` contains full and partial generated reset handlers.
For the selected advanced H8H profile the full handler ORs I into CCR (it does
not clear the other CCR bits), sets EXR to I|NC, reads vector words at 0 and 2,
then publishes the IRQ filter, prefetches at the vector PC, advances PC by two
and completes prefetch without IRQ selection. These are local source observations;
physical reset register values and global reset ordering remain unverified.
The partial handler has distinct continuation states for completed versus retried
accesses. Setting only the CPU state number would leave old partial state and
runner bookkeeping unless an explicit transition handles them.

`H8ExecutionBudget` requires contiguous slices after its first invocation.
A slice can end with debt from an already-completed synchronous bus access;
that debt is carried into the next slice. `H8ExecutionRunner` updates the host
at the slice start before applying debt and resuming a partial instruction.
Neither class currently exposes an in-place reset operation. Reconstructing the
runner would discard its continuity/debt checks; retaining it while changing CPU
state would require a documented policy for pre-reset completed work.

Before implementing reset, tests must independently observe vector-read order,
CCR preservation, filter publication, first prefetch, every partial cut, debt and
nonzero absolute time. Peripheral reset order, retained RX/IRQ levels and pending
events must be specified separately. No instruction-boundary or arbitrary-cycle
reset API is accepted by this review. Existing cold epoch reconstruction remains
the supported recovery operation and explicitly starts a new time epoch.

## Independent cold CPU execution tests — candidate 2026-10-31

`tests/h8_reset_execution_check.cpp` checks all 256 initial CCR bytes at absolute
origins 0,1000 and 2^40,each cut from zero through six cycles (5376 split runs).
A literal expected trace requires vector word reads at origin+2 and origin+4,
filter publication after the second vector read and before the first prefetch,
and the prefetch at origin+6. The filter callback has no timestamp argument:
its recorded coordinate is the preceding bus completion,not an independently
measured filter event time. Zero continuations must not duplicate effects.

Independent assertions require CCR=input|0x80,EXR=0x7F,the vector PC and partial
state8 after six cycles,one cycle of debt at odd cuts,and exactly one next NOP
fetch at origin+8 after continuation. Snapshot/trace partition equality provides
an additional check;it is not the sole expected-value oracle.

Strict warnings and targeted ASan/UBSan pass. The pinned original generated
H8H decoder/handlers plus execute_run differential also passes 200000 slices
with zero differences. This broader existing reference comparison is distinct
from a new original whole-machine warm-reset trace;the test starts newly
constructed CPU owners. Original ROMs and generated engine bodies are unchanged.

A temporary negative-control build replacing reset CCR OR with assignment produces
6858 assertion failures;the test therefore rejects clobbering preserved flags.

## GPIO retention extension — candidate 2026-10-31

The digital-port test covers three masks (00,A5,FF),all256 DDR values and
all256 data values:196608 configurations,each reset three times. It requires
unchanged DDR and masked DR/PCR/ODR readback,zero additional output callbacks,
and a per-bit pin-read oracle with forced input40 and external pins96.
The locked original device_reset identity still matches:it calls update_output
only. This test validates the local component contract,not framework traversal,
physical hardware retention or global reset timing. Engine code is unchanged.

## Active SCI reset extension — candidate 2026-10-31

For every cycle cut0..400,the shared channel test starts TX A5 and held-low RX,
services deadlines strictly before the cut,then resets at the cut. Independent
assertions require clock-high followed by TX-high,SSR84,SMR/SCR/RDR/phases0,
BRR/TDR FF,idle engines,no deadline and retained low physical RX. Draining must
produce no stale completion or extra IRQ. A second reset republishes both high
outputs. This includes cuts exactly on unserviced deadlines. It does not prove
reset traversal order,full receive-frame restart or global absolute-time policy.

## IRQ physical retention extension — candidate 2026-10-31

All256 held IRQ masks times all256 post-reset enable masks are checked against
an independent lowest-set-line oracle. Before reset,edge mode and pending
internal IRQ/NMI are active. Reset must clear registers but re-latch held levels,
retain the old publication until owner filter refresh,and preserve the NMI input
without inventing an edge. Re-enabling IRQs selects the lowest eligible line;
a genuinely new NMI edge still selects vector7. The locked original reset
identity remains the source reference. Whole-machine traversal is still open.

## Post-reset transmission — candidate 2026-10-31

Each byte0..255 starts at absolute cycle1000 after an unrelated55 frame is
aborted at73. A literal start/data/stop/idle waveform requires first edge1002,
data bits1034+32*bit,stop1290 and idle1322. Exactly one ready and end IRQ,
SSR84,idle state and no pending event are required. This validates local channel
reconfiguration after reset,not global machine time continuity or RX recovery.

## Post-reset reception — candidate 2026-10-31

Each byte0..255 is received at start1000 after an old low-input reception is
aborted at73. Re-enabling RE with held-low RX and writing the same low at900
must leave the engine idle with no event. A high at950 followed by a real frame
must yield the literal byte,SSR C4,exactly one RX IRQ,no error and no deadline.
DMA read then restores SSR84. The existing synthetic wire supplies independent
start/data/stop bits. This does not prove whole-machine time/input reset ordering.

## Framework traversal — merged #154,2026-11-01

`research/h8-reset-traversal.json` pins two original framework files and three
methods from the same locked archive. Reproduce with:

```sh
python3 tools/h8_reset_traversal.py --archive /absolute/path/to/mame-master.zip --check
```

The original device reset traversal runs interface pre-reset hooks,device_reset,
then recursively resets subdevices,then device_reset_after_children and interface
post-reset hooks. This establishes parent-before-child method ordering,not the
actual H83002 child insertion order or a whole-machine execution trace.

The execution interface pre-reset clears totalcycles and manages suspension.
Its post-reset resets input objects and configures interrupt timers. Therefore
retention of a physical input in an H8 component method alone does not prove
retention through the execution interface/framework. Next inspect input-object
reset semantics,device child construction order and scheduler local-time handling.
Do not implement global reset by merely calling the already-tested components.
The existing cold reconstruction remains the supported recovery operation.

Two new tests detect changed/missing method identities and verify that archive
verification failure prevents framework ZIP access. No source bodies or ROM
payload are copied into the report. This is lexical evidence,not execution proof.

## Verified archive snapshot — merged #156,2026-11-01

Both identity tools now hash a private temporary snapshot while copying the source,
then parse that same snapshot. Replacing the source path after verification cannot
change report input. A regression test replaces the original with invalid bytes
at ZIP-open time and requires the unchanged verified report. A private disk-backed
snapshot avoids holding the complete source archive in memory.

## Execution input reset — candidate 2026-11-01

The locked framework report now also identifies
`device_execute_interface::device_input::reset`. Its complete method assigns
only current/stored vectors to the default IRQ vector. It does not assign the
current physical state or queue index in that method. The broader post-reset
comment says vectors and queues,but that comment is not proof of queue clearing.
The earlier warning about framework input behavior is narrowed by this finding;
retention through other framework operations and actual scheduling remains open.

This method-local observation supports retaining held-level test coverage;it
must not be generalized to full reset event-order guarantees. Before an in-place
machine reset,inspect actual child construction order,queued input callbacks and
scheduler local-time handling. No engine reset API is added here.

## H83002 declaration order — candidate 2026-11-01

`tools/h83002_child_order.py --archive /absolute/path/to/mame-master.zip --check`
checks the locked source/method identity and ordered configuration declarations.
The22 declarations are INTC,ADC,DMA,DMA0,DMA1,ports4/6/7/8/9/A/B,timer16,
its five channels,SCI0,SCI1,watchdog,refresh. This is lexical configuration order,
not proof of direct-child hierarchy or device-list insertion/reset order.
DMA channels and timer channels may belong to different owners;that must be
resolved from device creation before flattening this list into reset operations.
The embedded research bus still lacks ADC/timer/watchdog/refresh integration.
Next inspect owner resolution/device insertion and local-time/input scheduling.

## Constructor tag hierarchy — candidate 2026-11-02

The pinned H83002 inventory now records constructor finder tags and rejects
missing tags for non-SCI declarations. DMA channel tags are dma:0 and dma:1;
timer channel tags are timer16:0 through timer16:4. The other recorded peripheral
tags have no colon. SCI array tags are initialized in a base constructor and
are deliberately not inferred here. The CPU/INTC argument to a configuration
macro is not,by itself,evidence of device ownership. Framework tag resolution
and actual insertion traversal still need inspection before a reset API.

## Framework owner resolution — candidate 2026-11-02

The locked framework report additionally pins the complete src/emu/mconfig.cpp
file (no method-body extraction for its non-void configuration methods).
Source inspection:resolve_owner walks colon-separated tags through existing
subdevice lists and rejects missing parents;add_device appends the new device
to that resolved owner's list before invoking its own configuration additions.
Together with recursive device_t::reset,this supports nested DMA/timer traversal
rather than treating all22 declarations as direct H83002 children.

This is a source-derived inference,not a captured runtime reset trace. Finder
configuration overloads,base SCI tags and any configuration additions must also
be checked before claiming an exhaustive concrete traversal. In particular,
queued input callbacks and scheduler local-time behavior remain separate gates.
No runtime warm-reset API or physical hardware equivalence is added.

## Queued RESET line — candidate 2026-11-02

The locked report now identifies the original empty_event_queue callback as well.
It processes queued entries in order,updating current state/vector per entry.
RESET ASSERT suspends execution;clearing a previously reset-suspended device
calls device.reset then resumes RESET suspension. A source FIXME explicitly
leaves peripheral output deactivation on assertion unresolved. Therefore do not
present a direct device reset call as complete RESET pin emulation.

Ordinary asserted/held lines call execute_set_input with ASSERT;clear passes
CLEAR. Non-clear ordinary inputs trigger interrupt wakeups. The queue counter
is cleared at callback end,not by the vector-only device_input::reset method.
Reset invoked inside queue processing has additional ordering/reentrancy effects;
this is source inspection,not proof of every multi-event reset case. Next pin
scheduler synchronization/local-time behavior and define the supported reset API
boundary before runtime implementation. No warm-reset API is introduced here.

## Reset time domains — candidate 2026-11-02

The locked framework report now includes schedule.cpp and exact identities of
local_time,total_cycles,abort_timeslice and scheduler synchronize. During active
execution,local_time adds elapsed-slice time to localtime,total_cycles adds the
same elapsed cycles to totalcycles. interface_pre_reset clears totalcycles but
does not itself assign localtime. These counters must not be conflated.
abort_timeslice absorbs positive remaining cycles and zeros the execution count;
synchronize creates a zero-delay temporary timer rather than directly invoking
the callback in that method. Actual timer dispatch order needs separate proof.

For the embedded engine,the accepted cold restart remains a new generation at
cycle0. A future in-place reset must explicitly keep absolute peripheral time
separate from reset-relative CPU counters and must define pending input/debt
handling. Next implement a bounded time-domain policy with independent tests
before exposing machine reset;do not copy the global scheduler implicitly.
This is source-derived timing evidence,not a new runtime reset implementation.

## Bounded time-domain policy — candidate 2026-11-02

ResetTimeDomain is a project-owned standalone arithmetic helper. advance_to is
monotonic;reset_origin changes only the relative origin and generation,never
absolute time. Relative-to-absolute conversion rejects overflow before mutation.
It does not model CPU debt,execute instructions,consume inputs or reset devices.
Only an owner-defined quiescent boundary may use it;integration remains open.
Tests use literal expected times at three origins,300 repeated resets,backward
rejection and UINT64_MAX conversion boundaries. This is policy,not MAME or physical
RESET pin equivalence. Cold machine reconstruction remains a separate new epoch.

## Machine clock integration candidate — 2026-11-02

The research machine now stores its epoch cursor in ResetTimeDomain and supplies
its absolute value to CPU execution. Only a successfully returned run advances
the clock;execution failure poisons the epoch. Invalid ranges are rejected before
execution. Cold reconstruction replaces the complete state and time domain after
candidate validation;the external machine generation continues to label epochs.
No reset_origin call is made:reset-relative timing and an in-place hardware reset
remain unintegrated. Pending input deadlines and CPU debt retain existing semantics.
The owner guard is the quiescent boundary for cold reconstruction and rejects
callback reentry. 401 split points compare CPU state,input consumption and TX
history against an independent unsplit machine,including zero and invalid slices.
Reference source remains MAME706dbb73281320c54d7c2f16262c1c3786f2be01,with
research/h8-reset-traversal.json identities;the time policy is project-owned.

## Deferred CPU reset request candidate — 2026-11-02

request_reset_after_debt is an explicit project-owned transition between run calls.
It preserves general registers,CCR and completed cycle debt,replaces instruction
state/substate with RESET/0,and clears requested/taken IRQ bookkeeping. The runner
is retained,including absolute slice continuity. Its normal carry application
repays completed bus debt before dispatching new reset accesses. A swallowed
callback request poisons the active run;failed runners cannot recover this way.
Repeated requests before execution are idempotent. No host callback occurs at
request time and no peripheral/RX/IRQ-controller state is implicitly reset.
This does not implement MAME's scheduler RESET assertion/release semantics.

Literal expected traces require vector reads at request_time+debt+2/+4,then CCR
filter publication and first prefetch at +6. Three nonzero/zero time origins,
21 old execution cuts and seven continuation cuts produce441 checks. Nonzero
register values survive while CCR I is set and EXR becomes7F through the existing
generated sequence. Existing all256-CCR cold-reset checks remain. Pinned reference
identities are unchanged;this policy is not an original whole-machine trace.
The earlier no-CPU-reset-API statements are historical;whole-machine warm reset,
peripheral ordering and RESET line acceptance remain unimplemented.

## Supported bus peripheral reset candidate — 2026-11-02

reset_supported_peripherals is guarded by the bus access owner. It neither advances
the timeline nor consumes pending input. Memory and GPIO state are retained;SYSCR
becomes09,IRQ state resets/re-latches held levels,then CPU filters are republished
from retained CCR. Ports4/6/7/8/9/A/B reset in that order,then SCI0/SCI1. This is
an explicit subset policy;omitted DMA/ADC/timer/watchdog/refresh devices and global
RESET scheduling are not modeled by it. Each SCI emits clock high then TX high
at now_,canceling old shift deadlines while retaining physical RX per component.
GPIO reset publishes only first/changed outputs. Reentry from GPIO or SCI output
fails before further outputs,even when the callback catches the inner error.
No CPU request or machine warm-reset API is added. Fixed reference remains
MAME706dbb73281320c54d7c2f16262c1c3786f2be01 and the pinned reset/child-order reports.

## Supported machine reset candidate — 2026-11-02

reset_supported_state requires a live stopped machine with zero CPU debt and
peripheral horizon no later than logical time. Otherwise it rejects before any
mutation. It settles current-time RX first,requests the CPU deferred sequence,
then resets the supported bus peripherals. Only after success does reset_origin
move to logical time and its count increment. Absolute time/cold generation and
retained memory/ports/future input do not rewind. A cold reconstruction replaces
the time domain and resets this subset counter separately from cold generation.

Machine reentry propagates poison_from_owner to the bus,allowing its output
checkpoints to stop immediately even if an outer sink swallows the rejection.
Reset failure is fail-stop and requires cold reconstruction. This does not model
physical RESET hold/release,unsupported devices or prove hardware-global order.
The earlier no-machine-reset-API statements are historical:this bounded subset
API is distinct from unaccepted full hardware warm-reset semantics.
Independent tests cover41 accepted and40 rejected cut points,literal reset wire
timestamps,register/memory retention,relative origins,pending/current-time RX and
reentry recovery. No new original whole-machine runtime differential is claimed.
