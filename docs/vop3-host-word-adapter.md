# VOP3 canonical full-word host adapter

Vop3HostWordAdapter<Backend> is a standalone project-owned routing layer. It
reuses the existing UploadControlContract and the bridge-style backend signature
write(chip,reg,value,mask)/read(chip,reg,mask,value_ref),return0 on success.
Backend lifetime exceeds adapter;copy is disabled. It does not allocate a VOP3
bridge,connect the CPU bus,run samples,load table ROM or provide audio.

Canonical windows440000..4400FF and480000..4800FF map aligned full words to
chip0/1 and register(address-base)/2. Out-of-range/odd/partial-mask requests
reject before backend calls. No24-bit alias or word-to-byte fallback;future bus
owner normalizes addresses. Caller owns MMIO time ordering;adapter itself has no
clock. Backend callback failure/nonzero result or reentry poisons the adapter;
subsequent access rejects. Invalid caller requests do not poison it.

Upload control register1 is disabled by default. Explicit policy
experimental_firmware_contract uses the already documented logical stopped/
released status bit15 with0/1 writes only,per chip. Writes forward to backend;
control state commits after successful callback. The contract's other zero bits
and acknowledgement timing are assumptions,not measured YSS236 behavior.
Reads of register1 bypass backend only in that explicit policy. Other reads
permit only model EG_STATUS21. In particular22 is not aliased to21 and does not
silently use the bridge's zero fallback. Writes elsewhere are forwarding only;
backend acceptance is not proof that every VOP3 register is fully implemented.

Reference context:established research/mame-live-register-bridge.patch applied
to locked MAME706dbb73281320c54d7c2f16262c1c3786f2be01 routes the same two
windows and overlays the same experimental upload control while forwarding
writes. Existing VOP3 C ABI2 enforces chip/reg/full masks but returns zeros on
unsupported reads;the new adapter intentionally exposes a stricter read surface.
This is project-owned wiring,not a new original-MAME runtime differential.
See live-register-milestone.md for earlier independent upload-state/trace evidence
and unimplemented registers/timing limitations. No upstream bodies are modified.

Independent synthetic backend tests sweep65024 chip/register/value combinations,
separate control state,bit15/release/invalid controls,all boundary classes,partial
masks,unknown-read rejection,backend failure and swallowed reentry detection.
No actual native bridge/firmware acceptance is claimed. Next connect the native
backend and explicit research sink with lifecycle/fail-stop/partition tests.
Full firmware boot,physical MEG status,DMS board selection,table/audio routing,
realtime engine and REAPER remain open.

## Explicit level-read policy candidate

The default above stays strict. A separately selected experimental_an1x_level
policy may expose22 only when the backend confirms lifetime-scoped ABI profile1.
This is independent of upload control and never aliases21. See
[vop3-explicit-level-readback.md](vop3-explicit-level-readback.md). No firmware
probe or model clock is enabled by selecting the standalone adapter policy.
