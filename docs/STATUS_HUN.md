# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött alap — 2026-11-13

A #281 PR beolvadt az `5cc1eba0cadc9abab98457ac824604419ff0f84f` főági commitba. A PR pontos fején és a főágon is mind az öt platform-, MSan- és MAME-ellenőrzés sikeres volt. A Codex automatikus review-eredmény nem jelent meg; GitHub-review és nyitott szál nem volt. A szabályzat nulla jóváhagyást követelt, kvótahibát nem jeleztek, megkerülés nem történt.

A forrásleltár 138 CTest-csoportot és 127 Python-esetet tartalmaz. A helyi firmware-próba 500 millió ciklusra és 100 millió callbackre korlátozott, csak olvas, és a nem támogatott MMIO-hozzáférésnél leáll. Négy legutóbbi futás elérte a cikluskorlátot hozzáférési hiba nélkül. A VOP3 hangminta-feldolgozás minden futásnál nulla maradt; `audio_accepted` hamis. Ez nem teljes boot és nem hangelfogadás.

## Nyitott elfogadási pontok

- További firmware/MMIO/CPU-IRQ integráció és külön friss diagnosztika.
- A boot folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
