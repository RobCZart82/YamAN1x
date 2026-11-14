# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött alap — 2026-11-14

A #283 PR pontos feje `3e0e96da1370cafc96cc5ba1c59d3bfd3fdab764`; a squash-beolvasztás főági commitja `2ff9f71c61ef8159a6bb815e76e3cd287b183635`, időpontja 2026-11-13 23:45:12 UTC. A Linux, macOS, Windows és MSan ellenőrzések a PR-fejen és a pontos főági commiton is sikeresek voltak. A MAME-kutatási ellenőrzés nem futott, mert a dokumentációs módosítás nem érintette az útvonal-szűrésben szereplő fájlokat.

A Codex a pontos PR-fejen nem talált hibát (`6867237322`); GitHub-review objektumok és review-szálak száma nulla. A `25910947` aktív szabályzat nulla jóváhagyást és lezárt szálakat követelt. Kvótahibát nem jeleztek és megkerülés nem történt. A #283 a #282 dokumentációs nyilvántartását egészítette ki; futási, hardver- vagy hangelfogadást nem módosított. Előző ellenőrzött alap: #282, `e718062c9989bca1d0cf4447df9b4c83ed5324b5`.

A #281-ben beolvasztott firmware-próba 500 millió ciklusra és 100 millió callbackre korlátozott. Négy friss futás elérte a cikluskorlátot hozzáférési hiba nélkül; a VOP3 feldolgozott hangmintái minden futásnál nullák, `audio_accepted` hamis. Ez korlátozott, konstans ADC-bemenetű szintetikus profil, nem teljes boot, hardveregyezés, MIDI vagy hangelfogadás. A helyi AN1x 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.

## Nyitott elfogadási pontok

- További firmware/MMIO/CPU-IRQ integráció és külön friss diagnosztika.
- A boot folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
