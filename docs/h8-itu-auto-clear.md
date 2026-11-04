# Internal-clock ITU auto-clear events

New H8ItuAutoClear composes the existing counter/status for A/B-selected auto-
clear. Cycle is selected TGR+1 in32 bits;comparison target is wrapped16-bit
TGR+1. Tick distances capture A/B matches and overflow with IRQ disabled,
including starts beyond clear boundary and TGRFFFF. Mutations settle old time;
invalid configuration/index/time preserves state. No external clear/capture,
deadline,IRQ delivery or MMIO start. Existing free-running component unchanged.
1048576 independent initial/cycle/target tick cases,4128 A/B-source/phase/splits,
polling/reconfigure/FFFF/uint64 tests.131072 comparisons with locked unchanged
update_counter pass for values/flags,IER0;original source only in temporary
private harness. Initial fixture assumed immediate clear after lowering threshold
below current count;fixed to check continued count until full wrap,then explicit
counter rewrite for the immediate-clear test. No implementation workaround.
Next bounded deadlines/IRQ dispatch and selected MMIO integration;no firmware
boot or audio acceptance. Original MAME reference unchanged. Own CI/review pending.

Reproduce original value/flag differential:

```sh
python3 tools/check_itu_auto_clear_reference.py --archive /path/to/mame-master.zip
```

Only internal clock,A/B clear,incrementing counters;no IRQ delivery/hardware proof.
