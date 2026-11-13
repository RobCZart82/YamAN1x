# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött alap — 2026-11-13

A #282 PR a `d87d70c00b732e7161a720ec7104f3bc06eb9c07` pontos fejéről beolvadt az `e718062c9989bca1d0cf4447df9b4c83ed5324b5` főági commitba. A PR-fej és a pontos főági commit mind az öt ellenőrzése sikeres volt: Linux, macOS, Windows, MSan és MAME-kutatási build.

A Codex korábbi PR-fejen egy P2 dokumentációs hibát jelzett; a hash-értékek és Mac-parancsblokkok javítása után a review-szálat lezárták. Új, végleges-fejre vonatkozó Codex-review összefoglaló nem jelent meg. A `25910947` aktív szabályzat nulla jóváhagyást és lezárt review-szálakat követelt. Kvótahibát nem jeleztek, a beolvasztás megkerülés nélkül történt. A dokumentáció nyelvi szétválasztása nem változtatott futási vagy hangelfogadási működést.

A korábbi ellenőrzött alap a #281 `5cc1eba0cadc9abab98457ac824604419ff0f84f` commitja volt.

A forrásleltár 138 CTest-csoportot és 127 Python-esetet tartalmaz. A helyi firmware-próba 500 millió ciklusra és 100 millió callbackre korlátozott, csak olvas, és a nem támogatott MMIO-hozzáférésnél leáll. Négy legutóbbi futás elérte a cikluskorlátot hozzáférési hiba nélkül. A VOP3 hangminta-feldolgozás minden futásnál nulla maradt; `audio_accepted` hamis. Ez nem teljes boot és nem hangelfogadás.

## Nyitott elfogadási pontok

- További firmware/MMIO/CPU-IRQ integráció és külön friss diagnosztika.
- A boot folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
