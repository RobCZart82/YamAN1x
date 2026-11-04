# Firmware level-read experiment

New explicit lcd-meg-vop3-level-experiment-16mhz profile configures the native
sink and host adapter for experimental_an1x_level;MEG zero stubs and upload
contract remain explicit. Older register-only profile still rejects22. No sample
advancement. Sink cold reconstruction preserves selected policy while clearing
state;subset reset retains levels. Synthetic CLI checks three slices,selected
read counters,old-profile rejection and invalid index;native sink verifies
-256=>C000,subset retention and policy-preserving cold reset.

Read-only unchanged1.04 ROM,swap16,100M cycles/20M callbacks,4096/1000000 slices:
identical stop at cycle11925102,PC2912(0xB60),data_writeFFFF60 byte,
Unmapped AN1x byte write. Native writes[11557,516],reads[448,0],samples[0,0];
MEG controls5/status_reads1,GPIO6316,TX/clock0. Completed-slice cycles
11923456/11000000,callbacks2000165/1839738 exclude the failed slice as designed.
ROM SHA4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe
unchanged before/after. Both-chip callbacks do not prove full upload acceptance;
448 read callbacks do not alone prove scan ordering or the specialC000 branch.
No complete boot,MIDI or audio acceptance. The next blocker is the ITU TSTR
start path atFFFF60:active timer counter/flag/IRQ/time behavior needs original
reference and independent tests before enabling it. Do not treat the old
stopped-ITU storage profile as an active timer implementation.
Pinned original MAME/bridge unchanged. Own CI/review pending.

