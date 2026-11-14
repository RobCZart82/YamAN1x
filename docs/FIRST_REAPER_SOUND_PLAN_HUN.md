# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

## Ellenőrzött dokumentációs alap — 2026-11-14

A #284 dokumentációs egyeztetés a `685a822901038c23168ce48e6f6de6ae0c0072d9`
PR-fejről a `bc5626b983b246d8f12ce78c162001471e274075` főági commitba olvadt be
2026-11-14 01:29:44 UTC-kor. A Linux, macOS, Windows és MSan ellenőrzések
mindkét pontos commiton sikeresek voltak; a MAME-kutatási ellenőrzés nem futott
az útvonal-szűrés miatt. A Codex a pontos PR-fejet hiba nélkül áttekintette
(`6868780442`); GitHub-review objektum vagy szál nem volt, a szabályzat nulla
jóváhagyást és lezárt szálakat követelt. A módosítás csak a nyilvántartásokat
egyeztette; a fejlesztési terv és a hangelfogadás nem változott.
## Mérföldkövek

1. **Firmware és perifériák:** kis, bizonyítékhoz kötött H8, busz, MMIO és IRQ lépések; minden elfogadott profil után új firmware-futtatás. A konstans bemenetű szintetikus diagnosztika nem hardverkalibráció.
2. **Boot, PKS és MIDI:** a firmware további indulása, a PKS szerepe, majd a note-on/note-off üzenetek vétele külön-külön bizonyítandó.
3. **DSP:** az EG célérték-megtartása, az órajel és a hullámtáblák formátuma/címzése ellenőrzendő. Ezután közös időalapon kell léptetni a H8-at és a két VOP3 chipet.
4. **Első száraz hang:** rögzített patch, note-on, tartás és elengedés; dokumentált kétchipes jelút és kimeneti skála; reprodukálható WAV, majd összehasonlítás a hangszerből rögzített referenciával.
5. **Hosztintegráció:** csak a jelút igazolása után jöhet a motor, MIDI-időzítés és minimális VST3, majd a REAPER-ben lejátszott első hang.

A REAPER-elfogadás az utolsó lépés. Részprofil, teljes hardverhű boot, MIDI, offline hang és REAPER-hang nem vonható össze egyetlen sikerállítássá.
