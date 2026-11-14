# YamAN1x — magyar összefoglaló

A YamAN1x a Yamaha AN1x hangszer firmware-alapú emulációját kutató projekt. A cél egy későbbi VST3/AU hangszerbővítmény. A jelenlegi kód kutatási alap: még nem ad AN1x-hangot, nem igazol teljes, hardverhű bootot, és nincs benne kész bővítmény vagy grafikus kezelőfelület.

A repository dokumentációja nyelvenként külön fájlban van: az angol dokumentumok nem tartalmaznak magyar prózát, a magyar összefoglalók pedig külön `_HUN.md` fájlokban találhatók. Az angol dokumentáció a kanonikus műszaki leírás. A gyökérben lévő `README.md` angolul tartalmazza a projekt bemutatását és állapotát. A részletes MAME-kutatási útmutató a [külön magyar fájlban](research/README_HUN.md) található. A korábbi magyar nyelvű dokumentumrészeket a [külön magyar archívum](docs/HUNGARIAN_NOTES_HUN.md) őrzi.

## Jelenlegi fejlesztési állapot — 2026-11-14

A #284 PR pontos feje `685a822901038c23168ce48e6f6de6ae0c0072d9`; a squash-
beolvasztás főági commitja `bc5626b983b246d8f12ce78c162001471e274075`,
időpontja 2026-11-14 01:29:44 UTC. A Linux, macOS, Windows és MSan
ellenőrzések sikeresek voltak mind a PR-fejen, mind a pontos főági commiton.
A MAME-kutatási ellenőrzés útvonal-szűrés miatt nem futott, mert a PR csak
dokumentációt módosított.

A Codex pontos PR-fejre szóló áttekintése nem talált hibát (megjegyzés:
`6868780442`); GitHub-review objektumok és review-szálak száma nulla. A
`25910947` szabályzat nulla jóváhagyást és lezárt review-szálakat követelt.
Kvótahibát nem jeleztek, megkerülés nélkül történt a beolvasztás. A #284 a
#283 beolvasztási adatait és a nyelvenként külön dokumentáció aktuális alapját
egyeztette; futási, hardver- vagy hangelfogadást nem módosított. Előző
ellenőrzött alap: #283, `2ff9f71c61ef8159a6bb815e76e3cd287b183635`.
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
