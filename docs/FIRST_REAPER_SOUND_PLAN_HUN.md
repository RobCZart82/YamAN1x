# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

## Ellenőrzött tervalap — #286 — 2026-11-14

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
