# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött főági alap — #286 — 2026-11-14

A [#286 PR](https://github.com/RobCZart82/YamAN1x/pull/286) 2026-11-14
04:59:45 UTC-kor squash módszerrel olvadt be. A pontos PR-fej
`e4275149ab59aa16ef248dd093a57619374b913c`, az új főági commit
`23a9ebd14cdd94f5caa0d2c7c2d8e50b264cf793`. A Linux, macOS, Windows, MSan és
MAME-kutatási ellenőrzések mind a PR-fejen, mind a pontos főági commiton
sikeresek voltak (PR-fej jobok: `132327910312`, `132327910121`, `132327910358`,
`132327911265`, `132327910017`; főági jobok: `132339274917`, `132339274797`,
`132339274953`, `132339274742`, `132339274608`). A Codex pontos PR-fejre
szóló áttekintése nem talált hibát (`6871541719`); GitHub-review objektum és
soron belüli megjegyzés nem volt. A `25910947` szabályzat nulla jóváhagyást és
lezárt review-szálakat követelt. Kvótahibát nem jeleztek, megkerülés nélkül
történt a beolvasztás.

A korlátozott firmware-próba felső határa 600 millió ciklus és 125 millió
visszahívás; egy szelet legfeljebb 1 millió ciklus. Négy friss, csak olvasó AN1x
1.04 futás hozzáférési hiba nélkül érte el a ciklushatárt. Ez szintetikus
részprofil, nem teljes indulási , hardver-, MIDI-, hang- vagy REAPER-elfogadás. A
változatlan ROM a repositoryn kívül maradt; a MAME forrás rögzített commitja
`706dbb73281320c54d7c2f16262c1c3786f2be01`; ehhez a lépéshez új helyi MAME
összeállítás/futtatás nem készült.

## Beolvadt korlátozott firmware-diagnosztika — #286 — 2026-11-14

A #286 PR a véges próbakeretet 600 millió H8-ciklusra és 125 millió
visszahívásra emelte; a szeletmaximum 1 millió marad. Négy friss, csak olvasó AN1x
1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletek) elérte a ciklushatárt
hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa `95 706 795`, az AN0–AN3
pásztázás kezdete `95 707 121`, az AN4–AN7 pásztázásé `127 793 965`; az időpontok
mind a négy futásban egyeztek. A végső PC AN3=0 esetén `62010`, AN3=1023
esetén `1896`; a visszahívások száma szeletfüggő. Mindegyik futásnál nulla
VOP3-minta, `audio_accepted:false` és `firmware_boot_accepted:false`. Ez
szintetikus korlátozott profil, nem teljes hardver-, indulási , MIDI-, hang- vagy
REAPER-elfogadás. A ROM változatlan és a repositoryn kívül maradt.

## Nyitott elfogadási pontok

- A 39A4 rutinba írt értékek forrásának és hívóinak követése, független érték-, cím-, flag-, időzítési és folytathatósági ellenőrzéssel.
- Az indulás folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
