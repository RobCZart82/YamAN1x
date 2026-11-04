# Isolated ITU internal-clock counter

New H8ItuCounter owns monotonic absolute time,count,enable,divider/phase and
explicit cycle1..65536. It settles old time before enable/count/config changes;
invalid profile/time rejects without mutation. Disabled time does not accumulate
later catch-up. Internal shifts0..3,zero/half phase,wrap and starts beyond new
clear boundary supported. No flags,IRQ,deadlines,MMIO or firmware start enabled.
Independent per-tick oracle covers2097152 initial-count/profile cases;258 splits,
stop/restart/reconfigure,invalid retention and uint64 time extremes tested.
Tick quotient avoids now+phase overflow;counter arithmetic avoids count+delta
overflow. These large-time extensions are independently checked,not claimed
original-method comparisons at overflowing source arithmetic.
131072 value comparisons with the unchanged locked update_counter PASS under
ASan/UBSan in a temporary harness. TGR count0/IER0 deliberately excludes flag/
IRQ behavior;cycle and clock configured directly. Original source never copied
into repository. Next:compare/overflow flags and disabled-IRQ polling tests,
then bounded deadlines/dispatch before bus integration. No ROM/audio acceptance.
Own CI/review pending.

Reproduce original value comparison:

```sh
python3 tools/check_itu_counter_reference.py --archive /path/to/mame-master.zip
```

Original references:research/h83002-active-itu-contract.json and
research/mame-source-lock.json;one unchanged update_counter method extracted
only from a private hash-verified snapshot. No active timer/hardware acceptance.
