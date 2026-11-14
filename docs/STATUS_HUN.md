# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött alap — 2026-11-14

A #285 PR pontos feje `34555e7dac8ed50170900b1fdd122d4507e1a6da`; a squash-
beolvasztás főági commitja `266345d257d1098df137ffc253a197fa7a874822`,
időpontja 2026-11-14 02:49:42 UTC. A Linux, macOS, Windows és MSan
ellenőrzések mindkét pontos commiton sikeresek voltak (PR-fej jobok:
`132294248573`, `132294248680`, `132294248311`, `132294248304`; főág:
`132305010072`, `132305009831`, `132305010191`, `132305010038`). A MAME-kutatási
ellenőrzés dokumentációs módosítás miatt útvonal-szűrés alapján nem futott.

A Codex pontos PR-fejre szóló áttekintése nem talált hibát (`6869999662`);
GitHub-review objektum és szál nem volt. A `25910947` szabályzat nulla
jóváhagyást és lezárt review-szálakat követelt; kvótahibát nem jeleztek, és
megkerülés nélkül történt a beolvasztás. A #285 csak a #284 beolvasztási
nyilvántartását egyeztette. Az előző ellenőrzött alap a #284
`bc5626b983b246d8f12ce78c162001471e274075` commitja volt.

## Korlátozott firmware-diagnosztikai jelölt — 2026-11-14

A következő jelölt a véges próbakeretet 600 millió H8-ciklusra és 125 millió
callbackre emeli; a szeletmaximum 1 millió marad. Négy friss, csak olvasó AN1x
1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletek) elérte a ciklushatárt
hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa `95 706 795`, az AN0–AN3
scan kezdete `95 707 121`, az AN4–AN7 scané `127 793 965`; az időpontok
mind a négy futásban egyeztek. A végső PC AN3=0 esetén `62010`, AN3=1023
esetén `1896`; a callback-darabszám szeletfüggő. Mindegyik futásnál nulla
VOP3-minta, `audio_accepted:false` és `firmware_boot_accepted:false`. Ez
szintetikus korlátozott profil, nem teljes hardver-, boot-, MIDI-, hang- vagy
REAPER-elfogadás. A ROM változatlan és a repositoryn kívül maradt.

## Nyitott elfogadási pontok

- További firmware/MMIO/CPU-IRQ integráció és külön friss diagnosztika.
- A boot folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
