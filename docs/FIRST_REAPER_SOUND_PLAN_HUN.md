# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

## Friss, korlátozott SCI TX firmware-vizsgálat — 2026-11-20

A `746697354478a403af47f6438e7b9d3eb58b51ce` forrásból három csak olvasó
próba készült a változatlan AN1x 1.04 ROM-mal, `swap16` nézettel,
`lcd-meg-vop3-idle-panel-experiment-16mhz` projektprofillal és nyolc nulla
ADC-bemenettel. Mindegyik elérte a 600 millió ciklust CPU-hiba nélkül.
Az SCI-megfigyelő 1 000 000 és 4096 ciklusos szeleteléssel ugyanazt a 238,
1-es csatornához tartozó eseményt rögzítette, eldobás nélkül. Az első alacsony
szint a 127 846 336., az utolsó magas szint az 597 556 608. ciklusnál volt.
A szeletméret csak a befejezett callbackek számát változtatta; a megfigyelő
kikapcsolása csak a megfigyelési mezőt. A teljes mérési azonosságok és jelentés-
hash-ek az angol dokumentumban és a strukturált naplóban szerepelnek.
A ROM változatlan maradt. Ez szintetikus részprofil soros vonalszint-eredménye;
a MIDI-dekódolás, fizikai UART, teljes boot, hardveregyezés, DSP, hang és
REAPER-elfogadás továbbra is nyitott. Friss MAME-futás nem készült.

## Ellenőrzött főági alap — #314 — 2026-11-20

A [#314 PR](https://github.com/RobCZart82/YamAN1x/pull/314) squash módszerrel
olvadt be `2026-11-20T16:18:16Z` időpontban. A pontos PR-fej
`259575202cec7bec5452e7f17a756578c205fc69`, az új főági commit `ede7a1bc56a3502ad47b3e76127d0e1f83e3f870`. Mind az öt PR-fej-ellenőrzés
sikerült: ROM-mentes Ubuntu, macOS és Windows (futás `44199714607`;
feladatok `134450359393`, `134450359355`, `134450358989`), initial-decode
MSan (futás `44199714385`, feladat `134450358364`) és MAME-kutatás
(futás `44199714367`, feladat `134450358493`).

A pontos beolvasztott főági commiton `ede7a1bc56a3502ad47b3e76127d0e1f83e3f870` mind az öt ellenőrzés szintén
sikerült: ROM-mentes Ubuntu, macOS és Windows (futás `44772033660`;
feladatok `136411908454`, `136411908397`, `136411907847`), initial-decode
MSan (futás `44772033696`, feladat `136411907679`) és MAME-kutatás
(futás `44772033600`, feladat `136411908644`). Az Ubuntu ROM-mentes feladat
kiterjesztett UBSan- és ASan-lépései is sikerültek. Függő vagy hibás kötelező
ellenőrzés nem maradt.

Nem volt Codex-review-összefoglaló/megjegyzés, GitHub review-beküldés,
beágyazott review-megjegyzés vagy review-szál. A `25910947` aktív szabályzat
a három platformellenőrzést és a review-szálak lezárását követeli; jóváhagyásból
nullát ír elő. Kvótahiba nem jelent meg, megkerülés nem történt.

A #314 korlátozott, bekapcsolható SCI-kimenetiszint-megfigyelőt ad a firmware-
vizsgálóhoz. Legfeljebb 4096 csatorna/szint/ciklus/sorrend eseményt és
túlcsordulásszámot rögzít; az ismétlődő, azonos szintű hívásokat elnyomja. Ez
csak soros vonalszint-adat, nem MIDI-dekódolás vagy fizikai UART-ellenőrzés.
A PR-fejen a helyi CTest 140/140 eredménnyel, az adattár-audit és a
licenclefedettség sikerült. Ehhez a jelölthöz friss firmware-futás nem készült;
ROM-ot nem módosítottunk és nem töltöttünk fel.

Következő lépés: az új megfigyelőt friss, csak olvasó helyi firmware-próbában
használni a pontos főágon, változatlan külső képpel és rögzített projektprofillal.
A korlátos nyom, a szeletméret és a megfigyelő be/ki kapcsolása közötti
azonosságot ellenőrizni kell. A vonalszintek önmagukban nem MIDI-dekódolás,
boot-, hardver-, DSP-, hang- vagy REAPER-elfogadás.

## Friss szintetikus ADC-szkennelési firmware-vizsgálat — 2026-11-17

Négy csak olvasó futás készült a pontos
`51159e9aff95ae0a1f9d125998be3700f9e8ef7a` főági forrásból, a
`2938008f192d91eea014d2aba0933c87fc4840a6043f8542e6eed6f04fddd2fc` SHA-256
értékű vizsgálóval és a változatlan, külső AN1x 1.04 képpel
(`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`). A
`swap16` nézetet, a rögzített
`lcd-meg-vop3-idle-panel-experiment-16mhz` projektprofilt, az opt-in TSTR-
megfigyelőt, 600 000 000 ciklust és 100 000 000-es callback-korlátot használtuk.
Két szintetikus, nyolccsatornás állandó bemenetet (mind a nyolc csatornán 0,
majd 1023) 1 000 000 és 4 096 ciklusos szelettel is lefuttattunk. A négy JSON
jelentés az adattáron kívüli `/private/tmp/yaman1x-pr313-probes/` könyvtárban
van; SHA-256 értékük sorrendben:
`d63e69664372f17cce95c204119a163767a78b5ea2e62c0a133dff896d6a9c8b`,
`c7d730912c4f94d8321e0c49087416f16bca31c098176c1c775ae01d5a69a812`,
`7ad2aebe8c201ece4b6e91510c8761d05364b5286957bfe799e41201636b695c`,
`02fa28532909535e22c5b7453e621d23d11a55edda6177797fde1a14fecabaff`.

Minden jelentés elérte a cikluskorlátot CPU-hiba és jelzett memóriahiba nélkül.
Mindegyikben ugyanaz a három sikeres, egybájtos TSTR-írás szerepelt eldobás
nélkül: `0xFF` a 11 925 102. ciklusnál (nyers CCR `0x29`), `0xFB` a
92 381 419. ciklusnál (`0x25`), majd `0xFF` a 92 384 136. ciklusnál
(`0x00`). A nyom és a szeletméreten kívüli jelentésmezők bemeneti
profilonként változatlanok maradtak; csak a `completed_slice_callbacks`
értéke változott a csoportosítás miatt. A nullás bemenet csatornánkénti szkennelési mintaszámai:
`[339077,336933,334600,332833,159762,158697,157565,156655]`; az 1023-asé:
`[339199,337046,334672,332892,159723,158628,157459,156568]`. Az első
mintavételi ciklusok rendre
`[95707121,95707387,95707643,95707899,127793965,127794231,127794487,127794743]`
és `[95707121,95707387,95707643,95707899,127794154,127794420,127794676,127794932]`.

Az órajelélek száma nulla, a VOP3 mintameneteké `[0,0]`; az audio- és
firmware-boot-elfogadás hamis maradt. A szintetikus bemeneti profilok korlátozott
projektprofil-eredményt adnak: ezen a futási útvonalon a feldolgozás túljut az
FFFF60 környékén és eléri a kért cikluskorlátot. Nem igazolják az ADC
hardverviselkedését, timer/IRQ-okozati kapcsolatot, fizikai hardveregyezést,
teljes bootot, MIDI-t, DSP-t, hangot vagy REAPER-elfogadást. A ROM változatlan,
és az adattáron kívül maradt. Ez részprofil, nem teljes hardverelfogadás.

## #308 PR — korlátozott FFFF60 TSTR-nyom — beolvadt

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

A PR-fej és a pontos főág ellenőrzései sikeresek. A PR-fej ROM-mentes CI-je
(`43945292893`), initial-decode MSan-ja (`43945292933`) és MAME-kutatása
(`43945292892`) zöld lett. A pontos főág platformellenőrzései
`43952611646` (2. próbálkozás), az MSan `43952611673`, a MAME-kutatás
`43952611677` (2. próbálkozás) alatt sikerült; az Ubuntu ASan/UBSan lépései
is zöldek. Az első főági próbában a macOS és MAME-feladat futtató és lépések
nélkül megszakadt, a második próbálkozás mindkettőt sikeresen lezárta.

A pontos PR-fej Codex-áttekintése nem talált hibát (összefoglaló
`6927307633`, nyugta `6927714031`). A korábbi, elavult PR-fejre érkezett P2
észrevétel javítása bekerült; a review-szál lezárt és elavult. Két review-beküldés,
két beágyazott megjegyzés és egy lezárt/elavult szál van. A `25910947` szabályzat
három platformellenőrzést, nulla jóváhagyást és lezárt review-szálakat követelt;
kvótahiba vagy megkerülés nem történt. Ez a korlátozott megfigyelő nem igazol
időzítő- vagy IRQ-egyezést, hardveregyezést, teljes bootot, MIDI-t, DSP-t,
hangot vagy REAPER-elfogadást.

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

## Következő korlátozott lépés — helyi jelölt

Az opt-in, csak olvasó FFFF60 TSTR-megfigyelő kódja, szintetikus próbái és két
azonos firmware-nyom elkészültek. Következőként a `c37ac585` főágra épített
jelölt pontos PR-fejének CI-jét és review-ját, majd a beolvadás utáni főági
ellenőrzéseket kell rögzíteni. A megfigyelés nem állít timer/IRQ-okozatot,
hardveregyezést vagy teljes elfogadást.
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

## Mérföldkövek

1. **Firmware és perifériák:** kis, bizonyítékhoz kötött H8, busz, MMIO és IRQ lépések; minden elfogadott profil után új firmware-futtatás. A konstans bemenetű szintetikus diagnosztika nem hardverkalibráció.
2. **Boot, PKS és MIDI:** a firmware további indulása, a PKS szerepe, majd a note-on/note-off üzenetek vétele külön-külön bizonyítandó.
3. **DSP:** az EG célérték-megtartása, az órajel és a hullámtáblák formátuma/címzése ellenőrzendő. Ezután közös időalapon kell léptetni a H8-at és a két VOP3 chipet.
4. **Első száraz hang:** rögzített patch, note-on, tartás és elengedés; dokumentált kétchipes jelút és kimeneti skála; reprodukálható WAV, majd összehasonlítás a hangszerből rögzített referenciával.
5. **Hosztintegráció:** csak a jelút igazolása után jöhet a motor, MIDI-időzítés és minimális VST3, majd a REAPER-ben lejátszott első hang.

A REAPER-elfogadás az utolsó lépés. Részprofil, teljes hardverhű indulás, MIDI, offline hang és REAPER-hang nem vonható össze egyetlen sikerállítássá.
