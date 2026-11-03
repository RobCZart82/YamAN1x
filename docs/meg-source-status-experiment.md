# Explicit original-MAME MEG status-stub experiment

Physical MEG status/handshake remains unknown. The inspected Yamaha service
manual gives pin/address/data wiring,not host register readiness semantics.
A focused public search found pin descriptions but no primary host-status
specification. Do not infer ready bits or processing times from the schematic.

Locked original MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
meg.cpp:s2_r/s10_r/s11_r log the access and return00. This is a source stub,
not a functional MEG implementation or hardware evidence. The optional local
reference checker now extracts24 unchanged methods,including these three reads;
768 stub-read comparisons,1536 logging-only control comparisons and720896
storage comparisons pass. This establishes source behavior only.

MegSourceStatus defaults to reject. Explicit Policy::original_zero_stubs permits
only canonical addresses400005/400021/400023 (offset2/10/11),returns00 and
counts successful reads. Invalid address/policy never increments the counter.
The research sink uses the existing shared monotonic-time/fail-stop owner.
Supported subset reset retains external state and counts;cold reconstruction
retains selected experiment policy but clears storage/counts. CPU read forwarding
requires a pure explicit opt-in query and a callback in the narrow MEG window.
No generic read fallback,word access or instruction fetch is introduced.

Probe profile lcd-meg-source-stubs-16mhz is separate from lcd-meg-observe-16mhz.
The latter continues rejecting every MEG input;default and LCD-only are unchanged.
Reports name the exact profile and add meg_source_status_reads. These values are
stub observations,not ready events,completed commands or physical measurements.
No profile ever sets firmware_boot_accepted true. This diagnostic cannot be used
as complete machine/audio acceptance evidence.

Independent tests:768 reads across strict/source policies,invalid addresses,
24-bit alias via bus,word negative control,state isolation,retaining reset,
policy-preserving cold reconstruction and129 CPU partitions. CLI tests verify
strict write-only behavior and explicit source-stub enablement/counter.

Local unchanged1.04 ROM SHA256:
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
100M requested cycles/20M callbacks,swap16,new source-stub profile,slices4096/1M:
identical stop data_write440002,width2,PC353780,peripheral10832302,
instruction_state27009/substate0,1 stub status read,5 observed controls,
6316 GPIO,0 TX/clock edges. Completed slices10829824/10000000,
callbacks1815357/1683663. Hash unchanged before/after;no ROM data exported.
A temporary experiment overriding only400005 toFF instead completed the100M
budget at PC55848 with no later peripheral access. Zero allowed the next VOP3
access. This shows firmware sensitivity,not the correct physical reply. Temporary
experiment binaries/sources stay outside the repository. Next work can inspect
VOP3 host integration under this explicitly limited profile;physical MEG status
requires independent primary specification or timed hardware capture.
