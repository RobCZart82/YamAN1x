# Active H83002 ITU contract and implementation gates

Research:2026-11-04. No active timer or new MMIO acceptance in this step.
Source lock:research/mame-source-lock.json,original commit
706dbb73281320c54d7c2f16262c1c3786f2be01. Three files/19 exact method identities
are pinned in research/h83002-active-itu-contract.json. Reproduce with:

```sh
python3 tools/check_active_itu_contract.py --archive /path/to/mame-master.zip
```

The checker uses the existing private hashed archive snapshot;it never reopens
the archive after hashing. It records identities,not copied method source or
execution evidence. Unit tests cover exact const-method hashing,missing/duplicate
rejection and changed-body detection. Original source is not modified.

## Current trigger and scope

The explicit clockless VOP3 level probe stops on byte writeFFFF60,TSTR,
cycle11925102,PC0xB60. Its prior callbacks are writes[11557,516],reads[448,0],
samples[0,0]. This is the experimental MEG/upload/level profile,not complete
boot or hardware proof. Current project bus has stopped-ITU register storage;
TSTR and running counters remain rejected. Do not enable TSTR as a no-op.

## Original behavior to preserve in a bounded profile

- H83002 maps TSTR atFFFF60,TCNT words atFFFF68/72/7C/86/96. It configures
  five H8H channels,each with two TGRs and interrupt bases24/28/32/36/40.
  Header constructor assigns A=base,B=base+1,overflow=base+2.
- TSTR stores the written byte,then applies each low-five enable bit. A channel
  first updates its counter at current CPU time,changes active state,then
  recalculates the event. Repeated enable/disable must settle elapsed time too.
- TCR,TIER,TCNT and TGR writes settle the old configuration before mutation and
  recalculate events afterward. TCNT/TSR normal reads update the counter;
  debugger side-effect-disabled reads do not. Byte-lane word merging matters.
- Internal clocks select shifts0..3 (divisors1/2/4/8),with phase choices for
  slower modes;external input clocks are separate and cannot be invented.
  TCR clear selections are none,A,B or external synchronization.
- Counter evolution uses absolute shifted times and phase,not elapsed blocks
  rounded independently. Disabled updates refresh the time anchor. Wrap and
  auto-clear must handle a starting counter already beyond its clear boundary.
- Compare uses TGR+1 with a u16 comparison temporary;FFFF therefore needs an
  explicit boundary test. Auto-clear cycle uses the wider TGR+1. Preserve and
  test this source distinction before deciding any physical-hardware change.
- Compare/overflow flags can be set while interrupts are disabled. The original
  scheduler selects IRQ-relevant deadlines;polling reads still update flags.
  H8H TIER bits0/1/2 enable A/B/overflow;TSR maps those flags with reservedF8.
  Writing zero clears the corresponding flag;writing one preserves it.
- internal_update processes every due event in a loop,updates at its scheduled
  timestamp,then recalculates. Equal-time events,overflow,large jumps and a
  nonprogressing deadline need explicit project guards and tests.
- Child reset clears configuration/counter/flags/time fields but deliberately
  leaves active state to the parent. Parent reset-after-children restores
  default TSTRE0 and applies five disabled channel bits. Current supported
  project reset/storage profile is not thereby proven fully hardware-equivalent.

## Small implementation sequence

1. Isolated monotonic internal-clock counter component,no bus start yet. Test
   independent values,divisors/phase,wrap/clear,stop/restart and split equality.
2. Add compare/overflow flags and H8H clear/enable behavior,independent disabled-
   IRQ polling cases and original-method differential at boundary values.
3. Add deadlines and bounded dispatch with exact interrupt vector/time tests.
   Reconfiguration at a compare boundary and same-time ordering are mandatory.
4. Integrate selected MMIO/reset/CPU scheduling,then explicitly accept only the
   supported TCR/TSTR modes. Retain rejection for external capture/PWM/chaining,
   synchronization/buffering or other unimplemented functionality.
5. Repeat bounded firmware probes and record the next actual stop. DSP sample
   advancement,MIDI and audio are independent remaining work.

No full active-ITU,firmware,audio or physical hardware acceptance is claimed by
this source-identity audit. No ROM access or Yamaha data is required.
