# YamAN1x — magyar összefoglaló

A YamAN1x a Yamaha AN1x hangszer firmware-alapú emulációját kutató projekt. A cél egy későbbi VST3/AU hangszerbővítmény. A jelenlegi kód kutatási alap: még nem ad AN1x-hangot, nem igazol teljes, hardverhű bootot, és nincs benne kész bővítmény vagy grafikus kezelőfelület.

A repository dokumentációja nyelvenként külön fájlban van: az angol dokumentumok nem tartalmaznak magyar prózát, a magyar összefoglalók pedig külön `_HUN.md` fájlokban találhatók. Az angol dokumentáció a kanonikus műszaki leírás. A gyökérben lévő `README.md` angolul tartalmazza a projekt bemutatását és állapotát. A részletes MAME-kutatási útmutató a [külön magyar fájlban](research/README_HUN.md) található. A korábbi magyar nyelvű dokumentumrészeket a [külön magyar archívum](docs/HUNGARIAN_NOTES_HUN.md) őrzi.

## Jelenlegi fejlesztési állapot — 2026-11-14

A #283 PR pontos feje `3e0e96da1370cafc96cc5ba1c59d3bfd3fdab764`; a squash-beolvasztás főági commitja `2ff9f71c61ef8159a6bb815e76e3cd287b183635`, időpontja 2026-11-13 23:45:12 UTC. A Linux, macOS, Windows és MSan ellenőrzések sikeresek voltak mind a PR-fejen, mind a pontos főági commiton. A MAME-kutatási workflow útvonal-szűrés miatt nem futott, mert a csak dokumentációt módosító változás nem érintette a figyelt fájlokat.

A Codex pontos PR-fejre szóló áttekintése nem talált hibát (megjegyzés: `6867237322`); GitHub-review objektum és review-szál nem volt. A `25910947` szabályzat nulla jóváhagyást és lezárt review-szálakat követelt; kvótahibát nem jeleztek, megkerülés nélkül történt a beolvasztás. A #283 a #282 dokumentációs eredményeit egyeztette; futási vagy hangelfogadási működést nem módosított. Az előző ellenőrzött főági állapot az `e718062c9989bca1d0cf4447df9b4c83ed5324b5` volt.

A #281-ben beolvasztott, csak olvasható firmware-próba felső korlátja 500 millió ciklus és 100 millió callback. Négy futás érte el a cikluskorlátot hozzáférési hiba nélkül; a VOP3 feldolgozott hangmintáinak száma nulla, `audio_accepted` hamis. Ez korlátozott szintetikus diagnosztika, nem teljes boot, hardveregyezés, MIDI vagy hangelfogadás. A helyi ROM változatlan és nincs a repositoryban; a MAME referencia rögzített forráscommitja `706dbb73281320c54d7c2f16262c1c3786f2be01`.

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
