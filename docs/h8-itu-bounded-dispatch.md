# Bounded ITU vector dispatch

Candidate based on merged #211;this component does not enable active timers in
the machine bus. Supports the existing free-running and A/B auto-clear channel
profiles only. Each owner selects one H83002 channel0–4;A/B/overflow vectors are
24+4*channel,25+4*channel,26+4*channel.

Call run_until(target,budget,sink) to advance absolute time,then reconfigure at
that settled timestamp. It selects the next enabled deadline,advances to it,
and reports (vector,time) in A/B/overflow order. Status flags do not suppress
recurring deadlines. Final settlement also captures flags with IRQ disabled.
A budget unit counts one timestamp,with at most three callbacks. Zero budget
and backwards target reject before advancement and retain a live owner.

Budget exhaustion,callback exception or reentry permanently poisons the owner;
previously delivered callbacks/time remain committed. Reconstruct to recover.
Even swallowed reentry exceptions stop outer dispatch. Snapshot inspection is
allowed from callbacks;mutation and nested dispatch are forbidden. No thread
safety is promised. An unrepresentable future deadline lies beyond any valid
target;settlement may proceed without delivering that event.

Validation includes exact vectors/times on all five channels,129 split points,
simultaneous overflow/compares,reconfiguration at a delivered boundary,IRQ-off
polling,callback failure,reentry and rejecting subsequent dispatch after failure.
The reference command is:

```sh
python3 tools/check_itu_dispatch_reference.py --archive /path/to/mame-master.zip
```

The checker verifies the archive lock,whole source identity and method hashes
against research/h83002-active-itu-contract.json. It executes unchanged
update_counter,recalc_event and internal_update in a temporary harness under
ASan/UBSan.10240 bounded cases compare vectors,timestamps,counter and flags,
using explicit positive times and channel-specific vectors. The interrupt sink
records callbacks;it does not emulate or validate a CPU interrupt controller.
The original source stays outside the repository. No ROM is needed.

Remaining:aggregate channels with the machine scheduler,connect actual CPU
interrupt eligibility/delivery and supported MMIO writes,then retest the local
firmware stop. Full hardware semantics,external clocks/capture,boot,MIDI,DSP
samples and REAPER acceptance are outside this bounded profile.
