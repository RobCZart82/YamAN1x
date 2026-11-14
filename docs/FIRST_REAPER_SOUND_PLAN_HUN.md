# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

## Ellenőrzött dokumentációs alap — 2026-11-14

A #285 dokumentációs egyeztetés a `34555e7dac8ed50170900b1fdd122d4507e1a6da`
PR-fejről a `266345d257d1098df137ffc253a197fa7a874822` főági commitba olvadt be
2026-11-14 02:49:42 UTC-kor. A Linux, macOS, Windows és MSan ellenőrzések
mindkét pontos commiton sikeresek voltak; a MAME-kutatási ellenőrzés nem futott
az útvonal-szűrés miatt. A Codex a pontos PR-fejet hiba nélkül áttekintette
(`6869999662`); GitHub-review objektum vagy szál nem volt, a szabályzat nulla
jóváhagyást és lezárt szálakat követelt. A módosítás csak a nyilvántartásokat
egyeztette; a fejlesztési terv és a hangelfogadás nem változott.

## Következő korlátozott firmware-diagnosztikai jelölt — 2026-11-14

Négy friss, csak olvasó AN1x 1.04 futás 600 millió ciklusig jutott hozzáférési
hiba nélkül; a profil 125 millió callbacknél és legfeljebb 1 millió ciklusos
szeleteknél megáll. AN3=0 és 1023 mellett, 4096 és 1 millió ciklusos
szeletekkel az első ADC- és scan-határidők egyeztek. A futások végén nulla
VOP3-minta volt, a boot és audio elfogadása hamis maradt. Ez korlátozott,
szintetikus megfigyelés; nem teljes hardver- vagy hangelfogadás.

## Mérföldkövek

1. **Firmware és perifériák:** kis, bizonyítékhoz kötött H8, busz, MMIO és IRQ lépések; minden elfogadott profil után új firmware-futtatás. A konstans bemenetű szintetikus diagnosztika nem hardverkalibráció.
2. **Boot, PKS és MIDI:** a firmware további indulása, a PKS szerepe, majd a note-on/note-off üzenetek vétele külön-külön bizonyítandó.
3. **DSP:** az EG célérték-megtartása, az órajel és a hullámtáblák formátuma/címzése ellenőrzendő. Ezután közös időalapon kell léptetni a H8-at és a két VOP3 chipet.
4. **Első száraz hang:** rögzített patch, note-on, tartás és elengedés; dokumentált kétchipes jelút és kimeneti skála; reprodukálható WAV, majd összehasonlítás a hangszerből rögzített referenciával.
5. **Hosztintegráció:** csak a jelút igazolása után jöhet a motor, MIDI-időzítés és minimális VST3, majd a REAPER-ben lejátszott első hang.

A REAPER-elfogadás az utolsó lépés. Részprofil, teljes hardverhű boot, MIDI, offline hang és REAPER-hang nem vonható össze egyetlen sikerállítássá.
