# Explicit observational MEG bus integration

An1xSciBus forwards only byte writes in400000..40003F when a sink provides
both pure meg_write_enabled() and meg_write_byte(address,cycle,value),and opts
in. The address is already24-bit normalized. MEG host adapter rejects wrong
lane,hole,unsupported offset. All reads,words and instruction fetches remain in
the strict existing memory route;no fabricated status or generic device fallback.
Default sinks and LCD-only sinks remain strict. Enabled window access cost uses
existing external area2 ASTCR/WCR/WCER profile,including24-bit aliases. The query
must be pure,non-reentrant and cannot mutate owner state. No physical WAIT-pin
or complete bus timing fidelity claim is added.

An1xMegResearchSink derives from the LCD research owner and adds an owned MEG
write adapter in explicit observe_original_stubs policy. Both share validated
monotonic time and fail-stop callback state. Unknown MEG writes poison sink/bus;
strict memory-read failures poison bus/CPU without fabricating a sink callback.
Existing supported machine/bus reset retains external LCD/MEG state. A stopped
cold epoch requires full derived sink.cold_reset before machine.cold_restart;
MEG cold zero is project policy,not original hardware reset. No audio/PKS/MIDI
response is introduced. Original MEG storage/log-only contracts and schematic
lane evidence remain documented in meg-host-write-adapter.md;the same locked
MAME source706dbb73281320c54d7c2f16262c1c3786f2be01 is the reference.

CLI optional profile lcd-meg-observe-16mhz explicitly enables the owner and
reports meg_observed_controls. The counter describes logging-only control writes,
not completed commands or effects. Existing disabled/lcd-16mhz profiles do not
accept MEG. Counts omit no accepted control writes before a subsequent failure.
No state bytes,ROM code or user paths are added to public diagnostic JSON.

Independent tests cover256 slot/storage/control values,alias routing and area2
wait cost,external-state retention,strict invalid writes,read/word rejection,
LCD-only negative control,time reversal/cold recovery and129 CPU partitions.
Synthetic CLI checks one observed control in explicit mode,default rejection and
unknown status failure. Targeted strict ASan/UBSan passed. Component original
reference checks remain separate from this project-owned bus integration.

Read-only1.04 ROM SHA256:
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
100000000 requested cycles,20000000 callbacks,swap16,lcd-meg-observe-16mhz:
4096/1000000 slices both stop on data_read400005,width1,PC0xDA0C,
peripheral10814942,instruction_state26712/substate0,4 observed controls,
6316 GPIO notifications,0 TX/clock edges. Returned slice cycles10813440/10000000,
callbacks1812847/1683663;failed-slice work is not counted as returned work.
Hash unchanged before/after;no ROM bytes exported. firmware_boot_accepted false.
Next establish MEG offset2 status semantics;original zero-read stub alone does
not prove physical ready/handshake behavior. No full boot,audio or effects acceptance.
