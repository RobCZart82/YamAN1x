# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

## Ellenőrzött főági alap — #303 — 2026-11-15

A [#303 PR](https://github.com/RobCZart82/YamAN1x/pull/303)
2026-11-15 16:40:12 UTC-kor squash módszerrel olvadt be. A pontos PR-fej
`aff5b966a98d1e9e94e260d85f9e60dcdd01b7b0`, az új főági commit
`00ba1d437e1b2e46813027534b2400a1d50d18c6`. A PR-fejen a ROM-mentes
ellenőrzés sikerült (run `43731388706`), az initial-decode MSan is
(`43731388714`). A pontos főági commiton a Linux, macOS és Windows
ellenőrzései sikerültek (run `43736772211`), az initial-decode MSan pedig
(`43736772200`); az Ubuntu kiterjesztett UBSan/ASan lépései is sikeresek.
A MAME-kutatás dokumentációs módosítás miatt útvonal szerint nem futott.

A pontos PR-fejre vonatkozó Codex-áttekintés nem talált hibát
(összefoglaló megjegyzés: `6897529514`; kézi ellenőrzés nyugtája:
`6897544092`). GitHub review-beküldés, beágyazott megjegyzés és review-szál
nem volt. A `25910947` aktív szabályzat három platformellenőrzést, nulla
jóváhagyást és lezárt review-szálakat követel; kvótahiba vagy megkerülés nem
történt. A #303 a #302 beolvadási adatait egyeztette; futásidejű működést vagy
elfogadást nem változtatott.

## PKS-bemeneti szó korábbi írásainak ellenőrzése — helyi jelölt — 2026-11-15

A jelölt elemzés minden megfigyelt hívói olvasást összevet a rögzített
operandusesemények között talált legutóbbi korábbi írással, amely ugyanarra az
igazított szóra és a kiválasztott bájtsávra vonatkozik. Az értékbájtot és az
események sorrendjét is ellenőrzi. Teljes szavas vagy azonos bájtsávú írás
lehet megfelelő; az ellentétes bájtsáv írása nem. Ha a korlátozott felvételben
nincs megfelelő korábbi írás, az eredmény ezt mondja: `no preceding write
observed`. Ez nem állítja, hogy a felvételi ablakon kívül sem történt írás.
A szintetikus ellenőrzések lefedik a felső és alsó bájtot, a teljes szavas
írást, az eltérő értéket, a hibás címet, az ellentétes bájtsávot és a későbbi
írást.

A lépés a meglévő, csak olvasó nyomkövetési adatokat használja fel; új
megfigyelőt vagy MAME-módosítást nem ad hozzá. A célzott utasítás-elemző
tesztjei normál és Python `-O` módban is 11/11 arányban sikerülnek; a teljes
Python-teszt 143/143, az adattár-audit pedig sikeres (701 fájl, 201 upstream
azonosság). Ehhez a jelölthöz nem készült új MAME-fordítás vagy futás. A
rögzített forrás továbbra is
`706dbb73281320c54d7c2f16262c1c3786f2be01`; a ROM változatlan, az adattáron
kívül marad. Ez a korlátozott ellenőrzés nem igazol adatjelentést, oksági
kapcsolatot, hardveregyezést, indulást, MIDI-t, DSP-t, hangot vagy REAPER-
elfogadást.
## PKS-bemeneti utasításablakok — beolvadt a #301 PR-ban — 2026-11-15

A beolvadt fejlesztés új, csak olvasó utasításmegfigyelőt adott a két ismétlődő PKS
bemeneti olvasás köré. Négy, egyenként 2048 utasításnyi ablakban rögzíti a
programszámlálót, opkódot, időbélyeget, nyers CCR-t és az R0–R7/ER0–ER7
regisztereket. Két passzív, kilenc másodperces futás a rögzített MAME-forrással
és azonos műszerezett programmal azonos 8192 utasítássort adott
(`2938a16d4bad57c0eeffe404d26b4d01e62ef8c9aef853af9cc92aca9e3b84e5`),
teljes lábléccel, elveszett rekord nélkül és a jegyzék 26 egyéb mezőjében is
egyezően. A MAME-program kivonata:
`c7f3125bebfffd3e7f26ad8a33dab3a9abadc83f94e367850c1cda4cec8dd8ae`.

A független ellenőrzés a kiválasztott címet, értéket és bájtsávot a külön
buszmegfigyeléssel, az első CPU-pillanatképpel, a statikus és dinamikus
címsorrenddel, a közös forrásíró pont nyers CCR- és regiszterpillanatképeivel,
az R1L bájtjával, az időrenddel, folytatással és adatvesztés nélküli korlátokkal
veti össze. A `2349EA` olvasás `40` bájtot és nyers `84` CCR-t választott ki;
ezt `E0 00 20` forrásírás követte 12 625, 20 125 és 27 625 ns múlva. A `237204`
olvasás `00` bájtot és nyers `80` CCR-t választott ki; ezután `E0 00 00`
következett 10 375, 17 875 és 25 375 ns múlva. A CCR értelmezés nélkül
megőrzött adat. A ROM SHA-256 értéke továbbra is
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`, a kép
Git-en kívül maradt. Az eredeti MAME-forrás rögzített commitja továbbra is
`706dbb73281320c54d7c2f16262c1c3786f2be01`.

Ellenőrzés: a teljes Release-build sikeres; a teljes CTest-futás 139/139
esettel sikeres; a Python 138/138, a célzott optimalizált Python-ellenőrzés
6/6, a célzott CTest 2/2 eredménnyel zárult. A repository audit 701 fájlt és
201 upstream-azonosságot ellenőrzött; a forrás- és értesítés-lefedettség 232
fájlra és öt licencszövegre sikerült. Friss forrás-előellenőrzés mind a hat
rögzített bemeneti fájlt igazolta, mind a tizennégy patchet alkalmazta, hét
segédfejlécet ellenőrzött, és mind a tizenhárom előkészített fájlt egyezőnek
találta a helyi MAME-fával. Egy korábbi teljes CTest-futás jelezte, hogy az új
kutatási patch hiányzik az értesítés-nyilvántartásból; a sort a meglévő, már
ellenőrzött MAME-patch kategóriába vettük fel, projektlicenc kiválasztása
nélkül, majd a teljes újrafutás sikeres lett. A [#301 PR](https://github.com/RobCZart82/YamAN1x/pull/301) squash
módszerrel olvadt be a pontos
`c3f715ecaf8be26d0dd798e6de9bb1906c3d52d0` PR-fejről a
`7ce948d8c4fb7ee12cbfba5769047fd1e1a09aa2` főági commitba. A PR-fej platform-,
MSan- és MAME-ellenőrzései rendre a `43713392460`, `43713392310` és
`43713392395` futásokban, a pontos főági ellenőrzések a `43716702337`,
`43716702419` és `43716702349` futásokban sikerültek; az Ubuntu kiterjesztett
UBSan/ASan lépései is sikeresek. A végső, pontos PR-fejre vonatkozó Codex-
áttekintés nem talált hibát (összefoglaló: `6893288095`, kézi nyugta:
`6894257644`). GitHub review-beküldés, beágyazott megjegyzés és review-szál
nem volt. A `25910947` szabályzat három platformellenőrzést, nulla jóváhagyást
és lezárt szálakat követelt; kvótahiba vagy megkerülés nem történt. A
műszerezés továbbra is korlátozott kutatási bizonyíték: nem igazol okozatiságot,
szemantikát, hardveregyezést, indulást, MIDI-t, DSP-t, hangot vagy
REAPER-elfogadást.

## Következő korlátozott munka — helyi jelöltirány

A következő jelölt a megfigyelt PKS-forrásírásokat megelőző utasítás- és
értékút követése, független cím-, érték-, bájt-sáv-, nyers flag-, időzítési és
folytathatósági ellenőrzéssel. Az eredményt megfigyelésként kell kezelni,
okozati jelentést, hardveregyezést vagy hangelfogadást nem szabad állítani.
Csak a fenti ellenőrzött főági alapból induljon, miután ez a dokumentációs
egyeztetés beolvadt és a szükséges főági ellenőrzések sikerültek.

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

## Mérföldkövek

1. **Firmware és perifériák:** kis, bizonyítékhoz kötött H8, busz, MMIO és IRQ lépések; minden elfogadott profil után új firmware-futtatás. A konstans bemenetű szintetikus diagnosztika nem hardverkalibráció.
2. **Boot, PKS és MIDI:** a firmware további indulása, a PKS szerepe, majd a note-on/note-off üzenetek vétele külön-külön bizonyítandó.
3. **DSP:** az EG célérték-megtartása, az órajel és a hullámtáblák formátuma/címzése ellenőrzendő. Ezután közös időalapon kell léptetni a H8-at és a két VOP3 chipet.
4. **Első száraz hang:** rögzített patch, note-on, tartás és elengedés; dokumentált kétchipes jelút és kimeneti skála; reprodukálható WAV, majd összehasonlítás a hangszerből rögzített referenciával.
5. **Hosztintegráció:** csak a jelút igazolása után jöhet a motor, MIDI-időzítés és minimális VST3, majd a REAPER-ben lejátszott első hang.

A REAPER-elfogadás az utolsó lépés. Részprofil, teljes hardverhű indulás, MIDI, offline hang és REAPER-hang nem vonható össze egyetlen sikerállítássá.
