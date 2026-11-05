# Selectable ITU clear mode

The existing H8ItuAutoClear channel now accepts ClearMode::compare_a,
ClearMode::compare_b and ClearMode::free_running through set_clear_mode(now,mode).
The default remains compare_a. The existing set_clear_index API accepts0 or1
and selects the corresponding auto-clear mode. Snapshot clear_index values0/1
select A/B and value2 identifies free running. Cycle is selected TGR+1 for A/B
and65536 for free running.

A mode write settles elapsed time under the previous mode first. Counter,
flags,enables and clock phase survive the switch. Selecting a cycle below the
current count preserves that count until the full16-bit wrap. Unsupported mode
values reject before settling time. The bounded dispatcher and shared scheduler
forward the mode write at their settled timestamp using their existing reentry
guards. Each of the five channels can select its own mode.

Independent validation covers65536 initial counts in free-running mode,
2304 before/after mode transitions with retained status/enables,65 partitioned
switch cases,invalid-mode state retention and an exact mixed A/B/free event
stream. Existing default auto-clear tests continue to exercise prior callers.
The locked original MAME checker now compares2048 mixed-mode five-channel
streams,plus10240 single-channel cases. The separate original auto-clear
checker covers131072 values/flags. Original source identity checks and temporary
ASan/UBSan builds remain in those tools.

Mode selection is a component API. Remaining gates include decoding supported
TCR values,MMIO timing,CPU IRQ routing and fresh firmware diagnostics,followed
by boot,MIDI,DSP sample and REAPER acceptance.
