# YamAN1x — magyar összefoglaló

A YamAN1x a Yamaha AN1x hangszer firmware-alapú emulációját kutató projekt. A cél egy későbbi VST3/AU hangszerbővítmény. A jelenlegi kód kutatási alap: még nem ad AN1x-hangot, nem igazol teljes, hardverhű bootot, és nincs benne kész bővítmény vagy grafikus kezelőfelület.

A repository dokumentációja nyelvenként külön fájlban van: az angol dokumentumok nem tartalmaznak magyar prózát, a magyar összefoglalók pedig külön `_HUN.md` fájlokban találhatók. Az angol dokumentáció a kanonikus műszaki leírás. A gyökérben lévő `README.md` angolul tartalmazza a projekt bemutatását és állapotát. A részletes MAME-kutatási útmutató a [külön magyar fájlban](research/README_HUN.md) található. A korábbi magyar nyelvű dokumentumrészeket a [külön magyar archívum](docs/HUNGARIAN_NOTES_HUN.md) őrzi.

## Jelenlegi fejlesztési állapot — 2026-11-14

A #285 PR pontos feje `34555e7dac8ed50170900b1fdd122d4507e1a6da`; a squash-
beolvasztás főági commitja `266345d257d1098df137ffc253a197fa7a874822`,
időpontja 2026-11-14 02:49:42 UTC. A Linux, macOS, Windows és MSan
ellenőrzések sikeresek voltak mind a PR-fejen, mind a pontos főági commiton.
A MAME-kutatási ellenőrzés útvonal-szűrés miatt nem futott, mert a PR csak
dokumentációt érintett.

A Codex pontos PR-fejre szóló áttekintése nem talált hibát (`6869999662`);
GitHub-review objektumok és review-szálak száma nulla. A `25910947` szabályzat
nulla jóváhagyást és lezárt review-szálakat követelt. Kvótahibát nem jeleztek,
megkerülés nélkül történt a beolvasztás. A #285 a #284 beolvasztási
nyilvántartását frissítette; futási vagy hangelfogadási működést nem módosított.
Előző ellenőrzött alap: #284, `bc5626b983b246d8f12ce78c162001471e274075`.

## Korlátozott firmware-diagnosztikai jelölt — 2026-11-14

A következő jelölt az explicit próba véges korlátját 600 millió H8-ciklusra
és 125 millió callbackre emeli; a szeletmaximum 1 millió marad. Négy friss,
csak olvasó AN1x 1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletekkel)
elérte a ciklushatárt hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa
`95 706 795`, az AN0–AN3 scan kezdete `95 707 121`, az AN4–AN7 scané
`127 793 965`; ezek az időpontok a négy futásban egyeztek. A végső PC AN3=0
esetén `62010`, AN3=1023 esetén `1896`; a callback-darabszám szeletfüggő.
Mind a négy futásnál nulla VOP3-minta, `audio_accepted:false` és
`firmware_boot_accepted:false`. Ez szintetikus korlátozott profil, nem
hardver-, teljes boot-, MIDI-, hang- vagy REAPER-elfogadás. A ROM változatlan
és a repositoryn kívül maradt; a rögzített MAME forrás nem épült újra és nem
futott.

## Mi van még hátra?

1. A firmware és a H8 perifériák további, bizonyítékokkal alátámasztott integrációja, majd a boot előrehaladásának friss mérése.
2. A PKS-szerep és a MIDI note-on/note-off út külön igazolása.
3. A DSP envelope- és órajel/táblázat-kérdéseinek rendezése, majd időzített H8–VOP3 futtatás.
4. Egy rögzített hangszínnel ellenőrizhető első hang, reprodukálható WAV és összehasonlítás valódi AN1x-felvétellel.
5. Ezután következhet a motor, a minimális hosztadapter és a REAPER-próba.

A szintetikus tesztprofilokat és a teljes hardverelfogadást külön kell kezelni. Egy korlátos diagnosztika sikere nem jelent hardverhűséget vagy kész hangszert.

## ROM-mentes build és ellenőrzés

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

A tesztek szintetikus képeket használnak ideiglenes könyvtárakban. A helyi Yamaha-firmware-kép nem szükséges a buildhez vagy ehhez a tesztkészlethez. A magyar állapotösszefoglalót a [STATUS_HUN.md](docs/STATUS_HUN.md), a mérföldkövek rövid tervét a [FIRST_REAPER_SOUND_PLAN_HUN.md](docs/FIRST_REAPER_SOUND_PLAN_HUN.md) tartalmazza. A részletes műszaki napló az angol [STATUS](docs/STATUS.md), [első hang terv](docs/FIRST_REAPER_SOUND_PLAN.md) és [fejlesztési napló](docs/DEVELOPMENT_LOG.md) dokumentumokban olvasható.
