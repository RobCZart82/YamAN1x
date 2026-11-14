# YamAN1x — magyar összefoglaló

A YamAN1x a Yamaha AN1x hangszer firmware-alapú emulációját kutató projekt. A cél egy későbbi VST3/AU hangszerbővítmény. A jelenlegi kód kutatási alap: még nem ad AN1x-hangot, nem igazol teljes, hardverhű bootot, és nincs benne kész bővítmény vagy grafikus kezelőfelület.

A az adattár dokumentációja nyelvenként külön fájlban van: az angol dokumentumok nem tartalmaznak magyar prózát, a magyar összefoglalók pedig külön `_HUN.md` fájlokban találhatók. Az angol dokumentáció a kanonikus műszaki leírás. A gyökérben lévő `README.md` angolul tartalmazza a projekt bemutatását és állapotát. A részletes MAME-kutatási útmutató a [külön magyar fájlban](research/README_HUN.md) található. A korábbi magyar nyelvű dokumentumrészeket a [külön magyar archívum](docs/HUNGARIAN_NOTES_HUN.md) őrzi.

## Jelenlegi ellenőrzött főág — #286 — 2026-11-14

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

A #286 PR az explicit próba véges korlátját 600 millió H8-ciklusra
és 125 millió visszahívásra emelte; a szeletmaximum 1 millió marad. Négy friss,
csak olvasó AN1x 1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletekkel)
elérte a ciklushatárt hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa
`95 706 795`, az AN0–AN3 pásztázás kezdete `95 707 121`, az AN4–AN7 pásztázásé
`127 793 965`; ezek az időpontok a négy futásban egyeztek. A végső PC AN3=0
esetén `62010`, AN3=1023 esetén `1896`; a visszahívások száma szeletfüggő.
Mind a négy futásnál nulla VOP3-minta, `audio_accepted:false` és
`firmware_boot_accepted:false`. Ez szintetikus korlátozott profil, nem
hardver-, teljes indulási , MIDI-, hang- vagy REAPER-elfogadás. A ROM változatlan
és a repositoryn kívül maradt; a rögzített MAME forrás nem épült újra és nem
futott.

## Mi van még hátra?

1. A firmware és a H8 perifériák további, bizonyítékokkal alátámasztott integrációja, majd az indulás előrehaladásának friss mérése.
2. A PKS-szerep és a MIDI note-on/note-off út külön igazolása; ezután következhet a firmware további indulási útjának vizsgálata.
3. A DSP envelope- és órajel/táblázat-kérdéseinek rendezése, majd időzített H8–VOP3 futtatás.
4. Egy rögzített hangszínnel ellenőrizhető első hang, reprodukálható WAV és összehasonlítás valódi AN1x-felvétellel.
5. Ezután következhet a motor, a minimális hosztadapter és a REAPER-próba.

A szintetikus tesztprofilokat és a teljes hardverelfogadást külön kell kezelni. Egy korlátos diagnosztika sikere nem jelent hardverhűséget vagy kész hangszert.

## ROM-mentes összeállítás és ellenőrzés

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

A tesztek szintetikus képeket használnak ideiglenes könyvtárakban. A helyi Yamaha-firmware-kép nem szükséges a buildhez vagy ehhez a tesztkészlethez. A magyar állapotösszefoglalót a [STATUS_HUN.md](docs/STATUS_HUN.md), a mérföldkövek rövid tervét a [FIRST_REAPER_SOUND_PLAN_HUN.md](docs/FIRST_REAPER_SOUND_PLAN_HUN.md) tartalmazza. A részletes műszaki napló az angol [STATUS](docs/STATUS.md), [első hang terv](docs/FIRST_REAPER_SOUND_PLAN.md) és [fejlesztési napló](docs/DEVELOPMENT_LOG.md) dokumentumokban olvasható.
