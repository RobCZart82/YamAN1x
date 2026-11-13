# YamAN1x — magyar összefoglaló

A YamAN1x a Yamaha AN1x hangszer firmware-alapú emulációját kutató projekt. A cél egy későbbi VST3/AU hangszerbővítmény. A jelenlegi kód kutatási alap: még nem ad AN1x-hangot, nem igazol teljes, hardverhű bootot, és nincs benne kész bővítmény vagy grafikus kezelőfelület.

A repository dokumentációja nyelvenként külön fájlban van: az angol dokumentumok nem tartalmaznak magyar prózát, a magyar összefoglalók pedig külön `_HUN.md` fájlokban találhatók. Az angol dokumentáció a kanonikus műszaki leírás. A gyökérben lévő `README.md` angolul tartalmazza a projekt bemutatását és állapotát. A részletes MAME-kutatási útmutató a [külön magyar fájlban](research/README_HUN.md) található. A korábbi magyar nyelvű dokumentumrészeket a [külön magyar archívum](docs/HUNGARIAN_NOTES_HUN.md) őrzi.

## Jelenlegi fejlesztési állapot — 2026-11-13

A főág ellenőrzött commitja `e718062c9989bca1d0cf4447df9b4c83ed5324b5` (#282). A PR és a pontos főági commit mind az öt ellenőrzése sikeres volt: Linux, macOS, Windows, MSan és MAME-kutatási build.

A Codex korábbi PR-fejen egy P2 dokumentációs hibát talált: hiányzó hash-értékeket és a Mac-átadási útmutatótól elszakadt parancsblokkokat. Ezek a végleges `d87d70c` PR-fejen javítva lettek, a review-szálat lezárták. Új, végleges-fejre vonatkozó Codex-review összefoglaló nem jelent meg. Az aktív szabályzat nulla jóváhagyást és lezárt review-szálakat követelt; kvótahibát nem jeleztek, megkerülés nem történt.

A #282 kizárólag az angol és magyar dokumentumok külön fájlokba rendezését és a nyilvántartás egyeztetését tartalmazta; futási vagy hangelfogadási viselkedést nem módosított. Az előző ellenőrzött főági állapot a #281 `5cc1eba0cadc9abab98457ac824604419ff0f84f` commitja volt.

Az ellenőrzött firmware-próba legfeljebb 500 millió CPU-ciklust és 100 millió callbacket enged; a próba korlátos, csak olvasható, és a nem támogatott hozzáféréseknél leáll. Négy diagnosztikai futás elérte a cikluskorlátot hozzáférési hiba nélkül. Mindegyikben nulla maradt a VOP3 hangmintafeldolgozása, és `audio_accepted` hamis volt. Ez szintetikus firmware-előrelépés, nem teljes boot, hardveregyezés, MIDI vagy hangelfogadás.

Az AN1x 1.04 ROM SHA-256 azonosítója `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; a fájl nem változott és nincs a repóban. A zárolt MAME-forrás commitja `706dbb73281320c54d7c2f16262c1c3786f2be01`; ehhez a legutóbbi lépéshez nem készült új MAME-fordítás vagy futtatás.

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
