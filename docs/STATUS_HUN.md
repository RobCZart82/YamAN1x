# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött főági alap — #298 — 2026-11-15

A [#298 PR](https://github.com/RobCZart82/YamAN1x/pull/298)
2026-11-15 06:22:12 UTC-kor, squash módszerrel olvadt be. A pontos PR-fej
`473e58b8fb6facffda6280fc18f5b7998226d623`, az új főági commit
`6ef26c4e1c5a43fdd4e41f8f6f03221a53fa8e38`. A PR-fejen a Linux, macOS és
Windows ROM-mentes ellenőrzések sikeresek voltak (`43681871059`), az
initial-decode MSan is sikerült (`43681871044`). A pontos főági commiton a
Linux, macOS és Windows ellenőrzések (`43685124410`), valamint az
initial-decode MSan (`43685124686`) sikeresek. Az Ubuntu-feladat kiterjesztett
UBSan/ASan lépései is sikeresek. A MAME-kutatási munkafolyamat útvonal alapján
nem futott, mert ez dokumentációs PR volt.

A PR megnyitásakor indult Codex-áttekintés a pontos PR-fejen nem talált hibát
(összefoglaló megjegyzés: `6889845658`). GitHub review-beküldés, beágyazott
review-megjegyzés és review-szál nem volt. A `25910947` aktív szabályzat három
platform-ellenőrzést, nulla jóváhagyást és lezárt review-szálakat követel;
kvótahiba vagy megkerülés nem történt. A #298 a #297 beolvadási adatait és a
korlátozott firmware-megfigyeléseket rögzítette, futásidejű működést vagy
elfogadást nem változtatott.

## Friss, korlátozott helyi firmware-megfigyelés — 2026-11-15

A projekt csak olvasó, saját diagnosztikai programja változatlan főági forráson
és változatlan, külső AN1x 1.04 képen futott. Bemenet nélküli futás 600 000 000
ciklust, 1 000 000 ciklusos szeletet és 125 000 000 callbackes korlátot kért,
de `95 706 795` perifériaciklusnál, `0x1B64` utólagos PC-nél megállt: a
`0xFFFFE8` címre írt egybájtos `0x23` értéket a program nem támogatott ADC-
konverzió/IRQ-engedélyezésként utasította el. `cpu_failed:true`, TX- és
órajel-él nem volt, az `audio_accepted` és `firmware_boot_accepted` hamis maradt.

Négy friss futás állandó, nyolccsatornás szintetikus pásztázási bemenettel
(minden csatorna 0 vagy 1023), 4 096 vagy 1 000 000 ciklusos szeletekkel
elérte a kért 600 000 000 ciklusos határt CPU- vagy hozzáférési hiba nélkül.
Az AN0–AN3 első pásztázási határidői mindkét bemenetnél és mindkét szeletméretnél
`[95707121,95707387,95707643,95707899]` voltak. Az AN4–AN7 határidői 0
bemenetnél `[127793965,127794231,127794487,127794743]`, 1023 bemenetnél
`[127794154,127794420,127794676,127794932]` voltak; ugyanazon bemenet két
szeletméretnél megismételhető eredményt adott. Mind a négy futásban a VOP3
mintaszám `[0,0]` maradt. A callback-összesítések szeletméret szerint eltértek.
Egyik futás sem állította igazra az `audio_accepted` vagy
`firmware_boot_accepted` mezőt.

A ROM SHA-256 értéke előtte és utána is
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; a kép
változatlan, Git-en kívül maradt. A diagnosztikai program SHA-256 értéke
`b75b711ed425d08efa9cc837fbe9315efbeb98dbee3f928b1b09edac2f9de2ca`. A rögzített
MAME-forrás továbbra is `706dbb73281320c54d7c2f16262c1c3786f2be01`; új MAME-
build vagy futás nem készült. Ezek korlátozott eredmények szintetikus bemeneti
profilokkal, nem hardveregyezés, teljes indulás, MIDI, DSP, hang vagy REAPER-
elfogadás.

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

A #286 PR a véges próbakeretet 600 millió H8-ciklusra és 125 millió
visszahívásra emelte; a szeletmaximum 1 millió marad. Négy friss, csak olvasó AN1x
1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletek) elérte a ciklushatárt
hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa `95 706 795`, az AN0–AN3
pásztázás kezdete `95 707 121`, az AN4–AN7 pásztázásé `127 793 965`; az időpontok
mind a négy futásban egyeztek. A végső PC AN3=0 esetén `62010`, AN3=1023
esetén `1896`; a visszahívások száma szeletfüggő. Mindegyik futásnál nulla
VOP3-minta, `audio_accepted:false` és `firmware_boot_accepted:false`. Ez
szintetikus korlátozott profil, nem teljes hardver-, indulási, MIDI-, hang- vagy
REAPER-elfogadás. A ROM változatlan és a repositoryn kívül maradt.

## Nyitott elfogadási pontok

- A 39A4 rutinba írt értékek forrásának és hívóinak követése, független érték-, cím-, flag-, időzítési és folytathatósági ellenőrzéssel.
- Az indulás folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
