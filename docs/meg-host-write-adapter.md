# MEG canonical host write adapter

Source:owner-supplied AN1x-ServiceManual.pdf,PDF page51,AN1x CIRCUIT DIAGRAM
(DM,DMS),IC10 MEG and IC6 decoder. Local PDF SHA256:
d31d48f74f8bded246b1f75f5c04b28b4c4bab375ab4d50a22ce248d277a9167.
Visually inspected complete page plus enlarged original render crops. No manual
pages or images copied into the repository. IC10 A0..A4 pins49..53 carry CPU
A1..A5 nets;D0..D7 pins54..61 carry D0..D7;WR48=LWR,RD47=RD,CS46=MEGCS.
IC6 74AC139 1A/1B use A18/A19 and1Y0/1Y1/1Y2 name MEGCS/V31CS/V32CS.
The complete select-enable/mirror decode is not claimed. Existing firmware and
schematic evidence identify the400000 effect-interface region,not all aliases.

H8 lower-byte/big-endian mapping gives canonical byte address400001+2*offset.
MEG five address bits permit offsets0..1F;this deliberately narrow adapter only
decodes0..18 through400031,matching the original MEG host register map's extent.
It accepts no even byte lane,word operation,CPU24-bit alias or wide chip-select
mirror. CPU-bus alias normalization remains a future owner responsibility.
400007 corresponds to offset3,whose original handler is logging-only.

MegHostWriteAdapter is standalone and owns the previous register storage. It
routes implemented storage writes and defaults to rejecting unknown control
effects. Explicit ControlPolicy::observe_original_stubs accepts only original
logging-only controls1/2/3/7/A/15 as counted offset/value observations. It changes
no register storage,returns no ready/status and executes no DSP/effect action.
All other holes/read-only/unsupported offsets reject before state/observation
mutation. Retaining reset preserves storage and observation history;no hardware
reset or command timing claim follows. It is not connected to the machine/probe;
the actual firmware still stops at400007 in the default strict research machine.

Pinned original MAME706dbb73281320c54d7c2f16262c1c3786f2be01 meg.cpp map,
storage and six logging-only handlers inspected via verified archive. Original
AN1x driver still lacks this host mapping. Expanded local reference checker
extracts21 unchanged methods into a temporary sanitized harness:720896 storage
comparisons and1536 control-storage invariance checks pass from matching initial
state. No original implementation copied into repo;no command effects or physical
bus equivalence asserted. Tests use independent host addresses,window/lane oracle,
1536 control values,65536 storage pairs and strict holes/mirror/reset controls.
Next connect only an explicitly named observational research profile to the bus,
with input/status accesses strict until their contracts are independently known.
