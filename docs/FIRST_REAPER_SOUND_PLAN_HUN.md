# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

## Ellenőrzött dokumentációs alap — 2026-11-14

A #283 dokumentációs egyeztetés a `3e0e96da1370cafc96cc5ba1c59d3bfd3fdab764` PR-fejről a `2ff9f71c61ef8159a6bb815e76e3cd287b183635` főági commitba olvadt be. A Linux, macOS, Windows és MSan ellenőrzések mindkét commiton sikeresek voltak; a MAME-kutatási ellenőrzés nem futott az útvonal-szűrés miatt. A Codex a pontos PR-fejet hiba nélkül áttekintette (`6867237322`); nem volt GitHub-review objektum vagy review-szál, a szabályzat nulla jóváhagyást és lezárt szálakat követelt. A módosítás csak a nyilvántartásokat egyeztette; a fejlesztési terv és a hangelfogadás nem változott.

## Mérföldkövek

1. **Firmware és perifériák:** kis, bizonyítékhoz kötött H8, busz, MMIO és IRQ lépések; minden elfogadott profil után új firmware-futtatás. A konstans bemenetű szintetikus diagnosztika nem hardverkalibráció.
2. **Boot, PKS és MIDI:** a firmware további indulása, a PKS szerepe, majd a note-on/note-off üzenetek vétele külön-külön bizonyítandó.
3. **DSP:** az EG célérték-megtartása, az órajel és a hullámtáblák formátuma/címzése ellenőrzendő. Ezután közös időalapon kell léptetni a H8-at és a két VOP3 chipet.
4. **Első száraz hang:** rögzített patch, note-on, tartás és elengedés; dokumentált kétchipes jelút és kimeneti skála; reprodukálható WAV, majd összehasonlítás a hangszerből rögzített referenciával.
5. **Hosztintegráció:** csak a jelút igazolása után jöhet a motor, MIDI-időzítés és minimális VST3, majd a REAPER-ben lejátszott első hang.

A REAPER-elfogadás az utolsó lépés. Részprofil, teljes hardverhű boot, MIDI, offline hang és REAPER-hang nem vonható össze egyetlen sikerállítássá.
