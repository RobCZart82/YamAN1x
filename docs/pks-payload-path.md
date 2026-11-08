# Bounded PKS payload path observations

Merged in #252; original observation base 0643ac56075ca0c518b8ba2f4f7c93d42036b322.

Four read-only local 1.04 probes extended the AN2-only and AN6-only high
scan profiles (initial single AN3=1023), with the same 130-million-cycle
horizon and slices 4,096/1,000,000. A temporary observer retained the last
128 instruction-word fetch addresses and emitted them at observed buffer
byte writes. All baseline JSON bytes remained identical and same-input
trace logs matched across slices. ROM hash before/after remained
4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.

Immediately before entering the common FAB0 fetch sequence ending at FAC6,
the observed predecessor fetches were:

| Profile / output bytes | Predecessor fetches |
| --- | --- |
| AN2 D0 7F | 59C1A, 59C26 |
| AN2 E0 00 20, then E0 00 00 | 59A06, 59A12, 59A1E, repeated |
| AN6 E0 3E 5F, then E0 7E 7F | 59A06, 59A12, 59A1E, repeated |

This separates the observed D0 and E0 execution paths before their common
buffer writer. It does not establish call opcodes, instruction-start PCs,
command meanings, complete stack traces or a payload formula. Fetches may
include operand words. Only addresses and derived observations are recorded;
no ROM contents are included.

Previously verified stages: ADC shared result FFFFE4, input-dependent RAM
words, byte writes starting 2360CC at last fetch FAC6, buffer reads at 18B0,
and matching TDR1 writes five cycles later. These stages do not yet prove
every dependency between ADC values and payload encoding.

Next: observe input data reads around 59Cxx and 59Axx and connect them to
the ADC intermediates. No behavioral PKS response is justified yet.
Original MAME reference remains locked at
706dbb73281320c54d7c2f16262c1c3786f2be01. No hardware comparison was run.
VOP3 samples remain zero; full boot, MIDI and audio are unaccepted.

## Staging byte reads and writes — candidate, 2026-11-08

Eight further read-only probes used the same AN2/AN6 profiles and two slice
sizes. Four observed producer data reads in last-fetch range 59900–59CFF;
four added byte-write observation at 23FB0A–23FB0C. Both observer stages
preserved every previous JSON byte. Same-input logs matched across slices.
ROM hash remained unchanged at the value above.

D0-path reads were 23FB0A at last fetch 59C12 and 23FB0C at 59C22.
E0-path reads were 23FB0A/0B/0C at 599FE/59A0E/59A1A. All eight AN2
and six AN6 staging reads matched the preceding observed byte write.
The staging bytes reproduce the already documented outgoing sequences.

Initial staging writes used last fetches 5A2A0, 5A2AA, 5A2B4 for offsets
0,1,2. In the AN2 D0 case, D0 7F 00 was subsequently rewritten to
D0 00 7F at last fetches 5A2CE, 5A2D6, 5A2E0 before the D0-path reads.
This is observed staging behavior; no general byte-reordering rule or
protocol interpretation is accepted from this case alone.

The observed chain now reaches staging writes, producer reads, queue-byte
writes, queue reads and TDR1 writes. The ADC-to-staging calculation remains
unproven. Next: observe data inputs to 5A2xx and connect them to ADC RAM
intermediates, without injecting replies. Boot, MIDI and audio remain open.
