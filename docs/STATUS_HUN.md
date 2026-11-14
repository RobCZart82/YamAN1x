# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött alap — 2026-11-14

A #284 PR pontos feje `685a822901038c23168ce48e6f6de6ae0c0072d9`; a squash-
beolvasztás főági commitja `bc5626b983b246d8f12ce78c162001471e274075`,
időpontja 2026-11-14 01:29:44 UTC. A Linux, macOS, Windows és MSan
ellenőrzések mindkét pontos commiton sikeresek voltak (PR-fej jobok:
`132268621677`, `132268622285`, `132268622070`, `132268621268`; főág:
`132281035972`, `132281035835`, `132281035535`, `132281036016`). A MAME-kutatási
ellenőrzés dokumentációs módosítás miatt útvonal-szűrés alapján nem futott.

A Codex pontos PR-fejre szóló áttekintése nem talált hibát (`6868780442`);
GitHub-review objektum és szál nem volt. A `25910947` szabályzat nulla
jóváhagyást és lezárt review-szálakat követelt; kvótahibát nem jeleztek, és
megkerülés nélkül történt a beolvasztás. A #284 csak a #283 beolvasztási
nyilvántartását és az angol/magyar dokumentumok aktuális alapját egyeztette.
Az előző ellenőrzött alap a #283 `2ff9f71c61ef8159a6bb815e76e3cd287b183635`
commitja volt.
## Nyitott elfogadási pontok

- További firmware/MMIO/CPU-IRQ integráció és külön friss diagnosztika.
- A boot folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
