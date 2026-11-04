# Isolated H8H ITU flag latch

New H8ItuStatus explicitly latches compareA/compareB/overflow events independent
of enables. Read status/enables normalize reserved bitsF8;status write zero
clears and one preserves. pending() reports only flags&enables eligibility,
not interrupt-controller pending state or callback delivery. Reset clears low
flags/enables and exposes normalizedF8:component reset policy,not whole original
raw-register/hardware reset equivalence. Invalid event bits reject unchanged.
524288 independent event/enable/clear cases plus cumulative disabled-latch,
reenable/reset/invalid controls PASS. Another524288 comparisons use three
unchanged original H8H methods in a temporary ASan/UBSan harness. Original ISR
events are supplied explicitly;this does not prove counter compare detection,
polling-driven event generation,deadline or IRQ delivery. Next:counter-to-status
event detection with independent boundary/polling tests,then deadlines/dispatch
before MMIO start. No ROM,firmware or audio acceptance. Own CI/review pending.

Reproduce original status-method comparison:

```sh
python3 tools/check_itu_status_reference.py --archive /path/to/mame-master.zip
```

Uses the locked private archive snapshot;no original method source is saved
in the repository. Existing counter remains a separate value-only component.
