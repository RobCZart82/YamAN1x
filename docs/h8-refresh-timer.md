# Standalone active refresh timer policy

Reference:locked MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
src/devices/cpu/h8/h8_refresh.cpp. This component is not yet connected to
An1xSciBus,machine time or the interrupt controller. The machine still rejects
running refresh clocks atFFFFAD;no firmware progress is claimed here.

The selected original policy counts at absolute divider boundaries for clock
selections0–7:halted,/2,/8,/32,/128,/512,/2048,/4096. Compare uses periodRTCOR+1,
sets CMF even with interrupts disabled,and normalizes the counter modulo period.
The query irq_requested exposes CMF AND CMIE;it does not dispatch callbacks.
Control write-zero clears CMF without a read-history condition,matching the
pinned source. This differs from a hardware read-before-clear requirement;
no hardware-equivalent CMF acknowledgement is claimed. Reset preserves absolute
time,restores count00/compareFF/control00. Writes occur only after the caller
advances to its desired cycle;the component has no hidden owner clock.

Large elapsed intervals reduce before addition to avoid the original source's
narrow next-count truncation. A counter write above compare normalizes on the
next elapsed tick. next_compare reports that next tick for this case;this avoids
the source's unsigned subtraction ambiguity. Unrepresentable deadlines return
no value;backward updates throw without mutation. These explicit bounded
policies are independently tested,not claimed identical original overflow quirks.

The independent test checks per-cycle ticks,16416 split histories,all256 control
writes,phase,reset,latched-enable and UINT64_MAX. The optional tool
`python3 tools/check_refresh_reference.py /path/to/mame-master.zip` verifies the
archive hash into a private snapshot,compiles unchanged selected original
handlers in temporary storage and compares524288 snapshots under ASan/UBSan.
No original source or ROM is exported by the tool. Its comparison covers normal
count/compare/clock histories and periodic flag clears,not overflow/standby or
IRQ callback multiplicity. Full active machine integration still needs event
ordering,reset,debt/reentry,IRQ scheduling and CPU continuation checks.
