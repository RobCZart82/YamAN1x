# YamAN1x — magyar összefoglaló

A YamAN1x a Yamaha AN1x hangszer firmware-alapú emulációját kutató projekt. A cél egy későbbi VST3/AU hangszerbővítmény. A jelenlegi kód kutatási alap: még nem ad AN1x-hangot, nem igazol teljes, hardverhű bootot, és nincs benne kész bővítmény vagy grafikus kezelőfelület.

Az adattár dokumentációja nyelvenként külön fájlban található: az angol dokumentumok nem tartalmaznak magyar prózát, a magyar összefoglalók pedig külön `_HUN.md` fájlokban találhatók. Az angol dokumentáció a kanonikus műszaki leírás. A gyökérben lévő `README.md` angolul tartalmazza a projekt bemutatását és állapotát. A részletes MAME-kutatási útmutató a [külön magyar fájlban](research/README_HUN.md) található. A korábbi magyar nyelvű dokumentumrészeket a [külön magyar archívum](docs/HUNGARIAN_NOTES_HUN.md) őrzi.

## Ellenőrzött főági alap — #307 — 2026-11-16

A [#307 PR](https://github.com/RobCZart82/YamAN1x/pull/307) squash módszerrel
olvadt be 2026-11-16 18:39:09 UTC-kor: a pontos PR-fej
`705555681f9aa77f86eaae444c2ff7fc7034258a`, az új főági commit
`c37ac585dc508641a413da201df24949a396a36c`. A PR-fej ROM-mentes CI-je a
`43770073433`, az initial-decode MSan a `43770073394` futásban sikerült; a
csak dokumentációt érintő PR-nél a MAME-kutatási futás útvonalszűrés miatt nem
indult el. A pontos főágon a három előírt Linux/macOS/Windows ellenőrzés
`43931077479`, az MSan `43931080584` futásban sikerült; az Ubuntu kiterjesztett
UBSan/ASan lépései is zöldek. A MAME-kutatás a főágon útvonalszűrt maradt.

A pontos PR-fej Codex-áttekintése nem talált hibát (összefoglaló `6902498493`,
kézi nyugta `6902515270`). GitHub review-beküldés és review-szál nem volt.
A `25910947` aktív szabályzat három platformellenőrzést, nulla jóváhagyást és
lezárt szálakat követelt; kvótahiba vagy megkerülés nem történt. A #307 a #306
beolvadását és a friss helyi firmware-vizsgálatot dokumentálta.
## Friss helyi firmware-vizsgálat az FFFF60 útvonalon — 2026-11-15

A csak olvasó boot-vizsgáló (`c95439962709ef09b5715ec8c5b4d5ef0a608d7b4139e4531d3bf7ae3d570416`)
a pontos főággal azonos forrásfán (`69e43a550406ebf5a1f32889dff27b3b7e1d90a7`)
és a változatlan, külső AN1x 1.04 képpel futott. Két, explicit rögzített
nyugalmi panelprofilt és ADC-mintát nem használó futás azonosan állt meg a
`95 706 795` perifériaciklusnál, `0x1B64` utasítás utáni PC-n, a `0x23` értékű
bájt `0xFFFFE8` címre írásakor (`Unsupported ADC conversion or IRQ enable`).
Egyiknél sem volt TX- vagy órajelél; az audio- és firmware-boot-elfogadás hamis
maradt.

Négy további futás kifejezetten szintetikus, nyolccsatornás, állandó ADC-szkennelést
használt (minden csatorna 0, illetve 1023), 4096 vagy 1 000 000 ciklusos
szeletekkel. Mind a négy elérte a kért 600 000 000 ciklust CPU-hiba vagy
hozzáférési hiba nélkül. Az AN0–AN3 első mintavételi határidői
`[95707121,95707387,95707643,95707899]`. Nullás bemenetnél az AN4–AN7
határidői `[127793965,127794231,127794487,127794743]`; 1023-nál
`[127794154,127794420,127794676,127794932]`. Mindkét profil eredménye
szeletmérettől függetlenül egyezett. A VOP3 mintamenetei `[0,0]` maradtak;
a TX-élek száma 1294 (nulla) és 1315 (1023), órajelél nem volt.

A korábban az FFFF60 címen jelentkező nem támogatott aktív mód hibája ezen a
korlátozott útvonalon nem tért vissza: a rögzített ITU/TSTR-profil továbbjutott
a későbbi ADC-határig. A vizsgáló végállapotot ad, nem MMIO-nkénti TSTR-naplót;
ezek a futások nem igazolják külön az írások pontos értékét vagy az IRQ okát.
Négy kezdeti, nyugalmi panelprofil nélküli próbálkozás korábban, a `0xFFFFD6`
címen állt le (`Unmodeled external GPIO input`); ezeket hibás beállításként
kihagytuk, nem firmware-bizonyítékként kezeljük. A ROM előtte és utána is
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe` SHA-256
értékű maradt; változatlan és az adattáron kívüli. A szintetikus profilok nem
igazolnak hardveregyezést, teljes bootot, MIDI-t, DSP-t, hangot vagy REAPER-
elfogadást.

## #308 PR — korlátozott FFFF60 TSTR-nyom — 2026-11-16

Az új, kikapcsolható megfigyelő csak sikeres, egybájtos `0xFFFF60` írásokat
rögzít: címet, szélességet, értéket, buszciklust, nyers CCR-t, az írás előtti
PC/PPC/utasításállapotot és a következő befejezett CPU-dispatch állapotát. A
szintetikus próbák külön ellenőrzik a pontos címet/értéket/flaget/ciklust,
a folytatást, a szeletmérettől való függetlenséget, a megfigyelő nélküli és
bekapcsolt futás kimenetét, valamint a 64 elemű korlátot és túlcsordulást.
Az eredeti MAME-hivatkozás a rögzített
`706dbb73281320c54d7c2f16262c1c3786f2be01` commit; a forrásfájlok és a
`h8_timer16_device::tstr_w` metódus azonossága a kutatási szerződésben szerepel.
A forrás a TSTR bájtot eltárolja és a biteket a timer csatornák
`set_enable` metódusának adja át; ebből nem következtetünk IRQ-okozatra vagy
időzítő-egyezésre.

Két friss futás a változatlan, adattáron kívüli AN1x 1.04 képpel, a rögzített
nyugalmi panelprofillal, ADC-bemenet nélkül és azonos futási korlátokkal ugyanazt
a három írást adta: `0xFF` a 11 925 102 ciklusnál (`CCR=0x29`), `0xFB` a
92 381 419. ciklusnál (`CCR=0x25`), majd `0xFF` a 92 384 136. ciklusnál
(`CCR=0x00`). Mindhárom egybájtos FFFF60-írás; a nyom SHA-256 értéke
`005288d52f2ffa89b8b94428150e4191973e259db7e5021f579c74c46ae73c35`, eldobott
elem nem volt. Mindkét futás később megállt az ismert, nem támogatott
`0xFFFFE8` ADC/IRQ írásnál (`0x23` érték, `0x1B64` PC, 95 706 795 ciklus);
TX- és órajelél nem keletkezett. A ROM előtte és utána is ugyanazt a
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`
SHA-256 értéket adta.

A teljes összeállítás és a 139 CTest, 147 Python-teszt, az adattár-audit
(701 fájl / 201 upstream azonosság), valamint a forrás- és értesítési lefedettség
(232 fájl / öt licencszöveg; jogi jóváhagyást nem állítunk) sikerült az alap commiton. A #308 PR nyitva van; a pontos PR-fej CI-je és
review-ja folyamatban van. Nem igazol teljes bootot, timer/IRQ-okságot,
hardveregyezést, MIDI-t, DSP-t, hangot vagy REAPER-elfogadást.

## Legutóbbi kódalap — #304 — 2026-11-15

A [#304 PR](https://github.com/RobCZart82/YamAN1x/pull/304)
2026-11-15 18:34:07 UTC-kor squash módszerrel olvadt be. A pontos PR-fej
`6e9380a6c01f2c645da7705942552fa96330f2a4`, az új főági commit
`217e8e1af87276ff52d6e7008d70303d3f624440`. A pontos PR-fejen a Linux,
macOS és Windows ellenőrzései sikerültek (ROM-mentes CI, `43743313289`);
az initial-decode MSan is sikerült (`43743313286`). A pontos főági
commiton a három platformellenőrzés sikerült (`43747490185`), ahogy az
initial-decode MSan is (`43747490162`). Az Ubuntu kiterjesztett UBSan/ASan
lépései szintén sikeresek.

A pontos PR-fejre vonatkozó Codex-áttekintés nem talált hibát
(összefoglaló megjegyzés: `6898697415`; kézi ellenőrzés nyugtája:
`6898711208`). GitHub review-beküldés, beágyazott megjegyzés és
review-szál nem volt. A `25910947` aktív szabályzat három
platformellenőrzést, nulla jóváhagyást és lezárt review-szálakat követel;
kvótahiba vagy megkerülés nem történt. A #304 korlátozott, csak olvasó
ellenőrzést ad a meglévő nyomkövetési rekordokhoz; nem igazol
adatjelentést, oksági kapcsolatot, hardveregyezést, indulást, MIDI-t, DSP-t,
hangot vagy REAPER-elfogadást.

## PKS-bemeneti szó korábbi írásainak ellenőrzése — beolvadt a #304 PR-ban — 2026-11-15

A beolvadt elemzés minden megfigyelt hívói olvasást összevet a rögzített
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
azonosság). Ehhez a lépéshez nem készült új MAME-fordítás vagy futás. A
rögzített forrás továbbra is
`706dbb73281320c54d7c2f16262c1c3786f2be01`; a ROM változatlan, az adattáron
kívül marad. Ez a korlátozott ellenőrzés nem igazol adatjelentést, oksági
kapcsolatot, hardveregyezést, indulást, MIDI-t, DSP-t, hangot vagy REAPER-
elfogadást.

## PKS hívói olvasások hosszabb előtörténete — beolvadt a #306 PR-ban — 2026-11-15

A rögzített MAME-forráson készült friss, csak olvasó felvétel mind a négy
megfigyelt hívói olvasás előtt 65 536, utána 2 048 H8-utasítás-pillanatképet
őriz meg. Két kilenc másodperces futás ugyanazzal a műszerezett programmal
(`8e87662234394eb9e8ebb69b870412e8e9be4e4e169a7512a34132ae305becc2`) és a
külső AN1x 1.04 képpel készült. Mindkét felvételben 270 336 utasításrekord,
négy teljes ablak, nulla elveszett rekord és 328 749 nyomvonal-sor volt. A
nyers nyom SHA-256 mindkét futásban
`482566f32ac2509db8dc0e06cef63fd77d71dcb8e4710730cfd16ba0ff5b9645`, a
kanonikus utasításesemények kivonata pedig
`56087248c4027da217d9dc84f7db9f83f7fc5451a35a453e67022c7c79e82c70` lett. A
jegyzék 26, nyomvonaltól független mezője is egyezett. A MAME-forrás rögzített
commitja `706dbb73281320c54d7c2f16262c1c3786f2be01`.

A hosszabb ablak tartalmazta mindkét olvasást megelőző, kiválasztott
bájtsávra írt korábbi értéket. A `2349EA` címnél a `40` bájtot
(`4000/FF00`, nyers CCR `84`) 36 875 ns-sal korábban megfelelő írás
előzte meg. A `237204` címnél a `00` bájtot (`FF00/00FF`, nyers CCR `80`)
7 340 375 ns-sal korábbi megfelelő írás előzte meg. Mindkét írásnál a CPU
PC/időpont szerinti pillanatképe egyezett a független operandus-környezeti
CCR- és ER0–ER7-rekorddal; a nyers flag értelmezése nem történt meg. Ez
korlátozott MAME-megfigyelés: nem igazol adatjelentést, okozatiságot,
hardveregyezést, indulást, MIDI-t, DSP-t, hangot vagy REAPER-elfogadást. A ROM
SHA-256 végig `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`
maradt; a változatlan kép Git-en kívül van.

Helyi ellenőrzés: a rögzített forrás előkészítése 14 patch-et alkalmazott,
13 előkészített fájlt és minden segédfejlécet igazolt; a teljes MAME-cél
macOS-en lefordult. A teljes Release-build és CTest 139/139, a Python
146/146, a célzott optimalizált elemzés 14/14, az adattár-audit 701/201 és a
forrás/értesítés-lefedettség 232/5 eredménnyel sikerült. A `git diff --check`
is tiszta. ROM nem módosult és nem került Git-be.

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

## Következő korlátozott lépés — helyi jelöltirány

A következő kis fejlesztés legyen egy csak olvasó, korlátos megfigyelés a
firmware FFFF60 TSTR-írásairól: cím, bájtérték, ciklus/idő, nyers CPU-flag és
a folytatás rögzítésével. Az értéket és az időrendet a rögzített MAME-forrás
független ellenőrzésével kell összevetni. Az FFFF60 útvonal a friss, rögzített
ITU-profil mellett átjutott, de a jelenlegi végállapot-vizsgáló nem rögzíti a
TSTR eseményt; ezért pontos írási és IRQ-állítást még nem teszünk. A
szintetikus bemenet nem hardverkalibráció, az eredmény nem boot-, audio- vagy
REAPER-elfogadás.
## Korábbi, korlátozott firmware-megfigyelés — 2026-11-15

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

## PKS-bemeneti időablakok — korábbi helyi alap, kibővítve a #306-ban — 2026-11-14

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

A #286 PR az explicit próba véges korlátját 600 millió H8-ciklusra
és 125 millió visszahívásra emelte; a szeletmaximum 1 millió marad. Négy friss,
csak olvasó AN1x 1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletekkel)
elérte a ciklushatárt hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa
`95 706 795`, az AN0–AN3 pásztázás kezdete `95 707 121`, az AN4–AN7 pásztázásé
`127 793 965`; ezek az időpontok a négy futásban egyeztek. A végső PC AN3=0
esetén `62010`, AN3=1023 esetén `1896`; a visszahívások száma szeletfüggő.
Mind a négy futásnál nulla VOP3-minta, `audio_accepted:false` és
`firmware_boot_accepted:false`. Ez szintetikus korlátozott profil, nem
hardver-, teljes indulási, MIDI-, hang- vagy REAPER-elfogadás. A ROM változatlan
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
