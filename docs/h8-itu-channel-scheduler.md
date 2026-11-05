# Shared ITU channel scheduling

The candidate owns five existing bounded dispatchers in one absolute epoch.
It chooses the earliest enabled deadline across all channels and advances each
channel to that edge before continuing. A shared timestamp consumes one budget
unit and delivers at most15 vector callbacks. Equal-time ordering follows
channel0 through4, then compareA,compareB,overflow within each channel. This
ordering is an explicit project scheduler policy.

Configuration writes use the settled scheduler time. Each instance uses one
channel profile, free-running or A-selected auto-clear. A runtime selection of
channel modes belongs to the subsequent MMIO integration. Existing channel
owners keep their arithmetic and event semantics. The callback receives the
vector and absolute cycle. time() exposes that cycle during delivery.

A callback exception, nested mutation or dispatch, or exhausted event budget
permanently fails the whole scheduler. Delivered effects remain committed.
On callback failure, channel snapshots may show partial progress through that
edge and serve diagnostics only. Recovery requires reconstruction. Invalid
channel indices, zero budget and backwards targets reject before work.
Snapshots and next_event are observational. Thread safety requires external
serialization.

Validation covers five-channel chronological order,33 split points,15
simultaneous vectors,shared budget exhaustion,swallowed reentry,callback failure,
stop and compare reconfiguration,auto-clear recurrence and invalid inputs.
The locked original-method harness adds2048 five-channel event-stream
comparisons over divider/phase/enable profiles and multiple wraps. It builds
unchanged original update_counter,recalc_event,internal_update methods only in
a temporary directory with ASan/UBSan. Each original channel emits its own
stream, then streams are sorted by time and vector under the project tie policy.
The existing10240 single-channel vector/time/count/flag comparisons remain.

```sh
python3 tools/check_itu_dispatch_reference.py --archive /path/to/mame-master.zip
```

The source lock and three method identities are verified before compilation.
Remaining acceptance gates include mixed-mode MMIO routing,CPU IRQ controller
and scheduler wiring,firmware boot,MIDI,DSP sample timing and REAPER audio.
