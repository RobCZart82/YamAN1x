# PKS payload útvonal — magyar összefoglaló

Az angol, kanonikus dokumentum a [PKS payload path](pks-payload-path.md).
Ez az oldal kizárólag magyar nyelvű összefoglalót tartalmaz.

## Bemeneti utasításablakok — helyi jelölt — 2026-11-15

A rögzített MAME-forrás új, csak olvasó megfigyelője négy, egyenként 2048 H8-
utasításos ablakot rögzít a `2349EA` és `237204` bemeneti olvasások körül. A
napló megőrzi a programszámlálót, opkódot, emulált időt, nyers CCR-t, valamint
az R0–R7 és ER0–ER7 regisztereket. Két passzív, kilenc másodperces futás
azonos, műszerezett MAME-programmal ugyanazt a 8192 eseményt adta, elveszett
rekord és befejezetlen ablak nélkül; a nem trace jellegű 26 jegyzékmező is
egyezett. Eseménykivonat: `2938a16d4bad57c0eeffe404d26b4d01e62ef8c9aef853af9cc92aca9e3b84e5`.
A program kivonata: `c7f3125bebfffd3e7f26ad8a33dab3a9abadc83f94e367850c1cda4cec8dd8ae`.

A független ellenőrzés összeveti a címet, értéket, bájtsávot, az első CPU-
regiszter-pillanatképet, az utasítások sorrendjét, a nyers flag- és
regiszteradatokat a külön buszmegfigyeléssel, az R1L értékét, az időrendet és a
folytatást. A `2349EA` olvasás `40` bájtot és nyers `84` CCR-t adott, majd
`E0 00 20` forrásírás következett 12 625, 20 125 és 27 625 ns múlva. A
`237204` olvasás `00` bájtot és nyers `80` CCR-t adott, majd `E0 00 00`
következett 10 375, 17 875 és 25 375 ns múlva. A CCR jelentését nem
értelmezzük. Ez a korlátozott MAME-megfigyelés nem bizonyít okozati vagy
szemantikai kapcsolatot, hardveregyezést, teljes indulást, MIDI-t, DSP-t, hangot
vagy REAPER-elfogadást. A ROM változatlan és Git-en kívül maradt. Ez helyi
jelölt; még nem PR-eredmény, és az ellenőrzése folyamatban van.
