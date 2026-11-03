# Native VOP3 firmware probe

New explicit lcd-meg-vop3-register-experiment-16mhz profile combines LCD16MHz,
original MEG zero-read stubs and experimental firmware upload control with the
native register-only dual VOP3 sink. Older profiles remain strict at VOP3.
JSON names the profile and reports per-chip writes/reads/sample_passes plus
audio_accepted=false;metrics availability is explicit (null counters if a native
handle failed),preserving the original execution-stop diagnosis;firmware_boot_accepted remains false. No DSP sample is
advanced;the model48kHz assumption is not silently applied to the CPU clock.
Synthetic CLI tests cover both chips,upload command,three slices,old-profile
rejection and unsupported read/byte faults,with unchanged input hashes.

Read-only local1.04 ROM (SHA2564f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe)
with swap16,100M cycles/20M callback cap and4096/1000000 slices yields identical
physical stop:cycle11260853,PC345720(0x54678),data_read440044 word,
error Unimplemented VOP3 register read. Native writes[10236,0],reads[0,0],
sample_passes[0,0],MEG controls5/status_reads1,GPIO6316,TX/clock0.
Completed-slice cycles11259904/11000000 and callbacks1886465/1839738 differ by
slice boundary as designed;they exclude work committed in the failed slice.
ROM SHA unchanged before/after. Counts show accepted core callbacks,not all
register semantics implemented or hardware-equivalent DSP program acceptance.
The next blocker is reg22 readback evidence/selected-index/timing semantics;
no zero fallback or undocumented ready response added. Existing bridge has an
experimental EG-readback path,but this profile deliberately keeps it disabled.
Pinned original MAME source lock unchanged;this is project integration,not new
original-method code. No boot,MIDI or audio acceptance. Own CI/review pending.

