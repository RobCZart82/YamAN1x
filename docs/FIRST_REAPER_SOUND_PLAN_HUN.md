# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

## Ellenőrzött tervalap — #287 — 2026-11-14

A [#287 PR](https://github.com/RobCZart82/YamAN1x/pull/287) 2026-11-14
06:39:15 UTC-kor squash módszerrel olvadt be. A pontos PR-fej
`6ebf0262c056d2fecaf68216c419c3a0a2f3ee42`, az új főági commit
`dd988f6c9aa2fc147b9f0022f188202f722aa844`. A Linux, macOS, Windows és MSan
ellenőrzések sikeresek voltak a PR-fejen (ROM-mentes CI `43563617346`, MSan
`43563617365`) és a pontos főági commiton (`43567393627`, `43567393625`). A
MAME-kutatási munkafolyamat a dokumentációs változáson útvonalszűrés miatt nem
futott. A Codex pontos PR-fejre szóló áttekintése nem talált hibát
(`6872683772`); GitHub-review beküldés, soron belüli megjegyzés és nyitott
review-szál nem volt. A `25910947` szabályzat nulla jóváhagyást és lezárt
review-szálakat követelt. Kvótahibát nem jeleztek, megkerülés nélkül történt a
beolvasztás.

A #287 a #286 beolvadási adatait és az aktuális főági alapot egyeztette. A
forráskódos összeállítás 138 CTest-csoportot és 127 Python-esetet tartalmaz. A
teljes indulás, hardveregyezés, MIDI, hang és REAPER-elfogadás továbbra sincs
igazolva. A ROM változatlan és a repositoryn kívül maradt. A MAME forrás rögzített
commitja `706dbb73281320c54d7c2f16262c1c3786f2be01`.

## PKS-bemeneti időablakok — helyi jelölt — 2026-11-14

A csak olvasó megfigyelő a korábban látott címek mellett a statikusan
azonosított, igazított `2349EA` szót is rögzíti. A rögzített MAME-forrással
két friss, kilenc másodperces, passzív alapbeállítású próba készült; mindkettő
hat `39AC` forrás-pufferírást fogott meg. A `002A08` utólagos programszámláló-
értéknél olvasott `2349EA` adat `4000/FF00` volt (kijelölt felső bájt: `40`);
az első három írást 12 625, 20 125 és 27 625 ns-mal előzte meg. Az írt bájtok
`E0 00 20` voltak. A `001FD8` utólagos értéknél olvasott `237204` igazított szó
adata `FF00/00FF` volt (kijelölt alsó bájt: `00`); a következő három írást
10 375, 17 875 és 25 375 ns-mal előzte meg. Az írt bájtok `E0 00 00` voltak.
A CCR-pillanatképek nyers adatként maradnak meg, értelmezés nélkül. A két futás
26 nem trace-jellegű mezője egyezett. Az időbeli közelség nem bizonyít okozati
kapcsolatot vagy adatjelentést; hardveregyezés, teljes indulás, MIDI, DSP-minta,
hang és REAPER-elfogadás sincs igazolva. A ROM változatlan maradt, és a Git-en
kívül van. A következő jelölt lépés a korábbi utasítás/adatérték út követése,
független cím-, érték-, bájt-lane-, flag-, időzítési és folytathatósági
ellenőrzéssel.

## Beolvadt korlátozott firmware-diagnosztika — #286 — 2026-11-14

Négy friss, csak olvasó AN1x 1.04 futás 600 millió ciklusig jutott hozzáférési
hiba nélkül; a profil 125 millió callbacknél és legfeljebb 1 millió ciklusos
szeleteknél megáll. AN3=0 és 1023 mellett, 4096 és 1 millió ciklusos
szeletekkel az első ADC- és pásztázási határidők egyeztek. A futások végén nulla
VOP3-minta volt, a teljes indulás és a hangelfogadás eredménye hamis maradt. Ez korlátozott,
szintetikus megfigyelés; nem teljes hardver- vagy hangelfogadás.

## Mérföldkövek

1. **Firmware és perifériák:** kis, bizonyítékhoz kötött H8, busz, MMIO és IRQ lépések; minden elfogadott profil után új firmware-futtatás. A konstans bemenetű szintetikus diagnosztika nem hardverkalibráció.
2. **Boot, PKS és MIDI:** a firmware további indulása, a PKS szerepe, majd a note-on/note-off üzenetek vétele külön-külön bizonyítandó.
3. **DSP:** az EG célérték-megtartása, az órajel és a hullámtáblák formátuma/címzése ellenőrzendő. Ezután közös időalapon kell léptetni a H8-at és a két VOP3 chipet.
4. **Első száraz hang:** rögzített patch, note-on, tartás és elengedés; dokumentált kétchipes jelút és kimeneti skála; reprodukálható WAV, majd összehasonlítás a hangszerből rögzített referenciával.
5. **Hosztintegráció:** csak a jelút igazolása után jöhet a motor, MIDI-időzítés és minimális VST3, majd a REAPER-ben lejátszott első hang.

A REAPER-elfogadás az utolsó lépés. Részprofil, teljes hardverhű indulás, MIDI, offline hang és REAPER-hang nem vonható össze egyetlen sikerállítássá.
