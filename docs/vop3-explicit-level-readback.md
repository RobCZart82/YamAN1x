# Explicit experimental level readback

A shared Vop3LevelReadPolicy keeps default reject and separately names
experimental_an1x_level. Native owner chooses ABI profile1 once at construction;
invalid policy or configuration failure rejects/destructs safely. Host adapter
allows22 only with explicit policy and backend capability confirmation. Reg21
remains separate. No unknown read fallback,no clock advance or firmware profile
change. Default adapter rejects22 even on an experimental backend.
Independent integer oracle checks131072 signed-input/chip cases,all16 channels
on both chips,and496 invalid selected indices. Bad index poisons the adapter,
backend read counters remain unchanged. Strict configuration/mask/enum controls
and native configuration-failure destruction are tested. Existing timing tests
cover fractional evolution and target/aim discrepancy;physical conversion is
still experimental. Next:separately named bounded firmware experiment without
sample advancement,then distinct clock/upload/timing work. Original bridge and
pinned MAME reference unchanged;no ROM accessed or boot/audio acceptance.
Own remote CI/review pending.

