# VOP3 word-bus research composition

Explicit optional sink hooks route only full-word data access in canonical
440000..4400FF and480000..4800FF windows after CPU24-bit normalization.
Byte/instruction accesses and sinks without opt-in stay strict. Odd addresses
and unsupported reads reject through the existing adapter. Pure wait query
uses external area2 only for selected VOP3 words;32 ASTCR/WCER/WCR combinations
are checked independently. No permissive MMIO fallback added.
An1xVop3ResearchSink composes LCD/MEG with privately owned native dual VOP3,
sharing the monotonic sink epoch and fail-stop policy. Upload status defaults
to reject;the pre-existing firmware-contract experiment requires explicit choice.
Subset bus reset retains external chips;explicit stopped-owner cold_reset
reconstructs both chips and the LCD/MEG epoch. No samples are advanced.
1024 independent coefficient/address/value cases,aliases,counters,read/width/
opt-in rejection,swallowed reentry,reset/rewind and129 synthetic CPU run cuts
check the integration. A MOV.W executed by CPU writes a verified BE coefficient.
No firmware CLI profile change,ROM access,boot/MIDI/audio acceptance or physical
clock/status proof. Existing pinned original MAME source lock is unchanged;
this step composes existing project adapters,not a new upstream-method port.
Next:explicit bounded firmware-probe profile with separate assumption labels.
Own remote CI/review pending.


## Cold reset through shared sink views — 2026-11-05

The public LCD/MEG/VOP3 hierarchy now has virtual cold_reset and a virtual base
destructor. Reset via either LCD or MEG reference reconstructs the full concrete
sink, retaining configured policies while clearing LCD/MEG/VOP3 epoch state.
Base-pointer ownership destroys the full composition. Qualified base calls inside
overrides deliberately reset the parent portion before committing the new child.
The owner must still stop the machine before reset; this introduces no automatic
machine/sink coordination, hardware reset, threading or audio acceptance.

Merged fix: [#213](https://github.com/RobCZart82/yaman1x/pull/213),
`d60e6eb9907d0a9c89938e1677d68e54b319da74`; all five final-head CI jobs passed, including
the extended integration target under Linux ASan/UBSan.
