# Magyar nyelvű, áthelyezett korábbi dokumentumrészek

Az alábbi eredeti magyar nyelvű részek külön dokumentumba kerültek. A fejezetcím a forrásfájlt jelöli. A kanonikus projekt-összefoglaló: [README_HUN.md](../README_HUN.md).

## README

PR [#276](https://github.com/RobCZart82/YamAN1x/pull/276) beolvadt
2026-11-12 15:50:27 UTC-kor, a végső `27fc4b0a0677b340a279439db7d54466c5f23f8d`
headről a `74f0dc545c79a89790ad6eef42370b06f73bd65a` main commitba.
A PR-head mind az öt ellenőrzése sikeres: Linux `131281742752`, macOS
`131281743186`, Windows `131281743094`, MSan `131281739988`, MAME research
`131281740553`. A Codex review a pontos headen észrevétel nélkül zárult
(issue-comment `6827690290`). GitHub review-objektum és review-szál nem volt;
a ruleset nulla jóváhagyást ír elő és rendezetlen szál nem volt. Kvótahiba
nincs jelezve, merge-bypass nem történt.
Az új main mindhárom kötelező platformellenőrzése, továbbá az MSan és MAME
research futása is sikeres: `131300475364`, `131300474485`, `131300475237`,
`131300473882`, `131300476046` (workflow runok `43252136161`,
`43252135897`, `43252136454`).

## PKS forrás-RAM íráselőzmény — jelölt — 2026-11-10

A következő, még be nem olvadt auditorváltozás a rögzített PKS-forrásolvasások
értékét a legutóbbi, ugyanarra az igazított RAM-szóra és bájtlane-re eső korábbi
írással is összeveti. Az ER0 indexének ki kell választania a megfigyelt szót és
a bájtlane-et; a byte-write és full-word write eseteket külön kezeli. Az
ellenőrzés a meglévő FA92 olvasás → FA94 célírás cím/érték/R0L/ER4/idő kapcsolat
előtt fut. Ez megfigyelt írás-olvasás eredetlánc, nem PKS/ADC-szemantika.

A 12 célzott forráskontextus-teszt és a teljes Python-tesztkészlet (125/125)
helyben sikerült. A 138 CTest-csoportot ez a Python-only változás nem módosítja;
teljes CTest, MAME-fordítás és új firmware-futtatás ehhez a jelölthöz még nincs
rögzítve. ROM-ot ez a lépés nem olvas, nem módosít és nem tesz a repóba. A
távoli ellenőrzések és a pontos PR-head review a jelölt PR kapui; ez a rész még
nincs beolvasztva, hardver/boot/MIDI/hang-elfogadást nem jelent.

PR [#269](https://github.com/RobCZart82/YamAN1x/pull/269) az eredeti
headről (`0afd0e86fd4845514c2e969d809d25880b9d7d64`) nyílt, a
`947d67950345c7e03362d40f25174fcbe3b1f268` alapra. Ezen a pontos headen
mindhárom kötelező platformellenőrzés sikeres (Linux `130053106191`, macOS
`130053106085`, Windows `130053105515`); az MSan is sikeres
(`130053107228`). GitHub review-objektum, review-szál és PR-komment egyaránt
0; kvótahibára nincs bizonyíték. A szabályzat 0 jóváhagyást ír elő, a review-
szálak rendezését követeli. A naplófrissítés új PR-headet készít, ezért az új
pontos head ellenőrzései még függőben vannak. A PR még nincs beolvasztva.

## DEVELOPMENT_LOG

## Sikertelen írások értéke — #276 beolvadt — 2026-11-12

A `research/boot-probe-failed-write-context` ág a bounded firmware probe
`fault_access` mezőjében a sikertelen bájt-/szóírás megkísérelt értékét is
rögzíti; a hibás hozzáférés továbbra is fail-closed. A célzott tesztek pontosan
ellenőrzik a címet, szélességet és értéket: köztük bájt `0x5A`, szó `0x1234`,
és ADC-regiszter `0xA5`.

Négy friss, csak olvasó firmware-futtatás AN1x 1.04 ROM-mal: idle-panel
profil, `swap16`, 100,000,000 ciklus, 20,000,000 callback, AN3=0/1023 és
4,096/1,000,000 ciklusos szeletek. Mind a négy egy mintát adott a
`95,706,795` ciklusnál, majd megállt az `FFFFE8` byte-írásnál; az új mező
mindegyiknél `0x33`, post-fetch PC `1B84`, perifériaciklus `95,707,121`, hiba
`Unsupported ADC control mode`. Az egy mintán túli CPU-idő nem bizonyít
mintabemenet-függő firmware-hatást: a releváns nem-szeletfüggő számlálók
megegyeztek. A ROM hash `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`;
nem változott és nem került a repóba.

A lezárt MAME hivatkozás `706dbb73281320c54d7c2f16262c1c3786f2be01`; az ADC
forrása az `H8_ADC_3337` `FFFFE8` vezérlőírás `0x33` értékét szoftveres
AN0–AN3 pásztázó módként dekódolja. Ez kizárólag MAME-forrásértelmezés,
nem Yamaha-hardvervalidáció; új MAME build/futtatás nem volt. A helyi Release
build és teljes 138/138 CTest sikeres, a repository-audit és whitespace-check
sikeres. A #276 beolvadt; sem ADC-pásztázás, sem boot, MIDI, hang vagy
REAPER-elfogadás nem került hozzáadásra.

PR [#276](https://github.com/RobCZart82/YamAN1x/pull/276) beolvadt
2026-11-12 15:50:27 UTC-kor, a végső `27fc4b0a0677b340a279439db7d54466c5f23f8d`
headről a `74f0dc545c79a89790ad6eef42370b06f73bd65a` main commitba.
A PR-head mind az öt ellenőrzése sikeres: Linux `131281742752`, macOS
`131281743186`, Windows `131281743094`, MSan `131281739988`, MAME research
`131281740553`. A Codex review a pontos headen észrevétel nélkül zárult
(issue-comment `6827690290`). GitHub review-objektum és review-szál nem volt;
a ruleset nulla jóváhagyást ír elő és rendezetlen szál nem volt. Kvótahiba
nincs jelezve, merge-bypass nem történt.
Az új main mindhárom kötelező platformellenőrzése, továbbá az MSan és MAME
research futása is sikeres: `131300475364`, `131300474485`, `131300475237`,
`131300473882`, `131300476046` (workflow runok `43252136161`,
`43252135897`, `43252136454`).

## Dokumentációs auditjavítás és #274 lezárás — 2026-11-12

## Repository-audit és forráskontextus-javítás — 2026-11-11

Aktuális PR- és merge-állapot: [#270](https://github.com/RobCZart82/YamAN1x/pull/270).

Átvizsgált main: #269, `97e341ef56ff81a511ae64b6f98ca8af01251f84`.
A #267–269 változások a PKS forrásbájt CPU-kontextusát és a kiválasztott lane
legutóbbi korábbi írását kapcsolják össze. A #269 2026-11-10T19:59:59Z-kor
beolvadt, végső head `a533507e311404eedb68866572aeec00dab87f63`; a végső
platform/MSan futások 42885941250/42885941221 mind a négy feladata PASS.
A korábbi „jelölt” szövegek a saját előkészítési állapotukat őrzik.

Reprodukálható rés: --baseline nélkül az új source-context CLI csak a ROM-
azonosítást vizsgálta. Most a közös teljes futásvalidátort használja minden
belépéskor. Hiányos/sikertelen/malformált adatlap negatív CLI-próbákkal elutasítva,
teljes sikeres szintetikus rekord elfogadva. A régi helper neve kompatibilis marad,
szerződése teljes, sikeres run_h8_probe adatlapot kér. A korábbi teljes helyi
kutatási felvételek újbóli elfogadását ez a javítás nem állítja.

Következő kis fejlesztés: a proveniencia-jelentés a kiválasztott korábbi írás
CCR/ER0–ER7 pillanatképét is megőrzi. A teszt ellenőrzi, hogy az író állapotát
adja vissza és a jelentés nem osztozik a bemeneti regiszterlistán. Így a következő
vizsgálat az értéket előállító regiszter/utasítás felé haladhat.

138 CTest/126 Python eset,13 patch. Helyi Release build és137/137 külön
futtatható CTest PASS(60,63s);Python125/126 PASS,egy WinError1314 symlink-korlát.
A13 célzott teszt PASS. UTF8/linkek/napló-ősök PASS;helyi sanitizer nem futott. Forrásaudit 688 fájl/201 upstream azonosság,
licencértesítés 231 fájl/5 szöveg PASS. Jogi/bináris megfelelés nyitott.
M0 részleges,M1 nincs elfogadva;hardver/firmware/MIDI/hang/REAPER kapuk nyitottak.
A javítás és a kis fejlesztés saját távoli állapota a kapcsolódó PR-en követhető.

## PKS forrás-RAM íráselőzmény — jelölt — 2026-11-10

A következő, még be nem olvadt auditorváltozás a rögzített PKS-forrásolvasások
értékét a legutóbbi, ugyanarra az igazított RAM-szóra és bájtlane-re eső korábbi
írással is összeveti. Az ER0 indexének ki kell választania a megfigyelt szót és
a bájtlane-et; a byte-write és full-word write eseteket külön kezeli. Az
ellenőrzés a meglévő FA92 olvasás → FA94 célírás cím/érték/R0L/ER4/idő kapcsolat
előtt fut. Ez megfigyelt írás-olvasás eredetlánc, nem PKS/ADC-szemantika.

A 12 célzott forráskontextus-teszt és a teljes Python-tesztkészlet (125/125)
helyben sikerült. A 138 CTest-csoportot ez a Python-only változás nem módosítja;
teljes CTest, MAME-fordítás és új firmware-futtatás ehhez a jelölthöz még nincs
rögzítve. ROM-ot ez a lépés nem olvas, nem módosít és nem tesz a repóba. A
távoli ellenőrzések és a pontos PR-head review a jelölt PR kapui; ez a rész még
nincs beolvasztva, hardver/boot/MIDI/hang-elfogadást nem jelent.

PR [#269](https://github.com/RobCZart82/YamAN1x/pull/269) az eredeti
headről (`0afd0e86fd4845514c2e969d809d25880b9d7d64`) nyílt, a
`947d67950345c7e03362d40f25174fcbe3b1f268` alapra. Ezen a pontos headen
mindhárom kötelező platformellenőrzés sikeres (Linux `130053106191`, macOS
`130053106085`, Windows `130053105515`); az MSan is sikeres
(`130053107228`). GitHub review-objektum, review-szál és PR-komment egyaránt
0; kvótahibára nincs bizonyíték. A szabályzat 0 jóváhagyást ír elő, a review-
szálak rendezését követeli. A naplófrissítés új PR-headet készít, ezért az új
pontos head ellenőrzései még függőben vannak. A PR még nincs beolvasztva.

## #267 beolvadt — PKS forrásbájt CPU-kontextus — 2026-11-10

PR #267 squash merge: `dae0d64db4d463d119ee7be1c452c90a8487513d`; ellenőrzött
PR head: `411524cd9797db6e0aa7b9f3d0b14340eb50b52c`; alap:
`b04ff5e8ed1d8537c51b8a9ad0d03cf11494d36a`. A korlátozott, csak olvasó MAME
megfigyelő, auditor, tesztek és helyi bizonyíték bekerültek a főágba.

A két, kilenc másodperces passzív futás 43 eseménye bájtról bájtra egyezett
(nyom SHA-256: `012b91ce66f3b3fdef937ef374f405d6004a53fc7703059d1e75c3bcecbe2d76`);
eseményvesztés nem volt. A korábbi 1928 soros PKS kontextusnyom is egyezett.
Hat forrásolvasás/célírás pár értéke, címe, R0L/ER4 pillanatképe és 250 ns-os
MAME modellidő-különbsége egyezett. A CCR rögzített, de nincs flag-szemantika
tulajdonítva. A változatlan AN1x 1.04 ROM SHA-256:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; a ROM
nincs a repóban.

A végső PR-head mind az öt checkje sikerült: Linux `129967758579`, macOS
`129967758860`, Windows `129967758798`, MSan `129967758081`, MAME
`129967758518`. Codex review ugyanazon a headen észrevétel nélkül zárult
(issue comment `6772060805`). GitHub review-objektum: 0; review-thread: 0;
kvótahiba nem volt jelezve. Az aktív szabály nulla approvalt és rendezett
threadeket ír elő. A merge commit mind az öt checkje sikerült:
`129989103239`, `129989103935`, `129989103739`, `129989105616`,
`129989104714`. A squash merge ellenőrzött SHA-ra hivatkozott, szabálykerülés
nélkül.

Az eredmény egy rövid passzív profil korlátozott ismételhetőségi bizonyítéka.
Nem igazol PKS/ADC-szemantikát, hardveregyezést, teljes bootot, MIDI-t vagy
hangot. Következő: követni a megfigyelt forrásbájtok előtti írásokat és
utasításkontextust, független cím/érték/flag/idő/folytathatósági ellenőrzésekkel.
Protokollra ne következtessünk, és megfigyelésből ne adjunk hozzá HLE-választ.
M0 részleges; M1 és teljes hanglánc-elfogadás nyitott.

A közvetlen dokumentációs/audit előzmény, #266, szintén beolvadt:
`b04ff5e8ed1d8537c51b8a9ad0d03cf11494d36a` (head
`f9bd2433c5714c95afc950599b412a7b872b93ad`). Öt PR-head és öt main check
sikerült; a végső Codex review észrevétel nélküli volt, a korábbi inline
észrevételt megoldották. A ruleset nulla approvalt és rendezett threadeket
követelt; squash merge történt bypass nélkül.

## Auditjavítások — 2026-11-10

Ellenőrzött előkészítési alap: #265, `5421585951258f430ea13fd40207bdabd23ed654`.
A #265 dokumentációs PR már beolvadt; végső headje `08d72e334458e203a9dc385acb0511cf5a17cb17`.
PR CI: 42681766560/42681766508; merge CI: 42688078789/42688079192, mind a négy
feladat sikeres. Review a korábbi `cc8b736430386cb1d219135d3a0762db0cf73312`
headre zárult észrevétel nélkül; végső headre külön review-t nem állítunk.

Három külön commitból álló javítás:
1. ADC: utolsó elfogadott időbélyeg, régebbi restart elutasítása változtatás nélkül.
   1044 stale-restart eset mindkét bankkal, függő minta/eredmény megőrzése,
   azonos időpont, hiba/túlcsordulás és explicit reset utáni új epoch ellenőrizve.
2. PKS: kötelező futásadatmezők és típusok, ismert ROM-azonosítás, sikeres kilépés,
   CPU-előrehaladás és profil/bridge/MIDI-konzisztencia vizsgálata. Bekapcsolt DSP-órajel
   esetén mindkét chip pozitív, egész mintafuttatási számlálója kötelező.
   Review-kiegészítés: a futtató az adatlap kiírása után utasítja el a hiányzó/
   nulla számlálót, ezért ezt az összehasonlítóban is ellenőrizni kell. Az üres,
   hiányos, sikertelen és hibás típusú adatok elutasítva. A hiányzó mező eltér a null-tól.
   Négy új Python-teszt; célzottan mind a tíz PKS-próba PASS, negatív CLI-vel is.
3. Dokumentáció: 12 patch, #213 korabeli 131-es tesztszám, #265 lezárása,
   az elkészült ITU/ADC-integráció és a következő PKS-kutatási lépés egyeztetve.

A forrásfa 137 CTest/113 Python esetet tartalmaz. Helyi Release-fordítás és
136/136 külön futtatható CTest PASS (51,54 s). Python 112/113 PASS; egy ismert
WinError1314 symlink-jogosultsági korlát marad. A tíz célzott PKS-próba -O
módban is PASS. Forrásaudit: 683 fájl/201 upstream azonosság; licencértesítés:
230 fájl/5 szöveg PASS (nem jogi/bináris engedély). UTF-8/linkek/napló-ősök PASS.
Helyi sanitizer nem futott; az ADC-célpont a Linux sanitizer CI-be bekerült.
Saját távoli ellenőrzések a javítás PR-jén követhetők, korábbi CI nem új bizonyíték.
A beolvadás állapotának hiteles forrása a [#266 PR](https://github.com/RobCZart82/YamAN1x/pull/266); nem becsülünk előre saját merge-SHA-t.
A régi lezárási és jelöltbejegyzések a saját dátumukra vonatkozó történeti adatok.
M0 részleges, M1 elfogadása nyitott. ROM/bank/privát trace/hardverpróba nem készült;
EG target-hold továbbra is csak hardveres bizonyíték alapján módosítható.

# YamAN1x fejlesztési napló

## #260 beolvadt — 2026-11-08

## Külső fejlesztési állapotjelentés beépítése — 2026-11-07

A felhasználó kérésére ellenőriztük a jelentést az élő GitHub PR-állapot,
az #1–#5 hibajegyek és a fejlesztési terv alapján. A lényegi megállapítások
egyeztek. A javasolt sorrendet függőségi feladatlistaként kapcsoltuk M0–M6-hoz,
a közvetlen firmware/ADC-munkát megtartva. Pontosítottuk a keretrendszer-
és függőségi licencek kérdését, a külön felhasználói projektlicenc-döntést
és az első REAPER-próbához szükséges VST3 célt. AU későbbi cél.

A részletes feladatlista a FIRST_REAPER_SOUND_PLAN „Ellenőrzött fejlesztési
prioritások” szakaszába került. A jelentés ellenőrzéséhez új fordítás vagy
hardveres összehasonlítás nem készült. A dokumentáció a #242 jelölt része;
beolvadását és az aktuális head ellenőrzéseit külön kell rögzíteni.

## Javítás lezárva — #213, 2026-11-05

Beolvadt: [#213](https://github.com/RobCZart82/yaman1x/pull/213),
merge `d60e6eb9907d0a9c89938e1677d68e54b319da74`,
tesztelt head `a421a261a6647e759dac7d5132567d82f2de32fc`.
Időpont: 2026-11-05 10:09:33 Europe/Budapest (09:09:33 UTC).
[Linux/macOS/Windows, UBSan és ASan/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686134),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686447),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686368): mind az öt feladat sikeres.
Automatikus review észrevétel nélkül lezárult a `95b7edb0feaa97be23d1105fe59b6f259596e640`
forráscommiton; utána csak három dokumentum PR-hivatkozása változott. A végső headre
külön review-t nem állítunk. Nyitott review-szál nem volt a beolvasztáskor.

A teljes külső LCD/MEG/VOP3 cold reset alapreferencián keresztül is működik,
és az alapmutatós felszabadítás teljes. Reprodukció: javítás előtt megtartott
MEG/VOP3 állapot miatti kudarc; javítás után két alapnézetes reset és két
alapmutatós destruktorpróba PASS. Helyi Release/130 futtatható CTest PASS;
Python 102/103 PASS, egy WinError1314 symlink-jogosultsági akadály. Teljes
CI-készlet: 131 CTest/103 Python. A sanitizer-bizonyíték a fenti Linux CI.

Nincs aktív motorjelölt. Ez a lezáró egyeztetés dokumentációs változás;
saját ellenőrzései a kapcsolódó PR-en követhetők, a fenti CI a #213-hoz tartozik.
Következő: aktív ITU gépütemező/MMIO- és CPU IRQ-bekötés, majd friss helyi
firmware-diagnosztika. M0 részleges; boot/MIDI/hang/plugin/REAPER elfogadás nyitott.

## Forrásaudit és cold-reset javítás — 2026-11-05

Ellenőrzött motoralap: #212, `5915ccb54f07f7649192ea3dbfa4277c56f0ce01`.
A tesztelt head `28f81562685616cb89bdf96f269c62bcc9a375de`; a platform/sanitizer,
MSan és MAME futások sikeresek (41922483350 / 41922483298 / 41922483383).
Automatikus review-eredmény az ellenőrzéskor nem volt rögzítve.
131 CTest/103 Python eset. A korábbi #211 „jelölt” megjegyzéseit történeti
adatként őrizzük; #211 és #212 beolvadt. A közös SYSCR (#141) és a korlátozott
reset (#168–169) már elkészült, a teljes hardver-reset és boot nincs elfogadva.

Reprodukált hiba: az An1xVop3ResearchSink LCD- vagy MEG-alapreferenciáján hívott
cold_reset csak az alapréteget nullázta; a MEG/VOP3 állapot és számlálók megmaradtak.
A cold_reset most virtuális, a leszármazottak override-ot használnak, az alap
virtuális destruktort kapott. Így az alapmutatón birtokolt teljes adapter is
felszabadul. Két alapnézetes resetpróba ellenőrzi a teljes állapotot, az új
nullaidős korszakot és a számlálókat; két alapmutatós tulajdonosi próba a teljes
objektum megsemmisítését. A teszt javítás előtt elbukott, utána PASS.
Ez a közös külső adapterfelület javítása; a jelenlegi konkrét típusos CLI-út
ugyanezt a cold resetet eddig is közvetlenül hívta.

Aktív javítás: [#213](https://github.com/RobCZart82/yaman1x/pull/213), fix/research-sink-cold-reset. Release build és 130/130 helyben
futtatható CTest PASS (53,85 s). Python: 102/103 PASS, egy ismert symlink-esetet
a Windows-jogosultság akadályozott (WinError1314). Forrásaudit: 665 fájl/201
upstream azonosság PASS; licencértesítés: 229 fájl/5 szöveg PASS, jogi/bináris
engedélyezést nem jelent. Belső linkek/UTF-8/napló-ősök PASS. Helyi sanitizer
nem futott. Saját távoli CI/review függőben; korábbi CI nem ennek az eredménye.
Következő motorlépés: aktív ITU ütemező/MMIO és CPU IRQ-bekötés; utána friss helyi
firmware-próba. PKS/MIDI, DSP/hang/plugin/REAPER elfogadása nyitott. Yamaha-adatot
nem használunk; EG target-hold módosítás továbbra is hardveres bizonyítékhoz kötött.

## Történeti fejlesztési és előkészítési bejegyzések

Az alábbi korábbi „jelölt”, „függőben” és „következő” állítások a saját dátumukra
vonatkoznak; az aktuális feladatsort a fenti összefoglaló és az M0-szakasz tartalmazza.

Frissítve: 2026-11-03. Időzóna: Europe/Budapest.
Ellenőrzött motoralap: `301eec75c72028b8f377ee4e099fce0b1df4de5e` (#143).

## VOP3 hostadapter lezárása — 2026-11-03

A #199 beolvadt `6f761b3027f2de27c7321d39c83f6e6e0c8c2e49` commitként.
Pontos head:`51366589a6828b06c952ff354b7b3fadc62952df`;mind5 CI sikeres.
Futások:41681455942,41681455958,41681455950. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T17:10:43.532192Z-kor:
https://github.com/RobCZart82/yaman1x/pull/199#issuecomment-6610502619
Nyitott review-szál vagy konfliktus nincs. Következő:natív VOP3-híd és adapter
összekötése,lifecycle/metrics/fail-stop tesztekkel,CPU-buszbekötés előtt.
Új lépés előtt main CI ellenőrzése szükséges;boot/effekt/hang nem elfogadott.

## VOP3 teljes szavas hostadapter — jelölt,2026-11-03

Önálló template adapter a két kanonikus440000/480000,egyenként100 bájtos
ablakot chip/regiszter/full-word backendhívásra képezi. Páratlan cím,részmaszk,
tükör és ablakon kívüli cím előzetesen elutasított. Reg1 feltöltésvezérlés csak
explicit experimental_firmware_contract módban,a korábbi UploadControlContract
újrahasználatával:chipenként külön bit15 státusz,csak0/1 command. A write a
backendhez is eljut;egyéb input csak a modellben implementált EG_STATUS21.
Nem gyártunk alap nullás választ22-re vagy más ismeretlen olvasásra.
Backendhiba vagy elnyelt újrabelépési kísérlet mérgező,nem folytatható.
65024 route/value eset,chip controls,bounds/mask/read/fail-stop/reentry strict
ASan/UBSan PASS. Tesztbackend szintetikus,nem firmware/élő VOP3 integráció.
Szerződés:[vop3-host-word-adapter.md](vop3-host-word-adapter.md).
Helyi121/121 CTest (51,28s),100 Python,strict célzott ASan/UBSan PASS.
Audit624 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer40 csoport.
Saját CI/review függőben. Következő:natív híd és kutatási
busz explicit bekötése;MEG fizikai státusz és valódi hang továbbra nyitott.

## MEG forrás-stub kísérlet lezárása — 2026-11-03

A #198 beolvadt `94bf3fe1e8c708b4a2fa31fd7d4081f080d439db` commitként.
Pontos head:`7dc0b587d8ae45ef036f26de5f4ecc451486c2fe`;mind5 CI sikeres.
Futások:41664675511,41664675355,41664675306. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T15:35:16.405619Z-kor:
https://github.com/RobCZart82/yaman1x/pull/198#issuecomment-6607930821
Nyitott review-szál vagy konfliktus nincs. Következő:VOP3-host kapcsolat
kutatása az explicit forrás-stub profilban;MEG fizikai státusz továbbra ismeretlen.
Új lépés előtt main CI ellenőrzése szükséges;boot/effekt/hang nem elfogadott.

## Explicit eredeti MEG-stub státuszkísérlet — jelölt,2026-11-03

MEG fizikai státusz/kézfogás szerződés továbbra ismeretlen. Rögzített eredeti
MAME s2_r/s10_r/s11_r mind00-t adó log-stub,nem ready-bit modell.
Ezért csak új explicit lcd-meg-source-stubs-16mhz profil kapcsolja a három
kanonikus olvasást00-ra,számlált megfigyeléssel. Alap/lcd/write-observe profil
szigorú marad;minden más read/word továbbra hibázik. Nincs fizikai vagy audio-
elfogadás.768 policy/read,lyuk/alias/word/reset/isolation és129 CPU-felosztás
strict ASan/UBSan PASS;768 eredeti zero-read összevetés PASS.

Ideiglenes,repo-n kívüli kontrollban00 válasszal VOP3-ig jut,FF állandó
válasszal100M ciklusig várakozó útvonal marad;ez nem mért hardveres válasz.
Új explicit profil:4096/1000000 szeletnél egy stubolvasás/öt vezérlőmegfigyelés
után azonos440002 word write stop,perifériaciklus10832302,PC353780,
6316 GPIO,0 TX/clock él. ROM változatlan. Szerződés:
[meg-source-status-experiment.md](meg-source-status-experiment.md).
Helyi120/120 CTest (62,25s),100 Python,strict ASan/UBSan és eredeti referencia
PASS. Audit621 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer39 csoport.
Saját CI/review függőben. Következő:VOP3-host kapcsolat
kutatása;MEG fizikai státuszhoz külön elsődleges specifikáció/mérés szükséges.

## MEG megfigyelő integráció lezárása — 2026-11-03

A #197 beolvadt `d7f08c1eee2ebf306cd35aeaa9b26434b0913e41` commitként.
Pontos head:`a45dc7c595c091bae5111feb648f479ce5ec1267`;mind5 CI sikeres.
Futások:41650965433,41650965655,41650965487. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T14:11:35.408230Z-kor:
https://github.com/RobCZart82/yaman1x/pull/197#issuecomment-6606002728
Nyitott review-szál vagy konfliktus nincs. Következő:MEG offset2 státusz/
kézfogás szerződés bizonyítéka;eredeti zero-read stub nem fizikai ready-igazolás.
Új lépés előtt main CI ellenőrzése szükséges;boot/effekt/hang még nem elfogadott.

## MEG megfigyelő buszprofil — jelölt,2026-11-03

MEG külső byte write csak explicit sink opt-in mellett a400000..40003F
szűk ablakban továbbítható;a hostadapter továbbra ellenőrzi az odd lane/offsetet.
Read/word/instruction továbbra strict,nincs generikus MMIO-fallback.
A tiszta enable-query mellett area2 ASTCR/WCR/WCER időzítés használatos,
24-bit alias költséggel együtt. An1xMegResearchSink saját MEG-tárolót birtokol,
közös LCD/MEG idő- és fail-stop epochot használ;subset reset megtartja,külön
cold_reset újraépíti. Probe csak új explicit lcd-meg-observe-16mhz módban
kapcsolja be;a régi alap/lcd profil nem változik. meg_observed_controls számláló
csak log-only megfigyelés,nem parancshatás vagy működő effekt.

256 bank/control,alias/wait/reset/strict/poison és129 CPU-felosztás strict
ASan/UBSan PASS. CLI explicit/profilnegatív és olvasási hibakontroll PASS.
Változatlan helyi ROM4096/1000000 szelettel:négy megfigyelt vezérlőírás után
azonos stop400005 byte read,PC0xDA0C,perifériaciklus10814942,6316 GPIO,
0 TX/clock él. Nincs fake MEG-ready és boot/hangelfogadás.
Szerződés:[an1x-meg-research.md](an1x-meg-research.md). Helyi119/119 CTest (71,81s),100 Python,strict célzott ASan/UBSan PASS.
Audit618 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer38 csoport.
Saját CI/review függőben. Következő a MEG státusz-szerződés bizonyítékának vizsgálata.

## MEG hostadapter lezárása — 2026-11-03

A #196 beolvadt `7e93410925450aa1715a65fca9babba395bd9128` commitként.
Pontos head:`651cad9f8d72210479484d2477b3ef94cd552a18`;mind5 CI sikeres.
Futások:41638949671,41638949433,41638949383. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:explicit megfigyelő MEG-sink/buszbekötés,ismeretlen olvasások és
státuszok szigorú elutasításával. Előbb main CI ellenőrzése szükséges.

## MEG hostcím-adapter — jelölt,2026-11-03

A helyi szervizrajz51.PDF-oldalán IC10 MEG A0..A4 a CPU A1..A5 hálózataira
csatlakozik,D0..D7 az alsó adatbájtra;LWR/RD/MEGCS külön jelzett. Kanonikus
kutatási ablak:400001+2*offset,offset0..18.400007 így3-as vezérlőoffset.
A teljes chip-select engedély és címtükrözés továbbra nincs igazolva.
Önálló MegHostWriteAdapter nem köti be a CPU-buszt. Alapból a nem implementált
vezérlők hibáznak;külön explicit observe_original_stubs módban hat eredeti
logging-only írás csak számlált megfigyelés,nincs hatás/status/ready/audio.
Minden nem támogatott bájtsáv/lyuk/tükör/offset elutasított,tároló állapotát nem
módosítja.1536 vezérlőérték,65536 tárolópár,ablak/lane/reset kontroll strict
ASan/UBSan PASS. Eredeti21 metódussal720896 tároló+1536 logging-only összevetés
PASS. Szerződés:[meg-host-write-adapter.md](meg-host-write-adapter.md).
Helyi118/118 CTest (51,59s),100 Python,strict ASan/UBSan és referencia PASS.
Audit615 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer37 csoport.
Saját CI/review függőben.
Következő:explicit kutatási sink-bekötés,szigorú ismeretlen olvasásokkal.

## MEG regisztertárolás lezárása — 2026-11-03

A #195 beolvadt `1c5d35fcaa0110cbae0dab6f280f0020b24105e7` commitként.
Pontos head:`d2f9dfec15e3fabc102a21164453bb1441ff6a3c`;mind5 aktuális CI sikeres.
Futások:41624961321,41624961195,41624961011. A macOS125879878868 feladat
hosted runner kapacitáshiány miatt nem kapott gépet;csak ezt kértük újra a
workflow befejezése után. Végső macOS125896759096 sikeres;a GitHub új próbában
új Windows/Ubuntu check-identitást is közölt,mindkettő sikeres. Futó munkát nem
indítottunk újra. Automatikus review-eredmény nem rögzített;nyitott review-szál
és konfliktus nincs,merge-state clean. Következő:MEG hostcím/bájtsáv/vezérlő-
szerződés bizonyíték alapján;400007 még tiltott. Előbb main CI ellenőrzése.

## MEG önálló regisztertárolás — jelölt,2026-11-03

A400000 körüli effektinterfész a korábbi rajzi/firmware-kutatás jelöltje;
a rögzített eredeti AN1x driver nem térképezi fel,ezért400007 buszelérését
most nem engedjük át. Önálló MegRegisterStorage:8-bit selector,tíz256-bájtos
bank,két256-szavas constant/offset bank független high/low lane-nel.
Nincs státuszolvasás,audio vagy vezérlőparancs-szimuláció;a logging-only
MAME-stubok nem bizonyítanak elfogadható választ. Nem támogatott offset hibázik.
Cold zero projektpolitika (eredeti start/reset üres);reset megtartja a tárolót.
720896 független bank/lane eset,65536 szóérték,isolation/strict/reset strict
ASan/UBSan PASS.15 eredeti metódussal720896 összevetés,azonos explicit kezdő-
állapotból;nem hardveres reset-/busz-/hangelfogadás. Szerződés:
[meg-register-storage.md](meg-register-storage.md). Helyi117/117 CTest (50,57s),100 Python,strict ASan/UBSan és eredeti
referencia PASS. Audit612 fájl,notice229 fájl/5 szöveg,diff-check PASS;
CI sanitizer36 csoport. Saját CI/review függőben. Következő:hostcím/vezérlőoffset bekötés bizonyítékának tisztázása.

## Leállított TIER-tárolás lezárása — 2026-11-03

A #194 beolvadt `1e67c1b379aebe0bec94dbac39e9b199adca9d3f` commitként.
Pontos head:`2681fdc5494aa6f8d2c792be406306ffcc40d16b`;mind5 CI sikeres.
Futások:41614096928,41614096943,41614096814. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:400007 periféria eredeti címtér- és működési szerződése.
Új fejlesztés előtt main CI ellenőrzése szükséges;boot/hang még nem elfogadott.

## Leállított ITU TIER-tárolás — jelölt,2026-11-03

Öt TIER önállóan tárolja a három engedélyező bitet,reserved read-oneF8,
resetF8. Minden1280 csatorna/raw kombináció elfogadott és visszaolvasható.
Ez konfigurációtárolás,nem aktív ITU IRQ-szolgáltatás:TSTR továbbra tiltott,
TSR üres,nincs counter/capture/event ami flaget vagy IRQ-t kelthetne.
Az eredeti tier_update m_tier|F8,IER A/B/V megfeleltetés és üres ISR alapján;
a számlálóindítás vagy külső esemény támogatása előtt aktív flag/IRQ kell.
A régi idle-teszt tiltáselvárását tároláselvárás váltja,de disabled/reset/status/
word kontrolljai megmaradnak. Új1280 érték,owner/reset/time/start-negatív és129
CPU-felosztás strict ASan/UBSan PASS.1280 eredeti TIER/üres-TSR összevetés PASS.

Változatlan helyi ROM,100000000 ciklus/20000000 callback,lcd-16mhz,
4096/1000000 szelet:következő stop400007 byte write,PC0xD9F2,
perifériaciklus9555129,6316 GPIO,0 TX/clock él. Nem boot vagy hangelfogadás.
Szerződés:[an1x-itu-tier-stopped.md](an1x-itu-tier-stopped.md). Helyi116/116 CTest (85,95s),100 Python,strict célzott ASan/UBSan és
eredeti referencia PASS. Audit608 fájl,notice229 fájl/5 szöveg,diff-check PASS;
CI sanitizer35 csoport. Saját CI/review függőben. Következő a400007 periféria eredeti címtér-szerződése.

## Firmware-diagnosztika lezárása — 2026-11-03

A #193 beolvadt `9a56f7ab5acab3d33f64e206cefdd9debe9bcbc8` commitként.
Pontos head:`d0864c1642eb1ec2e1c38f4afc98c763e5e82bac`;mind5 CI sikeres.
Futások:41603324777,41603324735,41603324731. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T08:27:56.524295Z-kor:
https://github.com/RobCZart82/yaman1x/pull/193#issuecomment-6598885247
Nyitott review-szál vagy konfliktus nincs. Következő:TIER4 IRQ-engedély és
szükséges ITU flag/aktív állapot szerződésének vizsgálata rögzített eredetiből.
Új lépés előtt main CI ellenőrzése szükséges;boot/hang továbbra nem elfogadott.

## Firmware-próba:korlát és valódi ITU-stop — jelölt,2026-11-03

A probe callback-korlátja továbbra explicit és véges,de legfeljebb20000000
lehet;a ciklus/szelet limitek változatlanok. Jelentés új mezői:stop_reason
(cycle_limit/callback_limit/execution_error),callback_limit_requested.
A callback-limit továbbra mérgező fail-stop,nem folytatható költségkeret.
A korábbi1000000 korlátnál mért megállás nem eszközhiba volt.

100000000 kért ciklus/20000000 callback,swap16,lcd-16mhz és4096/1000000
szelet mellett azonos valódi stop:FFFF94 TIER4 byte write,PC0x40A62,
perifériaciklus8798299,5269 GPIO,0 TX/clock él. Hiba:Unsupported ITU interrupt
enable;firmware_boot_accepted=false. ROM hash előtte/utána változatlan.
Következő a leállított TIER4 engedély és tényleges ITU flag/IRQ szükséglet
vizsgálata rögzített eredeti forrásból,nem az elutasítás egyszerű kikapcsolása.
Szerződés:[an1x-firmware-event-diagnostics.md](an1x-firmware-event-diagnostics.md).
Saját CI/review függőben. Helyi115/115 CTest (50,22s),100 Python PASS;
audit605 fájl,notice229 fájl/5 szöveg,diff-check PASS. CPU/periféria változatlan.

## P9 kutatási profil lezárása — 2026-11-03

A #192 beolvadt `8d53be572aab80db6bda2f9629366002b252bc2d` commitként.
Pontos head:`23dd23fba4d221b7e413e2377282745a458de94f`;mind5 CI sikeres.
Futások:41594348384,41594348397,41594348298. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T07:11:33.760494Z-kor:
https://github.com/RobCZart82/yaman1x/pull/192#issuecomment-6597624180
Nyitott review-szál vagy konfliktus nincs. Következő:korlátos firmware/esemény-
diagnosztika az LCD/P9 után;callback-limit nem eszközhiba és nem bootelfogadás.
Új lépés előtt main CI ellenőrzése szükséges.

## P9 kutatási bemeneti profil — jelölt,2026-11-03

A korábbi LCD MAME-patch kifejezetten read_port9=FF szintet ad,a rögzített
eredeti h8_device::port_default_r szinténFF-et ad nem bekötött portokra.
Csak az explicit LCD research sink P9 bemenetén vezetjük be ezt a headless
kutatási profilt. Nem fizikai huzalozásbizonyítás,nem általános GPIO-fallback.
A H8 portmodell DDR/kimenet/reserved bit keverése megmarad,más ismeretlen
bemenet továbbra mérgező hibát okoz. Alapértelmezett no-LCD probe nem változik.
Független bites oracle minden65536 DDR/data kombinációra,alias/idő/reset és
más portok fail-stop kontrolljai. Szerződés:[an1x-p9-input.md](an1x-p9-input.md).
Helyi115/115 CTest (52,63s),100 Python és strict ASan/UBSan PASS;
az optimalizált fixture újrafordítva,célzott CTest ismét PASS. CI/review függőben.
Audit604 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer34 csoport.
500000 ciklus1/64/257 szelettel hiba nélkül,azonos PC/state/periféria/22 GPIO;
hosszabb próbák callback-limitnél álltak meg,nem eszközhibánál. ROM változatlan.
Nem boot/hangelfogadás;következő további korlátos firmware- és eseménydiagnosztika.

## LCD-sink integráció lezárása — 2026-11-03

A #191 beolvadt `0952306f918f57582f928aea8f44fff5123d0749` commitként.
Pontos head:`5ffe1075fb8860d76cd6fa9b16470328e4622681`;mind5 CI sikeres.
Futások:41585964104,41585964059,41585963977. Automatikus kódreview ugyanazon
headen találat nélkül kész2026-11-03T05:49:09.070883Z-kor:
https://github.com/RobCZart82/yaman1x/pull/191#issuecomment-6596475450
Nyitott review-szál vagy konfliktus nincs. Következő:P9 bemenetek bekötési
szerződése bizonyíték alapján;előbb main CI ellenőrzése.

## LCD-sink gépintegráció — jelölt,2026-11-03

Saját LCD-sink kapcsolja össze a portadaptert és a headless vezérlőt;explicit
ciklus/µs skálával. Nem-LCD bemenet továbbra hibázik,egyéb kimenet nem gyárt
eszközválaszt. GPIO-idővisszalépés vagy eszközhiba mérgezi a sink-epochot;
explicit cold_reset rekonstruálja. Gépi részreset megtartja a külső LCD-t.
129 CPU-felosztás,busy/read-latch/command/DDR/reset/fail-stop célzott ASan/UBSan
PASS. Az első fixture tévesen R/W-t és E-t egyszerre ejtette,ami valódi írásélt
hozott létre és busy-t indított;helyes read-falling,majd RW-váltás sorrendre javítva.

A probe opcionális lcd-16mhz profilja16 ciklus/µs-t használ;eredeti yman1x.cpp
H83002(config,16_MHz_XTAL) alapján explicit kutatási profil,nem hardveridő-mérés.
Alapértelmezett no-LCD működés megmarad. ROM-free CLI-tesztek mindkét profilt és
ismeretlen profil elutasítását ellenőrzik. Valós helyi változatlan1.04 ROMmal
szelet1/64/257 mellett azonos új stop:FFFFD2 P9 byte read,PC0x3F66A,
perifériaciklus482170,10 GPIO,0 TX/clock él. KorábbiFFFFD6 olvasáson túljutott.
Nem teljes boot vagy hang. Következő:P9 bemenetek bekötési szerződése bizonyíték
alapján,majd új korlátos próba. Szerződés:[an1x-lcd-machine.md](an1x-lcd-machine.md).
CI/review függőben.
Helyi114/114 CTest (50,76s),100 Python,célzott strict ASan/UBSan PASS.
Forrásaudit602 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer33 csoport.

## LC7985 állapot/memória lezárása — 2026-11-03

A #190 beolvadt `0186721943c8243203ca58391317a69cc0dc0253` commitként.
Pontos head:`62279dac4602d6c3a5ede47a999e770da491eab4`;mind5 CI sikeres.
Futások:41578172689,41578172934,41578172711. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:explicit órajelű LCD-sink bekötés,GPIO/reset/fail-stop tesztek,
majd változatlan helyi firmware megfigyelése;előbb main CI ellenőrzése.

## LC7985 állapot és memória — jelölt,2026-11-03

Önálló headless vezérlő80 bájt DDRAM-mal és64 bájt CGRAM-mal,cím/mód/entry/
display/shift állapottal. A #189 busy-órát használja;utasítás busy alatt
figyelmen kívül marad,de eredeti dr_r/dr_w szerint az adatút nem busy-gated.
Egy/két soros címátmenetek,entry-shift,statuscím és reset kezelve. Nincs render,
font-ROM vagy firmware-probe-bekötés. Érvénytelen DDRAM-adatindex elutasított,
nem az eredeti potenciális tömbtúlindexelés reprodukciója.
114688 független memóriaeset,cím/status/ready/reset/GPIO-latch célzott ASan/UBSan
PASS.50000 művelet/állapot összevetés a hash-ellenőrzött eredeti MAME kilenc
változatlan metódusával PASS,biztonságos címprofilban. Ideiglenes harness,nincs
repo-ba másolt eredeti implementáció vagy Yamaha-adat. Ez nem hardverelfogadás.
Szerződés:[lc7985-controller.md](lc7985-controller.md). CI/review függőben.
Helyi113/113 CTest (50,56s),100 Python,strict ASan/UBSan és eredeti referencia
PASS;forrásaudit599 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer32 csoport.

## LC7985 busy-óra lezárása — 2026-11-03

A #189 beolvadt `d95364222f124ba6cec967de50f7b3427a0ff086` commitként.
Pontos head:`8cb6d09029aa55d9d33ef38246352e45088a9614`;mind5 CI sikeres.
Futások:41570074733,41570074872,41570074684. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:LC7985 cím/mód/memória állapotkezelés,előbb main CI ellenőrzése.
A busy-óra önálló komponens,firmware-probe-ba még nincs bekötve.

## LC7985 busy-óra — jelölt,2026-11-03

Önálló időkomponens:reset10000µs,beküldött utasítás busy alatt teljesen
figyelmen kívül hagyandó;00 nem indít busy-t,01/02/03=16400µs,minden04..FF
utasítás40µs a rögzített eredeti MAME ir_w prioritása alapján. Adathozzáférés
nem használja ezt az API-t;az eredeti dr_r/dr_w nem indít busy-t.
Explicit egész ciklus/µs skála,határpillanatban kész,monoton idő,overflow
előzetes elutasítás állapotváltozás nélkül. Nincs LCD-memória vagy teljes
vezérlő/probe-integráció;ez még nem oldja fel a Port B boot-megállást.
768 parancs/skála eset,10001 időfelosztás,reset/határ/overflow strict ASan/UBSan
PASS. Rögzített eredeti LC7985 teljes fájlhash ellenőrizve,aktív differenciál
nem készült. Szerződés:[lc7985-busy-clock.md](lc7985-busy-clock.md).
Saját CI/review függőben;következő a vezérlő állapot/memória szerződése.
Helyi112/112 CTest (50,86s),100 Python,strict ASan/UBSan PASS.
Forrásaudit595 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer31 csoport.

## LCD GPIO adapter lezárása — 2026-11-03

A #188 beolvadt `93e4180be0948501845ccc7303b7b53008fa271d` commitként.
Pontos head:`e9eb1cefae3078ab56c9b5572f8607a4619b00e4`;mind5 CI sikeres.
Futások:41562056261,41562056282,41562056303. Automatikus review-eredmény
nem rögzített;nem állítunk sikeres automatikus review-t. Formális review és
nyitott review-szál nincs,konfliktus nincs,GitHub merge-state clean.
Következő:LC7985 vezérlőállapot/busy ciklusszerződés a rögzített eredetiből,
majd kis tesztelt implementáció;előbb main CI ellenőrzése.

## LCD GPIO–vezérlő adapter — jelölt,2026-11-03

A meglévő LcdBus bekötést ciklusjelölt vezérlőhívásokhoz kapcsoló önálló
adapter készült. PA/PB/P9 kimeneti szinteket fogad,status/data readet E-felfutás,
command/data writeot E-lefutás indít;azonos szint nem ismétel tranzakciót.
Olvasott érték latchelt;nem LCD-bemenet hibázik,más portkimenet kezeletlenként
jelzett. A busy flag,memória és időkonverzió a későbbi vezérlő feladata.
512 független bájt/RS eset,él/latchelt adat/idő/routing ellenőrzés strict
ASan/UBSan alatt PASS. A fake vezérlő csak tesztfixture,nem LCD-emuláció.
Nincs firmware-probe bekötés vagy új bootelfogadás. Következő:LC7985 állapot/
busy időzítés szerződés a rögzített eredeti forrásból,majd integráció.
Szerződés:[an1x-lcd-gpio.md](an1x-lcd-gpio.md). CI/review függőben.
Helyi111/111 CTest (78,59s),100 Python,ASan/UBSan PASS;sanitizer CI30 csoport.
Forrásaudit592 fájl,notice229 fájl/5 szöveg és diff-check PASS.

## Port B diagnosztika lezárása — 2026-11-03

A #187 beolvadt `cddf9afa7cad0e23c45e66223b4cf87f31d27c1d` commitként.
Pontos head:`6ac84539108e9c567a8d79f715bbae21bb9a287c`;mind4 CI sikeres.
Futások:41555012549,41555012530. Kutatási build útvonalszűrő miatt nem indult
(kizárólag dokumentáció). Automatikus kódreview ugyanazon headen találat nélkül
kész2026-11-03T00:45:49.831659Z-kor:
https://github.com/RobCZart82/yaman1x/pull/187#issuecomment-6592328238
Nyitott review-szál vagy konfliktus nincs. Következő:Port B külső protokoll
bizonyítékának vizsgálata;új fejlesztés előtt a main CI ellenőrzése szükséges.

A #186 utáni main mind5 CI-ellenőrzése sikeres. Változatlan1.04 ROM,privát
swap16,10000000 kért ciklus,1000000 callback-limit mellett a próba482160.
perifériaciklusnál megáll:byte data_readFFFFD6 (Port B),PC0x3F666,
instruction_state1864224/substate0. Szeletméret1/64/257 esetén azonos fault,
PC és perifériaidő;teljes visszatért szeletciklus rendre482158/482112/482132,
callback739440/116288/104976.9 GPIO,0 TX/clock él. A hibás szelet munkája nem
szerepel a completed_slice számlálóban,ezért ezek eltérése önmagában nem hiba.
Ez diagnosztikai részprofil,nem firmware-boot-,hardver- vagy hangelfogadás.

Rögzített eredeti MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
h83002.cpp132. sor:FFFFD6 portb port_r/dr_w;159. sor utáni konfiguráció
PORT_B,resetDDR00,mask00. Ellenőrzött privát archívumpillanatképből olvasva.
A chipcímtér nem bizonyítja az AN1x külső huzalozását vagy bemeneti értékét.
Következő:Port B külső eszköz/protokoll szerződés vizsgálata meglévő kutatási
forrásokból,majd explicit bemeneti modell és független tesztek. A hibajelzést
nem oldjuk fel bizonyítatlan fix bemenettel. ROM SHA előtte/utána azonos:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.

## ITU üres státuszprofil lezárása — 2026-11-03

A #186 beolvadt `d869fa7de2246e1e4001ede06c2288488acb067e` commitként.
Pontos head:`0573354bb2be692d45ce8c3eda12645a25faad4c`;mind5 CI sikeres.
Futások:41542980274,41542980458,41542980204. Automatikus kódreview találat
nélkül kész ugyanazon headen2026-11-02T23:09:25.034545Z-kor:
https://github.com/RobCZart82/yaman1x/pull/186#issuecomment-6590790696
Nyitott review-szál és konfliktus nincs. Friss mainből következő kutatás:
ellenőrizhető boot-checkpoint és hosszabb korlátos diagnosztika;100000 ciklus
önmagában továbbra nem boot- vagy hangelfogadás. A main mind5 CI-ellenőrzése is sikeres.

## ITU megszakítások letiltott állapota — jelölt,2026-11-03

Öt TIER csak letiltott IRQ-profilt fogad:32 raw elfogadott/224 tiltott érték
csatornánként. Öt TSR üres jelzőállapotot ad;mind256 írás utánF8 marad,
szoftver nem állíthat flaget. TSTR tiltott,nincs aktív counter/event/IRQ.
Gyártói10.2.12–13 és rögzített eredeti tier_update/isr_update/isr_to_sr alapján.
2560 értékeset,alias/reset/no-effect/word és129 CPU-vágás ellenőrizve.
Helyi validáció:110/110 CTest (60,27s),100 Python,strict ASan/UBSan PASS.
A változatlan helyi ROM swap16 próbája100000 kért ciklust teljesít hiba nélkül:
25339 callback,perifériaciklus100002,PC634348,9 GPIO,0 TX/clock él.
Ez korlátos futási megfigyelés,nem teljes firmware-boot vagy hangelfogadás.
Szerződés:[an1x-itu-irq-idle.md](an1x-itu-irq-idle.md). Saját CI/review függőben.
Forrásaudit589 fájl,notice229 fájl/5 szöveg,diff-check PASS;ROM SHA változatlan.

## ITU I/O-profil lezárása — 2026-11-02

A #185 beolvadt `74bd4e0990c02d3b084d40c62e98e71ee343a7c1` commitként.
Ellenőrzött head:`adaa6e3d0e7140cdbb149a02a270719a4443f882`;mind5 CI sikeres.
Futások:41534258437,41534258773,41534258263. Automatikus kódreview ugyanazon
headen2026-11-02T22:08:31.050236Z-kor találat nélkül kész;bizonyíték:
https://github.com/RobCZart82/yaman1x/pull/185#issuecomment-6589650975
Nyitott review-szál és változtatáskérés nincs. Következő:FFFF66 TIER0 és a
leállított ITU statusflag/IRQ-engedély határok vizsgálata friss mainből.

## ITU compare-mód kimenet nélkül — jelölt,2026-11-02

Öt TIOR csak no-output compare profilt fogad:4 raw/252 tiltott csatornánként,
readback/reset88. Nincs jelalak/capture/külső él/compare-event/IRQ,csatornák állnak.
Gyártói10.2.11 alapján;eredeti MAME base TIOR read00/write-ignored stub,ebből
nem állítunk aktív I/O referenciát.1280 csatornaérték,alias/reset/no-effect/word
és129 CPU-vágás. Korábbi negatív kontroll a támogatott65 helyett69-et ellenőriz.
Firmware88 írása teljesül,következő stopFFFF66 TIER0,PC0xA7E,ciklus315;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-io.md](an1x-itu-io.md).
Helyi validáció:109/109 CTest (60,44s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit587 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan28 csoport;saját CI/review és merge függőben.

## ITU-csatornavezérlők lezárása — 2026-11-02

A #184 beolvadt `3e005e001022c5ac11272eb754cee6a0cda75bfd` commitként.
Ellenőrzött head:`41eb7461e9e530abe25b5f36118119e9f45fb719`;mind5 CI sikeres.
Futások:41525537583,41525537762,41525537584. Automatikus kódreview ugyanazon
headen2026-11-02T21:09:22.713478Z-kor találat nélkül kész;bizonyíték:
https://github.com/RobCZart82/yaman1x/pull/184#issuecomment-6588496395
Nyitott review-szál és változtatáskérés nincs. Következő:FFFF65 TIOR0
compare/no-output és capture korlátok vizsgálata friss mainből.

## Leállított ITU-csatornavezérlők — jelölt,2026-11-02

Öt TCR tárolja az óra/él/counter-clear konfigurációt,reserved bit7 read-one,
reset80. TSTR továbbra tiltott,nincs aktív számláló/külső él/clear/IRQ/esemény.
Gyártói10.2.10 és rögzített eredeti tcr_w/tcr_update alapján,aktív counter-
differenciál nélkül.1280 érték,alias/owner/reset/no-event/word és129 CPU-vágás.
Firmware83 írása teljesül,következő stopFFFF65 TIOR0,PC0xA7A,ciklus307;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-control.md](an1x-itu-control.md).
Helyi validáció:108/108 CTest (95,67s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit585 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan27 csoport;saját CI/review és merge függőben.

## ITU általános regiszterek lezárása — 2026-11-02

A #183 beolvadt `89267c4ef11dd5d08f286433baff32624cce52ba` commitként.
Ellenőrzött head:`9603b0067c2e08e0f89e20e32d166f83667151d0`;mind5 CI sikeres.
Futások:41516087446,41516087367,41516087375. Review-szál és változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:FFFF64 TCR0
óra/él/counter-clear konfigurációja leállított ITU-n,friss mainből.

## Leállított ITU általános regiszterek — jelölt,2026-11-02

Tíz GRA/GRB tároló szó/bájt hozzáféréssel,FFFF cold/reset. Önálló big-endian
bájtsávok,24-bit alias,páratlan szó előzetes elutasítás. Nincs aktív ITU-start,
compare/capture/flag/buffer/IRQ;counter és vezérlők továbbra tiltottak.
Rögzített eredeti tgr_r/w és gyártói10.2.8/10.3.1 alapján,aktív timer-differenciál
nélkül.655360 szóérték,bájtsávok,regiszterizoláció,reset/RAM/idő/no-event és129
CPU-vágás. Firmware következő stopFFFF64 TCR0,PC0xA76,perifériaciklus299;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-general.md](an1x-itu-general.md).
Helyi validáció:107/107 CTest (65,44s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit583 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan26 csoport;saját CI/review és merge függőben.

## ITU-kimenettiltás lezárás — 2026-11-02

A #182 beolvadt `bdf0ff6d14f1e2eea7f4486f927177a056b22d93` commitként.
Ellenőrzött head:`f4be4b9d717524c0290dfe2ecf271f8934ae1c3b`;mind5 CI sikeres.
Futások:41503635803,41503636111,41503635895. A research-macOS várakozó
feladatát nem indítottuk újra;végül sikeresen lefutott. Review-szál és
változtatáskérés nincs;automatikus review-eredmény nem volt rögzítve.
Következő:FFFF6A GRA0 és a leállított ITU általános regiszterek word/byte/reset
szerződése friss mainből,aktív counter vagy compare-esemény feltételezése nélkül.

## ITU-kimenetek letiltása — jelölt,2026-11-02

Gyártói10.2.5 alapjánTOER hideg/resetFF,de a firmwareC0 írása minden kimeneti
engedélyt töröl.4 raw elfogadott/252 engedélyező elutasított. Nincs aktív pinfunkció,
outputcallback/PWM/counter/IRQ;TOCR és páros word tiltott. Eredeti MAME-kezelő
read00/write-ignored stub,ettől a dokumentált kézikönyvi profil eltér.
256 érték,hideg/tiltott/reset különbség,alias/RAM/idő/word és129 CPU-vágás.
Firmware továbbjut,következő stopFFFF6A GRA0 szóírás,PC0xA6A,perifériaciklus277;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-output.md](an1x-itu-output.md).
Helyi validáció:106/106 CTest (59,64s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit581 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer25
csoport;saját CI/review és merge függőben.

## ITU normálmód lezárás — 2026-11-02

A #181 beolvadt `45ffd942a8d95f223622533684fc42df30681bc6` commitként.
Ellenőrzött head:`1b75363ddc73461c807ddd910a40893f05ac8130`;mind5 CI sikeres.
Futások:41494316497,41494316508,41494316498. Review-szál és változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:FFFF90 ITU
kimenetengedélyező szerződése és fizikai kimeneti korlátok vizsgálata friss mainből.

## ITU normál mód és puffer nélküli funkció — jelölt,2026-11-02

Gyártói10.2.3–10.2.4 alapjánTMDR csak normál mód,FDIR konfiguráció tárolt;
4 raw elfogadott/252 tiltott. TFCR normál/puffer nélküli profil,szintén4/252.
Eredeti rögzített MAME-kezelők itt read00/write-ignored stubok,nem aktív referencia.
Nincs PWM/fázisszámlálás/buffertranszfer/counter/IRQ;FDIR még nem hajt overflow-t.
512 regiszterérték,alias/reset/RAM/idő/word és129 CPU-vágás. FirmwareA0 írása
most teljesül,következő stopFFFF90 output-enable,PC0xA62,perifériaciklus263;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-mode.md](an1x-itu-mode.md).
Végleges helyi validáció:105/105 CTest (51,37s),100 Python,strict célzott
ASan/UBSan PASS. Forrásaudit579 fájl,notice229 fájl/5 szöveg,diff-check PASS.
CI sanitizer24 csoport;saját CI/review és merge függőben.

## ITU-szinkronprofil lezárás — 2026-11-02

A #180 beolvadt `7c3b7a05dc2bf19fe7e6da3c010196c633d67907` commitként.
Ellenőrzött head:`24244db847b0fac75afd9e0e4fb3bbb8e5df0b02`;mind5 CI sikeres.
Futások:41484649901,41484650551,41484649978. Review-szál és változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:FFFF62 TMDR
módvezérlő gyártói szerződésének és eredeti stubkorlátjának vizsgálata friss mainből.

## Megtartott ITU-szinkronprofil — jelölt,2026-11-02

Gyártói10.2.2 alapjánFFFF61 TSNC (MAME neveTSYR),readbackE0. Csak független
csatornák,8 raw elfogadott/248 szinkronengedély elutasított. Az eredeti rögzített
MAME-kezelő itt stub:read00/write ignored;ettől a gyártói profil tudatosan eltér.
Nincs aktív ITU-szinkronizálás/counter/IRQ vagy eredeti runtime-differenciál állítva.
256 érték,alias/reset/RAM/idő/word és129 CPU-vágás. Változatlan helyi firmwareE0
írása teljesül,következő stopFFFF62 TMDR,PC0xA5A,perifériaciklus247;
192 teljes szeletciklus/55 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-sync.md](an1x-itu-sync.md).
Helyi validáció:104/104 CTest (58,03s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit577 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer23
csoport;saját CI/review és merge függőben.

## Nyugalmi ADC lezárás — 2026-11-02

A #179 beolvadt `48ed518157ae3850b2cae70e89aa8a4ae8a4f5e1` commitként.
Ellenőrzött head:`c7ea69da70b221046536b03a608099040ff03475`;mind5 CI sikeres.
Futások:41474846838,41474846627,41474846596. A research-macOS igazolt
hosted-arm64 kapacitáshiba utáni újrapróbálása sikeres. Review-szál és
változtatáskérés nincs;automatikus review-eredmény nem volt rögzítve.
A #178 merge-commit main-futásainak két macOS kapacitáshibája is rendezett:
41471009802 és41471009805 végső feladatai sikeresek,MSan korábban sikeres.
Következő:FFFF61 ITU TSYR inicializálás szerződésének vizsgálata friss mainből.

## Nyugalmi ADC-konfiguráció — jelölt,2026-11-02

Friss main,#178 után. ADCSR FFFFE8 csakADST0/ADIE0:csatorna/scan/óra tárolt,
ADF szoftveresen nem állítható.64 elfogadott/192 elutasított raw érték.
Nincs analóg minta/konverzió/eredmény/trigger/IRQ;ADCR és páros word tiltott.
Gyártói14.2.2 és rögzített eredeti ADC-kezelők alapján,aktív ADC-differenciál nélkül.
256 érték,alias/reset/RAM/idő/no-effect/word és129 CPU-vágás. Firmware03 írása
most teljesül,következő stopFFFF61 ITU TSYR,PC0xA56,perifériaciklus239;
192 teljes szeletciklus/55 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-adc-idle.md](an1x-adc-idle.md).
Helyi validáció:103/103 CTest (58,11s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit575 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer22
csoport;saját CI/review és merge függőben.

## Aktív buszóra lezárás — 2026-11-02

A #178 beolvadt `51226f9524ca0c1cc39d487b8c31b7232c8ccddf` commitként.
Ellenőrzött head:`63e76d1f7cd3adc5fd0c5066311b5279e318f29e`;mind5 CI sikeres.
Futások:41466453406,41466453320,41466453532. Nyitott review-szál és
változtatáskérés nincs;automatikus review-eredmény nem volt rögzítve.
Következő:FFFFE8 ADC-megállás szerződésének vizsgálata és a még hiányzó
refresh IRQ/eseménybekötés logikus sorrendjének meghatározása friss mainből.

## Aktív refresh buszóra — jelölt,2026-11-02

A #177 beolvadt `3f0baf6696bedd423650dc2aeb094bddfd6f5aaf` commitként;
ellenőrzött head934936237d7d8fcb9cfc24d8dc2b317afe12052d,mind5 CI sikeres.
Review-szál/változtatáskérés nincs,automatikus review-eredmény nincs rögzítve.
Az önálló timer most a busz abszolút idejével halad,hét osztóval;CMIE továbbra
elutasított. Nincs IRQ-deadline vagy fizikai refresh/DRAM. Reset megtartja az időt.
Független hét ciklusorákulum,reset/CMF/debt és257 CPU-felosztás;eredeti524288
összehasonlítás továbbra PASS. Változatlan helyi ROM192 teljes szeletciklus/55
callback utánFFFFE8 ADC-írásnál áll meg,PC0xA44,perifériaciklus204,9 GPIO-jelzés.
Nem teljes boot vagy hang. Szerződés:[an1x-refresh-clock.md](an1x-refresh-clock.md).
Helyi validáció:102/102 CTest (63,59s),100 Python,célzott strict ASan/UBSan
és524288 eredeti referencia-összehasonlítás PASS. Forrásaudit573 fájl,notice229
fájl/5 szöveg,diff-check PASS. CI sanitizer21 csoport;saját CI/review függőben.

## Önálló aktív refresh-timer — jelölt,2026-11-02

Friss main,#176 után. Külön komponens,nincs még gépi/IRQ bekötés. Hét osztó,
abszolút fázis,compare/CMF,CMIE kéréslekérdezés,reset és korlátos deadline.
Független ciklusorákulum és16416 felosztás;524288 összehasonlítás változatlan,
privát snapshotból fordított eredeti MAME-kezelőkkel strict ASan/UBSan alatt.
Eltérő túlcsordulás/compare-fölötti számláló és CMF read-gate korlát dokumentált.
FFFFAD gépi megállás változatlan. Következő:eseményidővonal/IRQ/CPU bekötés.
Szerződés:[h8-refresh-timer.md](h8-refresh-timer.md).
Helyi validáció:101/101 CTest (50,40s),100 Python,célzott és eredeti referencia
ASan/UBSan PASS. Forrásaudit571 fájl,notice229 fájl/5 szöveg,diff-check PASS.
CI sanitizer20 csoport;saját CI/review és merge függőben.

## Leállított timer lezárás — 2026-11-02

A #176 beolvadt `ba29551ec8c3d4ab4da6bb7ec38d48f4381b84bf` commitként,
2026-11-02T13:53:22Z. Ellenőrzött head:`00157c36bdc90f19f7c3ac6a797c3fad630bdbf5`.
Mind az öt végső CI sikeres;futások41450001106,41450001129,41450001118.
Két korábbi macOS feladat hosted-arm64 kapacitáshiány miatt szakadt meg;
célzott újrapróbálás után sikeresek. Review-szál/változtatáskérés nincs;
automatikus review-eredmény nem volt rögzítve. Következő az aktív órajel,
compare és IRQ függetlenül ellenőrzött szerződése friss mainből.

## Leállított refresh-timer alap — jelölt,2026-11-02

Friss main,#175 után. RTMCSR csakCKS0/CMIE0 (16 raw érték),CMF szoftveresen
nem állítható;RTCNT/RTCOR bájtos tárolás és támogatott reset00/FF. Teljes256
vezérlés,65536 számláló/compare pár,alias/word/reset és129 CPU-vágás.
Eredeti rögzített MAME forrás és gyártói7.2.2–7.2.4 alapján;aktív timer
referencia-differenciál még nincs. Futó óra/IRQ továbbra tiltott;a helyi ROM0F
írásaFFFFAD címen ezért változatlanul megáll. Ez előkészítés,nem boot-előrelépés.
Szerződés:[an1x-refresh-stopped.md](an1x-refresh-stopped.md).
Validáció:teljes futás99 meglévő teszt PASS;az új teszt kivételtípus-javítás
utáni célzott CTest is PASS (0,35s),így mind100 csoport ellenőrzött.
100 Python,strict célzott ASan/UBSan,forrásaudit567 fájl,notice229 fájl/5 szöveg
és diff-check PASS. CI sanitizer19 csoport;saját CI/review függőben.

## RFSHCR lezárás — 2026-11-02

A #175 beolvadt `fa6af2f88821960bddcb1bad3858ffde4c13c44c` commitként.
Ellenőrzött head:`fb41b93953de9badfd0bb2bdc1417047a75f2b5e`;mind az öt CI sikeres.
Futások:41441860362,41441860316,41441860278. Nyitott review-szál/változtatáskérés
nincs;automatikus review-eredmény nem volt rögzítve. Következő:RTMCSR/timer
szerződés vizsgálata friss mainből,engedélyezett időzítés feltalálása nélkül.

## Megtartott RFSHCR profil — jelölt,2026-11-02

A beolvadt #174 után friss mainből:gyártói7.2.1 alapján FFFFAC=RFSHCR.
Csak00/02 írás fogadott,readback02;254 más érték elutasított. Timer/DRAM/PSRAM,
refresh-pin/mux/ciklus nincs szimulálva. Eredeti MAME map nem kezeli RFSHCR-t;
FFAD–FFAF timer címek továbbra tiltottak. 256 érték,alias/reset/RAM/idő/word
és129 CPU-vágás ellenőrzés. Részletek:[an1x-refresh-profile.md](an1x-refresh-profile.md).
Változatlan helyi ROM következő megállásaFFFFAD,PC0xA1C,perifériaciklus124;
64 teljes szeletciklus/19 callback,nem teljes boot vagy hang.
Helyi validáció:99/99 CTest (56,78s),100 Python és célzott strict ASan/UBSan PASS.
Forrásaudit565 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer18 csoport.
Saját CI/review és beolvasztás függőben.

## BRCR lezárás — 2026-11-02

A #174 beolvadt `e0bdde0048214930dc1a656b083b94af96147fc5` commitként.
Ellenőrzött head: `80097f42a2380b584e511d31286d4f3ea303f988`;mind az öt CI sikeres.
CI-futások:41435081433,41435081517,41435081672. Review-szál és változtatáskérés nincs;
automatikus review-eredmény nem volt rögzítve,nem tekintjük sikeres automatikus review-nak.
Következő lépés a 0xFFFFAC regiszter forrásalapú azonosítása friss mainből.

## Megtartott BRCR profil — korábbi jelölt, 2026-11-02

A #173 beolvadt `876050a1dd299dd44b9b95742d5b8067ecbc9404` commitként;
öt végső CI sikeres az e6fe544f12d00e858bce536800147c18b81954ed headen.
A macOS feladatok igazolt hosted-arm64 kapacitáshiány után újrapróbálva sikerültek;
kódhiba vagy szabálymegkerülés nem kellett. Automatikus review ugyanazon headen
2026-11-02T09:32:38Z-kor találat nélkül kész;bizonyíték a PR botösszegzésében.
Gyártói6.2.5 alapján BRCR0xFFFFF3 változatlanFE profil:16 tartalékbit-normalizált
írás elfogadott,240 buszátadást/cím-pinfunkciót módosító érték elutasított.
Nincs BREQ/BACK/arbitráció vagy fizikai pinmux-szimuláció;a logikai24-bit map marad.
256 érték,alias/reset/memória/idő,word előzetes elutasítás és129 CPU-vágás tesztelt.
A változatlan helyi ROM már64 teljes szeletciklust/19 callbacket teljesít,majd
0xFFFFAC bájtírásnál áll meg,PC0xA18,perifériaciklus116,öt GPIO-outputjelzéssel.
Ez nem teljes boot vagy hardveridő. Szerződés:[an1x-bus-release.md](an1x-bus-release.md).
Következő0xFFFFAC forrás/hardver-szerződésének azonosítása és korlátos kezelése.
Helyi validáció:98/98 CTest (50,51s),100 Python,célzott strict ASan/UBSan PASS.
Forrásaudit563 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer17 csoport.
Az IRQ-hole teszt korábbi BRCR-ismeretlencím elvárását frissítettük;word-őrök megmaradtak.
Saját CI/review és beolvasztás függőben.

## Programozott wait-state részprofil — jelölt, 2026-11-02

A #172 beolvadt `5aadb6279ea98f608550453b06953a7676e5372f` commitként;
öt CI sikeres aff9b87f42f56c806a3df3e3eb0e050df0a1d15b7 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Gyártói6.2.3/6.2.4 alapján WCR/WCER bájtoskezelés:programozott0–3 várakozó
állapot vagy controller-wait nélkül;pin-wait1/auto írás elutasítva. WCER-disabled
pin-wait0 ág explicit WAIT-inaktív kutatási feltétel,nem mért AN1x állapot.
WCR felső bitek read-one,WCER minden bit tárolt;csak engedélyezett külső3-state
ROM/SRAM kap3+count költséget. Kétállapotú/belső hozzáférés nem nyúlik meg.
Buszköltség-őr2..6-ra bővül;hideg/reset profilASTCR00/WCRF4/WCERFF,nem fizikai
alapállapot. Páros word és ismeretlen MMIO továbbra fail-closed.
256 WCR/256 WCER érték,független területmaszk,idő/counter/alulcsordulás,
reset/alias/word és257 CPU-vágás. Eredetnyilvántartás frissítve.
A változatlan helyi ROM már BRCR0xFFFFF3 írásnál áll meg,PC0x9FC,
perifériaciklus60. Teljes boot/hang/plugin továbbra sincs elfogadva.
Szerződés:[an1x-wait-profile.md](an1x-wait-profile.md). Következő korlátos BRCR profil.
Release build és mind97 CTest sikeres (81,60s);100 Python és strict célzott
ASan/UBSan sikeres,UINT64_MAX/overflow időhatárral is. CI sanitizer-szűrő16
csoportot választ. Forrásaudit561 fájl,notice229 fájl/5 szöveg,diff-check sikeres.
CI/review és beolvasztás függőben.

## ASTCR kiválasztott hozzáférési idő — jelölt, 2026-11-02

A #171 beolvadt `ecee85e7f8daf1039fac431e420072e8fe9e6491` commitként;
öt CI sikeres ac71f992a2c03049e6da13413765afa167c43f458 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Gyártói kézikönyv6.2.2:ASTCR0xFFFFED területenként2/3 állapotot választ.
A firmware0F-et ír,ezért a puszta latch nem lett volna elég. A buszsegéd új,
őrözött,pure access_cycles lekérdezése2/3 ciklust számol;default host továbbra2.
ROM/SRAM az ASTCR alsó bitjeit követi,belső területek régi2 ciklusos kutatási
költsége marad. ASTCR minden256 értéke tárolt. Reset00 kifejezetten no-wait kutatási
profil,NEM a kézikönyv fizikai FF resetje. WAIT/WCR/WCER késleltetés még nincs.
Független idő/counter/címteszt mind5 buszműveletre,költséghatár/reentry/alulcsordulás,
256 regiszterérték és193 CPU-vágás;adaptációs eredetnyilvántartás frissítve.
A változatlan helyi ROM most WCER0xFFFFEF bájtírásnál áll meg,PC0x9F4,
perifériaciklus44. Ez kutatási modell,nem hardveridő vagy bootelfogadás.
Részletek:[an1x-access-state.md](an1x-access-state.md). Következő WCER/wait-state profil.
Release build és mind96 CTest sikeres (72,86s);100 Python és strict célzott
ASan/UBSan sikeres. CI sanitizer-szűrő pontosan15 csoportot választ.
Forrásaudit559 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben.

## ABWCR leképezett-területi profil — jelölt, 2026-11-02

A #170 beolvadt `a9c1a2d4c3b6924463269c8985f38f62e1aa8611` commitként;
öt CI sikeres a19483ca548d73031933c523598568c9afda00fbd headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
A gyártói H8/3002 kézikönyv6.2.1 alapján0xFFFFEC az ABWCR. Projektprofil:
ROM/SRAM terület0/1 marad16-bit,ezért alsó2 bit nem állítható;felső6 bit
visszaolvasható,de az érintett külső területek továbbra sincsenek leképezve.
64 érték támogatott,192 elutasított. Reset00 explicit kutatási mód,nem igazolt
AN1x mode-pin állapot. Word/szomszédos ASTCR-hozzáférés továbbra is fail-closed.
Az első00-only policy a firmware08 írását elutasította;csak a területazonosítás
után bővült a megengedett profil. Forrás:[an1x-bus-width.md](an1x-bus-width.md).
Független256 érték,alias/reset/word/szomszédteszt,129 CPU-folytatási vágás.
A helyi változatlan ROM swap16 próbája már ASTCR0xFFFFED írásnál áll meg,
PC0x9F0,perifériaciklus36 (korábbanABWCR30). Teljes boot/hang/plugin nincs.
Release build és mind95 CTest sikeres (62,02s);100 Python és strict célzott
ASan/UBSan sikeres. CI sanitizer-szűrő pontosan14 csoportot választ.
Forrásaudit557 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben.
Következő:ASTCR és támogatott buszidőzítési profil célzott vizsgálata.

## Korlátos firmware-boot próba — jelölt, 2026-11-02

A #169 beolvadt `4cdc916e3bdd6e31eda23adfb184955b753b4ac1` commitként;
öt CI sikeres aza95352d262a4149ace591b908913ae5f76025938 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új in-process CLI explicit raw/swap16 memórianézettel,szigorú ciklus/szelet/callback
korlátokkal és JSON stopjelentéssel. A ROM csak olvasott,nem másoljuk repository-ba.
Hibás buszhozzáférés cím/típus/szélesség metaadata látható;ismeretlen GPIO-input
fail-closed. Nincs kitalált PKS/MIDI/LCD/DSP válasz vagy bootelfogadás.
Szintetikus CLI-teszt ellenőrzi a progresszt,ROM/MMIO/GPIO hibát,dekóder/callback
megállást,argumentumokat,bájtsorrendet,méretet és a változatlan bemeneti SHA-t.
Az első dekóderkontroll FFFF-je érvényes MOV volt;javítva invalid EEPMOV-kiterjesztésre.
Helyi1.04 ROM explicit swap16 nézetben reset0x9D8,első stop0xFFFFEC bájtírás,
PC0x9EC,perifériaciklus30. Eredeti SHA előtte/utána azonos. A kezdeti nyersnézet
bájtsorrendhibás kísérlet volt,nem firmware-bizonyíték;oddreset-őr bekerült.
Részletes szerződés:[an1x-boot-probe.md](an1x-boot-probe.md). Következő0xFFFFEC
forrás/hardver-szerződésének vizsgálata,majd célzott MMIO-fejlesztés.
Release build és mind94 CTest sikeres (51,24s);100 Python és strict célzott
ASan/UBSan CLI-teszt sikeres. CI sanitizer-szűrő pontosan13 csoportot választ.
Forrásaudit555 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben;teljes boot/hang/plugin továbbra sincs elfogadva.

## Gépszintű támogatott reset — jelölt, 2026-11-02

A #168 beolvadt `90e484efa59d66d7da0dae0ade34022afb11aefe` commitként;
öt CI sikeres a777d562505fe0966f8353d063238b6caf3e62f15 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új reset_supported_state gép-API:csak élő,álló gépen,CPU-adósság nélkül és nem
előrefutó perifériaidővel. Érvénytelen határ módosítás nélkül visszautasítva.
A jelen időpontra eső input előbb feldolgozódik,utána CPU-resetkérés,majd a
#168 szerinti buszperiféria-reset. Siker után külön reset-origin/számláló frissül;
abszolút idő,hideg generáció,RAM/GPIO és jövőbeli RX megmarad. Hideg újraépítés
külön nullás időpéldányt és nullás támogatottreset-számlálót ad.
Elkapott gépi reentry azonnal megjelöli a buszt is hibásnak,hogy ne legyenek
utána további kimenetek. Részleges resethiba fail-stop;hideg újraépítés helyreállít.
Teszt81 megállási pont:41 elfogadott,40 adósságos elutasítás;regiszter/RAM/idő,
literális SCI-kimenetsorrend,ismétlés,jövőbeli és pont időhatáron eső RX,reentry.
Ez a támogatott részhalmaz integrációja,nem fizikai RESET-assert/felengedés vagy
minden AN1x-eszköz elfogadása. Eredeti referencia MAME706dbb73281320c54d7c2f16262c1c3786f2be01
és rögzített reset/child-order források;új teljesgép upstream runtime trace nincs.
CI ASan/UBSan szűrő pontosan12 csoportot választ. Release build és mind93 CTest
sikeres (59,08s);100 Python és strict célzott ASan/UBSan sikeres. Elutasítás után
a ciklusadósság visszafizetése és az újabb reset is tesztelt. Rögzített reset és
konfigurációs források egyeznek;forrásaudit552 fájl,notice229 fájl/5 szöveg és
diff-check sikeres. CI/review és beolvasztás függőben.
Következő:korlátos firmware-boot diagnosztika és a hiányzó gépi MMIO/MIDI út felmérése.

## Támogatott buszperifériák resetje — jelölt, 2026-11-02

A #167 beolvadt `e0b8c9037913a054a8f37e5ccba19a0a571409ac` commitként;
öt CI sikeres a27ef4adae734e2beae33fb1611306c56242ccd10 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új busz API:reset_supported_peripherals álló tulajdonoson,SYSCR09/IRQ,majd
port4/6/7/8/9/A/B,SCI0/SCI1 sorrenddel. Az IRQ-szűrőt a megtartott CPU CCR-ből
újrapublikálja;fizikai szintek megmaradnak. RAM/GPIO-regiszter/inputütemezés és
abszolút idő változatlan;régi SCI-shifteresemények törlődnek. Az újra publikált
SCI órajel/TX-szintek a jelenlegi buszidőn jelennek meg. CPU-resetet nem indít.
A részhalmaz sorrendje projektbeli integráció,nem teljes MAME-eszközbejárás:
DMA/ADC/timer/watchdog/refresh és fizikai RESET-vonal nincs lefedve.
GPIO/SCI callbackben elnyelt reentry azonnal megállítja a további kimeneteket.
Független teszt3 abszolút kezdőidővel:szó szerinti kimenetsorrend,időbélyegek,
regiszterprofil,RAM/GPIO-megőrzés,tartott IRQ újralatch,függő RX,idempotens
GPIO-újrapublikálás és két reentry-határ. Az első elvárt nyom két GPIO-maszkját
javítottuk (port6:7F,port8:10 irány);motorkód miatt nem kellett módosítani.
A CI ASan/UBSan szűrő pontosan11 csoportot választ. Release build és mind92
CTest sikeres (72,06s);100 Python és strict célzott ASan/UBSan sikeres.
Rögzített reset/konfigurációforrások egyeznek;forrásaudit551 fájl,notice-lefedettség
229 fájl/5 szöveg és diff-check sikeres. CI/review és beolvasztás függőben.
Következő a CPU/busz reset összehangolása.

## Halasztott CPU-reset — jelölt, 2026-11-02

A #166 beolvadt `3bf3ed4dd4ff422b79918d8bc3889b812ec75e1f` commitként;
öt CI sikeres a 2b8528c0c2d7b57c1baa7925ceab9701b920b614 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új request_reset_after_debt CPU-átmenet csak álló,élő runneren:régi részutasítás
eldobása,reset-dispatch kijelölése,kért/felvett IRQ-könyvelés törlése.
Általános regiszterek,CCR és a már lezajlott buszhatások/ciklusadósság megmaradnak.
A következő futás előbb visszafizeti az adósságot,utána kezd reset-vektort olvasni.
Ez projektbeli halasztott CPU-szabály,nem fizikai RESET-vonal vagy teljesgép-reset.
A hostnak kell a perifériák/bemenetek resetjét külön rendeznie;gépszintű API nincs.
Független szó szerinti időnyom:3 abszolút kezdőidő,21 kérési pont,7 folytatási
vágás (441 eset),nemnulla regisztermegőrzés,ismételt kérés és reentry fail-stop.
Eredeti referencia változatlan:MAME706dbb73281320c54d7c2f16262c1c3786f2be01;
research/h83002-reset-contract.json és generált resetkezelők. Nem új teljes MAME
warm-reset runtime összevetés;az adaptációk eredetnyilvántartása frissítve.
Release build és mind91 CTest sikeres (85,62s);100 Python sikeres.
Strict célzott ASan/UBSan sikeres,a rögzített eredeti resetforrások egyeznek.
Forrásaudit550 fájl,notice-lefedettség229 fájl/5 szöveg,diff-check sikeres.
CI/review és beolvasztás még függőben.

## Gépi időtartomány-integráció — jelölt, 2026-11-02

A #165 beolvadt `09af5e8f9850d356f72c3557aa5c4bceb5ed74fe` commitként
(2026-11-02T02:42:54Z). Öt CI sikeres a bedad28721aa1d838a799c759ee9da3cfb13220e
headen;automatikus review-eredmény nincs rögzítve,nyitott review-szál nem volt.
A gép korábbi cursor_ számlálóját ResetTimeDomain abszolút ideje váltja fel.
Sikeres szelet végén frissül;hibás tartomány nem változtatja az időt,a részleges
végrehajtási hiba továbbra is fail-stop. Hideg újraépítés új nullás időpéldányt
kap,meglévő gépgenerációval. CPU-adósság és input-határidő nem kerül átírásra;
reset_origin nincs meghívva,nincs új meleg-reset API vagy hardveres elfogadás.
401 vágási ponton összevetjük az egyben/szétvágott CPU-állapotot,RX-fogyasztást
és TX-idősort;nullás/negatív szeletek és hideg újraindítás is ellenőrzött.
CI ASan/UBSan készlet bővül az idősegéddel és a teljes gépéletciklussal.
Helyi Release build és mind91 CTest sikeres (73,52s);100 Python sikeres.
Célzott strict ASan/UBSan gépteszt sikeres;CI-szűrő pontosan10 csoportot választ.
Forrásaudit550 fájl,notice-lefedettség229 fájl/5 szöveg és diff-check sikeres.
Távoli CI és végső review még függőben.
Következő:korlátos reset-owner szerződés a fennmaradó input/scheduler feltételekkel,
majd firmware-boot/MIDI és DSP-hangút;GUI és ROM-módosítás nélkül.

## Reset-időtartomány segéd — jelölt, 2026-11-02

A #164 beolvadt e620483c7d95e07aeb2bce69431fdb2a87c1fd03;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Új önálló ResetTimeDomain:
abszolút idő monoton,reset-origin/generáció külön,átváltási túlcsordulás elutasítva.
Három kezdőidő,300 reset,visszalépés és64 bites határesetek független tesztje.
Célzott strict ASan/UBSan sikeres. Még nincs bekötve a gépbe;CPU-adósságot,
inputsort vagy perifériát nem módosít,nem teljesgép-reset. A kezdeti fordítási
hiba hiányzó initializer_list fejléc volt,javítva. Következő a gép időkezelési
integrációja definiált nyugalmi határon.

## Reset időtartományok — jelölt, 2026-11-02

A #163 beolvadt d513d4fdf8ff7c9ba0eedc064fab9247a9355f64;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Négy eredeti időkezelési módszer
azonossága rögzítve:local_time,total_cycles,abort_timeslice,synchronize.
A resetelt CPU-ciklusszám nem azonos az abszolút perifériaidővel;szinkronizáció
zero-delay timerrel történik,nem közvetlen callbackkel. Következő:korlátos
időtartomány-szabály független tesztje,majd resetintegráció. Motorkód változatlan.
100 Python sikeres;teljes runtime készletet kutatási változás miatt nem futtattunk újra.

## Sorba állított RESET-vonal — jelölt, 2026-11-02

A #162 beolvadt36b71314dfae075b046b5b0bde34b92f0acb3c9b;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az eredeti empty_event_queue
callback azonossága is rögzítve. RESET ASSERT felfüggeszt,felengedés resetel,
majd folytat. A perifériakimenetek assertion alatti kezelésére upstream FIXME van.
A queue-index a callback végén törlődik,nem a device_input::reset-ben.
Új callback-azonosság teszt:megváltozott/hiányzó/dupla definíció.99 Python sikeres.
Motorkód változatlan;következő a scheduler-szinkronizáció és helyi idő vizsgálata.

## Framework owner-feloldás — jelölt, 2026-11-02

A #161 beolvadt a5b73ac6da7276f5afa4bbb1794b40a319a6a41f;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az eredeti mconfig.cpp teljes
fájlazonossága rögzítve. Forrásvizsgálat:resolve_owner kettőspontonként meglévő
szülőt keres,add_device a szülő listájának végére illeszt,majd konfigurál.
Ebből a nested DMA/timer-bejárás következik,nem22 közvetlen CPU-gyerek.
Ez forrásból levont következtetés,nem teljes runtime resettrace;SCI-base címkék,
konfigurációs overloadok és scheduler/bemeneti queue-idő továbbra is vizsgálandó.
Motorkód változatlan;98 Python és lockolt forrásazonosság sikeres.

## H83002 konstruktorcímkék — jelölt, 2026-11-02

A #160 beolvadt2b95cfe97bd6a292593871bbb51d0e407ab32988;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. A leltár most a konstruktor
finder-címkéit is ellenőrzi:DMA dma:0/1,időzítők timer16:0…4.
A SCI címkéit nem következteti ki a származtatott forrásból. Új független
teszt vizsgálja a beágyazott címkét és a hiányzó finder elutasítását.
Motorkód változatlan;a tényleges framework-owner/resetbejárás továbbra is nyitott.

## H83002 perifériakonfiguráció — jelölt, 2026-11-01

A #159 beolvadt0b61abcf2b03c28979f173a3cce30ec92f7bbd59;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az új leltár22 konfigurációs
bejegyzés sorrendjét és eredeti forrásazonosságát ellenőrzi. Különbség:
lexikai létrehozás nem egyenlő a tényleges szülő/gyerek resetbejárással.
Két új regressziós teszt:sorrendcsere/duplikáció és hiányzó módszer.
Motorkód nem változott;teljesgép-reset továbbra sincs. Következő:owner-feloldás,
eszközlista-beillesztés és idő/bemeneti eseménykezelés.

## Execution input reset pontosítása — jelölt, 2026-11-01

A #158 beolvadt a25f29c913c53e664d1019b6ce2d80dab694ee1e,
2026-11-01T21:42:20Z;négy CI sikeres,nem volt rögzített automatikus review-eredmény.
A negyedik rögzített framework-módszer a device_input::reset:csak az aktuális
és tárolt IRQ-vektort állítja alaphelyzetbe. Bemeneti szintet vagy queue-indexet
nem ír ebben a módszerben. A tágabb queue-törlési komment nem viselkedési bizonyíték.
A teljes reset-sorrend/idővonal továbbra is nyitott;motorkód nem változott.
95 Python sikeres;valódi lockolt archívum négy módszere egyezik.
Következő:gyerekek konkrét sorrendje és ütemezett bemenet/idő kezelés.

## Auditjavítás 3 — dokumentáció egységesítése, jelölt, 2026-11-01

A #157 beolvadt15217f7fe1ffa8e6894730b24357feac311c1c0d,
2026-11-01T19:52:16Z;pontos head49f64aa142409fef04fbf7393caeaada053971b0.
README/státusz/roadmap aktuális összefoglalója90 CTest/95 Python leltárra,
#154–157 beolvadt változásokra és a valóban nyitott gépintegrációs kapukra frissült.
A régi jelölt/pending bejegyzések történeti állapotként elkülönítve maradnak.
A teljes helyi90/95 bizonyíték #156-hoz,a nyolc sanitizer-csoport #157-hez tartozik.
Ez csak dokumentáció;runtime tesztet nem futtatunk újra. Jelölt,nem beolvadt.
Következő:execution-input reset,gyereksorrend és scheduler-idő kutatása/kis tesztelt
integrációja. Gépszintű reset,firmware/MIDI/PKS és hang továbbra sincs elfogadva.

## Auditjavítás 2 — sanitizer CI-lefedettség, jelölt, 2026-11-01

A #156 beolvadt a4f91cefc4ccfa7669e6dbc7eee3459d1137b43e,
2026-11-01T17:53:56Z;öt CI sikeres a39c421da743feecb3e5aea808f4cd012c9c026b9 headen.
Az új Ubuntu CI-lépés külön ASan+UBSan buildből futtatja a CPU-reset,SCI,
IRQ,GPIO,runner,busz,dispatcher és újrabelépési regresszió nyolc tesztjét.
A tesztszűrő nulla találatnál is hibát jelez (--no-tests=error).
Helyi ASan/UBSan:8/8 sikeres;a CI-regex pontosan8 regisztrált tesztet választ.
Motorkód változatlan;CI és napló módosul. A harmadik,dokumentációs egységesítési
csomag csak ennek beolvadása után következik. Jelölt,nem beolvadt.

## Hibakezelés és forrásazonosság — jelölt, 2026-11-01

A #155 beolvadt e581c7b36dad130efc3254fcd25b18c7f02ea5bb,
2026-11-01T17:32:32Z;négy CI sikeres a f9e2ae067ab0bed312be895aebc59daf30224511 headen.
Az első auditjavító csomag a dispatcher,CPU-runner és buszsegéd elnyelt
újrabelépési kivételét is végleges hibává teszi. A külső hívás sem adhat sikert,
és a hibás owner újra nem használható. A resetarchívumot hash-ellenőrzött
privát ideiglenes másolatból olvassuk;a forrásnév későbbi cseréje nem hat rá.
Új regressziós C++/Python kontrollok;95 Python és célzott ASan/UBSan sikeres.
Teljes build és90/90 CTest sikeres;95 Python sikeres;célzott ASan/UBSan sikeres.
A regressziós teszt a korábbi kódot elutasítja. Jelölt,nem beolvadt. A CI-bővítés és általános
állapotdokumentáció külön következő PR lesz,csak e csomag beolvadása után.

## #155 Windows SVG-hash javítás — 2026-11-01

A Windows CI 88 tesztet teljesített,de a repository-audit az SVG hashét elutasította.
A text=auto sortöréskonverzió módosította a bájtpontosan rögzített logót.
A .gitattributes most kizárja ezt az egy SVG-t a szövegkonverzióból;az eredeti
hash és az audit szigorúsága változatlan. A motor nem módosult.

## Arany vektorlogó a README elején — jelölt, 2026-11-01

A projektgazda kérésére az arany,valódi vektoros logó került a legfelső sorba.
A H1-blokk helyett kép és közvetlen sortörés következik,üres bekezdés nélkül;
a projektleírás közvetlenül alatta kezdődik. Az SVG átlátszó,beágyazott PNG nélkül.
Az asset ellenőrzőösszege és eredetleírása frissült. Motorkód változatlan.

## README logó és teljes aktualizálás — jelölt, 2026-11-01

A projektgazda átlátszó SVG-logója váltja a felső szöveges címet,520px szélességgel,
YamAN1x alternatív szöveggel és H1 szerkezettel. Pontos fájlhash-kivétel védi
az assetet;a forrás/engedély leírás frissült. A README tesztszámai89 CTest/94 Python,
a beolvadt és jelölt állapot elkülönül,a korábbi audit történeti dátuma egyértelmű.
A teljes README áttekintve;motor/ROM/GUI nem változott. A #154 része,még nem beolvadt.

## Keretrendszer reset-sorrend — jelölt, 2026-11-01

A #153 beolvadt1a2468855cf35ee5641d586bd1b6777a5e75f71f,
2026-10-31T19:07:13Z;aktuális head8f314a5c93471300b7ac6a478416c4d0514cd005.
Négy CI sikeres (41225516208,41225516296);sikeres automatikus review-t nem állítunk.
Az új archívumellenőrző két eredeti framework-fájl/három módszer azonosságát
rögzíti. Sorrend:interface pre-reset,szülő reset,gyerekek resetje,szülő utómunka,
interface post-reset. Az execution-interface külön törli a ciklusszámlálót és
reseteli a bemeneti objektumokat;ezért a komponensbemenet-megőrzési tesztek
nem bizonyítják a teljes framework viselkedését. Következő:bemeneti objektumok,
gyerek-létrehozási sorrend és scheduler-idő. Új melegreset-API nincs.
94 Python sikeres;teljes runtime készletet kutatási változás miatt nem futtattunk újra.
Az automatika továbbra is leállítva;ez a felhasználó által kért folytatás.

## README hardverfotó — jelölt, 2026-10-31

A projektgazda saját AN1x-fotója a README projektleírása alá került.
Angol képaláírás külön jelzi,hogy eredeti fizikai hardver,nem plugin vagy GUI.
Az eredeti képfájl változatlan;az upload-ellenőrző csak ezt az egy pontos
SHA256-azonosságú képet engedi át. Más bináris fájlok tiltása megmarad.
Külön képeredet/engedély leírás készült;új képlicencet nem feltételezünk.

## Aktuális összefoglaló az első REAPER-hangig — 2026-10-31

A #152 beolvadt `8b2930234de7b0b512f8ecdb9f04414782f14a53`,
2026-10-31T17:02:44Z (Budapest18:02:44). A pontos head
`cfca0c785364cb44f8d478735a2b8d82b84d16e7` négy CI-feladata sikeres:
41210850333 és41210850351. Az automatikus review kvótakorlát miatt nem készült el;
ez nem kódhibát bizonyító review,és nem sikeres review. Bizonyíték:
https://github.com/RobCZart82/yaman1x/pull/152#issuecomment-6544784804
A beolvadást élő GitHub-adat igazolja;az asszisztens korábban visszatartotta.
A húszperces automatika leállítva marad;ez a dokumentációfrissítés nem indítja újra.

Hátralévő fő lépések,függőségi sorrendben:

1. **Teljes gépintegráció:** CPU,busz,szükséges perifériák és DSP közös idővonala;
   indulási/reset-sorrend és megőrzött állapotok igazolása. A komponens-resettesztek
   beolvadtak,de gépszintű melegreset még nincs. Csak a firmware által ténylegesen
   igényelt hiányzó CPU/periféria-funkciókat kell célzottan pótolni.
2. **Firmware-indulás és MIDI:** a helyi eredeti firmware stabil indulása,
   PKS-kommunikáció szükséges viselkedésének tisztázása,SCI0 valódi byte-vétel,
   note-on/note-off firmware-szintű feldolgozás. A PKS ROM hiánya nincs megoldva;
   szükséges protokollhelyettesítés vagy további hardveres adat még vizsgálandó.
3. **Egy voice DSP-jének elfogadása:** táblarom-címzés,órajel,EG és visszaolvasások,
   CPU–DSP sorrend és chipközi jelút. A programfeltöltés önmagában nem hangbizonyíték.
4. **Első offline hang:** dokumentált száraz kimenet és PCM-skálázás;WAV note-on,
   tartás,note-off és lecsengés eseményekkel. Nemnulla,véges és reprodukálható jel.
   MEG csak akkor halasztható,ha a kiválasztott száraz hangút valóban működik nélküle.
5. **Valós idejű beágyazott motor:** időbélyeges MIDI,audioblokkok,mintavételi
   illesztés,előre foglalt memória,határidőmérés és beragadt hangok kezelése.
6. **Minimális VST3:** MIDI/audio adapter,példányonkénti állapot,helyi ROM-betöltés,
   csomagolás és validálás. GUI-fejlesztés továbbra sincs engedélyezve;az első próbához
   hosztparaméter/konfiguráció használható. AU és kidolgozott felület későbbi lépés.
7. **REAPER-elfogadás:** plugin felismerése,MIDI-hang indítása/elengedése,lehallgatás,
   WAV-mentés,offline összehasonlítás és projekt újranyitása rögzített környezetben.

Jelenleg nincs igazolt AN1x-hang,VST3 vagy AU. A sok sikeres CPU/perifériateszt
nem jelenti a firmware/DSP/hangút elfogadását. Az első hang legnagyobb nyitott
kockázata az indulási/MIDI–PKS út és az egy voice-hoz szükséges DSP-viselkedés.
A teljes hardver minden funkciójának emulálása nem előfeltétele az első hangnak.

## Korábbi előkészítési bejegyzések — történeti állapot

Az alábbi jelölt/függőben megjelölések a készítéskori állapotot őrzik.
A jelenlegi állapotot a fenti összefoglaló és a strukturált beolvadási napló adja.

## SCI reset utáni vétel — jelölt, 2026-10-31

A #151 beolvadt e4e79d4e0244a05784b7da705e9496f601446e99,
2026-10-31T14:14:36Z;négy CI sikeres,aktuális head review észrevétel nélkül
lezárult (issuecomment-6544283995).
Az új teszt mind256 bájtot fogadja reset után,1000 abszolút kezdőidővel.
Előtte73 ciklusnál megszakít egy régi vételt. Az alacsonyan tartott RX újraengedélyezése
nem képez hamis startélt;magasra váltás után az új keret pontos bájtot,SSR-t,
egyetlen vételi IRQ-t és hibamentes üres eseménysort eredményez. DMA-nyugtázás
csak az új vételi készjelzést törli. Ez komponensvizsgálat,nem gépszintű reset.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód változatlan;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## SCI reset utáni adás — jelölt, 2026-10-31

A #150 beolvadt1ef73ef6fe3f198c2a82d9e962a282860218dd79,
2026-10-31T13:34:33Z;négy CI sikeres,aktuális head review észrevétel nélkül
lezárult (issuecomment-6543843939).
Az új teszt mind256 bájtot újraküldi reset után,1000 abszolút kezdőidővel.
Előtte73 ciklusnál megszakít egy másik keretet. Független,bitenként és időponttal
megadott elvárás ellenőrzi a keretet,az egyszeri kész/befejezés IRQ-t és az
üres eseménysort. Ez komponenshelyreállítás,nem gépszintű melegreset.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód változatlan;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## IRQ-reset bemenetmegőrzés — jelölt, 2026-10-31

A #149 beolvadt1cb2f4a229fac079d5b318f5187ed2a340161838,
2026-10-31T12:54:35Z;négy CI sikeres,az aktuális head automatikus review-ja
észrevétel nélkül lezárult (issuecomment-6543431784).
Az új teszt65536 tartott IRQ/engedélyezési maszkot ellenőriz:reset után
újralatchelt szintek,törölt regiszterek,owner-frissítésig megtartott publikáció,
legalacsonyabb engedélyezett vektor,megtartott NMI-bemenet és új él elfogadása.
Ez komponensszerződés,nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt végső CI/review függőben.

## Aktív SCI reset teszt — jelölt, 2026-10-31

A #148 beolvadt1ec8ff3b657816e736930fe8029427409777f555,
2026-10-31T11:09:52Z;négy CI sikeres,automatikus review-eredmény nem volt rögzítve.
Az új komponensvizsgálat401 ciklusponton szakítja meg az egyidejű TX/RX-et:
clock magas majd TX magas,regiszteralapértékek,üres határidő,megőrzött alacsony
RX és régi befejezési/IRQ események hiánya. Ismételt reset mindkét magas
kimenetet újra publikálja. Ez nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest és ASan/UBSan,91 Python,eredeti resetazonosság sikeres.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## GPIO-reset megőrzési teszt — jelölt, 2026-10-31

A #147 beolvadt:b39643f330314f21e16a946c0e7e00e5a5eb41d0,
2026-10-31T09:59:11Z;négy CI sikeres,pontos head618667b31cd2ca7e6c80c063face8d844ed19aeb.
Automatikus review-eredmény nem volt rögzítve.
Az új teszt196608 maszk/irány/adat kombináción három ismételt resetet
ellenőriz. Független elvárás a DDR/DR/PCR/ODR megőrzése,a változatlan
kimenetértesítés elnyomása és a lábolvasás bitenként számolt eredménye.
Ez komponensvizsgálat,nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest,ASan/UBSan és91 Python sikeres;eredeti resetazonosság egyezik.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt végső CI/review még függőben.

## CPU-reset végrehajtási teszt — jelölt, 2026-10-31

A #146 beolvadt `41a1c1cea0b777653ede9e470ec5e6e7797b2ff2`,
2026-10-31T08:59:10Z;négy CI sikeres,review-eredmény nem volt rögzítve.
Az új független CPU-indulási teszt256 CCR×3 kezdőidő×7 szeletelés=5376
folytatási esetet ellenőriz:vektorolvasások,prefetch,filterközlés sorrendje,
megőrzött bitek,EXR,részállapot és időadósság. A filternek nincs időargumentuma;
a teszt az előző buszművelet idejét és a sorrendet rögzíti.
Strict fordítás és célzott ASan/UBSan sikeres;az eredeti generált MAME H8H
összehasonlítás200000 szeleten nulla eltérést mutat. Teljes91 Python sikeres.
Ez hideg CPU-indulás,nem új melegreset-API vagy teljesgép-reset bizonyíték.
Teljes build és89 CTest sikeres;CCR-törlési negatív kontroll6858 hibát jelez.
Végső CI/review függőben;a jelölt még nincs beolvadva.

## #145 lezárása és CPU-reset határvizsgálat — 2026-10-31

A #145 beolvadt: `e470f406c2d65eccda70e0c6381db1f826b2810b`,
2026-10-30T22:25:47Z;ellenőrzött head `90f8aea04fdddce17de60fe706b690b3b5f62c3b`.
Öt CI-feladat sikeres (41134650020,41134651634,41134649966).
Automatikus review-eredmény nem volt rögzítve;nem állítunk sikeres review-t.
A [resetdokumentum](h83002-warm-reset-contract.md) új helyi kódvizsgálata külön
rögzíti a CCR megőrzését,vektorolvasási/prefetch sorrendet,részállapotokat és
az időadósság/folytonos idővonal nyitott döntéseit. Ez dokumentációs jelölt,
nem végrehajtott upstream reset-differenciál és nem új melegreset-API.
Motorkód nem változott;runtime tesztet e dokumentáció miatt nem futtattunk újra.
Következő:a felsorolt resetmegfigyelések független végrehajtási tesztjei,
majd eszközsorrend és nem nulla időpont megtartása.

## Aktív kutatási lépés — rögzített melegreset-szerződés, 2026-10-30

Öt eredeti forrásfájl/hat device_reset módszer SHA256-azonossága reprodukálható
archívumellenőrzővel rögzítve. GPIO csak kimenetet frissít,SCI magas clock/TX-et
publikál,IRQ törlés után szintbemenetet újraértékel;nem általános nullázás.
[Mátrix és nyitott implementációs kapuk](h83002-warm-reset-contract.md).
Ez forráskutatás,nem melegreset-API vagy teljesgép/hardveregyezés.
Négy új független Python kontroll;teljes91 Python PASS. Motorkód nem változott;
a korábbi88 CTest/87 Python bizonyíték #143-hoz tartozik,nem újrafuttatott eredmény.
A #144 beolvadt9ddf040ec03ffe298dfdfcc259330c99ff1e570a,2026-10-30T21:52:30Z;
négy CI sikeres,automatikus review-eredmény nem volt rögzítve.
Következő:generált CPU-reset/runner és eszköz-reset-sorrend/időszabály ellenőrzése,
majd aktív SCI/IRQ/GPIO megőrzési és eseményközlési tesztek;teljes reset még nyitott.

## Aktuális lezárás — #143 hideg kutatási életciklus, 2026-10-30

Beolvadt `301eec75c72028b8f377ee4e099fce0b1df4de5e`,tesztelt head `d2da27c15c8ebf8c083f35bb4a2cef18eba4d8c4`.
Időpont:2026-10-30 22:27:12 Europe/Budapest (21:27:12 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127313949),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127314006),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127313987):mind az öt feladat sikeres.
Tényleges macOS15,a required check történeti macos-14 neve kompatibilitási okból marad.
Nincs rögzített automatikus review-eredmény;nincs sikeres review-állítás.
88 CTest/87 Python,célzott ASan/UBSan és szigorú fordítói ellenőrzés PASS.
401 újraindítási pont friss CPU/TX-példánnyal egyezik;teljes256KiB NVRAM8 epochon
megmarad,explicit clear nulláz. Érvénytelen bemeneti csere megtartja a régi epochot.
Hibás/reentráns futás csak rekonstrukcióval áll helyre;ROM/owner/RX-sor izoláció PASS.
NVRAM-visszatöltést kihagyó próbahiba410 elvárt kudarc.
[Hatókör](an1x-research-lifecycle.md):hideg kutatási rekonstrukció,nem hardveres meleg reset.
533 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
Nincs aktív motorjelölt. Következő:a rögzített forrás melegreset-megőrzési/kimenetközlési
mátrixa,globális reset-sorrend és idő/bemeneti szabályok;csak utána in-place API.
Aktív SCI,IRQ-pin,GPIO-latch,RAM és függő esemény független tesztje szükséges.
Teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER még nyitott.
Új motorlépés a lezáró dokumentáció beolvadása után indulhat.

## CI-javítás — macOS14 kivezetése, 2026-10-30

A #142 macOS-feladata és célzott újrafuttatása a GitHub tervezett brownoutja miatt
megszakadt,nem fordítási/teszthiba miatt. A futtató2026-11-02 dátummal megszűnik.
Mindkét aktív workflow macos-15-re vált. A kötelező check történeti neve
`build-and-test (macos-14)` marad a ruleset25910947 kompatibilitása miatt;
a tényleges runner macos-15,a teljes tesztlépések változatlanok. Nincs szabálymegkerülés.
A MAME-kutató build Xcode16.2 rögzítése marad:a macOS15 hivatalos képe tartalmazza.
Új CI/review az új headen szükséges;korábbi zöldek nem igazolják az új workflowt.
Forrás:https://github.com/actions/runner-images/issues/13518
Kép:https://github.com/actions/runner-images/blob/main/images/macos/macos-15-Readme.md

## Történeti lezárás — #141 közös SYSCR, 2026-10-30

A #141 beolvadt `eba55a2367f9cc5e319b4f3f26049fe4da55af16`,tesztelt head `d50532a812abd23a65a2aafaf24f28ea815c0217`.
Időpont:2026-10-30 17:23:15 Europe/Budapest (16:23:15 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891561),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891536),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891444):mind az öt feladat sikeres.
Automatikus review-eredmény nem volt rögzítve;nincs sikeres review-állítás.
87 CTest/87 Python és célzott ASan/UBSan PASS;7712 IRQ/NMI-partíció,512 TRAPA CCR-eset,
241 SLEEP közbeni módváltás PASS. Fix módra visszaállított próbahiba2056 elvárt kudarc.
[Hatókör](an1x-syscr.md):csak01/09,közös owner,azonnali filter,helyes IRQ/TRAPA-belépés.
Generált H8/H8H-törzsek újragenerált lockkal egyeznek;562 módszer/264 kiválasztott sor változatlan.
A korábbi15424-es köztes darabszámot7712-re javítottuk;az integrációs teszt ezt igazolja.
Nincs aktív motorjelölt. Következő:explicit reset-életciklus,owner-állapotok,NVRAM-megőrzés,
idő/bemeneti sor kezelése,ismételt reset és hibás owner kezelése független tesztekkel.
Teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER továbbra is nyitott.
Új motorlépés a lezáró dokumentáció beolvadása után indulhat.

## Lezárt előkészítő lépés — #139 SYSCR módlogika, 2026-10-30

Beolvadt `b84d3b20bbef1f51a14af64da1a57df9f5cf2a88`,tesztelt head
`4d5ca9ba548cd4b3c3547ba191c2660daeebfce4`,2026-10-30 16:12:00 Europe/Budapest
(15:12:00 UTC). Mind az öt CI sikeres:
[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41072881939),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41072881943),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/41072882034).
Nincs nyitott review-szál vagy rögzített automatikus review-eredmény;utóbbit nem állítjuk.

86 CTest/87 Python és célzott ASan/UBSan PASS.65536 mód/CCR eset és három eredeti
MAME-módszer összevetése:0 eltérés;hibás módszűrés8192 elvárt kudarc.
[Hatókör és következő integrációs kapuk](h83002-syscr-modes.md).
Ez önálló előkészítés:FFFFF2 továbbra is unmapped,a CPU rögzített profilja változatlan.
Az integrált gépalap #136 marad;nincs új firmware-/hangelfogadás.
Következő:közös SYSCR-owner,azonnali IRQ-filter és IRQ/TRAPA-belépés módváltás,
NMI/függőIRQ/SLEEP/RTE és partíciótesztek;utána teljes gépreset.
Nincs aktív motorjelölt;a lezáró dokumentáció beolvadása után indulhat új lépés.

## Dokumentáció-egyeztetés — 2026-10-30, #137 után

Ellenőrzött main: `7ee6db8bc418f696e67f0ea17fcc8dade073e47d`.
A [#137 dokumentációs lezárás](https://github.com/RobCZart82/yaman1x/pull/137)
2026-10-30 10:26:30 Europe/Budapest időpontban beolvadt (09:26:30 UTC).
Tesztelt head: `e7b5c345b2ed9dc0e38b1fa0ecc615b106602575`.
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41031322824),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41031322822): mind a négy feladat sikeres.
MAME ezen a dokumentációs változáson nem futott; a motor MAME-bizonyítéka a #136-hoz tartozik.
A #137 automatikus kódellenőrzése kvótalimit miatt nem fejeződött be; ez nem
sikeres review, és nem konkrét kódhiba. A #136 sikeres review-ja külön bizonyíték.

A motoralap továbbra is #136; nincs új motorfunkció vagy új firmware-/hangelfogadás.
A lezáró dokumentáció már beolvadt, a következő motorlépés megkezdhető:
közös CPU/host SYSCR-állapot, szűrés és IRQ/TRAPA-belépés módtesztjei, majd teljes gépreset.
Az alábbi korábbi lezárások és jelöltbejegyzések dátumhoz kötött történeti adatok;
régi „függőben” vagy „következő” megjegyzésük nem az aktuális feladatsor.

A mostani dokumentációjavítás rögzíti a #137 lezárását a strukturált naplóban,
frissíti az M0 elavult 31 hiányzó kódolássoros feladatát (jelenleg 264 kiválasztott,
0 hiányzó sor), és pontosítja a README korábbi komponensleírásainak hatókörét.
Helyi ellenőrzés: UTF-8 és belső linkek, JSON/egyedi PR-azonosítók/merge-ősök,
85 CTest/87 Python leltár, forrásaudit és licencértesítés-lefedettség PASS.
A futásidejű teszteket ehhez a kizárólag dokumentációs javításhoz nem futtattuk újra.
A javítás saját [#138 PR-ellenőrzései](https://github.com/RobCZart82/yaman1x/pull/138) ott követhetők; az itt hivatkozott korábbi CI
nem ennek az új dokumentációs változatnak az eredménye.

## Legutóbbi motorlezárás — #136, 2026-10-30

A [#136 PR](https://github.com/RobCZart82/yaman1x/pull/136) beolvadt:
merge `7aaa8d0178e06b8161c8508cda1cebdb20e2c072`,tesztelt head `6d58a3df9307b4513b61ea01d9eed3d70bd6e87a`.
Időpont:2026-10-30 10:04:38 Europe/Budapest (2026-10-30T09:04:38Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141599),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141600),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141620):mind az öt feladat sikeres.
Review:`completed_no_findings`;CI és review külön bizonyíték.

Beolvadt:**85 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
IRQ-MMIO:ISCR F4,IER F5,ISR F6,ICR0/1 F8/F9;szóalak F4/F8,24 bites címalias.
Odd/hiányos szó előzetesen elutasítva. ICR nem komplementált maszk és késleltetett
publikáció megőrizve (nem igazolt fizikai helyesség).65536 engedélyezés/szint,
65536 törlés,131072 szó,2048 él/szint és3856 CPU-partíció PASS. CPU-ból beállított
IRQ SLEEP-et ébreszt,ISR A5-öt ír NVRAM-ba,RTE SP-t helyreállít;I maszkol.
Eredeti18 IRQ-művelet/table200000 lépésben nulla eltérés. Fordított ICR-offset150185,
téves IER=>ISCR199554 hibával felismerve. Helyi84 futtatható CTest/86 Python PASS;
ismert Windows-symlink eset kizárva. Linux CI UBSan PASS;helyi ASan/UBSan nem futott.
525 fájl forrásauditja/229 fájl licencnyilvántartása PASS (nem jogi engedély).
[Pontos források,hatókör és SYSCR-feltárás](an1x-irq-mmio.md).

A SYSCR bit08 a CPU szűrését és IRQ/TRAPA-belépését együtt érinti. Fix0B profil
marad,F2 olvasás/írás elutasítva,nincs elhallgatott vagy kitalált működés.
Nincs aktív motorjelölt. Következő:külön CPU/host SYSCR-szerződés és módtesztek,
majd teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER elfogadás nyitott.
A lezáró dokumentáció #137-ben beolvadt; ez a sorrendi feltétel teljesült.

## Történeti lezárás — #134, 2026-10-30

A [#134 PR](https://github.com/RobCZart82/yaman1x/pull/134) beolvadt:
merge `7c46a56495a166ed9e6abbe4bea11915e08bbedd`,tesztelt head `963fc91ca7512a80e5830dc8a784c8c186c9e5e9`.
Időpont:2026-10-30 08:46:00 Europe/Budapest (2026-10-30T07:46:00Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299804),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299832),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299487):mind az öt feladat sikeres.
Review:`no_recorded_result_at_inspection`;CI és review külön bizonyíték.

Beolvadt:**84 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
GPIO-szóolvasás:FFFFCE,D0,D2;írás:D0,D2. Előzetes ellenőrzés az odd/hiányos/readonly
írás ellen. Big-endian,felső bájt előbb,azonos buszidőbélyeg:explicit szoftverszabály,
nem fizikai időzítési vagy teljes MAME-címtér-egyezési állítás. A CPU egy szóhozzáférést
számol. Első bájt reentry-je után a második nem fut;korábbi mellékhatás nem vonódik vissza.
196608 független eset,241 CPU-partíció PASS;DA65 NVRAM/R2,CCR89 ellenőrizve.
Eredeti portműveletek soros kompozíciója200000 lépésben nulla eltérés. Felcserélt
olvasási bájt262198,hibás alsóbájtírás455493 hibás ellenőrzéssel felismerve.
Helyi83 futtatható CTest/86 Python PASS;ismert Windows-symlink eset kizárva.
Linux CI UBSan PASS;helyi ASan/UBSan nem futott.523 fájl forrásauditja/229 fájl
licencnyilvántartása PASS (nem jogi engedély). [Pontos szerződés](an1x-gpio-words.md).
Nincs aktív motorjelölt. Következő:IRQ/SYSCR szűkített bekötésének feltárása,majd
teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER elfogadás nyitott. Új motorlépés
csak e lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #132, 2026-10-30

A [#132 PR](https://github.com/RobCZart82/yaman1x/pull/132) beolvadt:
merge `5ac2585e9b84497764f1fe0e2bbf426bbbab84a6`,tesztelt head `66f480ea43b73551e934ec6528ca1dae4a5c5171`.
Időpont:2026-10-30 06:59:33 Europe/Budapest (2026-10-30T05:59:33Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924793),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924918),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924833):mind az öt feladat sikeres.
Review állapot:`no_recorded_result_at_inspection`;CI- és review-bizonyíték külön kezelve.

Beolvadt:**83 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
PortB FFFFD4/FFFFD6,alap00/maszk00,külön állapot,callback ID11.262144 bit-orákulumos
eset,256 hideg bemenet,241 CPU-partíció PASS.0F/5A/15 =>1A NVRAM,CCR81.
Eredeti portműveletek200000 lépésben nulla eltérés. A referencia11 cpp-törzset
és inline FF-olvasást ellenőriz (összesen12 eredeti törzs). Téves maszk225082,
téves bemeneti port244961 hibás ellenőrzéssel felismerve;iterációnként két
ellenőrzési hely miatt a szám meghaladhatja a műveletszámot.
Helyi82 futtatható CTest/86 Python PASS;ismert Windows-symlink eset kizárva.
Linux CI UBSan PASS;helyi ASan/UBSan ebben a lépésben nem futott.
521 fájl forrásauditja és229 fájl licencnyilvántartása PASS (nem jogi engedély).

Auditjavítások:IR öt szóra frissítve,régi SLEEP/GPIO-korlátok történeti hatóköre
jelölve,README build-tesztszám66-ról83-ra javítva. A README eltérése az előző
áttekintésből kimaradt;most javítva. A ROADMAP régi #78-jelöltfelirata is történeti jelölést kapott. [Pontos források és korlátok](an1x-gpiob.md).
Nincs aktív motorjelölt. Következő:GPIO-szóhozzáférések pontos meghatározása és
tesztjei,majd IRQ/SYSCR/teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #130, 2026-10-30

A [#130 PR](https://github.com/RobCZart82/yaman1x/pull/130) beolvadt:
merge `fc9b483dcdd6c33385c1225303a0c0f2f0d7477c`,tesztelt head `e6f276dcf20e48d014a71f13dd73ac74b33da029`.
Időpont:2026-10-30 04:45:46 Europe/Budapest (03:45:46 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653002), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653007), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653087):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**82 CTest/87 Python**. PortA FFFFD1/FFFFD3,alap00/maszk00,callback ID10;
262144 nyolcbites eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>1A NVRAM,CCR81. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:téves maszk198248,rossz port252184 elvárt kudarc.
GPIO4/6/7/8/9/A/SCI ASan/UBSan PASS;519 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpioa.md).
Nincs aktív motorjelölt. Következő:portB DDR/adat,alap00/maszk00;utána a GPIO
szóhozzáféréseinek pontos meghatározása és tesztelése.
IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #128, 2026-10-30

A [#128 PR](https://github.com/RobCZart82/yaman1x/pull/128) beolvadt:
merge `efbf68fad922e7270fc60105a1e6947f972cee6a`,tesztelt head `00bff586f85f7a2b6d70c7648c57c91486eeddec`.
Időpont:2026-10-30 04:05:45 Europe/Budapest (03:05:45 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993876811), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993876814), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993877026):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**81 CTest/87 Python**. Port9 FFFFD0/FFFFD2,alap00/maszk C0;
262144 bit-orákulumos eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>DA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:hiányos maszk199031,hibás alap-DDR130 elvárt kudarc.
GPIO4/6/7/8/9/SCI ASan/UBSan PASS;517 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio9.md).
Nincs aktív motorjelölt. Következő:portA DDR/adat,alap00/maszk00;utána portB,
majd a GPIO-szóhozzáférés pontos meghatározása és tesztelése.
IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #126, 2026-10-30

A [#126 PR](https://github.com/RobCZart82/yaman1x/pull/126) beolvadt:
merge `a8b0ca4c87478aa188b902ef997b7c88c284bbf8`,tesztelt head `32dcba307e72f2acfcffd494c890afbc2894c57f`.
Időpont:2026-10-30 03:25:44 Europe/Budapest (02:25:44 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990014063), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990014152), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990013951):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**80 CTest/87 Python**. Port7 FFFFCE csak bemenet,DDR00/maszk00;
256 közvetlen érték,256 CPU-érték/flag és161 időbeli szétvágás PASS.
Port4/6/8/machine izoláció,portazonosító/időbélyeg,readonly írás,szóhozzáférési határ,
hiányzó callback és reentry ellenőrizve. Eredeti MAME:200000 olvasás,0 eltérés.
Rossz porthoz irányító próbahiba769 elvárt kudarc. GPIO4/6/7/8/SCI ASan/UBSan PASS.
515 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
[Forrásazonosság és korlátok](an1x-gpio7.md). Nincs aktív motorjelölt.
Következő:port9 DDR/adat,alap00/maszk C0;majd a többi GPIO-csoport.
Port7+8 szóolvasás,IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #124, 2026-10-30

A [#124 PR](https://github.com/RobCZart82/yaman1x/pull/124) beolvadt:
merge `73a5b5cac78617216901f9db19736902ccd1d0a1`,tesztelt head `33a7a85fc92252d949a7f3867e2d80bbe5b165a8`.
Időpont:2026-10-30 02:45:43 Europe/Budapest (01:45:43 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127930), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127955), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127919):mind az öt feladat sikeres.
[Automatizált kódreview](https://github.com/RobCZart82/yaman1x/pull/124#issuecomment-6512683713):completed,nincs finding az ellenőrzött headen
(2026-10-30T01:31:01.357600Z). Nyitott review-szál nincs;security review-eredmény nem érkezett.
A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**79 CTest/87 Python**. Port8 alap-DDR F0/maszk E0,FFFFCD/FFFFCF;
262144 bit-orákulumos eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>FA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:hiányos maszk268791,hibás alap-DDR130 elvárt kudarc.
GPIO4/6/8/SCI ASan/UBSan PASS;513 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio8.md).
Nincs aktív motorjelölt. Következő:csak olvasható port7 FFFFCE,bemeneti/izolációs/CPU
ellenőrzésekkel;majd a többi GPIO-csoport. IRQ/SYSCR és teljes gépreset nyitott.
Firmware/MIDI/PKS/DSP/hang/REAPER-elfogadás nincs kész.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #122, 2026-10-30

A [#122 PR](https://github.com/RobCZart82/yaman1x/pull/122) beolvadt:
merge `804c8a5100ac281945f643c4c9f8f969f263da14`,tesztelt head `32b36079e0a37f420193b410320f7705d728e29e`.
Időpont:2026-10-30 02:05:46 Europe/Budapest (01:05:46 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044271), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044353), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044283):mind az öt feladat sikeres.
[Automatizált kódreview](https://github.com/RobCZart82/yaman1x/pull/122#issuecomment-6512135987):az ellenőrzött headen completed,
nincs jelzett vagy nyitott finding (2026-10-30T00:50:58.978944Z).
Nyitott review-szál nincs;security review-eredmény nem érkezett.
A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**78 CTest/87 Python**. Port6 DDR80/maszk80,FFFFC9/FFFFCB,
262144 független maszkolt eset,241 CPU-partíció és kimenetváltozás-elnyomás PASS.
CPU:0F/5A/A5 =>AA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Maszkot eltávolító próbahiba:197637 elvárt kudarc;követett kód változatlan.
GPIO4/GPIO6/SCI ASan/UBSan PASS;511 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio6.md).
Nincs aktív motorjelölt. Következő:port8 alap-DDR F0/maszk E0,majd többi GPIO;
IRQ/SYSCR,alternatív pinfunkció és teljes gépreset továbbra is nyitott.
Firmware/MIDI/PKS/DSP/hang/REAPER-elfogadás nincs kész.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #120, 2026-10-30

A [#120 PR](https://github.com/RobCZart82/yaman1x/pull/120) beolvadt:
merge `41f8b6db0d55743495b4953c0ba746dfc4114446`,tesztelt head `a96bfc40bc518f1315aa3c828c0da1548abbd6aa`.
Időpont:2026-10-30 01:25:51 Europe/Budapest (00:25:51 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977946004), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977945988), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977945935):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice csak kapacitási tájékoztatás.

Beolvadt:**77 CTest/87 Python**. Port4 DDR/pins/PCR,buszidőbélyeges callbackek,
262144 bit-orákulumos eset,256 PCR-érték és241 CPU-partíció PASS.
CPU:F0 DDR,A5 latch,3C külső szint =>AC NVRAM-bájt,CCR89.
A kezdeti flag-orákulum N-bitjét javítottuk;CPU-kód nem változott.
Eredeti rögzített MAME-portmódszerek:200000 vegyes művelet,0 eltérés.
Próbahibák:DDR-visszaolvasás65281,pin helyett latch497267 elvárt kudarc.
GPIO4+SCI ASan/UBSan PASS;509 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
[Forrásazonosság és korlátok](an1x-gpio4.md). Nincs aktív motorjelölt.
Következő:port6 DDR/adat regiszterek rögzített alapértékkel/fenntartott bitekkel,
majd a többi portcsoport önálló tesztelt lépcsőkben. IRQ/SYSCR és alternatív pinfunkció nyitott.
Teljes gépreset,LCD/DSP/MEG,firmware/MIDI/PKS/hang/REAPER-elfogadás továbbra is nyitott.
Soros munka:a lezáró dokumentáció beolvadása után indulhat új motorfejlesztés.

## Történeti lezárás — #118, 2026-10-30

A [#118 PR](https://github.com/RobCZart82/yaman1x/pull/118) beolvadt:
merge `6f9e8b8f66d962e968af8d6f9cd4135d2c4d1ab3`,tesztelt head `85baf0a93896468e35f019845d9f9116763ecb29`.
Időpont:2026-10-30 00:46:23 Europe/Budapest (2026-10-29 23:46:23 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239180), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239137), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239160):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**76 CTest/87 Python**. ROM/RAM,SCI0/SCI1,IRQ és másolt RX-idősor közös
CPU-buszidővel;512 TX+512 RX keret,3204 szétvágás,belsőRAM-verem,NVRAM-ISR és SLEEP/RXI
ellenőrzött. Célzott ASan/UBSan PASS;határidő-próbahiba2738 elvárt kudarc.
Reentry belső kivételének elnyelése sem enged folytatást. [Hatókör](an1x-sci-bus.md).
Nincs aktív motorjelölt. Következő:GPIO/IRQ-vezérlő regisztertérkép és a rögzített CPU-profil
hiányainak ellenőrzése,majd egy kis regisztercsoport bekötése független tesztekkel.
Teljes gépreset,LCD/DSP/MEG,firmware/MIDI/PKS/hang/REAPER-elfogadás továbbra is nyitott.
Soros munkamenet:a lezáró dokumentáció beolvadása után indulhat új motorfejlesztés.

## Történeti lezárás — #116, 2026-10-29

A [#116 PR](https://github.com/RobCZart82/yaman1x/pull/116) beolvadt:
merge `7fe1d4bcb602ce96d40f34ec4ccd888430bf70e0`,tesztelt head `1982509b2c4632b7e9198d3ae23656f59e2c2a55`.
Időpont:2026-10-29 23:25:39 Europe/Budapest (22:25:39 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649310), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649254), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649115):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **75 CTest-csoport /87 Python-eset**;CPU-kezelők változatlanul562
(558 változatlan,4 korábbi adaptáció),264 kiválasztott/0 hiányzó/135 kizárt sor.
1MiB saját ROM-másolat,külső256KiB NVRAM és512 bájt belső RAM,24 bites címtükrözés.
Teljes ROM/RAM-söprés,határok/izoláció/NVRAM-roundtrip és241 CPU-partíció PASS.
Helyi Release75/75 CTest,87 Python és célzott ASan/UBSan PASS.
[Bizonyíték és kutatási szabályok](an1x-memory-map.md).
Nincs aktív motorjelölt. MMIO és ROM-írás fail-closed,nem fizikai open-bus modell.
Következő:SCI MMIO és IRQ közös címtérbe,CPU-buszidőbélyeg/periféria-határidők,
reset/izoláció és szétvágástól független RX/TX,SLEEP-ébresztés.
Teljes gép,GPIO/LCD/DSP/MEG,időzítés,firmware/MIDI/PKS/hang/plugin elfogadás nyitott.
Soros fejlesztés:egyszerre egy PR;az alábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #114, 2026-10-29

A [#114 PR](https://github.com/RobCZart82/yaman1x/pull/114) beolvadt:
merge `3172c55e9a1cea766e5326bfd56a98f68d6420f3`,tesztelt head `532c5937cfc2b0b3f142e935cebba8dfa4912000`.
Időpont:2026-10-29 22:45:44 Europe/Budapest (21:45:44 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229492), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229514), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229555):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **74 CTest-csoport /87 Python-eset;562 upstreamből származó kezelő
(558 változatlan,4 korábbi adaptáció);264 kiválasztott /0 hiányzó /135 kizárt sor**.
A leltározott H8H-kezelők kiválasztása teljes;ez nem teljes CPU/gép-elfogadás.
SLEEP és IRQ/NMI-ébresztés rögzített profillal,standby kizárva. Helyi Release74/74
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
ébresztést tiltó kontroll seed13/slice7-nél felismerve. 512 maszkkombináció és644
szétvágás,periféria-előrehaladás és határidő nélküli alvás ellenőrizve.
[Bizonyíték és korlátok](h8-sleep-wake.md).
Nincs aktív motorjelölt. Következő:AN1x címtér/MMIO és CPU-periféria közös gépütemező,
reset/izoláció/folytathatóság,utána eredeti helyi ROM indítási és MIDI/PKS megfigyelés.
M0 részleges;dinamikus SYSCR,standby,DMA/DTC,trace/retry,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nincs lezárva. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #112, 2026-10-29

A [#112 PR](https://github.com/RobCZart82/yaman1x/pull/112) beolvadt:
merge `b14cd16e8d4d149cdb622371ac29928904bbc295`,tesztelt head `41c5c34be49913dea5dc025f589cfde068a27736`.
Időpont:2026-10-29 22:05:04 Europe/Budapest (21:05:04 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371785), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371726), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371748):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **73 CTest-csoport /87 Python-eset;560 upstreamből származó kezelő
(556 változatlan,4 korábbi adaptáció);263 kiválasztott /1 hiányzó /135 kizárt sor**.
EEPMOV.B/W:bájtonkénti másolás8/16 bites számlálóval,két prefix,nyolc változatlan kezelő.
Helyi Release73/73 CTest,87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás forrásléptetés kontrollja
seed15/slice4-nél felismerve. Max65535 darab,átfedés,címkörbefordulás,362 szétvágás,
131072 prefixőr és12 memóriahiba ellenőrizve. [Bizonyíték](h8-eepmov-block.md).
Nincs aktív motorjelölt. Következő:SLEEP megállás/ébresztés és periféria-előrehaladás
host/ütemező-szerződésével. M0 részleges;teljes CPU/gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #110, 2026-10-29

A [#110 PR](https://github.com/RobCZart82/yaman1x/pull/110) beolvadt:
merge `3d0ce825bb341402f40dab92c0f1ec2f409b5ee7`,tesztelt head `903844cdb2a0466a93b2a61b056191afd0b37146`.
Időpont:2026-10-29 21:25:50 Europe/Budapest (20:25:50 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842276), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842076), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943841959):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **72 CTest-csoport /87 Python-eset;552 upstreamből származó kezelő
(548 változatlan,4 korábbi adaptáció);261 kiválasztott /3 hiányzó /135 kizárt sor**.
MOVFPE/MOVTPE:két művelet,négy változatlan kezelő. Az eredeti `TODO: timing`
megmarad:periféria/E-clock kézfogás és hardveridőzítés nincs implementálva/igazolva.
Helyi végső Release72/72 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás cím-előjelkiterjesztés
kontrollja seed15/slice1-nél felismerve. A túl tág dekódolási maszkot és az
elsőutasítás-leltárt javítottuk,teljes újrafordítás/regresszió után PASS.
[Bizonyíték és korlátok](h8-peripheral-byte-moves.md).
Nincs aktív motorjelölt. Következő:EEPMOV blokkmozgatás,majd SLEEP a szükséges
host/ütemező-szerződéssel. M0 részleges;teljes gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #108, 2026-10-29

A [#108 PR](https://github.com/RobCZart82/yaman1x/pull/108) beolvadt:
merge `3c7005a3a26da4149668f6ab9f4695e1672bdc6e`,tesztelt head `ffef22c9d5794ec78028a0f2d07cb9659ff9d5bb`.
Időpont:2026-10-29 20:25:40 Europe/Budapest (19:25:40 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469453), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469399), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469419):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **71 CTest-csoport /87 Python-eset;548 upstreamből származó kezelő
(544 változatlan,4 korábbi adaptáció);259 kiválasztott /5 hiányzó /135 kizárt sor**.
CCR displacement32:két művelet,két eredeti prefixfolytatás,nyolc változatlan kezelő.
Helyi Release71/71 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;felső eltolásszó elhagyásának
kontrollja seed0/slice1-nél felismerve. [Bizonyíték](h8-ccr-displacement32-memory.md).
Nincs aktív motorjelölt. Következő lépés:MOVFPE/MOVTPE perifériás bájtmozgatások,
majd EEPMOV és SLEEP a szükséges host/ütemező-szerződések tisztázásával.
M0 részleges;teljes gép/MMIO,hardveridőzítés,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #106, 2026-10-29

A [#106 PR](https://github.com/RobCZart82/yaman1x/pull/106) beolvadt:
merge `aee769ef224e2686ccd4a98bbd9d3851f8cc6399`, tesztelt head `e3af0b4453d19194da8d32b24e6d409d4ab8809f`.
Időpont:2026-10-29 19:21:59 Europe/Budapest (18:21:59 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577461), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577203), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577221): mind az öt feladat sikeres.
A jelenlegi GitHub-állapotban nincs rögzített automatikus review-eredmény vagy
review-szál; sikeres automatikus review-t nem állítunk.

Beolvadt: **70 CTest-csoport /87 Python-eset;540 upstreamből származó kezelő
(536 változatlan,4 korábbi adaptáció);257 kiválasztott /7 hiányzó /135 kizárt sor**.
CCR signed16 eltolás:két művelet,négy változatlan kezelő. Helyi Release70/70
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés;hibás előjel-kiterjesztés kontrollja seed1/slice2-nél felismerve.
[Hatókör és bizonyíték](h8-ccr-displacement16-memory.md).
Nincs aktív motorjelölt. Következő lépés:CCR displacement32 címzések.
M0 részleges;teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #104, 2026-10-29

A [#104 PR](https://github.com/RobCZart82/yaman1x/pull/104) beolvadt:
merge `92e52f45307dc1adad37347febd407d0d2106182`,végső tesztelt head `570d7b11d5dde819ac12211671f39fd47163f81b`.
Időpont:2026-10-29 18:30:16 Europe/Budapest (17:30:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264372),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264370),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264281):mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **69 CTest-csoport /87 Python-eset;536 upstreamből származó kezelő
(532 változatlan,4 korábbi adaptáció);255 kiválasztott /9 hiányzó /135 kizárt sor**.
CCR postincrement/predecrement,két művelet,négy változatlan eredeti kezelő.
524288 memóriaszó,24576 határeset,201 szétvágás,512 IRQ/NMI-maszk,
322 megszakításos szétvágás és16 memóriahiba PASS. A host-visszahívás a memóriaelérés
előtt megfigyeli a módosított mutatót és régi CCR-t. Helyi Release69/69 CTest,
87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
a hibás négybájtos növelés kontrollja seed2/slice3-nál felismerve.
[Hatókör és bizonyíték](h8-ccr-autoindex-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR signed16/displacement32 címzések,
majd a fennmaradó H8H-műveletek. M0 részleges;teljes gép/MMIO,firmware/MIDI/
hang/plugin elfogadás nyitott. Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #103 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #102, 2026-10-29

A [#102 PR](https://github.com/RobCZart82/yaman1x/pull/102) beolvadt:
merge `2dab39c1598533fc0a84da9017b3f2975854ff96`, végső tesztelt head `3c0f7947efc5cc170877fa99479f181d527bd195`.
Időpont:2026-10-29 17:50:13 Europe/Budapest (16:50:13 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848068),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848027),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848153): mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **68 CTest-csoport /87 Python-eset;532 upstreamből származó kezelő
(528 változatlan,4 korábbi adaptáció);253 kiválasztott /11 hiányzó /135 kizárt sor**.
Négy CCR abs16/abs32 betöltési/tárolási művelet,nyolc változatlan eredeti kezelő.
131072 memóriaszó,131072 abs16 cím,524288 abs32 címfél-söprés,9216 CCR-határeset,
241 szétvágás,1024 IRQ/NMI-maszk és644 megszakításos szétvágás PASS.
Helyi Release68/68 CTest és87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás alsóbájt-kiválasztás
kontrollja seed3/slice5-nél felismerve. [Hatókör és bizonyíték](h8-ccr-absolute-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR postincrement/predecrement,
majd eltolásos címzések és a fennmaradó H8H-műveletek. M0 részleges;
teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #101 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #100, 2026-10-29

A [#100 PR](https://github.com/RobCZart82/yaman1x/pull/100) beolvadt:
merge `1de8971b8fdd7694a8c3051ae86611a9ff6cd3cd`, végső tesztelt head `ba6db1c61a3ba9d696f8affee1410e5969c0aa9d`.
Időpont:2026-10-29 17:10:15 Europe/Budapest (16:10:15 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424971),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424908),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424743): mind az öt feladat sikeres.
Automatikus review a végső commiton befejeződött, hibamegállapítás és nyitott review-szál nélkül.

Beolvadt: **67 CTest-csoport /87 Python-eset;524 upstreamből származó kezelő
(520 változatlan,4 korábbi adaptáció);249 kiválasztott /15 hiányzó /135 kizárt sor**.
Két MOV.L művelet, eredeti kettős prefix és öt szavas utasításpuffer.
524288 eltolásfél-söprés,55296 határeset,241 szétvágás,65536 prefixfolytatás,
regiszteraliasok és első/második memóriaátviteli hibák PASS. Helyi Release67/67
CTest és87 Python, célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés; a felső eltolásszó elhagyásának hibás kontrollja seed4/slice4-nél felismerve.
[Hatókör és bizonyíték](h8-long-displacement32-memory.md).

Nincs aktív motorjelölt. Következő lépés: a fennmaradó CCR-címzések és további
hiányzó H8H-kódolások, majd teljes gép/MMIO-integráció. M0 részleges;
firmware/MIDI/hang/plugin elfogadás nyitott. Egyszerre egy PR; új fejlesztés csak beolvadás után.
A #99 dokumentációs PR beolvadását a strukturált nyilvántartás már tartalmazza.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #98, 2026-10-29

A [#98 PR](https://github.com/RobCZart82/yaman1x/pull/98) beolvadt:
merge `1c7ba628aceb29fd4c057db73fe31b2be4841558`, tesztelt head `a3c033256ec247555b44b0d820bb228dcaf9bc39`.
Időpont: 2026-10-29 13:36:16 Europe/Budapest (12:36:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870231488),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229509),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229586): mind az öt feladat sikeres.
Automatikus review szolgáltatási kvóta miatt nem állt rendelkezésre; sikeres review-t nem állítunk.

Beolvadt: **66 CTest-csoport /87 Python-eset;518 upstreamből származó kezelő
(514 változatlan,4 korábbi adaptáció);247 kiválasztott /17 hiányzó /135 kizárt kódolássor**.
524288 memóriaszó,24576 cím/CCR-eset,65536 prefixfolytatás,201 szétvágás,
512 IRQ/NMI-maszk és322 megszakításos szétvágás; eredeti referencia200000 szelet,
nulla eltérés. A hibás alsóbájt-kiválasztást a megerősített referencia felismerte.
A helyi korábbi Windows-futás65 futtatható CTest/86 Python-esetet ellenőrzött;
az ismert symlink-jogosultsági eset kizárását nem tekintjük sikeres tesztnek.
[Pontos bizonyíték és korlátok](h8-ccr-indirect-memory.md). M0 részleges; nincs aktív
motorjelölt. Teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.

A fejlesztés soros: egyszerre egy PR; következő fejlesztés csak az aktuális PR
beolvadása után. Következő címzési lépés a hosszúszavas32 bites eltolás,
majd a fennmaradó CCR-címzések és a leltár további hiányzó műveletei.

## Történeti lezárás — #96, 2026-10-29

Ellenőrzött motoralap: [#96](https://github.com/RobCZart82/yaman1x/pull/96),
merge `8b1c37eb569202e394ff1caeb3f64618bae78a69`, végső tesztelt head `fd5d1293a21cf63c4b39293b30ca7e650702efe4`.
Beolvadt: 2026-10-29 11:49:02 Europe/Budapest (10:49:02 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583149),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583161),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583141).
Linux UBSan:32 bites eltolásos bájt/szó-mozgatás és korábbi számtan PASS; eredeti
unsigned-túlcsordulási kontroll PASS. Automatikus kódellenőrzés kvótakorlát miatt
nem fejeződött be; sikeres review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **65 CTest-csoport /87 Python-eset;512 upstreamből származó kezelő
(508 változatlan,4 korábbi adaptáció);245 kiválasztott /19 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Négy MOV-csoport:1048576 eltolásfél-söprés (nem teljes2^32
keresztszorzat),55296 cím/regiszter/CCR-határeset,241 szétvágás,65536 prefixfolytatás,
memóriahibás fail-stop és újraindítás-tiltás PASS. Eredeti referencia:200000 szelet,
nulla eltérés; felső eltolásszó elhagyása kontroll seed6/slice5 felismerve. Helyi64
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
[Pontos hatókör és bizonyíték](h8-displacement32-memory.md). Hosszúszavas32 bites
eltolás külön nyitott. Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/
hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #94, 2026-10-29

Ellenőrzött motoralap: [#94](https://github.com/RobCZart82/yaman1x/pull/94),
merge `442949856a92392986f3bbc5927f36b7884a505b`, végső tesztelt head `9f3cda7fcbe545ee2b0da9cebc23d8dd5ab6b42a`.
Beolvadt: 2026-10-29 10:48:25 Europe/Budapest (09:48:25 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898827),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898826),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898831).
Linux UBSan: szélső előjeles szó-osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a beolvasztáskor még futott;
lezárt review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **64 CTest-csoport /87 Python-eset;502 upstreamből származó kezelő
(498 változatlan,4 korábbi adaptáció);241 kiválasztott /23 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.W:524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,
1024 szélső/CCR-eset,321 szétvágás,128 tiltott ER-szomszéd PASS. Eredeti referencia:
200000 szelet,nulla eltérés; fordított N kontroll seed7/slice7 felismerve. Helyi63
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
INT_MIN/-1: a matematikai +2147483648 hányadost az eredeti32 bites intre szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres mérés.
A forrás szintű szélsőeset-vizsgálat lezárult;[pontos hatókör](h8-signed-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending és korábbi nyitott szélsőeset-bejegyzések történetiek.

## Történeti lezárás — #92, 2026-10-29

Ellenőrzött motoralap: [#92](https://github.com/RobCZart82/yaman1x/pull/92),
merge `1adeb1030871083bb510f4e691da4467cd96a816`, végső tesztelt head `b719cf25b0d0d8b929c5b639d5971ddc424a1a49`.
Beolvadt: 2026-10-29 10:18:06 Europe/Budapest (09:18:06 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532370),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532267),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532335).
Linux UBSan: előjeles bájtos osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a végső headen megállapítás nélkül lezárult.

Beolvadt: **63 CTest-csoport /87 Python-eset;500 upstreamből származó kezelő
(496 változatlan,4 korábbi adaptáció);240 kiválasztott /24 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.B:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás,65536 prefixfolytatás PASS. Eredeti referencia:
200000 szelet,nulla eltérés; hányados+1 kontroll seed7/slice3 felismerve. Helyi62
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
Előjeles szó-osztás INT32_MIN/-1 és csomagolás továbbra is nyitott vizsgálat;
[pontos hatókör és bizonyítékok](h8-signed-byte-divide.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #90, 2026-10-29

Ellenőrzött motoralap: [#90](https://github.com/RobCZart82/yaman1x/pull/90),
merge `a4f52f600d2cfd85f14a5e8822eb5e025d28825d`, végső tesztelt head `f24ca47f3511764dfec4971e84c3c1487aaed443`.
Beolvadt: 2026-10-29 09:38:37 Europe/Budapest (08:38:37 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792043),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792142),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792129).
Linux UBSan: DAA/DAS és korábbi számtan PASS; eredeti unsigned-túlcsordulási kontroll
PASS. Automatikus kódellenőrzés a végső headen lezárult, megállapítás nélkül.

Beolvadt: **62 CTest-csoport /87 Python-eset;496 upstreamből származó kezelő
(492 változatlan,4 korábbi adaptáció);239 kiválasztott /25 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Decimális oracle:40000 érvényes számpár/CCR-eset,2048 regiszter-
határeset,241 szétvágás PASS;200000 eredeti referenciaszelet nulla eltéréssel.
DAA korrekció+1 kontroll seed7/slice0 felismerve. Helyi61 futtatható CTest/86 Python
PASS, ismert Windows-symlink eset kizárva. Érvénytelen BCD/flag kombinációkra nincs
állított teljes független helyességbizonyítás. [Hatókör](h8-decimal-adjust.md).
Nincs aktív motorjelölt. Ez M0-részlépés; M0 továbbra is részleges, teljes gép,
firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #88, 2026-10-29

Ellenőrzött motoralap: [#88](https://github.com/RobCZart82/yaman1x/pull/88),
merge `3c5f7dddfdf54532670d9cdddd0ade0aab7c92bd`, végső tesztelt head `fc0db2778270a5ffdc612ab46ccd7be0b0d9b4fe`.
Beolvadt: 2026-10-29 08:55:24 Europe/Budapest (07:55:24 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738076),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738102),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738069).
Linux UBSan: bájt/szó-osztás és korábbi szorzások PASS; eredeti unsigned-túlcsordulási
kontroll PASS. Beolvadt: **61 CTest-csoport /87 Python-eset;492 upstreamből származó
kezelő (488 változatlan,4 dokumentált adaptáció);237 kiválasztott /27 hiányzó /135 kizárt
kódolássor** (nem CPU-százalék). DIVXU.W:524288 osztó-söprés,30720 alias/CCR-eset,
20000 véletlen pár,241 szétvágás PASS. Nagy értékek független64 bites számítással;
200000 korlátozott eredeti referenciaszelet nulla eltéréssel; hányados+1 kontroll
seed9/slice26 felismerve. Helyi60 futtatható CTest/86 Python PASS, ismert Windows-symlink
eset kizárva. A q/r u32 típusmódosítás dokumentált; az eredeti osztóbit7 N-szabály
megőrzött, hardveresen nem igazolt. [Pontos hatókör](h8-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #86, 2026-10-29

Ellenőrzött motoralap: [#86](https://github.com/RobCZart82/yaman1x/pull/86),
merge `4dd788dadae4b09a21b4611f245640a63c56dcf5`, végső tesztelt head `189eeea02372839e8cf27f02853b990ed0625652`.
Beolvadt: 2026-10-29 08:26:31 Europe/Budapest (07:26:31 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892241),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892230),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892231).
Linux UBSan: bájtos unsigned osztás és korábbi szorzások PASS; az eredeti unsigned
int-promóciós hibát megkövetelő kontroll is PASS.

Beolvadt: **60 CTest-csoport /87 Python-eset;490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott /28 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. DIVXU.B:393216 osztandó-söprés,
2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS.
Eredeti referencia:200000 szelet,nulla eltérés; hányados+1 kontroll seed9/slice18
felismerve. Helyi59 futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
A nullás osztó, hányadoslevágás és osztófüggő N/Z az eredeti forrással egyezik,
nem külön hardvermérés. Nincs aktív motorjelölt. [Hatókör és bizonyíték](h8-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #84, 2026-10-29

Ellenőrzött motoralap: [#84](https://github.com/RobCZart82/yaman1x/pull/84),
merge `2540601aa985513a39d293ffa14fd8e0b76689da`, végső tesztelt head `7aee43a86ea21711254fd3e840728534e01b6a96`.
Beolvadt: 2026-10-29 07:45:33 Europe/Budapest (06:45:33 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383282),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383154),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383285).
Linux UBSan: előjeles bájt/szó és unsigned szó-szorzás PASS; az eredeti unsigned
int-promóciós hibát megkövetelő negatív kontroll is PASS.

Beolvadt: **59 CTest-csoport /87 Python-eset;488 upstreamből származó kezelő
(486 változatlan,2 korábbi adaptáció);235 kiválasztott /29 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. Előjeles szorzás:65536 bájtpár,
327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár,241 szétvágás és
65536 prefixfolytatás PASS. Eredeti referencia:200000 szelet,nulla eltérés;
product+1 kontroll seed9/slice14 felismerve. Helyi58 futtatható CTest/86 Python PASS;
egy ismert Windows-symlink eset kizárva. Nincs aktív motorjelölt.
[Szerződés és pontos bizonyítékok](h8-signed-multiply.md). M0 részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending
bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #82, 2026-10-29

Ellenőrzött motoralap: [#82](https://github.com/RobCZart82/yaman1x/pull/82),
merge `80b237ee7a9ad6b286a13c9253d076aa42bb0214`, végső tesztelt head `4f2cffc88c843cc9d431f5c0a6ec20ee2869d85b`.
Beolvadt: 2026-10-29 07:20:23 Europe/Budapest (06:20:23 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962559),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962568),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962569).
A Linux-lépésben a nagy szorzatok UBSan-tesztje és az eredeti kifejezés
előjeles túlcsordulását megkövetelő külön negatív kontroll is PASS.

Beolvadt: **58 CTest-csoport /87 Python-eset;482 upstreamből származó kezelő
(480 változatlan,2 dokumentáltan adaptált);233 kiválasztott /31 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. MULXU.W:327680 operandus-söprés,
9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS;200000 biztonságos
szorzatokkal végzett eredeti referenciaszelet nulla eltéréssel. A nagy szorzatokat
független64 bites számítás ellenőrzi; product+1 kontroll seed9/slice9 felismerve.
Helyi57 futtatható CTest és86 Python PASS, egy ismert Windows-symlink eset kizárva.
A szó-szorzás C++-promóciós kockázata rendezett a dokumentált unsigned-szélesítéssel;
[pontos hatókör és bizonyíték](h8-word-multiply.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/CI-pending/kockázati bejegyzések történetiek.

## Történeti lezárás — #80, 2026-10-29

Ellenőrzött motoralap: [#80](https://github.com/RobCZart82/yaman1x/pull/80),
merge `bddffa8743eac2e2af8e7fc1a3145046a45e6fa5`, végső tesztelt head `5bf4e97e64fc3ebbe00a42f8215a740fbca72d86`.
Beolvadt: 2026-10-29 06:41:58 Europe/Budapest (05:41:58 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422195),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422322),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422266).

Beolvadt: **57 CTest-csoport / 87 Python-eset;480 eredeti kezelő;
232 kiválasztott /32 hiányzó /135 kizárt profilú kódolássor** (nem CPU-százalék).
MULXU.B:65536 operanduspár,19200 alias/CCR-határeset,161 szétvágott futás PASS;
200000 referenciaszelet nulla eltéréssel; product+1 negatív kontroll felismerve.
Helyi56 futtatható CTest és86 Python PASS; egy ismert Windows-symlink eset kizárva.
Nincs aktív motorjelölt. A szó-szorzás int-promóciós kockázata külön nyitott;
[bekötési feltételek és bizonyítékok](h8-byte-multiply.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás továbbra is nyitott.
Az alábbi jelölt/CI-pending bejegyzések történeti előkészítési állapotok.

## Történeti lezárás — #78, 2026-10-29

Ellenőrzött motoralap: [#78](https://github.com/RobCZart82/yaman1x/pull/78),
merge `d4ef0e45bc416f06216d43559e1f73df1d39caa8`, végső tesztelt head
`ef6adefadb5b390f6822ae34c4db41edab5b1348`. Beolvadt: 2026-10-29 06:26:01 Europe/Budapest
(2026-10-29 05:26:01 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703637),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703598),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703615).

Beolvadt csomag: **56 CTest-csoport, 87 Python-eset; 478 eredeti kezelő;
231 kiválasztott / 33 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
Hat előjeles 16 bites eltolásos memóriautasítás-csoport készült el. Független
referencia: 200000 szelet, nulla eltérés; a szándékos előjelhiba felismerve.
Helyi Windows: 55 futtatható CTest és 86 Python PASS; egy ismert symlink-jogosultsági
eset kizárva. A CI a teljes csomagot ellenőrizte. M0 részleges; teljes AN1x-gép,
firmware-es MIDI, hang és plugin elfogadása nyitott. Nincs aktív motorjelölt.
A korábbi jelölt/CI-pending bejegyzések történetiek; lezárásuk itt és a JSON-naplóban szerepel.

## Történeti lezárás — #77, 2026-10-29

Ellenőrzött motoralap: [#77](https://github.com/RobCZart82/yaman1x/pull/77),
merge `8993a6024b0cedfe0d3f5c48a2819009c98643f1`, végső tesztelt head
`fb2bc0935aad976706b0652d21275e5a55d0efd8`. Beolvadt: 2026-10-29 00:49:52 Europe/Budapest
(2026-10-28 23:49:52 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788596091),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595788),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595917).

Beolvadt csomag: **55 CTest-csoport, 87 Python-eset; 466 eredeti kezelő;
225 kiválasztott / 39 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
A fix profilú IRQ/NMI/TRAPA és a szintetikus CPU–SCI RX/TX/hibakezelés elkészült;
a teljes AN1x-gép, firmware-es MIDI, hang és plugin elfogadása nyitott. M0 részleges.
Az alábbi korábbi jelölt/CI-pending bejegyzések történetiek. Az új eltolásos
memóriautasítások külön jelöltet alkotnak, nem írják felül ezt a lezárt motoralapot.

## Történeti lezárás — #57, 2026-10-28

Ellenőrzött motoralap: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), merge
`f8b6aa20834f209931e3e9bb9463cc47b5b7a540`. Végső tesztelt head:
`bf3e7158a9a4dac6edb36ea4deb659bdf3c54cb1`.
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365642),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365647),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365644).
A beolvadt csomag **39 CTest-csoportot és 84 Python-esetet** tartalmaz.
Helyi Windows-ellenőrzés: 38 futtatható CTest-csoport és 83 Python-eset PASS;
egy symlink-jogosultsági eset kizárva (WinError 1314). A CI a teljes csomagot futtatta.

A normál, rögzített H8H-profil verem-/vezérlésátadási családjai elkészültek;
366 eredeti full/partial kezelő szerepel a részhalmazban. Az RTE szintetikus
veremkép-visszaállítása tesztelt, az IRQ/TRAPA-belépés még nincs bekötve.
M0 részben teljesült; M1 nincs elfogadva; M2–M6 nyitott. Nincs igazolt AN1x-hang
vagy REAPER-plugin. A következő motorfeladatok: hiányzó H8H-utasítások/címzések
leltára, IRQ/TRAPA-belépés és szűrők, majd közös CPU/periféria-ütemezés és gépintegráció.

A korábbi „aktív”, „jelölt”, „CI pending” és „következő” bejegyzések az akkori
állapotot őrzik; a jelenlegi készültségre ez a lezárás és az aktuális mérföldkő-tábla
irányadó. A dokumentáció mostani javítása nem új motorfejlesztés vagy elfogadás.

## Ellenőrzött lezárások — #39–#44

Ellenőrizve: 2026-10-27. Mind a hat PR beolvadt; mindegyik végső headjén öt ellenőrzés PASS. Az alábbi lezárás az irányadó a korábbi, készítéskori „CI pending / review alatt” megjegyzésekkel szemben. Azok történeti pillanatképek, nem aktuális akadályok. A PR-head CI nem hardver-/firmware-/audioelfogadás.

| PR | Ellenőrzött head | Linux/macOS/Windows | MSan | MAME |
|---|---|---|---|---|
| [#39](https://github.com/RobCZart82/yaman1x/pull/39) | `9459a805af0834c5ad66329f0f1b037a9354b08d` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566377) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566528) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292565993) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549567/job/122292568462) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549032/job/122370894035) |
| [#40](https://github.com/RobCZart82/yaman1x/pull/40) | `34cec0074577a1add862772bcf3bc289de735b68` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453074) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453726) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453515) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557238091/job/122376454019) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237934/job/122376453689) |
| [#41](https://github.com/RobCZart82/yaman1x/pull/41) | `387c5e64c0ee9759b3d41dfa58bcda1b35594731` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943734) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943358) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943754) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011629/job/122407456482) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566012882/job/122408084911) |
| [#42](https://github.com/RobCZart82/yaman1x/pull/42) | `a67b599f880f0981d68e04b276b0d904ea3f7d29` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580997) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580920) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580479) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072700/job/122432444681) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072611/job/122432149653) |
| [#43](https://github.com/RobCZart82/yaman1x/pull/43) | `83caf332df3539037187888c60bc35ae422dc2a3` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457473798) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474477) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474285) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107561/job/122456981421) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107642/job/122457680585) |
| [#44](https://github.com/RobCZart82/yaman1x/pull/44) | `1bed694bdc066a66bcebb747ed74a052a4650c25` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074548) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074297) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074723) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879809/job/122480613181) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879880/job/122480584462) |

## A nyilvántartás feladata

Ez a repository-szintű változásjegyzék a fejlesztéseket, javításokat és
kapcsolódó bizonyítékokat köti a PR-ekhez. A részletes mérföldkő-elfogadás és a
PR #16–#36 végső CI-revízióinak táblája továbbra is a
[REAPER-tervben](FIRST_REAPER_SOUND_PLAN.md) marad. Az aktuális készültséget a
[STATUS](STATUS.md), az audit megállapításait a
[2026-10-28-as jelentés](REPOSITORY_AUDIT_2026-10-28.md) tartalmazza.

A korábbi napló tehát már létezett a tervben; ez a dokumentum teljesebb,
külön megtalálható könyvelést ad, és nem nyilvánít késznek nyitott mérföldkövet.
A [strukturált nyilvántartás](development-ledger.json) tartalmazza a teljes
merge- és PR-head SHA-kat, valamint az eredeti UTC-időbélyegeket.

## AUD-2026-10-27 — repository-audit és javítások

Állapot: [#38 PR](https://github.com/RobCZart82/yaman1x/pull/38) beolvadt 2026-10-27 15:36:15 UTC-kor; merge: `47950097b564f7ca48a1a2fd27a125066c3fc7a9`.

A helyben tesztelt kódfa a `de892c3ea278611d1a60e68ccbad8991e174b768`
GitHub-revízió fájlfájával azonos. A követő változás csak e napló metaadatait
rögzíti; a legutóbbi PR-head saját CI-je az irányadó.

- Átvizsgált terület: build/CI, saját C++/Python kód és tesztek, kutatási patch-lánc,
  forrás-/licencleltár, állapotdokumentumok és PR-előzmények.
- Javítás: az élő bridge auditor elfogadja a jelenlegi register-only ABI 2 fejlécet,
  megtartja a régi ABI 1/2 formátumot, és elutasítja az ellentmondó órajeladatot.
- Regresszió: az új teszt a javítás előtt hibázott; utána négy célzott teszt
  normál és `python -O` módban is PASS. Yamaha-adat nem kellett hozzá.
- Dokumentáció: elavult DSP-időléptetési, tesztszám- és változatlan upstream
  forrásra vonatkozó állítások javítva; aktuális és történeti eredmények jelölve.
- Ellenőrzés: tiszta macOS arm64 Release build; alapállapot 22/22 CTest és
  56 Python-eset PASS; a jelölt négy új Python-esetet ad ugyanazon csoporthoz.
  A részletes, végső eredmény az auditjelentésben és a PR saját checkjeiben van.
- Nyitott: SCI IRQ/MMIO és H8-ütemező/CPU-bekötés; firmware-es PKS/MIDI,
  AN1x órajel/EG/táblacímzés, audio és REAPER-elfogadás.

## SCI-MMIO-2026-10-27 — kétcsatornás SCI busz/IRQ-adapter

Állapot: [#39 PR](https://github.com/RobCZart82/yaman1x/pull/39) beolvadt 2026-10-27 18:06:08 UTC-kor; merge: `6c1dd8a0c83eac1d593abd4e70c040f88908bb48`.
A hatregiszteres bájt/szó MMIO és a csatornánkénti ERI/RXI/TXI/TEI útvonal elkészült, DMA-elsőbbséggel. Szintetikus 512 RX bájt/csatorna eset, TX, hibák, reset és izoláció ellenőrizve. Az eredeti PR-head öt platform/build ellenőrzése sikeres volt; a #38 főági dokumentációjával feloldott ütközés után új CI szükséges. A kombinált fa helyben 23/23 CTest-csoporttal és 60 Python-esettel PASS. M0 részben teljesült; CPU-ütemezés és firmware/audio továbbra is nyitott.

## #45 dokumentációs lezárás

Beolvadt 2026-10-27 21:58:12 UTC: `ed966cef79eb33eb309165e9e9d6edca920410ea`; head `eee27cef236fb54cb0d012f1df1716d2d03fa63c`. [Három platform PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542410), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542442), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542551). A strukturált leltár #45-öt is tartalmazza; az alábbi régebbi táblázat #44-ig történeti index.

## Beolvadt változások — a legutóbbi elöl

A következő 39 bejegyzés GitHub PR-metaadatból és az aktuális `main` Git-előzményéből
ellenőrzött visszamenőleges könyvelés (#6–#44). A beolvadás bizonyított; a
felsorolás önmagában nem állít minden PR-re újrafuttatott hardveres vagy helyi
validációt. A kezdeti importot a [történeti összefoglaló](HISTORY.md) őrzi.

| Beolvasztás (Budapest) | PR | Fejlesztés / javítás | Beolvasztási commit |
|---|---|---|---|
| 2026-10-27 22:43:08 | [#44](https://github.com/RobCZart82/yaman1x/pull/44) | H8 ciklustartozás és perifériahatáridők | `b644b6e1c471f8632ea086d75bd37d20b5ebbeb4` |
| 2026-10-27 21:49:51 | [#43](https://github.com/RobCZart82/yaman1x/pull/43) | H8H buszsegédek és eredetimetódus-összevetés | `f8b1f4fbd4a422eab33d633e2384f1c82c11623d` |
| 2026-10-27 21:33:08 | [#42](https://github.com/RobCZart82/yaman1x/pull/42) | H8 CPU gazdaszolgáltatás-határ és reprodukálható leltár | `e3f71a89f6d967034a3c05926a7a86c595ab9a49` |
| 2026-10-27 20:47:56 | [#41](https://github.com/RobCZart82/yaman1x/pull/41) | Kétcsatornás SCI-idővonal | `259338dbffed8b5b10b606c5f8f907bd791b1b74` |
| 2026-10-27 19:56:09 | [#40](https://github.com/RobCZart82/yaman1x/pull/40) | Korlátozott ciklus-eseményfuttató | `76787a6ee5450fa29a7403b658fc861ef908a297` |
| 2026-10-27 19:06:08 | [#39](https://github.com/RobCZart82/yaman1x/pull/39) | Két SCI-csatorna MMIO/IRQ-adaptere | `6c1dd8a0c83eac1d593abd4e70c040f88908bb48` |
| 2026-10-27 16:36:15 | [#38](https://github.com/RobCZart82/yaman1x/pull/38) | Repository-audit, bridge-kompatibilitási javítás és fejlesztési napló | `47950097b564f7ca48a1a2fd27a125066c3fc7a9` |
| 2026-10-27 13:32 | [#37](https://github.com/RobCZart82/yaman1x/pull/37) | Reconcile development plan with verified PR closures and historical snapshots | `276399870400` |
| 2026-10-27 13:10 | [#36](https://github.com/RobCZart82/yaman1x/pull/36) | Unify asynchronous H8 SCI transmit and receive under one channel | `33dc7e97c174` |
| 2026-10-27 12:22 | [#35](https://github.com/RobCZart82/yaman1x/pull/35) | Compose H8 asynchronous SCI receiver with sampled input and errors | `e7257ebe2bbc` |
| 2026-10-27 11:59 | [#34](https://github.com/RobCZart82/yaman1x/pull/34) | Compose H8 asynchronous SCI transmitter with status and cycle clock | `389594251b9b` |
| 2026-10-27 11:43 | [#33](https://github.com/RobCZart82/yaman1x/pull/33) | Update LICENSE | `6e5da15024e3` |
| 2026-10-27 11:27 | [#32](https://github.com/RobCZart82/yaman1x/pull/32) | Track reviewed third-party license coverage and enforce source notices | `cded711258a3` |
| 2026-10-27 11:00 | [#31](https://github.com/RobCZart82/yaman1x/pull/31) | Require replaceable PKS backends and a versioned firmware upgrade path | `e1bb9206b024` |
| 2026-10-27 10:56 | [#30](https://github.com/RobCZart82/yaman1x/pull/30) | Record PKS replacement ideas as unverified research options | `bc946bd39d2b` |
| 2026-10-27 10:47 | [#29](https://github.com/RobCZart82/yaman1x/pull/29) | Extract H8 SCI status latch with exhaustive flag transition checks | `3890122ffddf` |
| 2026-10-27 10:11 | [#28](https://github.com/RobCZart82/yaman1x/pull/28) | Extract internal H8 SCI cycle clock and refresh milestone plan | `019070d95f82` |
| 2026-10-27 09:46 | [#27](https://github.com/RobCZart82/yaman1x/pull/27) | Adapt H8H IRQ registers and pins preserving upstream publication | `3ec574095dfa` |
| 2026-10-27 09:09 | [#26](https://github.com/RobCZart82/yaman1x/pull/26) | Extract standalone H8H internal interrupt arbitration | `3a6cf4832025` |
| 2026-10-27 08:48 | [#25](https://github.com/RobCZart82/yaman1x/pull/25) | Record completed H8 peripheral CI evidence | `26dce3d3ceff` |
| 2026-10-27 08:44 | [#24](https://github.com/RobCZart82/yaman1x/pull/24) | Prepare H8 source boundary and extract standalone digital port | `14635883d5e3` |
| 2026-10-27 08:15 | [#23](https://github.com/RobCZart82/yaman1x/pull/23) | Prepare paired local M1 serial experiment for Mac continuation | `54757fbcb340` |
| 2026-10-27 07:35 | [#22](https://github.com/RobCZart82/yaman1x/pull/22) | Initialize VOP3 decoded operations before the first comparison | `94482f2e05a9` |
| 2026-10-27 07:08 | [#21](https://github.com/RobCZart82/yaman1x/pull/21) | Record verified MAME build evidence and remaining M1 acceptance | `c42c6f244631` |
| 2026-10-27 07:02 | [#20](https://github.com/RobCZart82/yaman1x/pull/20) | Pin MAME research source and compile the full patched target in CI | `4bca07273afa` |
| 2026-10-27 06:32 | [#19](https://github.com/RobCZart82/yaman1x/pull/19) | Add timestamped SCI access observation for M1 startup research | `291e04554c59` |
| 2026-10-27 06:23 | [#18](https://github.com/RobCZart82/yaman1x/pull/18) | Track development plan progress with dated validation evidence | `064ddbe56dd7` |
| 2026-10-27 06:19 | [#17](https://github.com/RobCZart82/yaman1x/pull/17) | Record M0 engine architecture and dependency baseline | `865e47930497` |
| 2026-10-27 06:17 | [#16](https://github.com/RobCZart82/yaman1x/pull/16) | Keep ROM-free ABI checks active under Python -O | `d70a309ca08a` |
| 2026-10-27 05:18 | [#15](https://github.com/RobCZart82/yaman1x/pull/15) | Plan development through the first audible voice in REAPER | `d24237292908` |
| 2026-10-27 00:03 | [#14](https://github.com/RobCZart82/yaman1x/pull/14) | Observe both H8 SCI devices and keyboard-port traffic | `1f9163c2866e` |
| 2026-10-26 23:53 | [#13](https://github.com/RobCZart82/yaman1x/pull/13) | Audit MIDI input at H8 SCI0 | `216af8b06070` |
| 2026-10-26 23:38 | [#12](https://github.com/RobCZart82/yaman1x/pull/12) | Route MAME MIDI input to H8 SCI0 | `db9028297e05` |
| 2026-10-26 23:29 | [#11](https://github.com/RobCZart82/yaman1x/pull/11) | Add opt-in timed H8 VOP3 experiment | `b735bd3e8cc6` |
| 2026-10-26 23:02 | [#10](https://github.com/RobCZart82/yaman1x/pull/10) | Document AN1x table-ROM findings and audit tool | `f19674cebede` |
| 2026-10-26 22:34 | [#9](https://github.com/RobCZart82/yaman1x/pull/9) | Add ROM-free cross-platform CI | `cdfce265590e` |
| 2026-10-26 22:26 | [#8](https://github.com/RobCZart82/yaman1x/pull/8) | Align branch ruleset docs with zero required approvals | `f79779625139` |
| 2026-10-26 21:29 | [#7](https://github.com/RobCZart82/yaman1x/pull/7) | Record public research release and active main-branch protection | `9978fb71b0ea` |
| 2026-10-26 20:54 | [#6](https://github.com/RobCZart82/yaman1x/pull/6) | Prepare main-branch protection ruleset; activation requires eligible plan | `89e54b953089` |

## Következő bejegyzések vezetése

Minden új fejlesztés és javítás saját dátumozott bejegyzést kap ugyanabban a
PR-ben. Kötelező adatok: probléma/cél, változás, érintett mérföldkő, PR vagy
commit, ellenőrzések és platform, maradó korlát, következő feladat. A PASS,
FAIL, NOT RUN és környezeti akadály külön szerepeljen. A végső merge SHA a
beolvasztás utáni egyeztetéskor kerül be; ismeretlen adat maradjon függőben.

A részletes vezetési szabály: [CONTRIBUTING](../CONTRIBUTING.md). A PR-sablon
emlékeztet a frissítésre; ez nem automatikus naplózási szolgáltatás. A történeti
adatokat megőrizzük, a téves következtetést új, indokolt korrekcióval javítjuk.

## SCHED-2026-10-27 — korlátozott ciklus-eseményfuttató

PR: [#40](https://github.com/RobCZart82/yaman1x/pull/40), ág: `feat/bounded-cycle-dispatcher`; [szerződés](cycle-dispatcher.md). Új, saját komponens, nem upstream átvétel. Költségkeret, folytatás, időmonotonitás, hibahatár és izoláció tesztelve; 1000 szintetikus eseménysor felosztásfüggetlen. Külön PR a #39-től; annak státuszát nem zárja le. A teljes CPU/periféria-ütemezés és M0 elfogadása nyitott.

Helyi záróellenőrzés: macOS arm64 Release, 23/23 CTest-csoport és 60 Python-eset PASS; natív ASan/UBSan PASS. Végső platform-CI a PR-head checkjeiben.

2026-10-27, #39 utáni szinkronizálás: a build- és naplóütközések feloldva, mindkét komponens megtartva. Az egyesített kódfa helyben 24/24 CTest-csoporttal és 60 Python-esettel PASS. Az előző 23-as eredmény a különálló ág történeti bizonyítéka. Új GitHub CI a szinkronizált #40-headen szükséges.

## SCI-TIMELINE-2026-10-27 — közös SCI-eseménytulajdonos

PR: [#41](https://github.com/RobCZart82/yaman1x/pull/41), ág: `feat/sci-cycle-timeline`, alap: beolvadt #40. Saját új komponens, új upstream átvétel nélkül. [Részletes szerződés és határok](sci-cycle-timeline.md). Időbélyegzett bemenet, két csatorna, eseményenkénti keret és determinisztikus sorrend; 256 teljes duplex minta három futtatási felosztása egyezik. CPU-végrehajtás és M0 lezárása nyitott.

Helyi ellenőrzés: macOS arm64 Release, 25/25 CTest-csoport és 60 Python-eset PASS; az új natív teszt ASan/UBSan alatt is PASS. Végső platform-CI a kapcsolódó PR-headhez kötve ellenőrizendő.

## H8-HOST-2026-10-27 — CPU gazdaszolgáltatás-határ

PR: [#42](https://github.com/RobCZart82/yaman1x/pull/42), ág: `feat/h8-host-services-boundary`, alap: beolvadt #41. Új ROM-mentes elemző és [áttekintett integrációs szerződés](h8-host-services-boundary.md). 34 forrás-/generált fájl, hash-ek és tokenhelyek, két független bájtazonos regenerálás. Nyolc új Python-regresszió; CPU-fordítás/végrehajtás és függőségzár továbbra is nyitott.

Helyi ellenőrzés: macOS arm64 Release; 25/25 CTest-csoport és 68 Python-eset sikeres; a forrás- és licencértesítési audit sikeres. A végleges PR-fej platformellenőrzése még függőben volt.

## H8H-BUS-2026-10-27 — CPU buszsegédek

PR: [#43](https://github.com/RobCZart82/yaman1x/pull/43), ág: `feat/h8h-bus-access`, alap: beolvadt #42. [Szerződés és bizonyíték](h8h-bus-access.md). BSD-forráseredet rögzítve, licenclefedettség frissítve. Öt önálló buszsegéd; 200000 műveletes eredetimetódus-összevetés 0 eltéréssel, 65536 szóérték és hibahatárok. CPU-futtató, firmware és hang továbbra is nyitott.

Helyi ellenőrzés: macOS arm64 Release; 26/26 CTest-csoport és 72 Python-eset sikeres; az új natív ASan/UBSan-vizsgálat sikeres. A végleges PR-fej platformellenőrzése külön bizonyíték.

## H8-BUDGET-2026-10-27 — végrehajtási keret számtana

PR: [#44](https://github.com/RobCZart82/yaman1x/pull/44), ág: `feat/h8-execution-budget`, alap: beolvadt #43. [Szerződés és bizonyíték](h8-execution-budget.md). Ciklustartozás, határidő és stabil CPU-counter a buszsegédhez; 1000 felosztási történet és 400000 eredetimetódus-összevetés 0 eltéréssel. Licenc- és eredetnyilvántartás frissítve. Teljes CPU-utasításvégrehajtás, firmware és hang nincs.

Helyi ellenőrzés: macOS arm64 Release; 27/27 CTest-csoport és 74 Python-eset sikeres; az új natív ASan/UBSan-, forrás- és licencértesítési audit sikeres. A végleges PR-fej ellenőrzése még függőben volt.

## H8-RUNNER-2026-10-28 — végrehajtási sorrend

PR: [#46](https://github.com/RobCZart82/yaman1x/pull/46). Ág: `feat/h8-execution-order`, alap: #45. Korabeli jelöltállapot: H8 full/partial/periféria-sorrend a meglévő cikluskeret fölött, nem teljes CPU. 200000 eseményes referencia-szelet: 338598 update, 761211 full, 276336 partial callback, nulla eltérés. Natív 1000 felosztás és hibahatárok; két új Python regresszió. A Windows helyi teljes Python-csoport symlink-jogosultsági korlátját külön kezeljük; végső PR-head CI szükséges. Következő: tényleges generált kezelők és CPU-regiszterállapot. M0 részben teljesült.

## H8-FIRST-2026-10-28 — első generált utasítások

Előző lépcső lezárása: [#46](https://github.com/RobCZart82/yaman1x/pull/46): merge `00e16ce805d542e4a654923ddde2d462ebac78bb`, végső head `478294b0570b0140b0ba2c5f78b65fcc99fecaa4`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467656), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467504), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467588).

PR: [#47](https://github.com/RobCZart82/yaman1x/pull/47). Ág: `feat/h8-first-generated-handlers`, alap: beolvadt #46. Korabeli jelöltállapot: nyolc eredeti generált full/partial kezelő (reset, NOP, MOV.B immediate, BRA rel8) fordul és fut saját CPU-állapottal a busz-/cikluskereten. Az összes többi opcode explicit hiba. 4096 regiszter/érték, 65536 jelzőbit-eset, 256 elágazási eltolás és 1000 futásfelosztás tesztelve. Eredeti H8H-dekóderrel/kezelőkkel/futtatóval 200000 szakasz és 899400 olvasás, nulla eltérés; szándékos regiszterhibát a kontroll észleli. Hat új Python regresszió. Részletek: [komponensszerződés](h8-first-instructions.md). Teljes CPU, IRQ/SCI integráció és firmware/audio nincs; M0 részben teljesült. A jelölt végső PR-head CI-je külön ellenőrizendő.

## H8-MEMORY-BRANCH-2026-10-28 — adatmemória és feltételes elágazások

Előző lépcső lezárása: [#47](https://github.com/RobCZart82/yaman1x/pull/47): merge `343f4c32c81229e89ec092b0d8e324f2a0305f8a`, végső head `6f8107ec35e511c6aa9d347295e67a882a7826d3`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640131062), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130940), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130959).

PR: [#48](https://github.com/RobCZart82/yaman1x/pull/48). Ág: `feat/h8-memory-conditional-branches`, alap: beolvadt #47. Korabeli jelöltállapot: 44 eredeti generált full/partial kezelő; MOV.L immediate32, indirekt bájtolvasás/-írás és mind a 16 rel8 feltétel. 65536 elágazási és 65536 dekódereset, 32768–32768 load/store eset, 1000 felosztás; két új Python regresszió. Eredeti H8H-kóddal 200000 szakasz, 844255 utasítás-, 27571 adatolvasás, 27574 adatírás: nulla eltérés; szándékos adatírási hibát a kontroll észleli. [Szerződés és korlátok](h8-memory-branches.md). M0 részben teljesült; IRQ/SCI CPU-bekötés, teljes CPU és firmware/audio továbbra is nyitott. A végső PR-head CI-je külön ellenőrizendő.

## H8-WORD-LONG-2026-10-28 — szó- és hosszúszó-átvitel

Előző lépcső lezárása: [#48](https://github.com/RobCZart82/yaman1x/pull/48): merge `800f8b4eabfe484b54e9918a652096c0acf50986`, végső head `a2da3e66797ec5d9f617e3e85b1e6505ccf26beb`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345253), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345183), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345179). Az automatikus review a beolvasztáskor még futott; nem lezárt eredményként szerepel. A beolvasztás a kézi forrásellenőrzés, eredeti kódos kontrollok és mind az öt sikeres CI alapján történt.

PR: [#49](https://github.com/RobCZart82/yaman1x/pull/49). Ág: `feat/h8-word-long-memory`, alap: beolvadt #48. Korabeli jelöltállapot: 56 eredeti generált full/partial kezelő; MOV.W immediate, indirekt word/long load/store és eredeti 0100 prefixállapot. Mind a 65536 szóérték, regiszter-/címátfedések, 65536 prefixkiterjesztés és 1000 felosztás ellenőrizve. Eredeti H8H-kóddal 200000 szakasz: 745100 utasításolvasás, 19304 bájtolvasás, 19310 bájtírás, 57833 szóolvasás, 57853 szóírás; nulla eltérés. Szándékos szó-bájtsorrendhibát a kontroll észleli. [Szerződés és korlátok](h8-word-long-memory.md). M0 részben teljesült; aritmetika, IRQ/SCI és teljes CPU/firmware/audio továbbra is nyitott. Végső PR-head CI külön ellenőrizendő.

## H8-ARITHMETIC-2026-10-28 — ADD/SUB/CMP és jelzőbitek

Előző lépcső lezárása: [#49](https://github.com/RobCZart82/yaman1x/pull/49): merge `4567d7bfe852100f5483a8104bcba81b50ae26bb`, végső head `f69503487de2fbbf3c575d81f54ca5a91e965f90`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971856), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971826), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971835). Az automatikus review a `5aff015d6157f166d9f26d0591e7b13d067cd171` forrásrevízión hibát nem jelzett; a követő commit csak PR-metaadatot rögzített a dokumentációban.

PR: [#50](https://github.com/RobCZart82/yaman1x/pull/50). Ág: `feat/h8-arithmetic-flags`, alap: beolvadt #49. Korabeli jelöltállapot: 78 eredeti generált full/partial kezelő; ADD/CMP byte immediate, ADD/SUB/CMP byte register és word/long immediate formák. 327680 byte-bemenetpár, 12288 regiszterátfedés, 100800 szélesebb eset és 1000 felosztás tesztelve. Matematikai overflow/borrow orákulum; CMP nem ír regisztert. Eredeti H8H-kóddal 200000 szakasz: 791089 utasításolvasás, 13555 byte-olvasás, 13557 byte-írás, 40593 word-olvasás és 40606 word-írás; nulla eltérés. Szándékos half-carry-hibát a kontroll észleli. [Szerződés és korlátok](h8-arithmetic-flags.md). M0 részben teljesült; további aritmetika/logika/shift, stack/IRQ és CPU/SCI-integráció nyitott. Végső PR-head CI külön ellenőrizendő.

## H8-CARRY-LOGIC-2026-10-28 — carry, regiszterek és logika

Előző lépcső lezárása: [#50](https://github.com/RobCZart82/yaman1x/pull/50): merge `0f8d3ae96eb32dcabb3a039f40e5c0fd0315a1b9`, végső head `4bbea3449f5d73f15031332b537919b493a5defc`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119562), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119555), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119545). Automatikus review a `ddc8d51221838e64c9e849101756c1aeca260dce` forrásrevízión: kész, nincs találat; a követő commit csak PR-metaadatot rögzített.

Ág: `feat/h8-carry-register-logic`, alap: beolvadt #50. Korabeli jelöltállapot: 124 eredeti full/partial kezelő; wide register ADD/SUB/CMP, byte ADDX/SUBX, byte register/immediate és wide immediate logika, LDC CCR. 1048576 carry/Z eset, 30720 wide register eset, 393216 byte logika, 12288 logikai átfedés, 36864 wide logika, 20000 négybyte-os lánc, 1000 felosztás. Eredeti kóddal 200000 szakasz, nulla eltérés; hibás Z-assertet a kontroll észleli. [Szerződés és korlátok](h8-carry-register-logic.md). M0 részben teljesült; LDC csak CCR-állapot, nem IRQ-elfogadás. Shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott. Végső PR-head CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: carry/regiszter/logika fejlesztés: [PR #51](https://github.com/RobCZart82/yaman1x/pull/51), `feat/h8-carry-register-logic`. Helyben 32 CTest-csoport és 83 Python-eset PASS; egy symlink-jogosultsági eset kihagyva. A végső head teljes CI-je külön ellenőrzendő; M0 részben teljesült.

## H8-WIDE-LOGIC-2026-10-28 — széles regiszteres logika

Előző lépcső lezárása: [#51](https://github.com/RobCZart82/yaman1x/pull/51), merge `ba2c91d8a156972bcf3c7b9adab98e36f7279b0c`, végső head `931a5a50dfe57c575d45d9d8b6b817889501a5a0`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821739), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821601), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821621). A forrásrevízió (`df6ce5574346e32b57a81dffb5c5b5f0e7929bbf`) automatikus review-ja kész, nincs találat; a követő commit dokumentációs PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-wide-register-logic`, alap: beolvadt #51. 138 eredeti generált full/partial kezelő: word/long regiszteres OR/XOR/AND és eredeti 01f0 prefix. 196608 word-eset, 245760 regiszter/CCR-kombináció, 65536 prefixkiterjesztés és 1000 felosztás PASS. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; XOR helyett OR hibát a kontroll észleli. [Szerződés és korlátok](h8-wide-register-logic.md). Helyben 33 natív CTest-csoport PASS; a teljes 34 csoport / 84 Python-eset végső CI-je külön ellenőrizendő. M0 részben teljesült; shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott.

Korabeli jelöltbejegyzés: széles regiszteres logika: [PR #52](https://github.com/RobCZart82/yaman1x/pull/52), `feat/h8-wide-register-logic`. Helyi 33 natív csoport / 83 Python-eset PASS; a teljes végső head CI-je külön ellenőrizendő.

## H8-SHIFT-ROTATE-2026-10-28 — egybites eltolás és forgatás

Előző lépcső lezárása: [#52](https://github.com/RobCZart82/yaman1x/pull/52), merge `88e88bd8551f78bc312230d459ea3c92d7b66f01`, végső head `eac6be6a95e22f1266730c6941efa0c766658913`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124816), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124527), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124813). A `c726582b72c43bc87cc917f1ff60c10f762a768a` forrásrevízió review-ja kész, nincs találat; a későbbi dekódersorrend-védelmet célzott és eredeti kódos teszt igazolja. Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-shift-rotate`, alap: beolvadt #52. 186 eredeti generált full/partial kezelő; SHLL/SHAL/SHLR/SHAR/ROTXL/ROTL/ROTXR/ROTR 8/16/32 biten. Független bitenkénti orákulum, signed-range overflow, 1744896 operandus/CCR/regisztersáv-eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; a megerősített explicit carry-program a kihagyott ROTXR carry-t seed 2/slice 8 helyen észleli. [Szerződés és korlátok](h8-shift-rotate.md). M0 részben teljesült; kétbites változatok, bitműveletek, stack/IRQ és CPU/SCI-integráció nyitott. A jelölt teljes CI-je külön ellenőrizendő.

Korabeli jelöltbejegyzés: egybites shift/rotate fejlesztés: [PR #53](https://github.com/RobCZart82/yaman1x/pull/53), `feat/h8-shift-rotate`. Helyi 34 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-REGISTER-BITS-2026-10-28 — regiszteres bitműveletek

Előző lépcső lezárása: [#53](https://github.com/RobCZart82/yaman1x/pull/53), merge `806ddf86a6373731f7b138c985a55ed41c583d6a`, végső head `b234b460a19d63d9a6c80e2086a4172a9a313a26`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288385), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288495), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288416). A `8ea376e0ba91bbc83ff94b521c5f0082bb093811` forrás review-ja kész, nincs találat; követő commit csak PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-register-bit-operations`, alap: beolvadt #53. 222 eredeti generált full/partial kezelő; regiszteres/immediate BSET/BNOT/BCLR/BTST, BST/BIST és carry logikai/load formák. 8126464 független operandus/bitindex/CCR/átfedési eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a BNOT helyett BSET hibát seed 3/slice 51 helyen felismeri a kontroll. [Szerződés és korlátok](h8-register-bit-operations.md). **Tervpontosítás:** a kétbites shift/rotate az eredeti listában `s20` (H8S/2000), a H8H-dekóderben nincs; nem H83002-fejlesztési feladat. Memóriás bitműveletek, stack/control flow és IRQ/SCI-integráció nyitott; M0 részben teljesült. Végső jelölt-CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: regiszteres bitműveleti fejlesztés: [PR #54](https://github.com/RobCZart82/yaman1x/pull/54), `feat/h8-register-bit-operations`. Helyi 35 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-MEMORY-BITS-2026-10-28 — indirekt memóriás bitműveletek

Előző lépcső lezárása: [#54](https://github.com/RobCZart82/yaman1x/pull/54), merge `efdb6df59ba53e8497d313fb33c7b40dcd9d76c4`, végső head `f9cab7c831a6aa2e880e2131b52a3623c049347c`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059555), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059577), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059727). Automatikus review-eredmény a lezáráskor nem volt; helyi forrás-/független/eredeti kódos kontroll ellenőrizve. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-memory-bit-operations`, alap: beolvadt #54. 262 eredeti generált full/partial kezelő: 7c/7d prefix és 18 indirekt bájtos bitművelet. 7340032 immediate eset, 131072 cím/indexátfedés, 131072 prefixkiterjesztés, 31 megszakítási pont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; read/prefetch/write sorrend és pontosan egyszeri mellékhatás ellenőrizve. [Szerződés és korlátok](h8-memory-bit-operations.md). M0 részben teljesült; abszolút memóriás formák, stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: indirekt memóriás bitműveleti fejlesztés: [PR #55](https://github.com/RobCZart82/yaman1x/pull/55), `feat/h8-memory-bit-operations`. Helyi 36 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-ABSOLUTE-BITS-2026-10-28 — abszolút memóriás bitműveletek

Előző lépcső lezárása: [#55](https://github.com/RobCZart82/yaman1x/pull/55), merge `e53b6fe7e2906b472e898c57d59ccfe1b791fa96`, végső head `452f9585afe1e67ceb57cd9cc8ac258202cd3cf7`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897203), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897191), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897202). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-absolute-bit-operations`, alap: beolvadt #55. 302 eredeti generált full/partial kezelő; 7e/7f prefix és 18 abs8 bitművelet. 7340032 immediate eset, 262144 cím/regisztersáv eset, 131072 prefixkiterjesztés, 31 futtatási vágáspont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés. [Szerződés és korlátok](h8-absolute-bit-operations.md). M0 részben teljesült; stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: abszolút memóriás bitműveleti fejlesztés: [PR #56](https://github.com/RobCZart82/yaman1x/pull/56), `feat/h8-absolute-bit-operations`. Helyi 37 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-STACK-CONTROL-2026-10-28 — veremkezelés és vezérlésátadás

Előző lépcső lezárása: [#56](https://github.com/RobCZart82/yaman1x/pull/56), merge `a3c2eb8ba25fc40eaa5057cc388260e4fc58b9df`, végső head `dee08721b3c2d3ccb16e1dfa4b4475625889c962`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502703), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502638), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502838). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-stack-control-flow`, alap: beolvadt #56. 366 eredeti full/partial kezelő; predecrement/postincrement MOV byte/word/long, JMP/JSR mindhárom címzése, BSR rel8/rel16, mind a 16 rel16 feltétel, RTS és szintetikus RTE-veremkép-visszaállítás. Független előjeles eltolás-/alias-/címhatár-/CCR-próbák, 201 beágyazott és 388 további hívás/RTE vágáspont, buszhiba-kontroll. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a hibás RTS SP+2 értéket seed 6/slice 5 helyen felismeri a kontroll. [Hatókör, bizonyíték és fennmaradó korlátok](h8-stack-control-flow.md). A normál verem/vezérlés utasításcsaládok ebben a rögzített H8H-profilban megvalósítva; IRQ/TRAPA-belépés, teljes CPU és CPU/SCI-integráció nyitott, M0 részben teljesült. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: verem/vezérlésátadási fejlesztés: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), `feat/h8-stack-control-flow`. Helyi 38 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő. Normál utasításcsaládok készültek el; IRQ/TRAPA-belépés továbbra is nyitott.

## DOC-RECONCILE-2026-10-28 — aktuális és történeti állapot egységesítése

A #57 lezárása a motoralaphoz rögzítve, a strukturált nyilvántartásban nincs nyitott motorjelölt. STATUS és roadmap egységesítve; korábbi jelöltbejegyzések megőrizve és történetiként jelölve. Az M0 következő feltétele már a hiányzó lefedettségre és gépintegrációra vonatkozik, nem az első CPU-fordításra. Dokumentációs javítás, új motor-/firmware-/audioelfogadás nélkül. Ellenőrzés: élő #57 merge/head és mind az öt CI-feladat, JSON-lezárás és egyedi PR-azonosítók, UTF-8 és helyi fájlhivatkozások, forrás- és értesítéslefedettségi audit.

A dokumentációs egységesítés PR-je: [#58](https://github.com/RobCZart82/yaman1x/pull/58). A motoralap továbbra is a lezárt #57; a dokumentációs PR saját aktuális CI-/merge-állapotát a hivatkozott PR mutatja.

## AUD-2026-10-28 — teljes repository-áttekintés

Ág: `audit/repository-2026-10-28`; átvizsgált dokumentációs főág: #58, `863d21abcde448d746b3b44c39a1eb61be455a15`. [Jelentés](REPOSITORY_AUDIT_2026-10-28.md). Helyi macOS: 39/39 CTest, 84/84 Python, 200000 eredetimetódus-szelet, célzott négy CPU ASan/UBSan-csoport PASS; forrás/licenc/napló/link ellenőrizve. Reprodukálható motorhiba nem jelentkezett. STATUS elavult „current 27” bekezdése javítva. A jelentés két CI-fejlesztési javaslatot és a nyitott CPU/firmware/audio határokat rögzíti; motorkód és mérföldkő-készültség változatlan.

Ág: `feat/h8-coverage-ccr`; alap: beolvadt #59. [Komponensjegyzőkönyv](h8-coverage-and-ccr.md). Reprodukálható 399 soros H8H-leltár és öt új CCR-család; 376 eredeti full/partial metódus, 196608 immediát CCR-eset, regiszterlane- és partíciópróbák. Eredeti kód 200000 szelet, 0 eltérés; ASan/UBSan PASS. A régi dekóderteszt 800 újonnan támogatott kódolással frissítve. M0 részben teljesült. A #59 saját négy ellenőrzése zöld volt, nyitott review-thread nélkül beolvasztva a felhasználó sorozatos merge-engedélye alapján.

Ág: `feat/h8-register-transfers`; [jegyzőkönyv](h8-register-transfers.md). Bájt/szó/hosszúszó regiszterközi MOV, hat új eredeti full/partial metódus (382 összesen), 576 további elsőszavas kódolás. 147456 lane/alias/flag eset, 1000 felosztási történet; 41/41 helyi CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. Szülő: #60; saját PR csak annak beolvasztása után, a főágra újraalapozva.

### Sorozatos lezárás — #60 beolvadt

#60 merge: `18f7b7fa1b708c67cf7ed0baa2109d5f73c750db`, 2026-10-28 15:04:13 UTC. Végső head `e8dee6975054c4143abe36b3b8731b7cea510cf4`; három platform, MSan és MAME mind PASS. A korábbi referenciafixture elérhetőségi hibája beolvadás előtt javítva. #61 a friss mainre újraalapozott, önálló CI-re kész regiszter-MOV jelölt; #62–#64 szülőfüggőség miatt draft.

Ág: `feat/h8-small-arithmetic`; [jegyzőkönyv](h8-small-arithmetic.md). 16 új csoport, 32 generált metódus (414 összesen). INC/DEC N/Z/V frissítés H/C megőrzéssel, ADDS/SUBS teljes CCR-megőrzéssel. Teljes szó/bájt tartomány és hosszúszó-határok, 1000 felosztás; 200000 eredetimetódus-szelet. Szülője a regiszter-MOV ág; PR csak annak beolvadása és újraalapozás után.

### #61 lezárása, #62 előkészítése

#61 mind az öt ellenőrzése zöld, nyitott review-thread és változtatáskérés nélkül beolvasztva: `9cedcdf92d0330103fbeac24932c5c13e3924b3a`, ellenőrzött head `c36473985c685147867f9b56ecd250b9377d037a`. A korábbi piros macOS-futások futtatóhiány miatt szakadtak meg; az újrafuttatások sikeresek. #62 önálló változtatása a friss mainre újraalapozva; saját végső CI-je még külön kapu. #63–#66 továbbra is függőségi draft.

### #62 CI-indítás javítása

A főágra célzás és ready állapot önmagában nem indított workflow-t: az újraalapozott commit feltöltésekor a PR még a korábbi feature ágat célozta, a workflow-k pedig csak main célágra indulnak. Az élő Actions API az `1d0cd3d2df4a6dfcb319822ec5a034c1aece8bc0` headhez nulla futást mutatott. Ez a naplófrissítés a már mainre célzott PR szinkronizálásával indítja a szükséges ellenőrzéseket. A következő lépcsőknél a main célág beállítása után történjen a végső feltöltés. Motorkód változatlan; nincs CI- vagy merge-szabály megkerülés.

### #62 Windows-fordítási javítás

A `9222223a7c4c4c57c3d214b34e7af87eac4410f9` head Windows CI-je MSVC C1061 hibát jelzett: a 127 ágból álló utasításválasztó else-if lánc túllépte a fordító beágyazási korlátját. A legkülső lánc sorrendtartó, korai visszatéréses if ágakká alakult; minden kiválasztott kezelő után pontosan egy kilépés történik, így a kezelő által módosított állapotra nem történik újabb kiválasztás. A generált utasítástörzsek és kódolások változatlanok. A macOS normál feladata külön futtatóhiány miatt szakadt meg; a MAME macOS referencia-, Linux- és MSan-feladat sikeres volt. Az új head saját teljes CI-je továbbra is kötelező.

A lapos választó helyi ellenőrzése: 42/42 CTest, 87 Python-eset, 200000 eredeti dekóder/execute_run referenciaszelet nulla eltéréssel; forrás-/licenc-/feltöltési audit PASS. A Windows-fordító eredménye az új CI futásától függ, helyi macOS-eredményből nincs Windows PASS állítás.

Ág: `feat/h8-unary-extension`; [jegyzőkönyv](h8-unary-extension.md). NOT/NEG/EXTU/EXTS tíz csoportban, 20 eredeti metódus, 434 összesen. 43/43 helyi CTest, 87 Python, független flag/adat orákulum, ASan/UBSan PASS. Elérhető referenciaciklus, minden új családra bejárási őrfeltétellel; 200000 szelet 0 eltérés. Szülője az INC/DEC ág; a szülők merge/CI feltételei miatt egyelőre egymásra épülő jelölt, nem main-elfogadás.

### #62 lezárása, #63 előkészítése

#62 végső head `3e1652252342b0b58d31f9f475af2a6bf14cd909`: mind az öt CI PASS, nyitott review-thread és változtatáskérés nincs. Merge: `f0b37f3e10e02248b8c9d0f52b3a555ae944c8d5`, 2026-10-28 17:44:11 UTC. A GitHub új stack #67 miatt a szinkron merge API nem támogatott; a webes felület kizárólag #62-re indított aszinkron squash merge-je sikeres, API-val visszaellenőrizve. #63 önálló unáris változtatása a friss mainre újraalapozva, a Windows-javítás lapos választójának megőrzésével. Saját végső CI külön kapu; #64–#66 draft függőség marad.

#63 újraalapozás utáni helyi ellenőrzés: 43/43 CTest, 87 Python, 200000 eredetimetódus-szelet nulla eltéréssel, forrás-/licenc-/feltöltési audit PASS. A főágra célzás már megtörtént; a végső feltöltés indítja a PR saját CI-jét.

Ág: `feat/h8-trapa-entry`; [jegyzőkönyv](h8-trapa-entry.md). Négy szinkron trapkódolás, 436 eredeti metódus. Rögzített reset-SYSCR, vektor8–11, stack-sorrend és observerhibák. 44/44 helyi CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. A stackbe írt m_NPC viselkedése forrásazonosság, nem hardverbizonyíték. Külső IRQ/NMI külön következő lépés; szülője az unáris ág.

### #63 lezárása és a #64–#66 Windows-javítása

#63 a GitHub szerint beolvadt: `dfa7af9bc21c8fe2b91367ad88b91bd7a56ba498`, 2026-10-28 18:12:45 UTC, végső head `ff7fa98a801ad33e0976206606673c7db0b17eca`, öt zöld CI. #64 újraalapozva a friss mainre; a régi Windows C1061 hibát okozó választólánc helyett a már ellenőrzött sorrendtartó korai kilépések megőrizve. #65 és #66 ugyanebben a függőségi sorrendben kapja meg ezt a javítást. A generált metódustörzsek változatlanok; minden új head saját CI-je szükséges.

Ág: `feat/h8-external-irq`; [jegyzőkönyv](h8-external-irq.md). Két eredeti state_irq metódus (438 összesen), opcionális kontrollerkötés, rögzített reset-SYSCR CCR.I szűrés, normál/noirq prefetch különválasztása. 512 maszkolás/NMI/RTE eset, 322 felosztás, külső edge pin, stackhibák és példányizoláció. 45/45 CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. Szülő #64; a merge kizárólag a szülők és saját végső CI miatt vár. #61 Linux/Windows/MSan zöld, két macOS-feladat még sorban áll; nem újrafuttatott hibák. Teljes gép- és audioelfogadás továbbra sincs.

#65 Windows-javítás: a #64 javított ágára újraalapozva, a régi mély else-if választó lecserélve sorrendtartó, azonnal kilépő ágakra. Új opcode vagy metódustörzs-változás nincs. A #64 függőség és a #65 saját végső CI-je továbbra is beolvasztási kapu.

### #64 lezárása, #65 aktuális jelölt

#64 beolvadt `ad1df979f17d7806d24f997f84d63fb087803675`, öt sikeres ellenőrzéssel, nyitott review-thread nélkül. A GitHub a #65 és #66 stackágakat automatikusan újraalapozta; #65 `d4fbd149d7141e05b47aee8262ce9973c9b5f2c8` teljes fájltartalma azonos a helyben validált `c091847` változattal. #65 már mainre céloz és review-ra kész; Windows/Linux/MSan zöld, két macOS-feladat vár. E naplófrissítés után az új head CI-je a merge-kapu. #66 még szülőfüggőség miatt draft.

Ág: `feat/h8-sci-cpu-receive`; [jegyzőkönyv](h8-sci-cpu-receive.md). Meglévő CPU/IRQ/SCI összekötése szintetikus teszthostban: 512 bájt/csatorna eset, 1602 felosztás; vezetéki él → RXI → ISR → RAM → RDRF-törlés → RTE. Új motorkód nincs. A host kizárólag előre konfigurált RX-et és időfüggetlen SSR-nyugtázást támogat; általános MMIO-írás időbélyeg és új határidő publikálása külön következő feladat. Szülő #65, merge/CI függőség miatt jelölt.

Helyi lezárás: 46/46 CTest és ASan/UBSan PASS; változatlan eszközök 87 Python-esete újra ellenőrizve. #60 beolvasztva `18f7b7fa1b708c67cf7ed0baa2109d5f73c750db` (öt zöld ellenőrzés), #61 saját macOS-feladatai még várakoznak. #65 az IRQ/NMI-jelölt PR-je.

#66 a javított #65 ágára újraalapozva; örökli a Windows-fordítási javítást. A három javított head közös teljes helyi tesztje és a legutolsó eredetimetódus-referenciája következik; a szülői merge-kapuk változatlanok.

A #64–#66 javított lánc helyi lezárása: 46/46 CTest, 87 Python-eset, 200000 eredeti dekóder/execute_run referenciaszelet nulla eltéréssel; forrás-/licenc-/feltöltési audit és diffellenőrzés PASS. #64 külön TRAPA-próbája 1024 eset/81 vágáspont mellett is sikeres. Windows PASS csak az új headek GitHub-fordítása után állítható; a régi piros headek történeti eredmények.

### #65 lezárása, #66 utolsó jelölt

#65 beolvadt `f59661ed79860460ba2989f6d2177554e0b56a05`, 2026-10-28 19:05:03 UTC, head `18f1fa382c8768e2424644464d7556170c9a8501`, öt sikeres CI, nyitott review-thread és változtatáskérés nélkül. #66 önálló fogadási tesztje a friss mainre újraalapozva; csak napló/ledger konfliktus volt. A végső head saját ellenőrzése még szükséges.

Ág: `feat/h8-bus-cycle-time`, alap: #66 beolvadt `817e9ce91db22c770291fe2acf666f86f29e1896`, head `66687f2313c5824e629cff570aeffd365da71d73`, négy sikeres CI, nyitott review-thread nélkül. A korábbi #66 jelöltleírás történeti. [Új szerződés](h8-bus-cycle-time.md): opcionális bus_cycle időbélyeg minden kétciklusos buszművelet után, a memóriaművelet előtt, túlcsordulás- és hibakezeléssel. Öt művelettípus és 201 felosztás független ellenőrzése sikeres. Ez a CPU-vezérelt soros adás előfeltétele; az MMIO által létrehozott új határidő beillesztése külön következő lépés.

Helyi lezárás: 47/47 CTest, 87 Python, 200000 eredetimetódus-szelet nulla eltéréssel; új célzott teszt és ASan/UBSan PASS, forrás-/licenc-/feltöltési audit PASS. Hiányzó időalap és negatív abszolút idő külön visszautasítva. Saját végső CI/review még külön merge-kapu.

Ág: `feat/h8-sci-cpu-transmit`, szülő #68. [Szerződés](h8-sci-cpu-transmit.md): minden sikeres buszművelet után opcionális tiszta next_bus_event lekérdezés frissíti a futtató és az aktuális generált kezelő eseményhatárát. CPU által konfigurált SCI-adás két csatornán, összesen 512 bájteset és 1602 felosztás; start/adat/stop/végső idle, 32 ciklusos bitköz. Eredeti metódustörzsek változatlanok; teljes teszt/ref/sanitizer lezárás folyamatban, szülőmerge és saját végső CI külön kapu.

Helyi lezárás: 48/48 CTest, 87 Python, ASan/UBSan, 200000 eredetimetódus-szelet nulla eltéréssel; forrás-/licenc-/feltöltési audit PASS. #68 mind az öt CI-je zöld, nyitott review-thread/változtatáskérés nélkül beolvasztva: `0fa40aaba818a0efd7265a1861650479795cbba1`, head `7867ec70308e4e06ad794758e4f7c05008534f38`. A TX-jelölt saját végső CI-je külön kapu.

Ág: `feat/h8-sci-cpu-duplex`; [szerződés](h8-sci-cpu-duplex.md). Motorváltoztatás nélkül egyidejű TX/RX, mind a négy csatornapárosítás, 1024 bájteset és 3604 felosztás. Független wire/adat/IRQ/RDRF/RTE ellenőrzés; a frame-ek átfedik egymást, de aktív TX közbeni RXI-preempciót nem állítunk. Szülő #69 öt sikeres CI és üres review-thread/változtatáskérés lista után beolvadt: `298b8465aee4e74cfe7ec8e1459402b21f2ce9aa`, head `08a18b492a2c7df3abc5c33edac1f37a994a080f`. Teljes helyi teszt/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 49/49 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Negatív kontroll: RXI engedélyezés nélkül a teszt hibát jelez, tehát a RAM/IRQ elfogadás nem pusztán a TX befejezését ellenőrzi. Eredeti CPU-/UART-motorkód változatlan; a #69 eredetimetódus-referenciája továbbra is a motoralap bizonyítéka.

Ág: `feat/h8-sci-cpu-framing`; [jegyzőkönyv](h8-sci-cpu-framing.md). Motorváltoztatás nélküli ERI/FER-integráció: 1024 bájt/csatorna eset, 3604 felosztás, 512 következő jó frame helyreállítási eset. CPU SSR-kiolvasás/RAM/nyugtázás/RTE, majd RXI és helyes adat. A hiányzó idle-high stimulus mind512 recovery esetben hibát jelzett, a tesztbemenet javítva. #70 négy zöld CI, üres review-thread/változtatáskérés lista után beolvadt `860815eb62afb2d9b55e1e2cc8318a85967d4d04`, head `a063232d4f877fc2d3eea43303b73273a8e872eb`. Saját végső CI külön kapu; overrun/paritás még nyitott.

Helyi lezárás: 50/50 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Negatív kontroll: FER-törlés kihagyása 2048 elfogadási hibát eredményez, tehát a teszt a nyugtázást és recovery-t is ellenőrzi. A közös fixture kiemelése után a korábbi duplex teszt változatlan alapbeállításokkal továbbra is sikeres. Saját végső CI/review még szükséges.

Ág: `feat/h8-sci-cpu-parity`; [jegyzőkönyv](h8-sci-cpu-parity.md). SMR0x20/0x30, jó/rossz paritás mindkét csatornán, 2048 bájteset és 8808 felosztás. CPU RXI/ERI, RAM/SSR, PER-törlés és RTE; független bit-count bemenet és TX-paritás/időzítés. A rögzített forrás paritáspolaritását követjük, hardver-validált even/odd állítás nélkül. #71 négy zöld CI, nyitott review-thread/változtatáskérés nélkül beolvadt `8e9c431f1a0629cc20db136c50c56eba81854202`, head `e49e6c0053c8c5179e4475fc51ee3628478b9177`. Teljes helyi teszt/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 51/51 CTest, 87 Python, célzott ASan/UBSan, forrás-/licenc-/feltöltési audit PASS. PER-törlés nélküli negatív kontroll hibát jelez. A közös fixture alapértelmezett no-parity és FER-próbái továbbra is sikeresek. Motorkód változatlan; saját végső CI/review még szükséges.

Ág: `feat/h8-sci-cpu-overrun`, szülő #72. [Jegyzőkönyv](h8-sci-cpu-overrun.md). Két kiolvasatlan frame maszkolt CPU mellett, ORER+RDRF és első adat megőrzése; késleltetett LDC0 után CPU SSR/RDR mentés, törlés és RTE. A korábban latch-elt RXI az ERI után külön RTE-kezelőt kap; ez rögzített modellviselkedés, nem hardverbizonyíték. 512 bájteset, 4802 felosztás célzott PASS. #72 Linux/Windows/MSan zöld, macOS futtatóhiány miatt megszakadt, újraindítva. Az overrun PR a szülő CI/merge miatt visszatartandó, nem több fejlesztési lépcső miatt.

Helyi lezárás: 52/52 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Második frame nélküli negatív kontroll 2048 elfogadási hibát jelez. Motorkód változatlan. #72 macOS újrafutása folyamatban; az overrun jelölt szülőmerge és saját végső CI miatt draft.

#72 lezárása: macOS újrafutás sikeres, négy zöld CI, üres review-thread/változtatáskérés lista; merge `f29f0373be4dc6ca866744c2a0d832d82eb7dfa6`, head `dceda74cfa1402f050649b14b91de531aa8d9fdf`. #73 tisztán újraalapozva a mainre és review-ra kész; a korábbi draft/szülővárakozás bejegyzés történeti. A végső head saját CI-je még merge-kapu.

Ág: `feat/h8-absolute-memory`; [jegyzőkönyv](h8-absolute-memory.md). Hat abs8/abs16 MOV-csoport, 12 eredeti full/partial metódus (450 összesen), leltár217 kiválasztott/47 hiányzó/135 kizárt. Összes abs16 cím, abs8 regisztersáv/érték/CCR, 201 felosztás és hat buszhibahely; elérhető eredeti referenciaciklus mindhat formára fetch-őrrel. #73 négy zöld CI és üres review-thread/változtatáskérés lista után beolvadt `8446d10b65c698b5fbc749279f696d69c100e5fc`, head `c37346094ed6b35f068d8969a2401df40f010636`. Teljes helyi teszt/ref/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 53/53 CTest, 87 Python, célzott ASan/UBSan, forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run: 200000 szelet nulla eltéréssel, mindhat abszolút forma fetch-őre teljesült. A végső adatbuszhiba-tesztekkel az új cél külön újrafordítva és újrafuttatva. A 65536 elsőszavas dekóder-orákulum az újonnan támogatott abszolút kódolásokkal frissítve; szomszédos tiltott formák továbbra is elutasítva. Saját végső CI/review szükséges.

Ág: `feat/h8-absolute32-memory`; [jegyzőkönyv](h8-absolute32-memory.md). Négy bájt/szó abs32 MOV-csoport, nyolc eredeti metódus (458 összesen), leltár221 kiválasztott/43 hiányzó/135 kizárt. 15 címhatár minden regisztersávval, formánként65536 korrelált32-bites címminta (nem teljes32-bites tartomány), 201 felosztás és négy buszhibahely. Külön elérhető eredeti referencia mindnégy fetch-őrrel. #74 öt zöld CI, nyitott review-thread/változtatáskérés nélkül beolvadt `758ad981253f8d3b5c72915f2fe5b60df560de97`, head `642977224868632203a46b1d220b926e506b0b5c`. Teljes helyi teszt/ref/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 54/54 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run: 200000 szelet nulla eltéréssel; mindnégy abs32 fetch-őr és minden korábbi család elérhetőségi őre teljesült. Teljes32-bites tartomány helyett határpontokat és65536 mintát ellenőriztünk formánként. Saját végső CI/review szükséges.

#75 lezárása az élő GitHub merge/head/időpont és öt sikeres ellenőrzés alapján rögzítve. A napló eleje, STATUS, REAPER-terv, README és strukturált ledger egységesítve; a korábbi jelöltleírások történetiek. A motoralap #75, nincs új motor-/firmware-/audioelfogadás. A dokumentációs PR saját CI-je külön követendő.

Ág: `feat/h8-long-absolute-memory`; [jegyzőkönyv](h8-long-absolute-memory.md). Négy abs16/abs32 MOV.L csoport, nyolc eredeti metódus (466 összesen),225 kiválasztott/39 hiányzó/135 kizárt sor. Négy szavas IR-puffer és snapshot/reference,0100 prefix ER-őrök; eredeti két szavas sorrend/átfordulás. Célzott összes abs16 cím, ER/CCR határpontok,201 felosztás és első/második írási hibák PASS. #76 dokumentációs PR még CI alatt; a mostani motorjelölt ettől külön kezelendő. Teljes teszt/ref/sanitizer lezárás folyamatban.

Helyi lezárás: 55/55 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run:200000 szelet nulla eltéréssel, mindnégy long-abs fetch-őr és minden korábbi őr teljesült. A régi65536 prefixkiterjesztés-orákulum32 új támogatott kódolással frissítve; érvénytelen ER8..15 szomszédok továbbra is tiltottak. #76 macOS várakozik; az új motor-PR csak a szülőmerge és saját végső CI miatt draft.

#76 lezárva: négy sikeres CI, üres review-thread/változtatáskérés lista; merge `0c67fce0e32450709523e4ac01ae477a64633916`, head `ae753a6bc1418be4885645a38ae3fc757e7e293c`. #77 tisztán újraalapozva a mainre; korábbi szülővárakozása megszűnt. Saját végső CI/review még merge-kapu.

## H8-DISPLACEMENT16-2026-10-29 — 16 bites eltolású memóriaátvitel

Ág: `feat/h8-displacement16-memory`, alap: beolvadt #77. Hat eredeti MOV byte/word/long
load/store csoport, 12 változatlan full/partial metódus: 478 összesen;
231 kiválasztott / 33 hiányzó / 135 kizárt kódolássor. Mind a 65536 eltolás,
regiszterátfedések, teljes ER-címkörbefordulás, CCR és szóigazítás tesztelve.
201 felosztás és kétlépcsős store-hiba; eredeti dekóderrel 200000 szakasz nulla eltéréssel.
Mind a hat új forma és a korábbi referencia-családok elérésellenőrzése megmarad.
[Részletes bizonyíték és korlátok](h8-displacement16-memory.md). A jelölt teljes csomagja
56 CTest / 87 Python; firmware/audio-elfogadás továbbra sincs. Végső PR-CI külön bizonyíték.

Az eltolásos memóriautasítások PR-je: [#78](https://github.com/RobCZart82/yaman1x/pull/78). A készítéskori jelöltállapot és a végső CI-/merge-bizonyíték külön kezelendő.

## 2026-10-29 — Előjel nélküli bájtszorzás jelölt

Az eredeti MULXU.B full/partial kezelők kiválasztva; fix H8H MAC nélküli időzítés.
65536 operanduspár,19200 alias/CCR-határeset és161 szétvágott futás PASS;
200000 eredeti referencia-szelet nulla eltéréssel; szándékos product+1 hiba felismerve.
Helyi56 CTest/86 Python PASS, ismert Windows-symlink eset kizárva; távoli CI függőben.
Jelölt:57 CTest/87 Python,480 eredeti módszer,232 kiválasztott/32 hiányzó/135 kizárt sor.
A szó-szorzás külön nyitott: az eredeti u16 operandusok int-promóciója nagy szorzatnál
C++-túlcsordulási kockázatot jelent. Bekötés előtt független nagyértékű teszt,
UBSan és dokumentált unsigned-szélesítés felülvizsgálata szükséges.
Ez lehetőség és igazolandó javítási feladat; nem hardveres hibamegállapítás.
Részletek: [szerződés és bizonyíték](h8-byte-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-szorzás jelölt

MULXU.W: két eredeti eredetű full/partial kezelő minimális, dokumentált javítással:
szorzás előtt u32-szélesítés, hogy a u16 operandusok int-promóciója ne csorduljon túl.
A korábbi480 kezelő változatlan;482 eredeti eredetű kezelőből2 adaptált.
327680 operandus-söprés,9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS.
200000 eredeti referenciaszelet nulla eltéréssel, csak biztonságos szorzatokkal;
a nagy szorzatokat független64 bites oracle ellenőrzi. Product+1 kontroll:seed9/slice9.
Helyi57 CTest/86 Python PASS, ismert Windows-symlink eset kizárva. Jelölt:58 CTest/87 Python,
233 kiválasztott/31 hiányzó/135 kizárt kódolássor. Linux UBSan és külön eredeti-hibakontroll
bekötve; tényleges távoli eredmény még függőben. A szó-szorzás nyitott kockázata
csak ezek sikeres lezárása után tekinthető rendezettnek; ez nem hardverelfogadás.
[Szerződés, pontos adaptáció és bizonyítékok](h8-word-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjeles bájt- és szó-szorzás jelölt

Eredeti01c0 prefix és MULXS.B/MULXS.W full/partial kezelők; külön őrzött folytatás.
Az eredeti set_nz16/set_nz32 csak N/Z-t módosít. Független64 bites számítás:
65536 bájtpár,327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár PASS;
241 szétvágott futás és65536 prefixfolytatás ellenőrzött. Eredeti referenciaszeletek:
200000,nulla eltérés; mindkét forma elérése kötelező. Product+1 kontroll seed9/slice14.
Jelölt:59 CTest/87 Python,488 upstreamből származó kezelő (486 változatlan,2 korábbi
unsigned-adaptáció);235 kiválasztott/29 hiányzó/135 kizárt kódolássor. Linux UBSan
mindkét új formát is teszteli; távoli CI függőben, M0 továbbra is részleges.
[Szerződés és részletes bizonyítékok](h8-signed-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli bájtos osztás jelölt

Eredeti DIVXU.B full/partial kezelők, külön5100/ff00 dekóderág. Nullás osztónál
célregiszter-megőrzés; hányados alacsony8 bitje és maradék külön bájtban; N/Z
az eredeti osztófüggő szabályt követi. Ez forrásegyezés, nem hardveres bizonyíték.
393216 osztandó-söprés,2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár
és241 szétvágás PASS;200000 referenciaszelet nulla eltéréssel. Hányados+1 kontroll
seed9/slice18 felismerve. Jelölt:60 CTest/87 Python,490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott/28 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve osztásra is; távoli CI függőben. M0 továbbra is részleges.
[Szerződés és ellenőrzési hatókör](h8-byte-divide.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-osztás jelölt

DIVXU.W, külön5300/ff08 dekóderág. A hányados/maradék u32 típusa explicit;
csomagolás unsigned32 marad. Két dokumentált típusadaptáció, nem C++20 UB-mérés.
Az eredeti N az osztó7. bitjét követi, nem a15.-et; hardvermérés nélkül ezt megőrizzük.
524288 osztó-söprés,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS;
200000 biztonságosan korlátozott referenciaszelet nulla eltéréssel. Nagy értékek
független64 bites számítással ellenőrizve; hányados+1 kontroll seed9/slice26.
Jelölt:61 CTest/87 Python,492 upstreamből származó kezelő (488 változatlan,4 adaptált);
237 kiválasztott/27 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve szó-osztásra;
távoli CI függőben, M0 továbbra is részleges. [Szerződés](h8-word-divide.md).
Yamaha-adat nem került be; firmware/MIDI/hang/plugin elfogadás nincs.

## 2026-10-29 — M0 decimális korrekciós részlépés: DAA/DAS jelölt

Négy változatlan eredeti full/partial kezelő, külön0f00/fff0 és1f00/fff0 dekóderág.
Független tízes számrendszerű számítás: minden0–99 operanduspár összeadás/kivonás,
két CCR-minta (40000 eset);2048 regiszterhatáreset és241 szétvágott futás PASS.
Érvénytelen BCD/flag kombinációkra nem állítunk teljes független helyességbizonyítást.
200000 eredeti referenciaszelet nulla eltéréssel, mindkét új forma és korábbi családok
elérése kötelező; DAA korrekció+1 kontroll seed7/slice0 felismerve. Jelölt:62 CTest/87 Python,
496 upstreamből származó kezelő (492 változatlan,4 korábbi adaptáció);239 kiválasztott/
25 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve. M0 továbbra is részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. [Szerződés](h8-decimal-adjust.md).
Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles bájtos osztás jelölt

Változatlan eredeti01d0 prefix és DIVXS.B kezelők, őrzött0xf0000 folytatás.
Független64 bites számítás:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás és65536 prefixfolytatás PASS. A hányados nullához
csonkolódik,maradék az osztandó előjelét követi; N a teljes hányados előjele,Z a nullás
osztó. Eredeti kommentek helyett a matematikát és futó kódot ellenőrizzük; hardverigazolás nincs.
200000 referenciaszelet nulla eltéréssel; hányados+1 kontroll seed7/slice3 felismerve.
Jelölt:63 CTest/87 Python,500 upstream-kezelő (496 változatlan,4 korábbi adaptáció),
240 kiválasztott/24 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve; CI függőben.
Szó-osztás INT32_MIN/-1 és csomagolás külön nyitott vizsgálat. [Szerződés](h8-signed-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles szó-osztás és szélső hányados jelölt

Két változatlan eredeti DIVXS.W kezelő, őrzött meglévő01d0 folytatás;32 bites int
fordítási feltétel. Az eredeti unsigned32 segédértékek elkerülik a közvetlen signed
INT_MIN/-1 osztást. A matematikai +2147483648 hányadost az eredeti int32-re szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres hibajavítás vagy mérés.
524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,1024 szélső/CCR-eset,321
szétvágás és128 tiltott ER-szomszéd PASS. Független64 bites számítás és külön
szűkítési flagmodell.200000 eredeti referenciaszelet nulla eltéréssel; fordított
N kontroll seed7/slice7 felismerve. Jelölt:64 CTest/87 Python,502 upstream-kezelő
(498 változatlan,4 korábbi adaptáció),241 kiválasztott/23 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve; CI függőben. [Hatókör és szélső eredmény](h8-signed-word-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nincs.

## 2026-10-29 — M0 32 bites eltolásos bájt/szó-memóriamozgatás jelölt

Eredeti7800 prefix és négy MOV-csoport,tíz változatlan full/partial kezelő.
Meglévő négyszavas IR,őrzött160000 folytatás,full32 cím és külön24 bites fizikai
leképezés.1048576 eltolásfél-söprés,55296 cím/regiszter/CCR-határeset,241 szétvágás,
65536 prefixfolytatás és memóriahibás fail-stop tesztek PASS. Nem teljes2^32 keresztszorzat.
A prefixkontroll nyolc hibás belső-állapot újradekódolást fogott; az elsőszó-maszk
szűkítése rendezte.200000 eredeti referenciaszelet nulla eltéréssel; felső eltolásszó
elhagyása kontroll seed6/slice5 felismerve. Jelölt:65 CTest/87 Python,512 upstream-kezelő
(508 változatlan,4 korábbi adaptáció),245 kiválasztott/19 hiányzó/135 kizárt sor.
Hosszú szó32-eltolással továbbra is nyitott. Linux UBSan kiterjesztve; CI függőben.
[Szerződés és hatókör](h8-displacement32-memory.md). M0 részleges; firmware/MIDI/hang/plugin
elfogadás nincs. Yamaha-adat nem került be.

## FIRST_REAPER_SOUND_PLAN

## FFFE8 határ — #276 beolvadt — 2026-11-12

A [#276 PR](https://github.com/RobCZart82/YamAN1x/pull/276) mostantól a
sikertelen bájt-/szóírás próbált értékét is rögzíti, miközben a nem támogatott
ADC-módot továbbra is elutasítja. A friss firmware-futások `0x33` értéket
rögzítettek; a zárolt MAME-forrás ezt AN0–AN3 szoftverindítású pásztázásként
értelmezi, de ez nem Yamaha-hardverbizonyíték. A továbblépéshez még nincs
igazolt AN0–AN2 bemeneti profil és hardveregyezés. Teljes boot, MIDI,
DSP/hanglánc, plugin-host és REAPER elfogadása nyitott.

PR [#276](https://github.com/RobCZart82/YamAN1x/pull/276) beolvadt
2026-11-12 15:50:27 UTC-kor, a végső `27fc4b0a0677b340a279439db7d54466c5f23f8d`
headről a `74f0dc545c79a89790ad6eef42370b06f73bd65a` main commitba.
A PR-head mind az öt ellenőrzése sikeres: Linux `131281742752`, macOS
`131281743186`, Windows `131281743094`, MSan `131281739988`, MAME research
`131281740553`. A Codex review a pontos headen észrevétel nélkül zárult
(issue-comment `6827690290`). GitHub review-objektum és review-szál nem volt;
a ruleset nulla jóváhagyást ír elő és rendezetlen szál nem volt. Kvótahiba
nincs jelezve, merge-bypass nem történt.
Az új main mindhárom kötelező platformellenőrzése, továbbá az MSan és MAME
research futása is sikeres: `131300475364`, `131300474485`, `131300475237`,
`131300473882`, `131300476046` (workflow runok `43252136161`,
`43252135897`, `43252136454`).

## A #274 lezárása és az aktuális főág — 2026-11-12

A [#274 PR](https://github.com/RobCZart82/YamAN1x/pull/274) beolvadt
2026-11-11 21:56:03 UTC-kor (22:56:03 Europe/Budapest). Végső head:
`329a7bf626e251c39a3273eef69dad24ad3e161e`; ellenőrzött merge-main:
`d9f9ffb04e5dee587efebf33b6cf6bfc17b95f94`.
A végső PR-head platform/MSan/MAME futásai sikeresek
(`43102583960`, `43102583923`, `43102583907`), és az azonos main
[platform](https://github.com/RobCZart82/YamAN1x/actions/runs/43107390172),
[MSan](https://github.com/RobCZart82/YamAN1x/actions/runs/43107390248),
[MAME](https://github.com/RobCZart82/YamAN1x/actions/runs/43107390414)
futásai is zöldek. A végső head Codex-review-ja észrevétel nélkül zárult
(komment `6806170735`); a korábbi eredményolvasási finding javítva,
a review-szál rendezve.

Az explicit `--adc-an3=0..1023` mintabemenet elkészült. Minta nélkül az
ADDRD word/high/low olvasások továbbra is pontos unmapped hibát adnak.
A leltár 138 CTest-csoport és 127 Python-eset. A november 12-i helyi audit:
Release build és 137/137 natív CTest sikeres; a külön futtatott Python
126/127 sikeres, egy WinError 1314 symlink-jogosultsági hiba. A 14 célzott
source-context eset optimalizált Python alatt is sikeres. Ez külön mérés
a #274 korábbi fejlesztési teszteredményeitől. Új firmware-próba nem történt.
A következő kutatási határ a lent rögzített, nem támogatott ADC-vezérlési mód;
előbb a forrás és a vezérlőbitek bizonyítékát kell tisztázni.

Az alábbi firmware-megfigyelések a #274 fejlesztése során készültek.

A változatlan, csak olvasott AN1x 1.04 ROM-mal végzett, idle-paneles swap16
diagnosztika az AN3 0 és 1023 bemenetet is egy alkalommal, ciklus `95,706,795`
értéknél mintázta; 4,096-os és 1,000,000-os CPU-szeletekkel is azonos
perifériaállapotig jutott. Mind a négy futás a későbbi `FFFFE8` byte írásnál
állt meg (post-fetch PC `1B84`, ciklus `95,707,121`), `Unsupported ADC control
mode` hibával. A különbségmentes folytatás azt igazolja, hogy a callback
bekötődött és a 266 ciklusos konverzió lefutott; a futás nem mutat különbséget
a két bemeneti érték firmware-hatásában. Nem igazol teljes bootot, fizikai
ADC-egyezést, MIDI-t, hangot vagy REAPER-t. A ROM SHA változatlan:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; a ROM
nincs a repóban. A zárolt MAME-forrás azonosítója
`706dbb73281320c54d7c2f16262c1c3786f2be01`; ehhez a kis próbalépéshez új
MAME-fordítás/futtatás nem történt.

## A #272 main-ellenőrzés és a PKS íróregiszter-követés — 2026-11-11

A #272 PR ellenőrzött headje `69a38a8a9d255afb967e9be55589c4ac2e1310f2`,
merge-SHA-ja és current main `d37a305779b1c2ea53e014ac2eaefd3c67c9b63b`.
Mind a négy szükséges PR-head és main ellenőrzés sikerült; MAME a dokumentációs
módosításnál útvonal szerint nem futott. GitHubon nem volt review, szál vagy
komment, automatikus review-eredmény vagy kvótahiba sem volt rögzítve; a
szabályzat nulla jóváhagyást követelt, megkerülés nem történt.

A következő korlátozott kódjelölt a megfigyelt forrásíró mentett R1L alsó
bájtját ellenőrzi a `0039AC` post-fetch PC-nél rögzített írási bájtlane-nel.
Az eredeti MAME-hivatkozás kilenc másodperces passzív profiljának mind a hat
ilyen írása megfelel. Ez CPU-regiszter/buszérték-egyezés; nem azonosítja az
utasítás kezdetét, a korábbi számítást vagy az adat jelentését. A célzott
tesztek 14/14, a teljes Python-suite 127/127; az audit a rögzített nyomot is
elfogadja. A ROM változatlan és a repositoryn kívül maradt. M0 részleges;
hardver, teljes boot, MIDI, hang és REAPER továbbra sincs elfogadva.

## PKS forrás-RAM íráselőzmény — jelölt — 2026-11-10

A következő, még be nem olvadt auditorváltozás a rögzített PKS-forrásolvasások
értékét a legutóbbi, ugyanarra az igazított RAM-szóra és bájtlane-re eső korábbi
írással is összeveti. Az ER0 indexének ki kell választania a megfigyelt szót és
a bájtlane-et; a byte-write és full-word write eseteket külön kezeli. Az
ellenőrzés a meglévő FA92 olvasás → FA94 célírás cím/érték/R0L/ER4/idő kapcsolat
előtt fut. Ez megfigyelt írás-olvasás eredetlánc, nem PKS/ADC-szemantika.

A 12 célzott forráskontextus-teszt és a teljes Python-tesztkészlet (125/125)
helyben sikerült. A 138 CTest-csoportot ez a Python-only változás nem módosítja;
teljes CTest, MAME-fordítás és új firmware-futtatás ehhez a jelölthöz még nincs
rögzítve. ROM-ot ez a lépés nem olvas, nem módosít és nem tesz a repóba. A
távoli ellenőrzések és a pontos PR-head review a jelölt PR kapui; ez a rész még
nincs beolvasztva, hardver/boot/MIDI/hang-elfogadást nem jelent.

PR [#269](https://github.com/RobCZart82/YamAN1x/pull/269) az eredeti
headről (`0afd0e86fd4845514c2e969d809d25880b9d7d64`) nyílt, a
`947d67950345c7e03362d40f25174fcbe3b1f268` alapra. Ezen a pontos headen
mindhárom kötelező platformellenőrzés sikeres (Linux `130053106191`, macOS
`130053106085`, Windows `130053105515`); az MSan is sikeres
(`130053107228`). GitHub review-objektum, review-szál és PR-komment egyaránt
0; kvótahibára nincs bizonyíték. A szabályzat 0 jóváhagyást ír elő, a review-
szálak rendezését követeli. A naplófrissítés új PR-headet készít, ezért az új
pontos head ellenőrzései még függőben vannak. A PR még nincs beolvasztva.

## PKS forrásbájt CPU-kontextus — #267 beolvadt — 2026-11-10

A #267 PR `dae0d64db4d463d119ee7be1c452c90a8487513d` merge-SHA-val beolvadt
a `411524cd9797db6e0aa7b9f3d0b14340eb50b52c` ellenőrzött headről.
A korlátozott, csak naplózó MAME-megfigyelő, a független auditor és a tesztek
a főág részei. Két passzív futás azonos, 43 eseményes nyomot és hat egyező
forrásolvasás/célírás párt adott. Ez nem igazol protokollt, ADC-t, hardvert,
teljes bootot, MIDI-t vagy hangot; a ROM változatlan és a repón kívül maradt.

A PR pontos headjén a Linux, macOS, Windows, MSan és MAME ellenőrzés sikeres
volt; a Codex review ugyanazon a headen észrevétel nélkül zárult. A merge-SHA
mind az öt kapcsolódó ellenőrzése szintén sikeres. Részletek a
[fejlesztési naplóban](DEVELOPMENT_LOG.md) és a [forrásút megfigyelésében](pks-payload-path.md).

Következő lépés: a rögzített forrás/cél megfigyelések előtti írások és
utasításkontextus követése független cím-, érték-, flag-, időzítés- és
folytathatósági ellenőrzésekkel. Az észlelésekből ne következtessünk protokollra,
és ne adjunk hozzá HLE-választ. M0 részleges; M1, teljes boot, MIDI, DSP,
hang, VST3/AU és REAPER-elfogadás nyitott.

## Auditjavítások — 2026-11-10

Ellenőrzött előkészítési alap: #265, `5421585951258f430ea13fd40207bdabd23ed654`.
A #265 dokumentációs PR már beolvadt; végső headje `08d72e334458e203a9dc385acb0511cf5a17cb17`.
PR CI: 42681766560/42681766508; merge CI: 42688078789/42688079192, mind a négy
feladat sikeres. Review a korábbi `cc8b736430386cb1d219135d3a0762db0cf73312`
headre zárult észrevétel nélkül; végső headre külön review-t nem állítunk.

Három külön commitból álló javítás:
1. ADC: utolsó elfogadott időbélyeg, régebbi restart elutasítása változtatás nélkül.
   1044 stale-restart eset mindkét bankkal, függő minta/eredmény megőrzése,
   azonos időpont, hiba/túlcsordulás és explicit reset utáni új epoch ellenőrizve.
2. PKS: kötelező futásadatmezők és típusok, ismert ROM-azonosítás, sikeres kilépés,
   CPU-előrehaladás és profil/bridge/MIDI-konzisztencia vizsgálata. Bekapcsolt DSP-órajel
   esetén mindkét chip pozitív, egész mintafuttatási számlálója kötelező.
   Review-kiegészítés: a futtató az adatlap kiírása után utasítja el a hiányzó/
   nulla számlálót, ezért ezt az összehasonlítóban is ellenőrizni kell. Az üres,
   hiányos, sikertelen és hibás típusú adatok elutasítva. A hiányzó mező eltér a null-tól.
   Négy új Python-teszt; célzottan mind a tíz PKS-próba PASS, negatív CLI-vel is.
3. Dokumentáció: 12 patch, #213 korabeli 131-es tesztszám, #265 lezárása,
   az elkészült ITU/ADC-integráció és a következő PKS-kutatási lépés egyeztetve.

A forrásfa 137 CTest/113 Python esetet tartalmaz. Helyi Release-fordítás és
136/136 külön futtatható CTest PASS (51,54 s). Python 112/113 PASS; egy ismert
WinError1314 symlink-jogosultsági korlát marad. A tíz célzott PKS-próba -O
módban is PASS. Forrásaudit: 683 fájl/201 upstream azonosság; licencértesítés:
230 fájl/5 szöveg PASS (nem jogi/bináris engedély). UTF-8/linkek/napló-ősök PASS.
Helyi sanitizer nem futott; az ADC-célpont a Linux sanitizer CI-be bekerült.
Saját távoli ellenőrzések a javítás PR-jén követhetők, korábbi CI nem új bizonyíték.
A beolvadás állapotának hiteles forrása a [#266 PR](https://github.com/RobCZart82/YamAN1x/pull/266); nem becsülünk előre saját merge-SHA-t.
A régi lezárási és jelöltbejegyzések a saját dátumukra vonatkozó történeti adatok.
M0 részleges, M1 elfogadása nyitott. ROM/bank/privát trace/hardverpróba nem készült;
EG target-hold továbbra is csak hardveres bizonyíték alapján módosítható.

# YamAN1x: fejlesztési terv az első REAPER-ben megszólaló hangig

## Ellenőrzött fejlesztési prioritások — 2026-11-07

A repository, az öt nyitott műszaki hibajegy és a fejlesztési terv alapján
ellenőrzött külső állapotjelentés megállapításait az alábbi feladatokhoz
kapcsoljuk. A jelentéshez nem készült új fordítás vagy hardveres összehasonlítás.

1. **Firmware és perifériák (M0–M1).** Az AN4–AN7 pásztázás buszintegrációja
   már a #242 része; a #279 opt-in nyolccsatornás mintaprofillal egészítette ki.
   A következő bizonyítandó lépés a firmware további, támogatott MMIO/CPU-IRQ
   útjának feltárása, majd az indulás és a PKS szerepének külön vizsgálata. A
   tényleges MIDI note-on/note-off feldolgozás önálló elfogadási pont marad. A szintetikus ADC-értékeket minden mérésben jelölni kell.
2. **DSP-előfeltételek (M2).** Az [EG célérték-megtartása](https://github.com/RobCZart82/yaman1x/issues/1)
   reprodukált modell/leírás-eltérés. A hardverre vonatkozó javításhoz elsődleges
   bizonyíték vagy referenciafelvétel kell. Az [órajel és tábla-ROM](https://github.com/RobCZart82/yaman1x/issues/2)
   formátumát, címzését és skálázását ellenőrizni kell. A modell 48 kHz-es
   beállítása önmagában nem igazolja az AN1x fizikai mintavételét.
3. **Időzített DSP-futtatás (M2).** A [H8–VOP3 integráció](https://github.com/RobCZart82/yaman1x/issues/3)
   közös időalapra kapcsolja a mintaléptetést és a regiszterműveleteket.
   Vizsgálandó a feltöltés alatti stop/run és az azonos idejű események sorrendje.
   A jelenlegi firmware-próbák VOP3 mintaszámlálói nullán maradtak.
4. **Első száraz referenciahang (M1–M3).** A [MIDI és hangút feladat](https://github.com/RobCZart82/yaman1x/issues/4)
   egy rögzített hangszínt, indítást, tartást és elengedést igényel, dokumentált
   kétchipes útvonallal és kimeneti skálával. Az offline WAV reprodukálhatóságát
   és a valódi hangszer referenciafelvételével való összehasonlítást rögzíteni kell.
5. **Motor és hosztadapter (M4–M6).** A validált jelút után következik az
   audioblokkos feldolgozás, MIDI-időzítés, állapotmentés és a minimális VST3,
   majd a REAPER-próba. Az [integrációs hibajegy](https://github.com/RobCZart82/yaman1x/issues/5)
   a választott keretrendszer és függőségek licenceinek dokumentálását is kéri.
   A projekt saját licencének kiválasztása külön felhasználói döntés.
   Az első REAPER-hang célja VST3; az AU későbbi integrációs cél.

Ez függőségi sorrend. Az EG- és órajelkutatás a firmware-munkával párhuzamosan
is előkészíthető. A jelenlegi engedélyezett fejlesztés soros PR-folyamatot követ.
A következő elfogadási pont az ADC-integráció utáni firmware-előrelépés;
teljes boot, MIDI-vétel, offline hang és REAPER-hang külön mérföldkövek.
A CPU-, busz-, periféria- és platformtesztek sikerét külön kell rögzíteni
a hardverhűség és a hangút elfogadásától.

## Javítás lezárva — #213, 2026-11-05

Beolvadt: [#213](https://github.com/RobCZart82/yaman1x/pull/213),
merge `d60e6eb9907d0a9c89938e1677d68e54b319da74`,
tesztelt head `a421a261a6647e759dac7d5132567d82f2de32fc`.
Időpont: 2026-11-05 10:09:33 Europe/Budapest (09:09:33 UTC).
[Linux/macOS/Windows, UBSan és ASan/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686134),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686447),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41996686368): mind az öt feladat sikeres.
Automatikus review észrevétel nélkül lezárult a `95b7edb0feaa97be23d1105fe59b6f259596e640`
forráscommiton; utána csak három dokumentum PR-hivatkozása változott. A végső headre
külön review-t nem állítunk. Nyitott review-szál nem volt a beolvasztáskor.

A teljes külső LCD/MEG/VOP3 cold reset alapreferencián keresztül is működik,
és az alapmutatós felszabadítás teljes. Reprodukció: javítás előtt megtartott
MEG/VOP3 állapot miatti kudarc; javítás után két alapnézetes reset és két
alapmutatós destruktorpróba PASS. Helyi Release/130 futtatható CTest PASS;
Python 102/103 PASS, egy WinError1314 symlink-jogosultsági akadály. Teljes
CI-készlet: 131 CTest/103 Python. A sanitizer-bizonyíték a fenti Linux CI.

Nincs aktív motorjelölt. Ez a lezáró egyeztetés dokumentációs változás;
saját ellenőrzései a kapcsolódó PR-en követhetők, a fenti CI a #213-hoz tartozik.
Következő: aktív ITU gépütemező/MMIO- és CPU IRQ-bekötés, majd friss helyi
firmware-diagnosztika. M0 részleges; boot/MIDI/hang/plugin/REAPER elfogadás nyitott.

## Forrásaudit és cold-reset javítás — 2026-11-05

Ellenőrzött motoralap: #212, `5915ccb54f07f7649192ea3dbfa4277c56f0ce01`.
A tesztelt head `28f81562685616cb89bdf96f269c62bcc9a375de`; a platform/sanitizer,
MSan és MAME futások sikeresek (41922483350 / 41922483298 / 41922483383).
Automatikus review-eredmény az ellenőrzéskor nem volt rögzítve.
131 CTest/103 Python eset. A korábbi #211 „jelölt” megjegyzéseit történeti
adatként őrizzük; #211 és #212 beolvadt. A közös SYSCR (#141) és a korlátozott
reset (#168–169) már elkészült, a teljes hardver-reset és boot nincs elfogadva.

Reprodukált hiba: az An1xVop3ResearchSink LCD- vagy MEG-alapreferenciáján hívott
cold_reset csak az alapréteget nullázta; a MEG/VOP3 állapot és számlálók megmaradtak.
A cold_reset most virtuális, a leszármazottak override-ot használnak, az alap
virtuális destruktort kapott. Így az alapmutatón birtokolt teljes adapter is
felszabadul. Két alapnézetes resetpróba ellenőrzi a teljes állapotot, az új
nullaidős korszakot és a számlálókat; két alapmutatós tulajdonosi próba a teljes
objektum megsemmisítését. A teszt javítás előtt elbukott, utána PASS.
Ez a közös külső adapterfelület javítása; a jelenlegi konkrét típusos CLI-út
ugyanezt a cold resetet eddig is közvetlenül hívta.

Aktív javítás: [#213](https://github.com/RobCZart82/yaman1x/pull/213), fix/research-sink-cold-reset. Release build és 130/130 helyben
futtatható CTest PASS (53,85 s). Python: 102/103 PASS, egy ismert symlink-esetet
a Windows-jogosultság akadályozott (WinError1314). Forrásaudit: 665 fájl/201
upstream azonosság PASS; licencértesítés: 229 fájl/5 szöveg PASS, jogi/bináris
engedélyezést nem jelent. Belső linkek/UTF-8/napló-ősök PASS. Helyi sanitizer
nem futott. Saját távoli CI/review függőben; korábbi CI nem ennek az eredménye.
Következő motorlépés: aktív ITU ütemező/MMIO és CPU IRQ-bekötés; utána friss helyi
firmware-próba. PKS/MIDI, DSP/hang/plugin/REAPER elfogadása nyitott. Yamaha-adatot
nem használunk; EG target-hold módosítás továbbra is hardveres bizonyítékhoz kötött.

## Történeti fejlesztési és előkészítési bejegyzések

Az alábbi korábbi „jelölt”, „függőben” és „következő” állítások a saját dátumukra
vonatkoznak; az aktuális feladatsort a fenti összefoglaló és az M0-szakasz tartalmazza.

Rögzítve: 2026-10-27. Kiindulópont: a #14 PR-ig beolvadt kutatási eredmények.
Ez végrehajtási terv; az alábbi, még nyitott mérföldkövek nem kész funkciók.
Állapotellenőrzés:2026-10-30,motoralap:#143 (`301eec75c72028b8f377ee4e099fce0b1df4de5e`).
A működő H8H-részhalmaz nem teljes CPU- vagy firmware-elfogadás.

## VOP3 teljes szavas hostadapter — jelölt,2026-11-03

Önálló template adapter a két kanonikus440000/480000,egyenként100 bájtos
ablakot chip/regiszter/full-word backendhívásra képezi. Páratlan cím,részmaszk,
tükör és ablakon kívüli cím előzetesen elutasított. Reg1 feltöltésvezérlés csak
explicit experimental_firmware_contract módban,a korábbi UploadControlContract
újrahasználatával:chipenként külön bit15 státusz,csak0/1 command. A write a
backendhez is eljut;egyéb input csak a modellben implementált EG_STATUS21.
Nem gyártunk alap nullás választ22-re vagy más ismeretlen olvasásra.
Backendhiba vagy elnyelt újrabelépési kísérlet mérgező,nem folytatható.
65024 route/value eset,chip controls,bounds/mask/read/fail-stop/reentry strict
ASan/UBSan PASS. Tesztbackend szintetikus,nem firmware/élő VOP3 integráció.
Szerződés:[vop3-host-word-adapter.md](vop3-host-word-adapter.md).
Helyi121/121 CTest (51,28s),100 Python,strict célzott ASan/UBSan PASS.
Audit624 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer40 csoport.
Saját CI/review függőben. Következő:natív híd és kutatási
busz explicit bekötése;MEG fizikai státusz és valódi hang továbbra nyitott.

## Explicit eredeti MEG-stub státuszkísérlet — jelölt,2026-11-03

MEG fizikai státusz/kézfogás szerződés továbbra ismeretlen. Rögzített eredeti
MAME s2_r/s10_r/s11_r mind00-t adó log-stub,nem ready-bit modell.
Ezért csak új explicit lcd-meg-source-stubs-16mhz profil kapcsolja a három
kanonikus olvasást00-ra,számlált megfigyeléssel. Alap/lcd/write-observe profil
szigorú marad;minden más read/word továbbra hibázik. Nincs fizikai vagy audio-
elfogadás.768 policy/read,lyuk/alias/word/reset/isolation és129 CPU-felosztás
strict ASan/UBSan PASS;768 eredeti zero-read összevetés PASS.

Ideiglenes,repo-n kívüli kontrollban00 válasszal VOP3-ig jut,FF állandó
válasszal100M ciklusig várakozó útvonal marad;ez nem mért hardveres válasz.
Új explicit profil:4096/1000000 szeletnél egy stubolvasás/öt vezérlőmegfigyelés
után azonos440002 word write stop,perifériaciklus10832302,PC353780,
6316 GPIO,0 TX/clock él. ROM változatlan. Szerződés:
[meg-source-status-experiment.md](meg-source-status-experiment.md).
Helyi120/120 CTest (62,25s),100 Python,strict ASan/UBSan és eredeti referencia
PASS. Audit621 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer39 csoport.
Saját CI/review függőben. Következő:VOP3-host kapcsolat
kutatása;MEG fizikai státuszhoz külön elsődleges specifikáció/mérés szükséges.

## MEG megfigyelő buszprofil — jelölt,2026-11-03

MEG külső byte write csak explicit sink opt-in mellett a400000..40003F
szűk ablakban továbbítható;a hostadapter továbbra ellenőrzi az odd lane/offsetet.
Read/word/instruction továbbra strict,nincs generikus MMIO-fallback.
A tiszta enable-query mellett area2 ASTCR/WCR/WCER időzítés használatos,
24-bit alias költséggel együtt. An1xMegResearchSink saját MEG-tárolót birtokol,
közös LCD/MEG idő- és fail-stop epochot használ;subset reset megtartja,külön
cold_reset újraépíti. Probe csak új explicit lcd-meg-observe-16mhz módban
kapcsolja be;a régi alap/lcd profil nem változik. meg_observed_controls számláló
csak log-only megfigyelés,nem parancshatás vagy működő effekt.

256 bank/control,alias/wait/reset/strict/poison és129 CPU-felosztás strict
ASan/UBSan PASS. CLI explicit/profilnegatív és olvasási hibakontroll PASS.
Változatlan helyi ROM4096/1000000 szelettel:négy megfigyelt vezérlőírás után
azonos stop400005 byte read,PC0xDA0C,perifériaciklus10814942,6316 GPIO,
0 TX/clock él. Nincs fake MEG-ready és boot/hangelfogadás.
Szerződés:[an1x-meg-research.md](an1x-meg-research.md). Helyi119/119 CTest (71,81s),100 Python,strict célzott ASan/UBSan PASS.
Audit618 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer38 csoport.
Saját CI/review függőben. Következő a MEG státusz-szerződés bizonyítékának vizsgálata.

## MEG hostcím-adapter — jelölt,2026-11-03

A helyi szervizrajz51.PDF-oldalán IC10 MEG A0..A4 a CPU A1..A5 hálózataira
csatlakozik,D0..D7 az alsó adatbájtra;LWR/RD/MEGCS külön jelzett. Kanonikus
kutatási ablak:400001+2*offset,offset0..18.400007 így3-as vezérlőoffset.
A teljes chip-select engedély és címtükrözés továbbra nincs igazolva.
Önálló MegHostWriteAdapter nem köti be a CPU-buszt. Alapból a nem implementált
vezérlők hibáznak;külön explicit observe_original_stubs módban hat eredeti
logging-only írás csak számlált megfigyelés,nincs hatás/status/ready/audio.
Minden nem támogatott bájtsáv/lyuk/tükör/offset elutasított,tároló állapotát nem
módosítja.1536 vezérlőérték,65536 tárolópár,ablak/lane/reset kontroll strict
ASan/UBSan PASS. Eredeti21 metódussal720896 tároló+1536 logging-only összevetés
PASS. Szerződés:[meg-host-write-adapter.md](meg-host-write-adapter.md).
Helyi118/118 CTest (51,59s),100 Python,strict ASan/UBSan és referencia PASS.
Audit615 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer37 csoport.
Saját CI/review függőben.
Következő:explicit kutatási sink-bekötés,szigorú ismeretlen olvasásokkal.

## MEG önálló regisztertárolás — jelölt,2026-11-03

A400000 körüli effektinterfész a korábbi rajzi/firmware-kutatás jelöltje;
a rögzített eredeti AN1x driver nem térképezi fel,ezért400007 buszelérését
most nem engedjük át. Önálló MegRegisterStorage:8-bit selector,tíz256-bájtos
bank,két256-szavas constant/offset bank független high/low lane-nel.
Nincs státuszolvasás,audio vagy vezérlőparancs-szimuláció;a logging-only
MAME-stubok nem bizonyítanak elfogadható választ. Nem támogatott offset hibázik.
Cold zero projektpolitika (eredeti start/reset üres);reset megtartja a tárolót.
720896 független bank/lane eset,65536 szóérték,isolation/strict/reset strict
ASan/UBSan PASS.15 eredeti metódussal720896 összevetés,azonos explicit kezdő-
állapotból;nem hardveres reset-/busz-/hangelfogadás. Szerződés:
[meg-register-storage.md](meg-register-storage.md). Helyi117/117 CTest (50,57s),100 Python,strict ASan/UBSan és eredeti
referencia PASS. Audit612 fájl,notice229 fájl/5 szöveg,diff-check PASS;
CI sanitizer36 csoport. Saját CI/review függőben. Következő:hostcím/vezérlőoffset bekötés bizonyítékának tisztázása.

## Leállított ITU TIER-tárolás — jelölt,2026-11-03

Öt TIER önállóan tárolja a három engedélyező bitet,reserved read-oneF8,
resetF8. Minden1280 csatorna/raw kombináció elfogadott és visszaolvasható.
Ez konfigurációtárolás,nem aktív ITU IRQ-szolgáltatás:TSTR továbbra tiltott,
TSR üres,nincs counter/capture/event ami flaget vagy IRQ-t kelthetne.
Az eredeti tier_update m_tier|F8,IER A/B/V megfeleltetés és üres ISR alapján;
a számlálóindítás vagy külső esemény támogatása előtt aktív flag/IRQ kell.
A régi idle-teszt tiltáselvárását tároláselvárás váltja,de disabled/reset/status/
word kontrolljai megmaradnak. Új1280 érték,owner/reset/time/start-negatív és129
CPU-felosztás strict ASan/UBSan PASS.1280 eredeti TIER/üres-TSR összevetés PASS.

Változatlan helyi ROM,100000000 ciklus/20000000 callback,lcd-16mhz,
4096/1000000 szelet:következő stop400007 byte write,PC0xD9F2,
perifériaciklus9555129,6316 GPIO,0 TX/clock él. Nem boot vagy hangelfogadás.
Szerződés:[an1x-itu-tier-stopped.md](an1x-itu-tier-stopped.md). Helyi116/116 CTest (85,95s),100 Python,strict célzott ASan/UBSan és
eredeti referencia PASS. Audit608 fájl,notice229 fájl/5 szöveg,diff-check PASS;
CI sanitizer35 csoport. Saját CI/review függőben. Következő a400007 periféria eredeti címtér-szerződése.

## Firmware-próba:korlát és valódi ITU-stop — jelölt,2026-11-03

A probe callback-korlátja továbbra explicit és véges,de legfeljebb20000000
lehet;a ciklus/szelet limitek változatlanok. Jelentés új mezői:stop_reason
(cycle_limit/callback_limit/execution_error),callback_limit_requested.
A callback-limit továbbra mérgező fail-stop,nem folytatható költségkeret.
A korábbi1000000 korlátnál mért megállás nem eszközhiba volt.

100000000 kért ciklus/20000000 callback,swap16,lcd-16mhz és4096/1000000
szelet mellett azonos valódi stop:FFFF94 TIER4 byte write,PC0x40A62,
perifériaciklus8798299,5269 GPIO,0 TX/clock él. Hiba:Unsupported ITU interrupt
enable;firmware_boot_accepted=false. ROM hash előtte/utána változatlan.
Következő a leállított TIER4 engedély és tényleges ITU flag/IRQ szükséglet
vizsgálata rögzített eredeti forrásból,nem az elutasítás egyszerű kikapcsolása.
Szerződés:[an1x-firmware-event-diagnostics.md](an1x-firmware-event-diagnostics.md).
Saját CI/review függőben. Helyi115/115 CTest (50,22s),100 Python PASS;
audit605 fájl,notice229 fájl/5 szöveg,diff-check PASS. CPU/periféria változatlan.

## P9 kutatási bemeneti profil — jelölt,2026-11-03

A korábbi LCD MAME-patch kifejezetten read_port9=FF szintet ad,a rögzített
eredeti h8_device::port_default_r szinténFF-et ad nem bekötött portokra.
Csak az explicit LCD research sink P9 bemenetén vezetjük be ezt a headless
kutatási profilt. Nem fizikai huzalozásbizonyítás,nem általános GPIO-fallback.
A H8 portmodell DDR/kimenet/reserved bit keverése megmarad,más ismeretlen
bemenet továbbra mérgező hibát okoz. Alapértelmezett no-LCD probe nem változik.
Független bites oracle minden65536 DDR/data kombinációra,alias/idő/reset és
más portok fail-stop kontrolljai. Szerződés:[an1x-p9-input.md](an1x-p9-input.md).
Helyi115/115 CTest (52,63s),100 Python és strict ASan/UBSan PASS;
az optimalizált fixture újrafordítva,célzott CTest ismét PASS. CI/review függőben.
Audit604 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer34 csoport.
500000 ciklus1/64/257 szelettel hiba nélkül,azonos PC/state/periféria/22 GPIO;
hosszabb próbák callback-limitnél álltak meg,nem eszközhibánál. ROM változatlan.
Nem boot/hangelfogadás;következő további korlátos firmware- és eseménydiagnosztika.

## LCD-sink gépintegráció — jelölt,2026-11-03

Saját LCD-sink kapcsolja össze a portadaptert és a headless vezérlőt;explicit
ciklus/µs skálával. Nem-LCD bemenet továbbra hibázik,egyéb kimenet nem gyárt
eszközválaszt. GPIO-idővisszalépés vagy eszközhiba mérgezi a sink-epochot;
explicit cold_reset rekonstruálja. Gépi részreset megtartja a külső LCD-t.
129 CPU-felosztás,busy/read-latch/command/DDR/reset/fail-stop célzott ASan/UBSan
PASS. Az első fixture tévesen R/W-t és E-t egyszerre ejtette,ami valódi írásélt
hozott létre és busy-t indított;helyes read-falling,majd RW-váltás sorrendre javítva.

A probe opcionális lcd-16mhz profilja16 ciklus/µs-t használ;eredeti yman1x.cpp
H83002(config,16_MHz_XTAL) alapján explicit kutatási profil,nem hardveridő-mérés.
Alapértelmezett no-LCD működés megmarad. ROM-free CLI-tesztek mindkét profilt és
ismeretlen profil elutasítását ellenőrzik. Valós helyi változatlan1.04 ROMmal
szelet1/64/257 mellett azonos új stop:FFFFD2 P9 byte read,PC0x3F66A,
perifériaciklus482170,10 GPIO,0 TX/clock él. KorábbiFFFFD6 olvasáson túljutott.
Nem teljes boot vagy hang. Következő:P9 bemenetek bekötési szerződése bizonyíték
alapján,majd új korlátos próba. Szerződés:[an1x-lcd-machine.md](an1x-lcd-machine.md).
CI/review függőben.
Helyi114/114 CTest (50,76s),100 Python,célzott strict ASan/UBSan PASS.
Forrásaudit602 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer33 csoport.

## LC7985 állapot és memória — jelölt,2026-11-03

Önálló headless vezérlő80 bájt DDRAM-mal és64 bájt CGRAM-mal,cím/mód/entry/
display/shift állapottal. A #189 busy-órát használja;utasítás busy alatt
figyelmen kívül marad,de eredeti dr_r/dr_w szerint az adatút nem busy-gated.
Egy/két soros címátmenetek,entry-shift,statuscím és reset kezelve. Nincs render,
font-ROM vagy firmware-probe-bekötés. Érvénytelen DDRAM-adatindex elutasított,
nem az eredeti potenciális tömbtúlindexelés reprodukciója.
114688 független memóriaeset,cím/status/ready/reset/GPIO-latch célzott ASan/UBSan
PASS.50000 művelet/állapot összevetés a hash-ellenőrzött eredeti MAME kilenc
változatlan metódusával PASS,biztonságos címprofilban. Ideiglenes harness,nincs
repo-ba másolt eredeti implementáció vagy Yamaha-adat. Ez nem hardverelfogadás.
Szerződés:[lc7985-controller.md](lc7985-controller.md). CI/review függőben.
Helyi113/113 CTest (50,56s),100 Python,strict ASan/UBSan és eredeti referencia
PASS;forrásaudit599 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer32 csoport.

## LC7985 busy-óra — jelölt,2026-11-03

Önálló időkomponens:reset10000µs,beküldött utasítás busy alatt teljesen
figyelmen kívül hagyandó;00 nem indít busy-t,01/02/03=16400µs,minden04..FF
utasítás40µs a rögzített eredeti MAME ir_w prioritása alapján. Adathozzáférés
nem használja ezt az API-t;az eredeti dr_r/dr_w nem indít busy-t.
Explicit egész ciklus/µs skála,határpillanatban kész,monoton idő,overflow
előzetes elutasítás állapotváltozás nélkül. Nincs LCD-memória vagy teljes
vezérlő/probe-integráció;ez még nem oldja fel a Port B boot-megállást.
768 parancs/skála eset,10001 időfelosztás,reset/határ/overflow strict ASan/UBSan
PASS. Rögzített eredeti LC7985 teljes fájlhash ellenőrizve,aktív differenciál
nem készült. Szerződés:[lc7985-busy-clock.md](lc7985-busy-clock.md).
Saját CI/review függőben;következő a vezérlő állapot/memória szerződése.
Helyi112/112 CTest (50,86s),100 Python,strict ASan/UBSan PASS.
Forrásaudit595 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer31 csoport.

## LCD GPIO–vezérlő adapter — jelölt,2026-11-03

A meglévő LcdBus bekötést ciklusjelölt vezérlőhívásokhoz kapcsoló önálló
adapter készült. PA/PB/P9 kimeneti szinteket fogad,status/data readet E-felfutás,
command/data writeot E-lefutás indít;azonos szint nem ismétel tranzakciót.
Olvasott érték latchelt;nem LCD-bemenet hibázik,más portkimenet kezeletlenként
jelzett. A busy flag,memória és időkonverzió a későbbi vezérlő feladata.
512 független bájt/RS eset,él/latchelt adat/idő/routing ellenőrzés strict
ASan/UBSan alatt PASS. A fake vezérlő csak tesztfixture,nem LCD-emuláció.
Nincs firmware-probe bekötés vagy új bootelfogadás. Következő:LC7985 állapot/
busy időzítés szerződés a rögzített eredeti forrásból,majd integráció.
Szerződés:[an1x-lcd-gpio.md](an1x-lcd-gpio.md). CI/review függőben.
Helyi111/111 CTest (78,59s),100 Python,ASan/UBSan PASS;sanitizer CI30 csoport.
Forrásaudit592 fájl,notice229 fájl/5 szöveg és diff-check PASS.

A #186 utáni main mind5 CI-ellenőrzése sikeres. Változatlan1.04 ROM,privát
swap16,10000000 kért ciklus,1000000 callback-limit mellett a próba482160.
perifériaciklusnál megáll:byte data_readFFFFD6 (Port B),PC0x3F666,
instruction_state1864224/substate0. Szeletméret1/64/257 esetén azonos fault,
PC és perifériaidő;teljes visszatért szeletciklus rendre482158/482112/482132,
callback739440/116288/104976.9 GPIO,0 TX/clock él. A hibás szelet munkája nem
szerepel a completed_slice számlálóban,ezért ezek eltérése önmagában nem hiba.
Ez diagnosztikai részprofil,nem firmware-boot-,hardver- vagy hangelfogadás.

Rögzített eredeti MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
h83002.cpp132. sor:FFFFD6 portb port_r/dr_w;159. sor utáni konfiguráció
PORT_B,resetDDR00,mask00. Ellenőrzött privát archívumpillanatképből olvasva.
A chipcímtér nem bizonyítja az AN1x külső huzalozását vagy bemeneti értékét.
Következő:Port B külső eszköz/protokoll szerződés vizsgálata meglévő kutatási
forrásokból,majd explicit bemeneti modell és független tesztek. A hibajelzést
nem oldjuk fel bizonyítatlan fix bemenettel. ROM SHA előtte/utána azonos:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.

## ITU megszakítások letiltott állapota — jelölt,2026-11-03

Öt TIER csak letiltott IRQ-profilt fogad:32 raw elfogadott/224 tiltott érték
csatornánként. Öt TSR üres jelzőállapotot ad;mind256 írás utánF8 marad,
szoftver nem állíthat flaget. TSTR tiltott,nincs aktív counter/event/IRQ.
Gyártói10.2.12–13 és rögzített eredeti tier_update/isr_update/isr_to_sr alapján.
2560 értékeset,alias/reset/no-effect/word és129 CPU-vágás ellenőrizve.
Helyi validáció:110/110 CTest (60,27s),100 Python,strict ASan/UBSan PASS.
A változatlan helyi ROM swap16 próbája100000 kért ciklust teljesít hiba nélkül:
25339 callback,perifériaciklus100002,PC634348,9 GPIO,0 TX/clock él.
Ez korlátos futási megfigyelés,nem teljes firmware-boot vagy hangelfogadás.
Szerződés:[an1x-itu-irq-idle.md](an1x-itu-irq-idle.md). Saját CI/review függőben.

## ITU compare-mód kimenet nélkül — jelölt,2026-11-02

Öt TIOR csak no-output compare profilt fogad:4 raw/252 tiltott csatornánként,
readback/reset88. Nincs jelalak/capture/külső él/compare-event/IRQ,csatornák állnak.
Gyártói10.2.11 alapján;eredeti MAME base TIOR read00/write-ignored stub,ebből
nem állítunk aktív I/O referenciát.1280 csatornaérték,alias/reset/no-effect/word
és129 CPU-vágás. Korábbi negatív kontroll a támogatott65 helyett69-et ellenőriz.
Firmware88 írása teljesül,következő stopFFFF66 TIER0,PC0xA7E,ciklus315;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-io.md](an1x-itu-io.md).
Helyi validáció:109/109 CTest (60,44s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit587 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan28 csoport;saját CI/review és merge függőben.

## Leállított ITU-csatornavezérlők — jelölt,2026-11-02

Öt TCR tárolja az óra/él/counter-clear konfigurációt,reserved bit7 read-one,
reset80. TSTR továbbra tiltott,nincs aktív számláló/külső él/clear/IRQ/esemény.
Gyártói10.2.10 és rögzített eredeti tcr_w/tcr_update alapján,aktív counter-
differenciál nélkül.1280 érték,alias/owner/reset/no-event/word és129 CPU-vágás.
Firmware83 írása teljesül,következő stopFFFF65 TIOR0,PC0xA7A,ciklus307;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-control.md](an1x-itu-control.md).
Helyi validáció:108/108 CTest (95,67s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit585 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan27 csoport;saját CI/review és merge függőben.

## Leállított ITU általános regiszterek — jelölt,2026-11-02

Tíz GRA/GRB tároló szó/bájt hozzáféréssel,FFFF cold/reset. Önálló big-endian
bájtsávok,24-bit alias,páratlan szó előzetes elutasítás. Nincs aktív ITU-start,
compare/capture/flag/buffer/IRQ;counter és vezérlők továbbra tiltottak.
Rögzített eredeti tgr_r/w és gyártói10.2.8/10.3.1 alapján,aktív timer-differenciál
nélkül.655360 szóérték,bájtsávok,regiszterizoláció,reset/RAM/idő/no-event és129
CPU-vágás. Firmware következő stopFFFF64 TCR0,PC0xA76,perifériaciklus299;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-general.md](an1x-itu-general.md).
Helyi validáció:107/107 CTest (65,44s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit583 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer
szűrő pontosan26 csoport;saját CI/review és merge függőben.

## ITU-kimenetek letiltása — jelölt,2026-11-02

Gyártói10.2.5 alapjánTOER hideg/resetFF,de a firmwareC0 írása minden kimeneti
engedélyt töröl.4 raw elfogadott/252 engedélyező elutasított. Nincs aktív pinfunkció,
outputcallback/PWM/counter/IRQ;TOCR és páros word tiltott. Eredeti MAME-kezelő
read00/write-ignored stub,ettől a dokumentált kézikönyvi profil eltér.
256 érték,hideg/tiltott/reset különbség,alias/RAM/idő/word és129 CPU-vágás.
Firmware továbbjut,következő stopFFFF6A GRA0 szóírás,PC0xA6A,perifériaciklus277;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-output.md](an1x-itu-output.md).
Helyi validáció:106/106 CTest (59,64s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit581 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer25
csoport;saját CI/review és merge függőben.

## ITU normál mód és puffer nélküli funkció — jelölt,2026-11-02

Gyártói10.2.3–10.2.4 alapjánTMDR csak normál mód,FDIR konfiguráció tárolt;
4 raw elfogadott/252 tiltott. TFCR normál/puffer nélküli profil,szintén4/252.
Eredeti rögzített MAME-kezelők itt read00/write-ignored stubok,nem aktív referencia.
Nincs PWM/fázisszámlálás/buffertranszfer/counter/IRQ;FDIR még nem hajt overflow-t.
512 regiszterérték,alias/reset/RAM/idő/word és129 CPU-vágás. FirmwareA0 írása
most teljesül,következő stopFFFF90 output-enable,PC0xA62,perifériaciklus263;
256 teljes szeletciklus/69 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-mode.md](an1x-itu-mode.md).
Végleges helyi validáció:105/105 CTest (51,37s),100 Python,strict célzott
ASan/UBSan PASS. Forrásaudit579 fájl,notice229 fájl/5 szöveg,diff-check PASS.
CI sanitizer24 csoport;saját CI/review és merge függőben.

## Megtartott ITU-szinkronprofil — jelölt,2026-11-02

Gyártói10.2.2 alapjánFFFF61 TSNC (MAME neveTSYR),readbackE0. Csak független
csatornák,8 raw elfogadott/248 szinkronengedély elutasított. Az eredeti rögzített
MAME-kezelő itt stub:read00/write ignored;ettől a gyártói profil tudatosan eltér.
Nincs aktív ITU-szinkronizálás/counter/IRQ vagy eredeti runtime-differenciál állítva.
256 érték,alias/reset/RAM/idő/word és129 CPU-vágás. Változatlan helyi firmwareE0
írása teljesül,következő stopFFFF62 TMDR,PC0xA5A,perifériaciklus247;
192 teljes szeletciklus/55 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-itu-sync.md](an1x-itu-sync.md).
Helyi validáció:104/104 CTest (58,03s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit577 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer23
csoport;saját CI/review és merge függőben.

## Nyugalmi ADC-konfiguráció — jelölt,2026-11-02

Friss main,#178 után. ADCSR FFFFE8 csakADST0/ADIE0:csatorna/scan/óra tárolt,
ADF szoftveresen nem állítható.64 elfogadott/192 elutasított raw érték.
Nincs analóg minta/konverzió/eredmény/trigger/IRQ;ADCR és páros word tiltott.
Gyártói14.2.2 és rögzített eredeti ADC-kezelők alapján,aktív ADC-differenciál nélkül.
256 érték,alias/reset/RAM/idő/no-effect/word és129 CPU-vágás. Firmware03 írása
most teljesül,következő stopFFFF61 ITU TSYR,PC0xA56,perifériaciklus239;
192 teljes szeletciklus/55 callback,9 GPIO. Nem teljes boot vagy hang.
Szerződés:[an1x-adc-idle.md](an1x-adc-idle.md).
Helyi validáció:103/103 CTest (58,11s),100 Python,strict célzott ASan/UBSan
PASS. Forrásaudit575 fájl,notice229 fájl/5 szöveg,diff-check PASS. CI sanitizer22
csoport;saját CI/review és merge függőben.

## Aktív refresh buszóra — jelölt,2026-11-02

A #177 beolvadt `3f0baf6696bedd423650dc2aeb094bddfd6f5aaf` commitként;
ellenőrzött head934936237d7d8fcb9cfc24d8dc2b317afe12052d,mind5 CI sikeres.
Review-szál/változtatáskérés nincs,automatikus review-eredmény nincs rögzítve.
Az önálló timer most a busz abszolút idejével halad,hét osztóval;CMIE továbbra
elutasított. Nincs IRQ-deadline vagy fizikai refresh/DRAM. Reset megtartja az időt.
Független hét ciklusorákulum,reset/CMF/debt és257 CPU-felosztás;eredeti524288
összehasonlítás továbbra PASS. Változatlan helyi ROM192 teljes szeletciklus/55
callback utánFFFFE8 ADC-írásnál áll meg,PC0xA44,perifériaciklus204,9 GPIO-jelzés.
Nem teljes boot vagy hang. Szerződés:[an1x-refresh-clock.md](an1x-refresh-clock.md).
Helyi validáció:102/102 CTest (63,59s),100 Python,célzott strict ASan/UBSan
és524288 eredeti referencia-összehasonlítás PASS. Forrásaudit573 fájl,notice229
fájl/5 szöveg,diff-check PASS. CI sanitizer21 csoport;saját CI/review függőben.

## Önálló aktív refresh-timer — jelölt,2026-11-02

Friss main,#176 után. Külön komponens,nincs még gépi/IRQ bekötés. Hét osztó,
abszolút fázis,compare/CMF,CMIE kéréslekérdezés,reset és korlátos deadline.
Független ciklusorákulum és16416 felosztás;524288 összehasonlítás változatlan,
privát snapshotból fordított eredeti MAME-kezelőkkel strict ASan/UBSan alatt.
Eltérő túlcsordulás/compare-fölötti számláló és CMF read-gate korlát dokumentált.
FFFFAD gépi megállás változatlan. Következő:eseményidővonal/IRQ/CPU bekötés.
Szerződés:[h8-refresh-timer.md](h8-refresh-timer.md).
Helyi validáció:101/101 CTest (50,40s),100 Python,célzott és eredeti referencia
ASan/UBSan PASS. Forrásaudit571 fájl,notice229 fájl/5 szöveg,diff-check PASS.
CI sanitizer20 csoport;saját CI/review és merge függőben.

## Leállított refresh-timer alap — jelölt,2026-11-02

Friss main,#175 után. RTMCSR csakCKS0/CMIE0 (16 raw érték),CMF szoftveresen
nem állítható;RTCNT/RTCOR bájtos tárolás és támogatott reset00/FF. Teljes256
vezérlés,65536 számláló/compare pár,alias/word/reset és129 CPU-vágás.
Eredeti rögzített MAME forrás és gyártói7.2.2–7.2.4 alapján;aktív timer
referencia-differenciál még nincs. Futó óra/IRQ továbbra tiltott;a helyi ROM0F
írásaFFFFAD címen ezért változatlanul megáll. Ez előkészítés,nem boot-előrelépés.
Szerződés:[an1x-refresh-stopped.md](an1x-refresh-stopped.md).
Validáció:teljes futás99 meglévő teszt PASS;az új teszt kivételtípus-javítás
utáni célzott CTest is PASS (0,35s),így mind100 csoport ellenőrzött.
100 Python,strict célzott ASan/UBSan,forrásaudit567 fájl,notice229 fájl/5 szöveg
és diff-check PASS. CI sanitizer19 csoport;saját CI/review függőben.

## Megtartott RFSHCR profil — jelölt,2026-11-02

A beolvadt #174 után friss mainből:gyártói7.2.1 alapján FFFFAC=RFSHCR.
Csak00/02 írás fogadott,readback02;254 más érték elutasított. Timer/DRAM/PSRAM,
refresh-pin/mux/ciklus nincs szimulálva. Eredeti MAME map nem kezeli RFSHCR-t;
FFAD–FFAF timer címek továbbra tiltottak. 256 érték,alias/reset/RAM/idő/word
és129 CPU-vágás ellenőrzés. Részletek:[an1x-refresh-profile.md](an1x-refresh-profile.md).
Változatlan helyi ROM következő megállásaFFFFAD,PC0xA1C,perifériaciklus124;
64 teljes szeletciklus/19 callback,nem teljes boot vagy hang.
Helyi validáció:99/99 CTest (56,78s),100 Python és célzott strict ASan/UBSan PASS.
Forrásaudit565 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer18 csoport.
Saját CI/review és beolvasztás függőben.

## Megtartott BRCR profil — jelölt, 2026-11-02

A #173 beolvadt `876050a1dd299dd44b9b95742d5b8067ecbc9404` commitként;
öt végső CI sikeres az e6fe544f12d00e858bce536800147c18b81954ed headen.
A macOS feladatok igazolt hosted-arm64 kapacitáshiány után újrapróbálva sikerültek;
kódhiba vagy szabálymegkerülés nem kellett. Automatikus review ugyanazon headen
2026-11-02T09:32:38Z-kor találat nélkül kész;bizonyíték a PR botösszegzésében.
Gyártói6.2.5 alapján BRCR0xFFFFF3 változatlanFE profil:16 tartalékbit-normalizált
írás elfogadott,240 buszátadást/cím-pinfunkciót módosító érték elutasított.
Nincs BREQ/BACK/arbitráció vagy fizikai pinmux-szimuláció;a logikai24-bit map marad.
256 érték,alias/reset/memória/idő,word előzetes elutasítás és129 CPU-vágás tesztelt.
A változatlan helyi ROM már64 teljes szeletciklust/19 callbacket teljesít,majd
0xFFFFAC bájtírásnál áll meg,PC0xA18,perifériaciklus116,öt GPIO-outputjelzéssel.
Ez nem teljes boot vagy hardveridő. Szerződés:[an1x-bus-release.md](an1x-bus-release.md).
Következő0xFFFFAC forrás/hardver-szerződésének azonosítása és korlátos kezelése.
Helyi validáció:98/98 CTest (50,51s),100 Python,célzott strict ASan/UBSan PASS.
Forrásaudit563 fájl,notice229 fájl/5 szöveg,diff-check PASS;CI sanitizer17 csoport.
Az IRQ-hole teszt korábbi BRCR-ismeretlencím elvárását frissítettük;word-őrök megmaradtak.
Saját CI/review és beolvasztás függőben.

## Programozott wait-state részprofil — jelölt, 2026-11-02

A #172 beolvadt `5aadb6279ea98f608550453b06953a7676e5372f` commitként;
öt CI sikeres aff9b87f42f56c806a3df3e3eb0e050df0a1d15b7 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Gyártói6.2.3/6.2.4 alapján WCR/WCER bájtoskezelés:programozott0–3 várakozó
állapot vagy controller-wait nélkül;pin-wait1/auto írás elutasítva. WCER-disabled
pin-wait0 ág explicit WAIT-inaktív kutatási feltétel,nem mért AN1x állapot.
WCR felső bitek read-one,WCER minden bit tárolt;csak engedélyezett külső3-state
ROM/SRAM kap3+count költséget. Kétállapotú/belső hozzáférés nem nyúlik meg.
Buszköltség-őr2..6-ra bővül;hideg/reset profilASTCR00/WCRF4/WCERFF,nem fizikai
alapállapot. Páros word és ismeretlen MMIO továbbra fail-closed.
256 WCR/256 WCER érték,független területmaszk,idő/counter/alulcsordulás,
reset/alias/word és257 CPU-vágás. Eredetnyilvántartás frissítve.
A változatlan helyi ROM már BRCR0xFFFFF3 írásnál áll meg,PC0x9FC,
perifériaciklus60. Teljes boot/hang/plugin továbbra sincs elfogadva.
Szerződés:[an1x-wait-profile.md](an1x-wait-profile.md). Következő korlátos BRCR profil.
Release build és mind97 CTest sikeres (81,60s);100 Python és strict célzott
ASan/UBSan sikeres,UINT64_MAX/overflow időhatárral is. CI sanitizer-szűrő16
csoportot választ. Forrásaudit561 fájl,notice229 fájl/5 szöveg,diff-check sikeres.
CI/review és beolvasztás függőben.

## ASTCR kiválasztott hozzáférési idő — jelölt, 2026-11-02

A #171 beolvadt `ecee85e7f8daf1039fac431e420072e8fe9e6491` commitként;
öt CI sikeres ac71f992a2c03049e6da13413765afa167c43f458 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Gyártói kézikönyv6.2.2:ASTCR0xFFFFED területenként2/3 állapotot választ.
A firmware0F-et ír,ezért a puszta latch nem lett volna elég. A buszsegéd új,
őrözött,pure access_cycles lekérdezése2/3 ciklust számol;default host továbbra2.
ROM/SRAM az ASTCR alsó bitjeit követi,belső területek régi2 ciklusos kutatási
költsége marad. ASTCR minden256 értéke tárolt. Reset00 kifejezetten no-wait kutatási
profil,NEM a kézikönyv fizikai FF resetje. WAIT/WCR/WCER késleltetés még nincs.
Független idő/counter/címteszt mind5 buszműveletre,költséghatár/reentry/alulcsordulás,
256 regiszterérték és193 CPU-vágás;adaptációs eredetnyilvántartás frissítve.
A változatlan helyi ROM most WCER0xFFFFEF bájtírásnál áll meg,PC0x9F4,
perifériaciklus44. Ez kutatási modell,nem hardveridő vagy bootelfogadás.
Részletek:[an1x-access-state.md](an1x-access-state.md). Következő WCER/wait-state profil.
Release build és mind96 CTest sikeres (72,86s);100 Python és strict célzott
ASan/UBSan sikeres. CI sanitizer-szűrő pontosan15 csoportot választ.
Forrásaudit559 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben.

## ABWCR leképezett-területi profil — jelölt, 2026-11-02

A #170 beolvadt `a9c1a2d4c3b6924463269c8985f38f62e1aa8611` commitként;
öt CI sikeres a19483ca548d73031933c523598568c9afda00fbd headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
A gyártói H8/3002 kézikönyv6.2.1 alapján0xFFFFEC az ABWCR. Projektprofil:
ROM/SRAM terület0/1 marad16-bit,ezért alsó2 bit nem állítható;felső6 bit
visszaolvasható,de az érintett külső területek továbbra sincsenek leképezve.
64 érték támogatott,192 elutasított. Reset00 explicit kutatási mód,nem igazolt
AN1x mode-pin állapot. Word/szomszédos ASTCR-hozzáférés továbbra is fail-closed.
Az első00-only policy a firmware08 írását elutasította;csak a területazonosítás
után bővült a megengedett profil. Forrás:[an1x-bus-width.md](an1x-bus-width.md).
Független256 érték,alias/reset/word/szomszédteszt,129 CPU-folytatási vágás.
A helyi változatlan ROM swap16 próbája már ASTCR0xFFFFED írásnál áll meg,
PC0x9F0,perifériaciklus36 (korábbanABWCR30). Teljes boot/hang/plugin nincs.
Release build és mind95 CTest sikeres (62,02s);100 Python és strict célzott
ASan/UBSan sikeres. CI sanitizer-szűrő pontosan14 csoportot választ.
Forrásaudit557 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben.
Következő:ASTCR és támogatott buszidőzítési profil célzott vizsgálata.

## Korlátos firmware-boot próba — jelölt, 2026-11-02

A #169 beolvadt `4cdc916e3bdd6e31eda23adfb184955b753b4ac1` commitként;
öt CI sikeres aza95352d262a4149ace591b908913ae5f76025938 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új in-process CLI explicit raw/swap16 memórianézettel,szigorú ciklus/szelet/callback
korlátokkal és JSON stopjelentéssel. A ROM csak olvasott,nem másoljuk repository-ba.
Hibás buszhozzáférés cím/típus/szélesség metaadata látható;ismeretlen GPIO-input
fail-closed. Nincs kitalált PKS/MIDI/LCD/DSP válasz vagy bootelfogadás.
Szintetikus CLI-teszt ellenőrzi a progresszt,ROM/MMIO/GPIO hibát,dekóder/callback
megállást,argumentumokat,bájtsorrendet,méretet és a változatlan bemeneti SHA-t.
Az első dekóderkontroll FFFF-je érvényes MOV volt;javítva invalid EEPMOV-kiterjesztésre.
Helyi1.04 ROM explicit swap16 nézetben reset0x9D8,első stop0xFFFFEC bájtírás,
PC0x9EC,perifériaciklus30. Eredeti SHA előtte/utána azonos. A kezdeti nyersnézet
bájtsorrendhibás kísérlet volt,nem firmware-bizonyíték;oddreset-őr bekerült.
Részletes szerződés:[an1x-boot-probe.md](an1x-boot-probe.md). Következő0xFFFFEC
forrás/hardver-szerződésének vizsgálata,majd célzott MMIO-fejlesztés.
Release build és mind94 CTest sikeres (51,24s);100 Python és strict célzott
ASan/UBSan CLI-teszt sikeres. CI sanitizer-szűrő pontosan13 csoportot választ.
Forrásaudit555 fájl,notice229 fájl/5 szöveg és diff-check sikeres.
CI/review és beolvasztás függőben;teljes boot/hang/plugin továbbra sincs elfogadva.

## Gépszintű támogatott reset — jelölt, 2026-11-02

A #168 beolvadt `90e484efa59d66d7da0dae0ade34022afb11aefe` commitként;
öt CI sikeres a777d562505fe0966f8353d063238b6caf3e62f15 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új reset_supported_state gép-API:csak élő,álló gépen,CPU-adósság nélkül és nem
előrefutó perifériaidővel. Érvénytelen határ módosítás nélkül visszautasítva.
A jelen időpontra eső input előbb feldolgozódik,utána CPU-resetkérés,majd a
#168 szerinti buszperiféria-reset. Siker után külön reset-origin/számláló frissül;
abszolút idő,hideg generáció,RAM/GPIO és jövőbeli RX megmarad. Hideg újraépítés
külön nullás időpéldányt és nullás támogatottreset-számlálót ad.
Elkapott gépi reentry azonnal megjelöli a buszt is hibásnak,hogy ne legyenek
utána további kimenetek. Részleges resethiba fail-stop;hideg újraépítés helyreállít.
Teszt81 megállási pont:41 elfogadott,40 adósságos elutasítás;regiszter/RAM/idő,
literális SCI-kimenetsorrend,ismétlés,jövőbeli és pont időhatáron eső RX,reentry.
Ez a támogatott részhalmaz integrációja,nem fizikai RESET-assert/felengedés vagy
minden AN1x-eszköz elfogadása. Eredeti referencia MAME706dbb73281320c54d7c2f16262c1c3786f2be01
és rögzített reset/child-order források;új teljesgép upstream runtime trace nincs.
CI ASan/UBSan szűrő pontosan12 csoportot választ. Release build és mind93 CTest
sikeres (59,08s);100 Python és strict célzott ASan/UBSan sikeres. Elutasítás után
a ciklusadósság visszafizetése és az újabb reset is tesztelt. Rögzített reset és
konfigurációs források egyeznek;forrásaudit552 fájl,notice229 fájl/5 szöveg és
diff-check sikeres. CI/review és beolvasztás függőben.
Következő:korlátos firmware-boot diagnosztika és a hiányzó gépi MMIO/MIDI út felmérése.

## Támogatott buszperifériák resetje — jelölt, 2026-11-02

A #167 beolvadt `e0b8c9037913a054a8f37e5ccba19a0a571409ac` commitként;
öt CI sikeres a27ef4adae734e2beae33fb1611306c56242ccd10 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új busz API:reset_supported_peripherals álló tulajdonoson,SYSCR09/IRQ,majd
port4/6/7/8/9/A/B,SCI0/SCI1 sorrenddel. Az IRQ-szűrőt a megtartott CPU CCR-ből
újrapublikálja;fizikai szintek megmaradnak. RAM/GPIO-regiszter/inputütemezés és
abszolút idő változatlan;régi SCI-shifteresemények törlődnek. Az újra publikált
SCI órajel/TX-szintek a jelenlegi buszidőn jelennek meg. CPU-resetet nem indít.
A részhalmaz sorrendje projektbeli integráció,nem teljes MAME-eszközbejárás:
DMA/ADC/timer/watchdog/refresh és fizikai RESET-vonal nincs lefedve.
GPIO/SCI callbackben elnyelt reentry azonnal megállítja a további kimeneteket.
Független teszt3 abszolút kezdőidővel:szó szerinti kimenetsorrend,időbélyegek,
regiszterprofil,RAM/GPIO-megőrzés,tartott IRQ újralatch,függő RX,idempotens
GPIO-újrapublikálás és két reentry-határ. Az első elvárt nyom két GPIO-maszkját
javítottuk (port6:7F,port8:10 irány);motorkód miatt nem kellett módosítani.
A CI ASan/UBSan szűrő pontosan11 csoportot választ. Release build és mind92
CTest sikeres (72,06s);100 Python és strict célzott ASan/UBSan sikeres.
Rögzített reset/konfigurációforrások egyeznek;forrásaudit551 fájl,notice-lefedettség
229 fájl/5 szöveg és diff-check sikeres. CI/review és beolvasztás függőben.
Következő a CPU/busz reset összehangolása.

## Halasztott CPU-reset — jelölt, 2026-11-02

A #166 beolvadt `3bf3ed4dd4ff422b79918d8bc3889b812ec75e1f` commitként;
öt CI sikeres a 2b8528c0c2d7b57c1baa7925ceab9701b920b614 headen.
Automatikus review-eredmény nem volt rögzítve,nyitott review-szál nem volt.
Új request_reset_after_debt CPU-átmenet csak álló,élő runneren:régi részutasítás
eldobása,reset-dispatch kijelölése,kért/felvett IRQ-könyvelés törlése.
Általános regiszterek,CCR és a már lezajlott buszhatások/ciklusadósság megmaradnak.
A következő futás előbb visszafizeti az adósságot,utána kezd reset-vektort olvasni.
Ez projektbeli halasztott CPU-szabály,nem fizikai RESET-vonal vagy teljesgép-reset.
A hostnak kell a perifériák/bemenetek resetjét külön rendeznie;gépszintű API nincs.
Független szó szerinti időnyom:3 abszolút kezdőidő,21 kérési pont,7 folytatási
vágás (441 eset),nemnulla regisztermegőrzés,ismételt kérés és reentry fail-stop.
Eredeti referencia változatlan:MAME706dbb73281320c54d7c2f16262c1c3786f2be01;
research/h83002-reset-contract.json és generált resetkezelők. Nem új teljes MAME
warm-reset runtime összevetés;az adaptációk eredetnyilvántartása frissítve.
Release build és mind91 CTest sikeres (85,62s);100 Python sikeres.
Strict célzott ASan/UBSan sikeres,a rögzített eredeti resetforrások egyeznek.
Forrásaudit550 fájl,notice-lefedettség229 fájl/5 szöveg,diff-check sikeres.
CI/review és beolvasztás még függőben.

## Gépi időtartomány-integráció — jelölt, 2026-11-02

A #165 beolvadt `09af5e8f9850d356f72c3557aa5c4bceb5ed74fe` commitként
(2026-11-02T02:42:54Z). Öt CI sikeres a bedad28721aa1d838a799c759ee9da3cfb13220e
headen;automatikus review-eredmény nincs rögzítve,nyitott review-szál nem volt.
A gép korábbi cursor_ számlálóját ResetTimeDomain abszolút ideje váltja fel.
Sikeres szelet végén frissül;hibás tartomány nem változtatja az időt,a részleges
végrehajtási hiba továbbra is fail-stop. Hideg újraépítés új nullás időpéldányt
kap,meglévő gépgenerációval. CPU-adósság és input-határidő nem kerül átírásra;
reset_origin nincs meghívva,nincs új meleg-reset API vagy hardveres elfogadás.
401 vágási ponton összevetjük az egyben/szétvágott CPU-állapotot,RX-fogyasztást
és TX-idősort;nullás/negatív szeletek és hideg újraindítás is ellenőrzött.
CI ASan/UBSan készlet bővül az idősegéddel és a teljes gépéletciklussal.
Helyi Release build és mind91 CTest sikeres (73,52s);100 Python sikeres.
Célzott strict ASan/UBSan gépteszt sikeres;CI-szűrő pontosan10 csoportot választ.
Forrásaudit550 fájl,notice-lefedettség229 fájl/5 szöveg és diff-check sikeres.
Távoli CI és végső review még függőben.
Következő:korlátos reset-owner szerződés a fennmaradó input/scheduler feltételekkel,
majd firmware-boot/MIDI és DSP-hangút;GUI és ROM-módosítás nélkül.

## Reset-időtartomány segéd — jelölt, 2026-11-02

A #164 beolvadt e620483c7d95e07aeb2bce69431fdb2a87c1fd03;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Új önálló ResetTimeDomain:
abszolút idő monoton,reset-origin/generáció külön,átváltási túlcsordulás elutasítva.
Három kezdőidő,300 reset,visszalépés és64 bites határesetek független tesztje.
Célzott strict ASan/UBSan sikeres. Még nincs bekötve a gépbe;CPU-adósságot,
inputsort vagy perifériát nem módosít,nem teljesgép-reset. A kezdeti fordítási
hiba hiányzó initializer_list fejléc volt,javítva. Következő a gép időkezelési
integrációja definiált nyugalmi határon.

## Reset időtartományok — jelölt, 2026-11-02

A #163 beolvadt d513d4fdf8ff7c9ba0eedc064fab9247a9355f64;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Négy eredeti időkezelési módszer
azonossága rögzítve:local_time,total_cycles,abort_timeslice,synchronize.
A resetelt CPU-ciklusszám nem azonos az abszolút perifériaidővel;szinkronizáció
zero-delay timerrel történik,nem közvetlen callbackkel. Következő:korlátos
időtartomány-szabály független tesztje,majd resetintegráció. Motorkód változatlan.
100 Python sikeres;teljes runtime készletet kutatási változás miatt nem futtattunk újra.

## Sorba állított RESET-vonal — jelölt, 2026-11-02

A #162 beolvadt36b71314dfae075b046b5b0bde34b92f0acb3c9b;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az eredeti empty_event_queue
callback azonossága is rögzítve. RESET ASSERT felfüggeszt,felengedés resetel,
majd folytat. A perifériakimenetek assertion alatti kezelésére upstream FIXME van.
A queue-index a callback végén törlődik,nem a device_input::reset-ben.
Új callback-azonosság teszt:megváltozott/hiányzó/dupla definíció.99 Python sikeres.
Motorkód változatlan;következő a scheduler-szinkronizáció és helyi idő vizsgálata.

## Framework owner-feloldás — jelölt, 2026-11-02

A #161 beolvadt a5b73ac6da7276f5afa4bbb1794b40a319a6a41f;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az eredeti mconfig.cpp teljes
fájlazonossága rögzítve. Forrásvizsgálat:resolve_owner kettőspontonként meglévő
szülőt keres,add_device a szülő listájának végére illeszt,majd konfigurál.
Ebből a nested DMA/timer-bejárás következik,nem22 közvetlen CPU-gyerek.
Ez forrásból levont következtetés,nem teljes runtime resettrace;SCI-base címkék,
konfigurációs overloadok és scheduler/bemeneti queue-idő továbbra is vizsgálandó.
Motorkód változatlan;98 Python és lockolt forrásazonosság sikeres.

## H83002 konstruktorcímkék — jelölt, 2026-11-02

A #160 beolvadt2b95cfe97bd6a292593871bbb51d0e407ab32988;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. A leltár most a konstruktor
finder-címkéit is ellenőrzi:DMA dma:0/1,időzítők timer16:0…4.
A SCI címkéit nem következteti ki a származtatott forrásból. Új független
teszt vizsgálja a beágyazott címkét és a hiányzó finder elutasítását.
Motorkód változatlan;a tényleges framework-owner/resetbejárás továbbra is nyitott.

## H83002 perifériakonfiguráció — jelölt, 2026-11-01

A #159 beolvadt0b61abcf2b03c28979f173a3cce30ec92f7bbd59;öt CI sikeres,
automatikus review-eredmény nem volt rögzítve. Az új leltár22 konfigurációs
bejegyzés sorrendjét és eredeti forrásazonosságát ellenőrzi. Különbség:
lexikai létrehozás nem egyenlő a tényleges szülő/gyerek resetbejárással.
Két új regressziós teszt:sorrendcsere/duplikáció és hiányzó módszer.
Motorkód nem változott;teljesgép-reset továbbra sincs. Következő:owner-feloldás,
eszközlista-beillesztés és idő/bemeneti eseménykezelés.

## Execution input reset pontosítása — jelölt, 2026-11-01

A #158 beolvadt a25f29c913c53e664d1019b6ce2d80dab694ee1e,
2026-11-01T21:42:20Z;négy CI sikeres,nem volt rögzített automatikus review-eredmény.
A negyedik rögzített framework-módszer a device_input::reset:csak az aktuális
és tárolt IRQ-vektort állítja alaphelyzetbe. Bemeneti szintet vagy queue-indexet
nem ír ebben a módszerben. A tágabb queue-törlési komment nem viselkedési bizonyíték.
A teljes reset-sorrend/idővonal továbbra is nyitott;motorkód nem változott.
95 Python sikeres;valódi lockolt archívum négy módszere egyezik.
Következő:gyerekek konkrét sorrendje és ütemezett bemenet/idő kezelés.

## Auditjavítás 3 — dokumentáció egységesítése, jelölt, 2026-11-01

A #157 beolvadt15217f7fe1ffa8e6894730b24357feac311c1c0d,
2026-11-01T19:52:16Z;pontos head49f64aa142409fef04fbf7393caeaada053971b0.
README/státusz/roadmap aktuális összefoglalója90 CTest/95 Python leltárra,
#154–157 beolvadt változásokra és a valóban nyitott gépintegrációs kapukra frissült.
A régi jelölt/pending bejegyzések történeti állapotként elkülönítve maradnak.
A teljes helyi90/95 bizonyíték #156-hoz,a nyolc sanitizer-csoport #157-hez tartozik.
Ez csak dokumentáció;runtime tesztet nem futtatunk újra. Jelölt,nem beolvadt.
Következő:execution-input reset,gyereksorrend és scheduler-idő kutatása/kis tesztelt
integrációja. Gépszintű reset,firmware/MIDI/PKS és hang továbbra sincs elfogadva.

## Keretrendszer reset-sorrend — jelölt, 2026-11-01

A #153 beolvadt1a2468855cf35ee5641d586bd1b6777a5e75f71f,
2026-10-31T19:07:13Z;aktuális head8f314a5c93471300b7ac6a478416c4d0514cd005.
Négy CI sikeres (41225516208,41225516296);sikeres automatikus review-t nem állítunk.
Az új archívumellenőrző két eredeti framework-fájl/három módszer azonosságát
rögzíti. Sorrend:interface pre-reset,szülő reset,gyerekek resetje,szülő utómunka,
interface post-reset. Az execution-interface külön törli a ciklusszámlálót és
reseteli a bemeneti objektumokat;ezért a komponensbemenet-megőrzési tesztek
nem bizonyítják a teljes framework viselkedését. Következő:bemeneti objektumok,
gyerek-létrehozási sorrend és scheduler-idő. Új melegreset-API nincs.
94 Python sikeres;teljes runtime készletet kutatási változás miatt nem futtattunk újra.
Az automatika továbbra is leállítva;ez a felhasználó által kért folytatás.

## Aktuális összefoglaló az első REAPER-hangig — 2026-10-31

A #152 beolvadt `8b2930234de7b0b512f8ecdb9f04414782f14a53`,
2026-10-31T17:02:44Z (Budapest18:02:44). A pontos head
`cfca0c785364cb44f8d478735a2b8d82b84d16e7` négy CI-feladata sikeres:
41210850333 és41210850351. Az automatikus review kvótakorlát miatt nem készült el;
ez nem kódhibát bizonyító review,és nem sikeres review. Bizonyíték:
https://github.com/RobCZart82/yaman1x/pull/152#issuecomment-6544784804
A beolvadást élő GitHub-adat igazolja;az asszisztens korábban visszatartotta.
A húszperces automatika leállítva marad;ez a dokumentációfrissítés nem indítja újra.

Hátralévő fő lépések,függőségi sorrendben:

1. **Teljes gépintegráció:** CPU,busz,szükséges perifériák és DSP közös idővonala;
   indulási/reset-sorrend és megőrzött állapotok igazolása. A komponens-resettesztek
   beolvadtak,de gépszintű melegreset még nincs. Csak a firmware által ténylegesen
   igényelt hiányzó CPU/periféria-funkciókat kell célzottan pótolni.
2. **Firmware-indulás és MIDI:** a helyi eredeti firmware stabil indulása,
   PKS-kommunikáció szükséges viselkedésének tisztázása,SCI0 valódi byte-vétel,
   note-on/note-off firmware-szintű feldolgozás. A PKS ROM hiánya nincs megoldva;
   szükséges protokollhelyettesítés vagy további hardveres adat még vizsgálandó.
3. **Egy voice DSP-jének elfogadása:** táblarom-címzés,órajel,EG és visszaolvasások,
   CPU–DSP sorrend és chipközi jelút. A programfeltöltés önmagában nem hangbizonyíték.
4. **Első offline hang:** dokumentált száraz kimenet és PCM-skálázás;WAV note-on,
   tartás,note-off és lecsengés eseményekkel. Nemnulla,véges és reprodukálható jel.
   MEG csak akkor halasztható,ha a kiválasztott száraz hangút valóban működik nélküle.
5. **Valós idejű beágyazott motor:** időbélyeges MIDI,audioblokkok,mintavételi
   illesztés,előre foglalt memória,határidőmérés és beragadt hangok kezelése.
6. **Minimális VST3:** MIDI/audio adapter,példányonkénti állapot,helyi ROM-betöltés,
   csomagolás és validálás. GUI-fejlesztés továbbra sincs engedélyezve;az első próbához
   hosztparaméter/konfiguráció használható. AU és kidolgozott felület későbbi lépés.
7. **REAPER-elfogadás:** plugin felismerése,MIDI-hang indítása/elengedése,lehallgatás,
   WAV-mentés,offline összehasonlítás és projekt újranyitása rögzített környezetben.

Jelenleg nincs igazolt AN1x-hang,VST3 vagy AU. A sok sikeres CPU/perifériateszt
nem jelenti a firmware/DSP/hangút elfogadását. Az első hang legnagyobb nyitott
kockázata az indulási/MIDI–PKS út és az egy voice-hoz szükséges DSP-viselkedés.
A teljes hardver minden funkciójának emulálása nem előfeltétele az első hangnak.

## Korábbi előkészítési bejegyzések — történeti állapot

Az alábbi jelölt/függőben megjelölések a készítéskori állapotot őrzik.
A jelenlegi állapotot a fenti összefoglaló és a strukturált beolvadási napló adja.

## SCI reset utáni vétel — jelölt, 2026-10-31

A #151 beolvadt e4e79d4e0244a05784b7da705e9496f601446e99,
2026-10-31T14:14:36Z;négy CI sikeres,aktuális head review észrevétel nélkül
lezárult (issuecomment-6544283995).
Az új teszt mind256 bájtot fogadja reset után,1000 abszolút kezdőidővel.
Előtte73 ciklusnál megszakít egy régi vételt. Az alacsonyan tartott RX újraengedélyezése
nem képez hamis startélt;magasra váltás után az új keret pontos bájtot,SSR-t,
egyetlen vételi IRQ-t és hibamentes üres eseménysort eredményez. DMA-nyugtázás
csak az új vételi készjelzést törli. Ez komponensvizsgálat,nem gépszintű reset.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód változatlan;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## SCI reset utáni adás — jelölt, 2026-10-31

A #150 beolvadt1ef73ef6fe3f198c2a82d9e962a282860218dd79,
2026-10-31T13:34:33Z;négy CI sikeres,aktuális head review észrevétel nélkül
lezárult (issuecomment-6543843939).
Az új teszt mind256 bájtot újraküldi reset után,1000 abszolút kezdőidővel.
Előtte73 ciklusnál megszakít egy másik keretet. Független,bitenként és időponttal
megadott elvárás ellenőrzi a keretet,az egyszeri kész/befejezés IRQ-t és az
üres eseménysort. Ez komponenshelyreállítás,nem gépszintű melegreset.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód változatlan;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## IRQ-reset bemenetmegőrzés — jelölt, 2026-10-31

A #149 beolvadt1cb2f4a229fac079d5b318f5187ed2a340161838,
2026-10-31T12:54:35Z;négy CI sikeres,az aktuális head automatikus review-ja
észrevétel nélkül lezárult (issuecomment-6543431784).
Az új teszt65536 tartott IRQ/engedélyezési maszkot ellenőriz:reset után
újralatchelt szintek,törölt regiszterek,owner-frissítésig megtartott publikáció,
legalacsonyabb engedélyezett vektor,megtartott NMI-bemenet és új él elfogadása.
Ez komponensszerződés,nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest,strict ASan/UBSan,91 Python és eredeti resetazonosság sikeres.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt végső CI/review függőben.

## Aktív SCI reset teszt — jelölt, 2026-10-31

A #148 beolvadt1ec8ff3b657816e736930fe8029427409777f555,
2026-10-31T11:09:52Z;négy CI sikeres,automatikus review-eredmény nem volt rögzítve.
Az új komponensvizsgálat401 ciklusponton szakítja meg az egyidejű TX/RX-et:
clock magas majd TX magas,regiszteralapértékek,üres határidő,megőrzött alacsony
RX és régi befejezési/IRQ események hiánya. Ismételt reset mindkét magas
kimenetet újra publikálja. Ez nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest és ASan/UBSan,91 Python,eredeti resetazonosság sikeres.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt CI/review függőben.

## GPIO-reset megőrzési teszt — jelölt, 2026-10-31

A #147 beolvadt:b39643f330314f21e16a946c0e7e00e5a5eb41d0,
2026-10-31T09:59:11Z;négy CI sikeres,pontos head618667b31cd2ca7e6c80c063face8d844ed19aeb.
Automatikus review-eredmény nem volt rögzítve.
Az új teszt196608 maszk/irány/adat kombináción három ismételt resetet
ellenőriz. Független elvárás a DDR/DR/PCR/ODR megőrzése,a változatlan
kimenetértesítés elnyomása és a lábolvasás bitenként számolt eredménye.
Ez komponensvizsgálat,nem teljesgép-reset sorrend vagy hardverbizonyíték.
Célzott CTest,ASan/UBSan és91 Python sikeres;eredeti resetazonosság egyezik.
Motorkód nem változott;teljes runtime készletet nem futtattunk újra.
A jelölt végső CI/review még függőben.

## CPU-reset végrehajtási teszt — jelölt, 2026-10-31

A #146 beolvadt `41a1c1cea0b777653ede9e470ec5e6e7797b2ff2`,
2026-10-31T08:59:10Z;négy CI sikeres,review-eredmény nem volt rögzítve.
Az új független CPU-indulási teszt256 CCR×3 kezdőidő×7 szeletelés=5376
folytatási esetet ellenőriz:vektorolvasások,prefetch,filterközlés sorrendje,
megőrzött bitek,EXR,részállapot és időadósság. A filternek nincs időargumentuma;
a teszt az előző buszművelet idejét és a sorrendet rögzíti.
Strict fordítás és célzott ASan/UBSan sikeres;az eredeti generált MAME H8H
összehasonlítás200000 szeleten nulla eltérést mutat. Teljes91 Python sikeres.
Ez hideg CPU-indulás,nem új melegreset-API vagy teljesgép-reset bizonyíték.
Teljes build és89 CTest sikeres;CCR-törlési negatív kontroll6858 hibát jelez.
Végső CI/review függőben;a jelölt még nincs beolvadva.

## #145 lezárása és CPU-reset határvizsgálat — 2026-10-31

A #145 beolvadt: `e470f406c2d65eccda70e0c6381db1f826b2810b`,
2026-10-30T22:25:47Z;ellenőrzött head `90f8aea04fdddce17de60fe706b690b3b5f62c3b`.
Öt CI-feladat sikeres (41134650020,41134651634,41134649966).
Automatikus review-eredmény nem volt rögzítve;nem állítunk sikeres review-t.
A [resetdokumentum](h83002-warm-reset-contract.md) új helyi kódvizsgálata külön
rögzíti a CCR megőrzését,vektorolvasási/prefetch sorrendet,részállapotokat és
az időadósság/folytonos idővonal nyitott döntéseit. Ez dokumentációs jelölt,
nem végrehajtott upstream reset-differenciál és nem új melegreset-API.
Motorkód nem változott;runtime tesztet e dokumentáció miatt nem futtattunk újra.
Következő:a felsorolt resetmegfigyelések független végrehajtási tesztjei,
majd eszközsorrend és nem nulla időpont megtartása.

## Aktív kutatási lépés — rögzített melegreset-szerződés, 2026-10-30

Öt eredeti forrásfájl/hat device_reset módszer SHA256-azonossága reprodukálható
archívumellenőrzővel rögzítve. GPIO csak kimenetet frissít,SCI magas clock/TX-et
publikál,IRQ törlés után szintbemenetet újraértékel;nem általános nullázás.
[Mátrix és nyitott implementációs kapuk](h83002-warm-reset-contract.md).
Ez forráskutatás,nem melegreset-API vagy teljesgép/hardveregyezés.
Négy új független Python kontroll;teljes91 Python PASS. Motorkód nem változott;
a korábbi88 CTest/87 Python bizonyíték #143-hoz tartozik,nem újrafuttatott eredmény.
A #144 beolvadt9ddf040ec03ffe298dfdfcc259330c99ff1e570a,2026-10-30T21:52:30Z;
négy CI sikeres,automatikus review-eredmény nem volt rögzítve.
Következő:generált CPU-reset/runner és eszköz-reset-sorrend/időszabály ellenőrzése,
majd aktív SCI/IRQ/GPIO megőrzési és eseményközlési tesztek;teljes reset még nyitott.

## Aktuális lezárás — #143 hideg kutatási életciklus, 2026-10-30

Beolvadt `301eec75c72028b8f377ee4e099fce0b1df4de5e`,tesztelt head `d2da27c15c8ebf8c083f35bb4a2cef18eba4d8c4`.
Időpont:2026-10-30 22:27:12 Europe/Budapest (21:27:12 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127313949),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127314006),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41127313987):mind az öt feladat sikeres.
Tényleges macOS15,a required check történeti macos-14 neve kompatibilitási okból marad.
Nincs rögzített automatikus review-eredmény;nincs sikeres review-állítás.
88 CTest/87 Python,célzott ASan/UBSan és szigorú fordítói ellenőrzés PASS.
401 újraindítási pont friss CPU/TX-példánnyal egyezik;teljes256KiB NVRAM8 epochon
megmarad,explicit clear nulláz. Érvénytelen bemeneti csere megtartja a régi epochot.
Hibás/reentráns futás csak rekonstrukcióval áll helyre;ROM/owner/RX-sor izoláció PASS.
NVRAM-visszatöltést kihagyó próbahiba410 elvárt kudarc.
[Hatókör](an1x-research-lifecycle.md):hideg kutatási rekonstrukció,nem hardveres meleg reset.
533 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
Nincs aktív motorjelölt. Következő:a rögzített forrás melegreset-megőrzési/kimenetközlési
mátrixa,globális reset-sorrend és idő/bemeneti szabályok;csak utána in-place API.
Aktív SCI,IRQ-pin,GPIO-latch,RAM és függő esemény független tesztje szükséges.
Teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER még nyitott.
Új motorlépés a lezáró dokumentáció beolvadása után indulhat.

## CI-javítás — macOS14 kivezetése, 2026-10-30

A #142 macOS-feladata és célzott újrafuttatása a GitHub tervezett brownoutja miatt
megszakadt,nem fordítási/teszthiba miatt. A futtató2026-11-02 dátummal megszűnik.
Mindkét aktív workflow macos-15-re vált. A kötelező check történeti neve
`build-and-test (macos-14)` marad a ruleset25910947 kompatibilitása miatt;
a tényleges runner macos-15,a teljes tesztlépések változatlanok. Nincs szabálymegkerülés.
A MAME-kutató build Xcode16.2 rögzítése marad:a macOS15 hivatalos képe tartalmazza.
Új CI/review az új headen szükséges;korábbi zöldek nem igazolják az új workflowt.
Forrás:https://github.com/actions/runner-images/issues/13518
Kép:https://github.com/actions/runner-images/blob/main/images/macos/macos-15-Readme.md

## Történeti lezárás — #141 közös SYSCR, 2026-10-30

A #141 beolvadt `eba55a2367f9cc5e319b4f3f26049fe4da55af16`,tesztelt head `d50532a812abd23a65a2aafaf24f28ea815c0217`.
Időpont:2026-10-30 17:23:15 Europe/Budapest (16:23:15 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891561),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891536),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41085891444):mind az öt feladat sikeres.
Automatikus review-eredmény nem volt rögzítve;nincs sikeres review-állítás.
87 CTest/87 Python és célzott ASan/UBSan PASS;7712 IRQ/NMI-partíció,512 TRAPA CCR-eset,
241 SLEEP közbeni módváltás PASS. Fix módra visszaállított próbahiba2056 elvárt kudarc.
[Hatókör](an1x-syscr.md):csak01/09,közös owner,azonnali filter,helyes IRQ/TRAPA-belépés.
Generált H8/H8H-törzsek újragenerált lockkal egyeznek;562 módszer/264 kiválasztott sor változatlan.
A korábbi15424-es köztes darabszámot7712-re javítottuk;az integrációs teszt ezt igazolja.
Nincs aktív motorjelölt. Következő:explicit reset-életciklus,owner-állapotok,NVRAM-megőrzés,
idő/bemeneti sor kezelése,ismételt reset és hibás owner kezelése független tesztekkel.
Teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER továbbra is nyitott.
Új motorlépés a lezáró dokumentáció beolvadása után indulhat.

## Lezárt előkészítő lépés — #139 SYSCR módlogika, 2026-10-30

Beolvadt `b84d3b20bbef1f51a14af64da1a57df9f5cf2a88`,tesztelt head
`4d5ca9ba548cd4b3c3547ba191c2660daeebfce4`,2026-10-30 16:12:00 Europe/Budapest
(15:12:00 UTC). Mind az öt CI sikeres:
[platform/UBSan](https://github.com/RobCZart82/yaman1x/actions/runs/41072881939),
[MSan](https://github.com/RobCZart82/yaman1x/actions/runs/41072881943),
[MAME](https://github.com/RobCZart82/yaman1x/actions/runs/41072882034).
Nincs nyitott review-szál vagy rögzített automatikus review-eredmény;utóbbit nem állítjuk.

86 CTest/87 Python és célzott ASan/UBSan PASS.65536 mód/CCR eset és három eredeti
MAME-módszer összevetése:0 eltérés;hibás módszűrés8192 elvárt kudarc.
[Hatókör és következő integrációs kapuk](h83002-syscr-modes.md).
Ez önálló előkészítés:FFFFF2 továbbra is unmapped,a CPU rögzített profilja változatlan.
Az integrált gépalap #136 marad;nincs új firmware-/hangelfogadás.
Következő:közös SYSCR-owner,azonnali IRQ-filter és IRQ/TRAPA-belépés módváltás,
NMI/függőIRQ/SLEEP/RTE és partíciótesztek;utána teljes gépreset.
Nincs aktív motorjelölt;a lezáró dokumentáció beolvadása után indulhat új lépés.

## Dokumentáció-egyeztetés — 2026-10-30, #137 után

Ellenőrzött main: `7ee6db8bc418f696e67f0ea17fcc8dade073e47d`.
A [#137 dokumentációs lezárás](https://github.com/RobCZart82/yaman1x/pull/137)
2026-10-30 10:26:30 Europe/Budapest időpontban beolvadt (09:26:30 UTC).
Tesztelt head: `e7b5c345b2ed9dc0e38b1fa0ecc615b106602575`.
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41031322824),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41031322822): mind a négy feladat sikeres.
MAME ezen a dokumentációs változáson nem futott; a motor MAME-bizonyítéka a #136-hoz tartozik.
A #137 automatikus kódellenőrzése kvótalimit miatt nem fejeződött be; ez nem
sikeres review, és nem konkrét kódhiba. A #136 sikeres review-ja külön bizonyíték.

A motoralap továbbra is #136; nincs új motorfunkció vagy új firmware-/hangelfogadás.
A lezáró dokumentáció már beolvadt, a következő motorlépés megkezdhető:
közös CPU/host SYSCR-állapot, szűrés és IRQ/TRAPA-belépés módtesztjei, majd teljes gépreset.
Az alábbi korábbi lezárások és jelöltbejegyzések dátumhoz kötött történeti adatok;
régi „függőben” vagy „következő” megjegyzésük nem az aktuális feladatsor.

A mostani dokumentációjavítás rögzíti a #137 lezárását a strukturált naplóban,
frissíti az M0 elavult 31 hiányzó kódolássoros feladatát (jelenleg 264 kiválasztott,
0 hiányzó sor), és pontosítja a README korábbi komponensleírásainak hatókörét.
Helyi ellenőrzés: UTF-8 és belső linkek, JSON/egyedi PR-azonosítók/merge-ősök,
85 CTest/87 Python leltár, forrásaudit és licencértesítés-lefedettség PASS.
A futásidejű teszteket ehhez a kizárólag dokumentációs javításhoz nem futtattuk újra.
A javítás saját [#138 PR-ellenőrzései](https://github.com/RobCZart82/yaman1x/pull/138) ott követhetők; az itt hivatkozott korábbi CI
nem ennek az új dokumentációs változatnak az eredménye.

## Legutóbbi motorlezárás — #136, 2026-10-30

A [#136 PR](https://github.com/RobCZart82/yaman1x/pull/136) beolvadt:
merge `7aaa8d0178e06b8161c8508cda1cebdb20e2c072`,tesztelt head `6d58a3df9307b4513b61ea01d9eed3d70bd6e87a`.
Időpont:2026-10-30 10:04:38 Europe/Budapest (2026-10-30T09:04:38Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141599),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141600),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41028141620):mind az öt feladat sikeres.
Review:`completed_no_findings`;CI és review külön bizonyíték.

Beolvadt:**85 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
IRQ-MMIO:ISCR F4,IER F5,ISR F6,ICR0/1 F8/F9;szóalak F4/F8,24 bites címalias.
Odd/hiányos szó előzetesen elutasítva. ICR nem komplementált maszk és késleltetett
publikáció megőrizve (nem igazolt fizikai helyesség).65536 engedélyezés/szint,
65536 törlés,131072 szó,2048 él/szint és3856 CPU-partíció PASS. CPU-ból beállított
IRQ SLEEP-et ébreszt,ISR A5-öt ír NVRAM-ba,RTE SP-t helyreállít;I maszkol.
Eredeti18 IRQ-művelet/table200000 lépésben nulla eltérés. Fordított ICR-offset150185,
téves IER=>ISCR199554 hibával felismerve. Helyi84 futtatható CTest/86 Python PASS;
ismert Windows-symlink eset kizárva. Linux CI UBSan PASS;helyi ASan/UBSan nem futott.
525 fájl forrásauditja/229 fájl licencnyilvántartása PASS (nem jogi engedély).
[Pontos források,hatókör és SYSCR-feltárás](an1x-irq-mmio.md).

A SYSCR bit08 a CPU szűrését és IRQ/TRAPA-belépését együtt érinti. Fix0B profil
marad,F2 olvasás/írás elutasítva,nincs elhallgatott vagy kitalált működés.
Nincs aktív motorjelölt. Következő:külön CPU/host SYSCR-szerződés és módtesztek,
majd teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER elfogadás nyitott.
A lezáró dokumentáció #137-ben beolvadt; ez a sorrendi feltétel teljesült.

## Történeti lezárás — #134, 2026-10-30

A [#134 PR](https://github.com/RobCZart82/yaman1x/pull/134) beolvadt:
merge `7c46a56495a166ed9e6abbe4bea11915e08bbedd`,tesztelt head `963fc91ca7512a80e5830dc8a784c8c186c9e5e9`.
Időpont:2026-10-30 08:46:00 Europe/Budapest (2026-10-30T07:46:00Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299804),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299832),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41020299487):mind az öt feladat sikeres.
Review:`no_recorded_result_at_inspection`;CI és review külön bizonyíték.

Beolvadt:**84 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
GPIO-szóolvasás:FFFFCE,D0,D2;írás:D0,D2. Előzetes ellenőrzés az odd/hiányos/readonly
írás ellen. Big-endian,felső bájt előbb,azonos buszidőbélyeg:explicit szoftverszabály,
nem fizikai időzítési vagy teljes MAME-címtér-egyezési állítás. A CPU egy szóhozzáférést
számol. Első bájt reentry-je után a második nem fut;korábbi mellékhatás nem vonódik vissza.
196608 független eset,241 CPU-partíció PASS;DA65 NVRAM/R2,CCR89 ellenőrizve.
Eredeti portműveletek soros kompozíciója200000 lépésben nulla eltérés. Felcserélt
olvasási bájt262198,hibás alsóbájtírás455493 hibás ellenőrzéssel felismerve.
Helyi83 futtatható CTest/86 Python PASS;ismert Windows-symlink eset kizárva.
Linux CI UBSan PASS;helyi ASan/UBSan nem futott.523 fájl forrásauditja/229 fájl
licencnyilvántartása PASS (nem jogi engedély). [Pontos szerződés](an1x-gpio-words.md).
Nincs aktív motorjelölt. Következő:IRQ/SYSCR szűkített bekötésének feltárása,majd
teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER elfogadás nyitott. Új motorlépés
csak e lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #132, 2026-10-30

A [#132 PR](https://github.com/RobCZart82/yaman1x/pull/132) beolvadt:
merge `5ac2585e9b84497764f1fe0e2bbf426bbbab84a6`,tesztelt head `66f480ea43b73551e934ec6528ca1dae4a5c5171`.
Időpont:2026-10-30 06:59:33 Europe/Budapest (2026-10-30T05:59:33Z).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924793),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924918),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/41009924833):mind az öt feladat sikeres.
Review állapot:`no_recorded_result_at_inspection`;CI- és review-bizonyíték külön kezelve.

Beolvadt:**83 CTest/87 Python**;CPU562 kezelője és264 kiválasztott sora változatlan.
PortB FFFFD4/FFFFD6,alap00/maszk00,külön állapot,callback ID11.262144 bit-orákulumos
eset,256 hideg bemenet,241 CPU-partíció PASS.0F/5A/15 =>1A NVRAM,CCR81.
Eredeti portműveletek200000 lépésben nulla eltérés. A referencia11 cpp-törzset
és inline FF-olvasást ellenőriz (összesen12 eredeti törzs). Téves maszk225082,
téves bemeneti port244961 hibás ellenőrzéssel felismerve;iterációnként két
ellenőrzési hely miatt a szám meghaladhatja a műveletszámot.
Helyi82 futtatható CTest/86 Python PASS;ismert Windows-symlink eset kizárva.
Linux CI UBSan PASS;helyi ASan/UBSan ebben a lépésben nem futott.
521 fájl forrásauditja és229 fájl licencnyilvántartása PASS (nem jogi engedély).

Auditjavítások:IR öt szóra frissítve,régi SLEEP/GPIO-korlátok történeti hatóköre
jelölve,README build-tesztszám66-ról83-ra javítva. A README eltérése az előző
áttekintésből kimaradt;most javítva. A ROADMAP régi #78-jelöltfelirata is történeti jelölést kapott. [Pontos források és korlátok](an1x-gpiob.md).
Nincs aktív motorjelölt. Következő:GPIO-szóhozzáférések pontos meghatározása és
tesztjei,majd IRQ/SYSCR/teljes gépreset. Firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #130, 2026-10-30

A [#130 PR](https://github.com/RobCZart82/yaman1x/pull/130) beolvadt:
merge `fc9b483dcdd6c33385c1225303a0c0f2f0d7477c`,tesztelt head `e6f276dcf20e48d014a71f13dd73ac74b33da029`.
Időpont:2026-10-30 04:45:46 Europe/Budapest (03:45:46 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653002), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653007), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40997653087):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**82 CTest/87 Python**. PortA FFFFD1/FFFFD3,alap00/maszk00,callback ID10;
262144 nyolcbites eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>1A NVRAM,CCR81. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:téves maszk198248,rossz port252184 elvárt kudarc.
GPIO4/6/7/8/9/A/SCI ASan/UBSan PASS;519 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpioa.md).
Nincs aktív motorjelölt. Következő:portB DDR/adat,alap00/maszk00;utána a GPIO
szóhozzáféréseinek pontos meghatározása és tesztelése.
IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #128, 2026-10-30

A [#128 PR](https://github.com/RobCZart82/yaman1x/pull/128) beolvadt:
merge `efbf68fad922e7270fc60105a1e6947f972cee6a`,tesztelt head `00bff586f85f7a2b6d70c7648c57c91486eeddec`.
Időpont:2026-10-30 04:05:45 Europe/Budapest (03:05:45 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993876811), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993876814), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40993877026):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**81 CTest/87 Python**. Port9 FFFFD0/FFFFD2,alap00/maszk C0;
262144 bit-orákulumos eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>DA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:hiányos maszk199031,hibás alap-DDR130 elvárt kudarc.
GPIO4/6/7/8/9/SCI ASan/UBSan PASS;517 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio9.md).
Nincs aktív motorjelölt. Következő:portA DDR/adat,alap00/maszk00;utána portB,
majd a GPIO-szóhozzáférés pontos meghatározása és tesztelése.
IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #126, 2026-10-30

A [#126 PR](https://github.com/RobCZart82/yaman1x/pull/126) beolvadt:
merge `a8b0ca4c87478aa188b902ef997b7c88c284bbf8`,tesztelt head `32dcba307e72f2acfcffd494c890afbc2894c57f`.
Időpont:2026-10-30 03:25:44 Europe/Budapest (02:25:44 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990014063), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990014152), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40990013951):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice kapacitási tájékoztatás.

Beolvadt:**80 CTest/87 Python**. Port7 FFFFCE csak bemenet,DDR00/maszk00;
256 közvetlen érték,256 CPU-érték/flag és161 időbeli szétvágás PASS.
Port4/6/8/machine izoláció,portazonosító/időbélyeg,readonly írás,szóhozzáférési határ,
hiányzó callback és reentry ellenőrizve. Eredeti MAME:200000 olvasás,0 eltérés.
Rossz porthoz irányító próbahiba769 elvárt kudarc. GPIO4/6/7/8/SCI ASan/UBSan PASS.
515 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
[Forrásazonosság és korlátok](an1x-gpio7.md). Nincs aktív motorjelölt.
Következő:port9 DDR/adat,alap00/maszk C0;majd a többi GPIO-csoport.
Port7+8 szóolvasás,IRQ/SYSCR,teljes gépreset,firmware/MIDI/PKS/DSP/hang/REAPER nyitott.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #124, 2026-10-30

A [#124 PR](https://github.com/RobCZart82/yaman1x/pull/124) beolvadt:
merge `73a5b5cac78617216901f9db19736902ccd1d0a1`,tesztelt head `33a7a85fc92252d949a7f3867e2d80bbe5b165a8`.
Időpont:2026-10-30 02:45:43 Europe/Budapest (01:45:43 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127930), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127955), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40986127919):mind az öt feladat sikeres.
[Automatizált kódreview](https://github.com/RobCZart82/yaman1x/pull/124#issuecomment-6512683713):completed,nincs finding az ellenőrzött headen
(2026-10-30T01:31:01.357600Z). Nyitott review-szál nincs;security review-eredmény nem érkezett.
A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**79 CTest/87 Python**. Port8 alap-DDR F0/maszk E0,FFFFCD/FFFFCF;
262144 bit-orákulumos eset,256 alapállapot-bemenet,241 CPU-partíció PASS.
CPU:0F/5A/15 =>FA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Próbahibák:hiányos maszk268791,hibás alap-DDR130 elvárt kudarc.
GPIO4/6/8/SCI ASan/UBSan PASS;513 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio8.md).
Nincs aktív motorjelölt. Következő:csak olvasható port7 FFFFCE,bemeneti/izolációs/CPU
ellenőrzésekkel;majd a többi GPIO-csoport. IRQ/SYSCR és teljes gépreset nyitott.
Firmware/MIDI/PKS/DSP/hang/REAPER-elfogadás nincs kész.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #122, 2026-10-30

A [#122 PR](https://github.com/RobCZart82/yaman1x/pull/122) beolvadt:
merge `804c8a5100ac281945f643c4c9f8f969f263da14`,tesztelt head `32b36079e0a37f420193b410320f7705d728e29e`.
Időpont:2026-10-30 02:05:46 Europe/Budapest (01:05:46 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044271), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044353), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40982044283):mind az öt feladat sikeres.
[Automatizált kódreview](https://github.com/RobCZart82/yaman1x/pull/122#issuecomment-6512135987):az ellenőrzött headen completed,
nincs jelzett vagy nyitott finding (2026-10-30T00:50:58.978944Z).
Nyitott review-szál nincs;security review-eredmény nem érkezett.
A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**78 CTest/87 Python**. Port6 DDR80/maszk80,FFFFC9/FFFFCB,
262144 független maszkolt eset,241 CPU-partíció és kimenetváltozás-elnyomás PASS.
CPU:0F/5A/A5 =>AA NVRAM,CCR89. Eredeti MAME:200000 művelet,0 eltérés.
Maszkot eltávolító próbahiba:197637 elvárt kudarc;követett kód változatlan.
GPIO4/GPIO6/SCI ASan/UBSan PASS;511 fájl forrásauditja és229 licencjelölés PASS
(nem jogi engedély). [Forrásazonosság és korlátok](an1x-gpio6.md).
Nincs aktív motorjelölt. Következő:port8 alap-DDR F0/maszk E0,majd többi GPIO;
IRQ/SYSCR,alternatív pinfunkció és teljes gépreset továbbra is nyitott.
Firmware/MIDI/PKS/DSP/hang/REAPER-elfogadás nincs kész.
Soros munka:új motorlépés csak a lezáró dokumentáció beolvadása után indul.

## Történeti lezárás — #120, 2026-10-30

A [#120 PR](https://github.com/RobCZart82/yaman1x/pull/120) beolvadt:
merge `41f8b6db0d55743495b4953c0ba746dfc4114446`,tesztelt head `a96bfc40bc518f1315aa3c828c0da1548abbd6aa`.
Időpont:2026-10-30 01:25:51 Europe/Budapest (00:25:51 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977946004), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977945988), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40977945935):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice csak kapacitási tájékoztatás.

Beolvadt:**77 CTest/87 Python**. Port4 DDR/pins/PCR,buszidőbélyeges callbackek,
262144 bit-orákulumos eset,256 PCR-érték és241 CPU-partíció PASS.
CPU:F0 DDR,A5 latch,3C külső szint =>AC NVRAM-bájt,CCR89.
A kezdeti flag-orákulum N-bitjét javítottuk;CPU-kód nem változott.
Eredeti rögzített MAME-portmódszerek:200000 vegyes művelet,0 eltérés.
Próbahibák:DDR-visszaolvasás65281,pin helyett latch497267 elvárt kudarc.
GPIO4+SCI ASan/UBSan PASS;509 fájl forrásauditja és229 licencjelölés PASS (nem jogi engedély).
[Forrásazonosság és korlátok](an1x-gpio4.md). Nincs aktív motorjelölt.
Következő:port6 DDR/adat regiszterek rögzített alapértékkel/fenntartott bitekkel,
majd a többi portcsoport önálló tesztelt lépcsőkben. IRQ/SYSCR és alternatív pinfunkció nyitott.
Teljes gépreset,LCD/DSP/MEG,firmware/MIDI/PKS/hang/REAPER-elfogadás továbbra is nyitott.
Soros munka:a lezáró dokumentáció beolvadása után indulhat új motorfejlesztés.

## Történeti lezárás — #118, 2026-10-30

A [#118 PR](https://github.com/RobCZart82/yaman1x/pull/118) beolvadt:
merge `6f9e8b8f66d962e968af8d6f9cd4135d2c4d1ab3`,tesztelt head `85baf0a93896468e35f019845d9f9116763ecb29`.
Időpont:2026-10-30 00:46:23 Europe/Budapest (2026-10-29 23:46:23 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239180), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239137), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40973239160):mind az öt feladat sikeres.
Nyitott review-szál és rögzített automatikus review-eredmény nincs;
sikeres automatikus review-t nem állítunk. A két macOS notice csak futtatókapacitási tájékoztatás.

Beolvadt:**76 CTest/87 Python**. ROM/RAM,SCI0/SCI1,IRQ és másolt RX-idősor közös
CPU-buszidővel;512 TX+512 RX keret,3204 szétvágás,belsőRAM-verem,NVRAM-ISR és SLEEP/RXI
ellenőrzött. Célzott ASan/UBSan PASS;határidő-próbahiba2738 elvárt kudarc.
Reentry belső kivételének elnyelése sem enged folytatást. [Hatókör](an1x-sci-bus.md).
Nincs aktív motorjelölt. Következő:GPIO/IRQ-vezérlő regisztertérkép és a rögzített CPU-profil
hiányainak ellenőrzése,majd egy kis regisztercsoport bekötése független tesztekkel.
Teljes gépreset,LCD/DSP/MEG,firmware/MIDI/PKS/hang/REAPER-elfogadás továbbra is nyitott.
Soros munkamenet:a lezáró dokumentáció beolvadása után indulhat új motorfejlesztés.

## Történeti lezárás — #116, 2026-10-29

A [#116 PR](https://github.com/RobCZart82/yaman1x/pull/116) beolvadt:
merge `7fe1d4bcb602ce96d40f34ec4ccd888430bf70e0`,tesztelt head `1982509b2c4632b7e9198d3ae23656f59e2c2a55`.
Időpont:2026-10-29 23:25:39 Europe/Budapest (22:25:39 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649310), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649254), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649115):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **75 CTest-csoport /87 Python-eset**;CPU-kezelők változatlanul562
(558 változatlan,4 korábbi adaptáció),264 kiválasztott/0 hiányzó/135 kizárt sor.
1MiB saját ROM-másolat,külső256KiB NVRAM és512 bájt belső RAM,24 bites címtükrözés.
Teljes ROM/RAM-söprés,határok/izoláció/NVRAM-roundtrip és241 CPU-partíció PASS.
Helyi Release75/75 CTest,87 Python és célzott ASan/UBSan PASS.
[Bizonyíték és kutatási szabályok](an1x-memory-map.md).
Nincs aktív motorjelölt. MMIO és ROM-írás fail-closed,nem fizikai open-bus modell.
Következő:SCI MMIO és IRQ közös címtérbe,CPU-buszidőbélyeg/periféria-határidők,
reset/izoláció és szétvágástól független RX/TX,SLEEP-ébresztés.
Teljes gép,GPIO/LCD/DSP/MEG,időzítés,firmware/MIDI/PKS/hang/plugin elfogadás nyitott.
Soros fejlesztés:egyszerre egy PR;az alábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #114, 2026-10-29

A [#114 PR](https://github.com/RobCZart82/yaman1x/pull/114) beolvadt:
merge `3172c55e9a1cea766e5326bfd56a98f68d6420f3`,tesztelt head `532c5937cfc2b0b3f142e935cebba8dfa4912000`.
Időpont:2026-10-29 22:45:44 Europe/Budapest (21:45:44 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229492), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229514), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229555):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **74 CTest-csoport /87 Python-eset;562 upstreamből származó kezelő
(558 változatlan,4 korábbi adaptáció);264 kiválasztott /0 hiányzó /135 kizárt sor**.
A leltározott H8H-kezelők kiválasztása teljes;ez nem teljes CPU/gép-elfogadás.
SLEEP és IRQ/NMI-ébresztés rögzített profillal,standby kizárva. Helyi Release74/74
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
ébresztést tiltó kontroll seed13/slice7-nél felismerve. 512 maszkkombináció és644
szétvágás,periféria-előrehaladás és határidő nélküli alvás ellenőrizve.
[Bizonyíték és korlátok](h8-sleep-wake.md).
Nincs aktív motorjelölt. Következő:AN1x címtér/MMIO és CPU-periféria közös gépütemező,
reset/izoláció/folytathatóság,utána eredeti helyi ROM indítási és MIDI/PKS megfigyelés.
M0 részleges;dinamikus SYSCR,standby,DMA/DTC,trace/retry,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nincs lezárva. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #112, 2026-10-29

A [#112 PR](https://github.com/RobCZart82/yaman1x/pull/112) beolvadt:
merge `b14cd16e8d4d149cdb622371ac29928904bbc295`,tesztelt head `41c5c34be49913dea5dc025f589cfde068a27736`.
Időpont:2026-10-29 22:05:04 Europe/Budapest (21:05:04 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371785), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371726), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371748):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **73 CTest-csoport /87 Python-eset;560 upstreamből származó kezelő
(556 változatlan,4 korábbi adaptáció);263 kiválasztott /1 hiányzó /135 kizárt sor**.
EEPMOV.B/W:bájtonkénti másolás8/16 bites számlálóval,két prefix,nyolc változatlan kezelő.
Helyi Release73/73 CTest,87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás forrásléptetés kontrollja
seed15/slice4-nél felismerve. Max65535 darab,átfedés,címkörbefordulás,362 szétvágás,
131072 prefixőr és12 memóriahiba ellenőrizve. [Bizonyíték](h8-eepmov-block.md).
Nincs aktív motorjelölt. Következő:SLEEP megállás/ébresztés és periféria-előrehaladás
host/ütemező-szerződésével. M0 részleges;teljes CPU/gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #110, 2026-10-29

A [#110 PR](https://github.com/RobCZart82/yaman1x/pull/110) beolvadt:
merge `3d0ce825bb341402f40dab92c0f1ec2f409b5ee7`,tesztelt head `903844cdb2a0466a93b2a61b056191afd0b37146`.
Időpont:2026-10-29 21:25:50 Europe/Budapest (20:25:50 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842276), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842076), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943841959):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **72 CTest-csoport /87 Python-eset;552 upstreamből származó kezelő
(548 változatlan,4 korábbi adaptáció);261 kiválasztott /3 hiányzó /135 kizárt sor**.
MOVFPE/MOVTPE:két művelet,négy változatlan kezelő. Az eredeti `TODO: timing`
megmarad:periféria/E-clock kézfogás és hardveridőzítés nincs implementálva/igazolva.
Helyi végső Release72/72 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás cím-előjelkiterjesztés
kontrollja seed15/slice1-nél felismerve. A túl tág dekódolási maszkot és az
elsőutasítás-leltárt javítottuk,teljes újrafordítás/regresszió után PASS.
[Bizonyíték és korlátok](h8-peripheral-byte-moves.md).
Nincs aktív motorjelölt. Következő:EEPMOV blokkmozgatás,majd SLEEP a szükséges
host/ütemező-szerződéssel. M0 részleges;teljes gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #108, 2026-10-29

A [#108 PR](https://github.com/RobCZart82/yaman1x/pull/108) beolvadt:
merge `3c7005a3a26da4149668f6ab9f4695e1672bdc6e`,tesztelt head `ffef22c9d5794ec78028a0f2d07cb9659ff9d5bb`.
Időpont:2026-10-29 20:25:40 Europe/Budapest (19:25:40 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469453), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469399), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469419):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **71 CTest-csoport /87 Python-eset;548 upstreamből származó kezelő
(544 változatlan,4 korábbi adaptáció);259 kiválasztott /5 hiányzó /135 kizárt sor**.
CCR displacement32:két művelet,két eredeti prefixfolytatás,nyolc változatlan kezelő.
Helyi Release71/71 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;felső eltolásszó elhagyásának
kontrollja seed0/slice1-nél felismerve. [Bizonyíték](h8-ccr-displacement32-memory.md).
Nincs aktív motorjelölt. Következő lépés:MOVFPE/MOVTPE perifériás bájtmozgatások,
majd EEPMOV és SLEEP a szükséges host/ütemező-szerződések tisztázásával.
M0 részleges;teljes gép/MMIO,hardveridőzítés,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #106, 2026-10-29

A [#106 PR](https://github.com/RobCZart82/yaman1x/pull/106) beolvadt:
merge `aee769ef224e2686ccd4a98bbd9d3851f8cc6399`, tesztelt head `e3af0b4453d19194da8d32b24e6d409d4ab8809f`.
Időpont:2026-10-29 19:21:59 Europe/Budapest (18:21:59 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577461), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577203), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577221): mind az öt feladat sikeres.
A jelenlegi GitHub-állapotban nincs rögzített automatikus review-eredmény vagy
review-szál; sikeres automatikus review-t nem állítunk.

Beolvadt: **70 CTest-csoport /87 Python-eset;540 upstreamből származó kezelő
(536 változatlan,4 korábbi adaptáció);257 kiválasztott /7 hiányzó /135 kizárt sor**.
CCR signed16 eltolás:két művelet,négy változatlan kezelő. Helyi Release70/70
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés;hibás előjel-kiterjesztés kontrollja seed1/slice2-nél felismerve.
[Hatókör és bizonyíték](h8-ccr-displacement16-memory.md).
Nincs aktív motorjelölt. Következő lépés:CCR displacement32 címzések.
M0 részleges;teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #104, 2026-10-29

A [#104 PR](https://github.com/RobCZart82/yaman1x/pull/104) beolvadt:
merge `92e52f45307dc1adad37347febd407d0d2106182`,végső tesztelt head `570d7b11d5dde819ac12211671f39fd47163f81b`.
Időpont:2026-10-29 18:30:16 Europe/Budapest (17:30:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264372),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264370),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264281):mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **69 CTest-csoport /87 Python-eset;536 upstreamből származó kezelő
(532 változatlan,4 korábbi adaptáció);255 kiválasztott /9 hiányzó /135 kizárt sor**.
CCR postincrement/predecrement,két művelet,négy változatlan eredeti kezelő.
524288 memóriaszó,24576 határeset,201 szétvágás,512 IRQ/NMI-maszk,
322 megszakításos szétvágás és16 memóriahiba PASS. A host-visszahívás a memóriaelérés
előtt megfigyeli a módosított mutatót és régi CCR-t. Helyi Release69/69 CTest,
87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
a hibás négybájtos növelés kontrollja seed2/slice3-nál felismerve.
[Hatókör és bizonyíték](h8-ccr-autoindex-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR signed16/displacement32 címzések,
majd a fennmaradó H8H-műveletek. M0 részleges;teljes gép/MMIO,firmware/MIDI/
hang/plugin elfogadás nyitott. Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #103 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #102, 2026-10-29

A [#102 PR](https://github.com/RobCZart82/yaman1x/pull/102) beolvadt:
merge `2dab39c1598533fc0a84da9017b3f2975854ff96`, végső tesztelt head `3c0f7947efc5cc170877fa99479f181d527bd195`.
Időpont:2026-10-29 17:50:13 Europe/Budapest (16:50:13 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848068),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848027),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848153): mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **68 CTest-csoport /87 Python-eset;532 upstreamből származó kezelő
(528 változatlan,4 korábbi adaptáció);253 kiválasztott /11 hiányzó /135 kizárt sor**.
Négy CCR abs16/abs32 betöltési/tárolási művelet,nyolc változatlan eredeti kezelő.
131072 memóriaszó,131072 abs16 cím,524288 abs32 címfél-söprés,9216 CCR-határeset,
241 szétvágás,1024 IRQ/NMI-maszk és644 megszakításos szétvágás PASS.
Helyi Release68/68 CTest és87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás alsóbájt-kiválasztás
kontrollja seed3/slice5-nél felismerve. [Hatókör és bizonyíték](h8-ccr-absolute-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR postincrement/predecrement,
majd eltolásos címzések és a fennmaradó H8H-műveletek. M0 részleges;
teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #101 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #100, 2026-10-29

A [#100 PR](https://github.com/RobCZart82/yaman1x/pull/100) beolvadt:
merge `1de8971b8fdd7694a8c3051ae86611a9ff6cd3cd`, végső tesztelt head `ba6db1c61a3ba9d696f8affee1410e5969c0aa9d`.
Időpont:2026-10-29 17:10:15 Europe/Budapest (16:10:15 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424971),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424908),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424743): mind az öt feladat sikeres.
Automatikus review a végső commiton befejeződött, hibamegállapítás és nyitott review-szál nélkül.

Beolvadt: **67 CTest-csoport /87 Python-eset;524 upstreamből származó kezelő
(520 változatlan,4 korábbi adaptáció);249 kiválasztott /15 hiányzó /135 kizárt sor**.
Két MOV.L művelet, eredeti kettős prefix és öt szavas utasításpuffer.
524288 eltolásfél-söprés,55296 határeset,241 szétvágás,65536 prefixfolytatás,
regiszteraliasok és első/második memóriaátviteli hibák PASS. Helyi Release67/67
CTest és87 Python, célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés; a felső eltolásszó elhagyásának hibás kontrollja seed4/slice4-nél felismerve.
[Hatókör és bizonyíték](h8-long-displacement32-memory.md).

Nincs aktív motorjelölt. Következő lépés: a fennmaradó CCR-címzések és további
hiányzó H8H-kódolások, majd teljes gép/MMIO-integráció. M0 részleges;
firmware/MIDI/hang/plugin elfogadás nyitott. Egyszerre egy PR; új fejlesztés csak beolvadás után.
A #99 dokumentációs PR beolvadását a strukturált nyilvántartás már tartalmazza.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #98, 2026-10-29

A [#98 PR](https://github.com/RobCZart82/yaman1x/pull/98) beolvadt:
merge `1c7ba628aceb29fd4c057db73fe31b2be4841558`, tesztelt head `a3c033256ec247555b44b0d820bb228dcaf9bc39`.
Időpont: 2026-10-29 13:36:16 Europe/Budapest (12:36:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870231488),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229509),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229586): mind az öt feladat sikeres.
Automatikus review szolgáltatási kvóta miatt nem állt rendelkezésre; sikeres review-t nem állítunk.

Beolvadt: **66 CTest-csoport /87 Python-eset;518 upstreamből származó kezelő
(514 változatlan,4 korábbi adaptáció);247 kiválasztott /17 hiányzó /135 kizárt kódolássor**.
524288 memóriaszó,24576 cím/CCR-eset,65536 prefixfolytatás,201 szétvágás,
512 IRQ/NMI-maszk és322 megszakításos szétvágás; eredeti referencia200000 szelet,
nulla eltérés. A hibás alsóbájt-kiválasztást a megerősített referencia felismerte.
A helyi korábbi Windows-futás65 futtatható CTest/86 Python-esetet ellenőrzött;
az ismert symlink-jogosultsági eset kizárását nem tekintjük sikeres tesztnek.
[Pontos bizonyíték és korlátok](h8-ccr-indirect-memory.md). M0 részleges; nincs aktív
motorjelölt. Teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.

A fejlesztés soros: egyszerre egy PR; következő fejlesztés csak az aktuális PR
beolvadása után. Következő címzési lépés a hosszúszavas32 bites eltolás,
majd a fennmaradó CCR-címzések és a leltár további hiányzó műveletei.

## Történeti lezárás — #96, 2026-10-29

Ellenőrzött motoralap: [#96](https://github.com/RobCZart82/yaman1x/pull/96),
merge `8b1c37eb569202e394ff1caeb3f64618bae78a69`, végső tesztelt head `fd5d1293a21cf63c4b39293b30ca7e650702efe4`.
Beolvadt: 2026-10-29 11:49:02 Europe/Budapest (10:49:02 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583149),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583161),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583141).
Linux UBSan:32 bites eltolásos bájt/szó-mozgatás és korábbi számtan PASS; eredeti
unsigned-túlcsordulási kontroll PASS. Automatikus kódellenőrzés kvótakorlát miatt
nem fejeződött be; sikeres review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **65 CTest-csoport /87 Python-eset;512 upstreamből származó kezelő
(508 változatlan,4 korábbi adaptáció);245 kiválasztott /19 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Négy MOV-csoport:1048576 eltolásfél-söprés (nem teljes2^32
keresztszorzat),55296 cím/regiszter/CCR-határeset,241 szétvágás,65536 prefixfolytatás,
memóriahibás fail-stop és újraindítás-tiltás PASS. Eredeti referencia:200000 szelet,
nulla eltérés; felső eltolásszó elhagyása kontroll seed6/slice5 felismerve. Helyi64
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
[Pontos hatókör és bizonyíték](h8-displacement32-memory.md). Hosszúszavas32 bites
eltolás külön nyitott. Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/
hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #94, 2026-10-29

Ellenőrzött motoralap: [#94](https://github.com/RobCZart82/yaman1x/pull/94),
merge `442949856a92392986f3bbc5927f36b7884a505b`, végső tesztelt head `9f3cda7fcbe545ee2b0da9cebc23d8dd5ab6b42a`.
Beolvadt: 2026-10-29 10:48:25 Europe/Budapest (09:48:25 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898827),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898826),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898831).
Linux UBSan: szélső előjeles szó-osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a beolvasztáskor még futott;
lezárt review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **64 CTest-csoport /87 Python-eset;502 upstreamből származó kezelő
(498 változatlan,4 korábbi adaptáció);241 kiválasztott /23 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.W:524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,
1024 szélső/CCR-eset,321 szétvágás,128 tiltott ER-szomszéd PASS. Eredeti referencia:
200000 szelet,nulla eltérés; fordított N kontroll seed7/slice7 felismerve. Helyi63
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
INT_MIN/-1: a matematikai +2147483648 hányadost az eredeti32 bites intre szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres mérés.
A forrás szintű szélsőeset-vizsgálat lezárult;[pontos hatókör](h8-signed-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending és korábbi nyitott szélsőeset-bejegyzések történetiek.

## Történeti lezárás — #92, 2026-10-29

Ellenőrzött motoralap: [#92](https://github.com/RobCZart82/yaman1x/pull/92),
merge `1adeb1030871083bb510f4e691da4467cd96a816`, végső tesztelt head `b719cf25b0d0d8b929c5b639d5971ddc424a1a49`.
Beolvadt: 2026-10-29 10:18:06 Europe/Budapest (09:18:06 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532370),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532267),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532335).
Linux UBSan: előjeles bájtos osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a végső headen megállapítás nélkül lezárult.

Beolvadt: **63 CTest-csoport /87 Python-eset;500 upstreamből származó kezelő
(496 változatlan,4 korábbi adaptáció);240 kiválasztott /24 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.B:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás,65536 prefixfolytatás PASS. Eredeti referencia:
200000 szelet,nulla eltérés; hányados+1 kontroll seed7/slice3 felismerve. Helyi62
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
Előjeles szó-osztás INT32_MIN/-1 és csomagolás továbbra is nyitott vizsgálat;
[pontos hatókör és bizonyítékok](h8-signed-byte-divide.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #90, 2026-10-29

Ellenőrzött motoralap: [#90](https://github.com/RobCZart82/yaman1x/pull/90),
merge `a4f52f600d2cfd85f14a5e8822eb5e025d28825d`, végső tesztelt head `f24ca47f3511764dfec4971e84c3c1487aaed443`.
Beolvadt: 2026-10-29 09:38:37 Europe/Budapest (08:38:37 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792043),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792142),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792129).
Linux UBSan: DAA/DAS és korábbi számtan PASS; eredeti unsigned-túlcsordulási kontroll
PASS. Automatikus kódellenőrzés a végső headen lezárult, megállapítás nélkül.

Beolvadt: **62 CTest-csoport /87 Python-eset;496 upstreamből származó kezelő
(492 változatlan,4 korábbi adaptáció);239 kiválasztott /25 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Decimális oracle:40000 érvényes számpár/CCR-eset,2048 regiszter-
határeset,241 szétvágás PASS;200000 eredeti referenciaszelet nulla eltéréssel.
DAA korrekció+1 kontroll seed7/slice0 felismerve. Helyi61 futtatható CTest/86 Python
PASS, ismert Windows-symlink eset kizárva. Érvénytelen BCD/flag kombinációkra nincs
állított teljes független helyességbizonyítás. [Hatókör](h8-decimal-adjust.md).
Nincs aktív motorjelölt. Ez M0-részlépés; M0 továbbra is részleges, teljes gép,
firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #88, 2026-10-29

Ellenőrzött motoralap: [#88](https://github.com/RobCZart82/yaman1x/pull/88),
merge `3c5f7dddfdf54532670d9cdddd0ade0aab7c92bd`, végső tesztelt head `fc0db2778270a5ffdc612ab46ccd7be0b0d9b4fe`.
Beolvadt: 2026-10-29 08:55:24 Europe/Budapest (07:55:24 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738076),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738102),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738069).
Linux UBSan: bájt/szó-osztás és korábbi szorzások PASS; eredeti unsigned-túlcsordulási
kontroll PASS. Beolvadt: **61 CTest-csoport /87 Python-eset;492 upstreamből származó
kezelő (488 változatlan,4 dokumentált adaptáció);237 kiválasztott /27 hiányzó /135 kizárt
kódolássor** (nem CPU-százalék). DIVXU.W:524288 osztó-söprés,30720 alias/CCR-eset,
20000 véletlen pár,241 szétvágás PASS. Nagy értékek független64 bites számítással;
200000 korlátozott eredeti referenciaszelet nulla eltéréssel; hányados+1 kontroll
seed9/slice26 felismerve. Helyi60 futtatható CTest/86 Python PASS, ismert Windows-symlink
eset kizárva. A q/r u32 típusmódosítás dokumentált; az eredeti osztóbit7 N-szabály
megőrzött, hardveresen nem igazolt. [Pontos hatókör](h8-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #86, 2026-10-29

Ellenőrzött motoralap: [#86](https://github.com/RobCZart82/yaman1x/pull/86),
merge `4dd788dadae4b09a21b4611f245640a63c56dcf5`, végső tesztelt head `189eeea02372839e8cf27f02853b990ed0625652`.
Beolvadt: 2026-10-29 08:26:31 Europe/Budapest (07:26:31 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892241),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892230),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892231).
Linux UBSan: bájtos unsigned osztás és korábbi szorzások PASS; az eredeti unsigned
int-promóciós hibát megkövetelő kontroll is PASS.

Beolvadt: **60 CTest-csoport /87 Python-eset;490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott /28 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. DIVXU.B:393216 osztandó-söprés,
2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS.
Eredeti referencia:200000 szelet,nulla eltérés; hányados+1 kontroll seed9/slice18
felismerve. Helyi59 futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
A nullás osztó, hányadoslevágás és osztófüggő N/Z az eredeti forrással egyezik,
nem külön hardvermérés. Nincs aktív motorjelölt. [Hatókör és bizonyíték](h8-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #84, 2026-10-29

Ellenőrzött motoralap: [#84](https://github.com/RobCZart82/yaman1x/pull/84),
merge `2540601aa985513a39d293ffa14fd8e0b76689da`, végső tesztelt head `7aee43a86ea21711254fd3e840728534e01b6a96`.
Beolvadt: 2026-10-29 07:45:33 Europe/Budapest (06:45:33 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383282),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383154),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383285).
Linux UBSan: előjeles bájt/szó és unsigned szó-szorzás PASS; az eredeti unsigned
int-promóciós hibát megkövetelő negatív kontroll is PASS.

Beolvadt: **59 CTest-csoport /87 Python-eset;488 upstreamből származó kezelő
(486 változatlan,2 korábbi adaptáció);235 kiválasztott /29 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. Előjeles szorzás:65536 bájtpár,
327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár,241 szétvágás és
65536 prefixfolytatás PASS. Eredeti referencia:200000 szelet,nulla eltérés;
product+1 kontroll seed9/slice14 felismerve. Helyi58 futtatható CTest/86 Python PASS;
egy ismert Windows-symlink eset kizárva. Nincs aktív motorjelölt.
[Szerződés és pontos bizonyítékok](h8-signed-multiply.md). M0 részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending
bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #82, 2026-10-29

Ellenőrzött motoralap: [#82](https://github.com/RobCZart82/yaman1x/pull/82),
merge `80b237ee7a9ad6b286a13c9253d076aa42bb0214`, végső tesztelt head `4f2cffc88c843cc9d431f5c0a6ec20ee2869d85b`.
Beolvadt: 2026-10-29 07:20:23 Europe/Budapest (06:20:23 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962559),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962568),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962569).
A Linux-lépésben a nagy szorzatok UBSan-tesztje és az eredeti kifejezés
előjeles túlcsordulását megkövetelő külön negatív kontroll is PASS.

Beolvadt: **58 CTest-csoport /87 Python-eset;482 upstreamből származó kezelő
(480 változatlan,2 dokumentáltan adaptált);233 kiválasztott /31 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. MULXU.W:327680 operandus-söprés,
9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS;200000 biztonságos
szorzatokkal végzett eredeti referenciaszelet nulla eltéréssel. A nagy szorzatokat
független64 bites számítás ellenőrzi; product+1 kontroll seed9/slice9 felismerve.
Helyi57 futtatható CTest és86 Python PASS, egy ismert Windows-symlink eset kizárva.
A szó-szorzás C++-promóciós kockázata rendezett a dokumentált unsigned-szélesítéssel;
[pontos hatókör és bizonyíték](h8-word-multiply.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/CI-pending/kockázati bejegyzések történetiek.

## Történeti lezárás — #80, 2026-10-29

Ellenőrzött motoralap: [#80](https://github.com/RobCZart82/yaman1x/pull/80),
merge `bddffa8743eac2e2af8e7fc1a3145046a45e6fa5`, végső tesztelt head `5bf4e97e64fc3ebbe00a42f8215a740fbca72d86`.
Beolvadt: 2026-10-29 06:41:58 Europe/Budapest (05:41:58 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422195),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422322),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422266).

Beolvadt: **57 CTest-csoport / 87 Python-eset;480 eredeti kezelő;
232 kiválasztott /32 hiányzó /135 kizárt profilú kódolássor** (nem CPU-százalék).
MULXU.B:65536 operanduspár,19200 alias/CCR-határeset,161 szétvágott futás PASS;
200000 referenciaszelet nulla eltéréssel; product+1 negatív kontroll felismerve.
Helyi56 futtatható CTest és86 Python PASS; egy ismert Windows-symlink eset kizárva.
Nincs aktív motorjelölt. A szó-szorzás int-promóciós kockázata külön nyitott;
[bekötési feltételek és bizonyítékok](h8-byte-multiply.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás továbbra is nyitott.
Az alábbi jelölt/CI-pending bejegyzések történeti előkészítési állapotok.

## Történeti lezárás — #78, 2026-10-29

Ellenőrzött motoralap: [#78](https://github.com/RobCZart82/yaman1x/pull/78),
merge `d4ef0e45bc416f06216d43559e1f73df1d39caa8`, végső tesztelt head
`ef6adefadb5b390f6822ae34c4db41edab5b1348`. Beolvadt: 2026-10-29 06:26:01 Europe/Budapest
(2026-10-29 05:26:01 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703637),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703598),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703615).

Beolvadt csomag: **56 CTest-csoport, 87 Python-eset; 478 eredeti kezelő;
231 kiválasztott / 33 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
Hat előjeles 16 bites eltolásos memóriautasítás-csoport készült el. Független
referencia: 200000 szelet, nulla eltérés; a szándékos előjelhiba felismerve.
Helyi Windows: 55 futtatható CTest és 86 Python PASS; egy ismert symlink-jogosultsági
eset kizárva. A CI a teljes csomagot ellenőrizte. M0 részleges; teljes AN1x-gép,
firmware-es MIDI, hang és plugin elfogadása nyitott. Nincs aktív motorjelölt.
A korábbi jelölt/CI-pending bejegyzések történetiek; lezárásuk itt és a JSON-naplóban szerepel.

## Történeti lezárás — #77, 2026-10-29

Ellenőrzött motoralap: [#77](https://github.com/RobCZart82/yaman1x/pull/77),
merge `8993a6024b0cedfe0d3f5c48a2819009c98643f1`, végső tesztelt head
`fb2bc0935aad976706b0652d21275e5a55d0efd8`. Beolvadt: 2026-10-29 00:49:52 Europe/Budapest
(2026-10-28 23:49:52 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788596091),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595788),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595917).

Beolvadt csomag: **55 CTest-csoport, 87 Python-eset; 466 eredeti kezelő;
225 kiválasztott / 39 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
A fix profilú IRQ/NMI/TRAPA és a szintetikus CPU–SCI RX/TX/hibakezelés elkészült;
a teljes AN1x-gép, firmware-es MIDI, hang és plugin elfogadása nyitott. M0 részleges.
Az alábbi korábbi jelölt/CI-pending bejegyzések történetiek. Az új eltolásos
memóriautasítások külön jelöltet alkotnak, nem írják felül ezt a lezárt motoralapot.

## Történeti lezárás — #57, 2026-10-28

Ellenőrzött motoralap: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), merge
`f8b6aa20834f209931e3e9bb9463cc47b5b7a540`. Végső tesztelt head:
`bf3e7158a9a4dac6edb36ea4deb659bdf3c54cb1`.
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365642),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365647),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365644).
A beolvadt csomag **39 CTest-csoportot és 84 Python-esetet** tartalmaz.
Helyi Windows-ellenőrzés: 38 futtatható CTest-csoport és 83 Python-eset PASS;
egy symlink-jogosultsági eset kizárva (WinError 1314). A CI a teljes csomagot futtatta.

A normál, rögzített H8H-profil verem-/vezérlésátadási családjai elkészültek;
366 eredeti full/partial kezelő szerepel a részhalmazban. Az RTE szintetikus
veremkép-visszaállítása tesztelt, az IRQ/TRAPA-belépés még nincs bekötve.
M0 részben teljesült; M1 nincs elfogadva; M2–M6 nyitott. Nincs igazolt AN1x-hang
vagy REAPER-plugin. A következő motorfeladatok: hiányzó H8H-utasítások/címzések
leltára, IRQ/TRAPA-belépés és szűrők, majd közös CPU/periféria-ütemezés és gépintegráció.

A korábbi „aktív”, „jelölt”, „CI pending” és „következő” bejegyzések az akkori
állapotot őrzik; a jelenlegi készültségre a fenti #116 lezárás és az aktuális mérföldkő-tábla
irányadó. A dokumentáció mostani javítása nem új motorfejlesztés vagy elfogadás.

## Aktuális mérföldkő-állapot — 2026-10-29

| Mérföldkő | Aktuális állapot | Következő elfogadási feltétel |
|---|---|---|
| M0 | **Részben teljesült.** Architektúra és forrás-/licencnyilvántartás; tesztelt GPIO-, IRQ- és SCI-komponensek; működő önálló H8H-részhalmaz 562 upstreamből származó kezelővel (558 változatlan,4 adaptált), rögzített SYSCR-es IRQ/NMI/TRAPA-belépéssel, buszidőbélyeggel és szintetikus CPU–SCI RX/TX/hibakezeléssel. | A264 leltározott H8H-kódolássor kezelője kiválasztva; a tesztelt CPU/periféria-ütemezés teljes AN1x címtér/MMIO-gépbe integrálása és gépizoláció. A részhalmaz már fordul és fut; a teljes CPU és a terjesztési licenc lezárása nyitott. |
| M1 | **Előkészítve, nem teljesült.** A külön macOS MAME-build és a párosított helyi futtató rendelkezésre áll. | Friss Mac-firmware-próba: SCI0 tényleges bájtfogadás és note-on/note-off feldolgozás bizonyítása; PKS-kézfogás feltárása. |
| M2–M3 | **Kutatási feladatok nyitva.** Nincs igazolt referenciahang. | AN1x órajel, táblacímzés, EG és soros hangút bizonyítása, majd megismételhető offline hang. |
| M4–M6 | **Tervezett.** Nincs elkészült beágyazható hangszer vagy REAPER-elfogadás. | Validált offline referencia után audio-blokkmotor, VST3 és helyi REAPER-próba. |

Az önálló komponens tesztje nem teljes CPU-/firmware-elfogadás. A jelenlegi
állapotot ez a táblázat és az M0/M1 részletes szakasza mutatja. A lezárási tábla
az ellenőrzött végső PR-revíziókat és CI-futásokat rögzíti; a korabeli napló
az akkori munkamenet állapotát őrzi.

Korábbi, #36-hoz tartozó platformeredmény: [#36](https://github.com/RobCZart82/yaman1x/pull/36),
`6f3fbf6c22b9f3bfc28f78d0fc1bad55ed98ed8c`: Linux/macOS/Windows platformonként
**22/22 tesztcsoport PASS**, MSan és teljes macOS MAME-build PASS. A konkrét
futások a lezárási táblában vannak. Helyben Windows alatt 21 futtatható csoport
PASS; a Python unittest-csoportot a korábban igazolt symlink-jogosultsági korlát
miatt kizártuk a helyi futásból. A teljes csoport CI-ben sikeres. Ez nem teljes
helyi tesztcsomag-PASS, és nem firmware-es MIDI-elfogadás.

## Ellenőrzött lezárások — #39–#44

Ellenőrizve: 2026-10-27. Mind a hat PR beolvadt; mindegyik végső headjén öt ellenőrzés PASS. Az alábbi lezárás az irányadó a korábbi, készítéskori „CI pending / review alatt” megjegyzésekkel szemben. Azok történeti pillanatképek, nem aktuális akadályok. A PR-head CI nem hardver-/firmware-/audioelfogadás.

| PR | Ellenőrzött head | Linux/macOS/Windows | MSan | MAME |
|---|---|---|---|---|
| [#39](https://github.com/RobCZart82/yaman1x/pull/39) | `9459a805af0834c5ad66329f0f1b037a9354b08d` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566377) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566528) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292565993) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549567/job/122292568462) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549032/job/122370894035) |
| [#40](https://github.com/RobCZart82/yaman1x/pull/40) | `34cec0074577a1add862772bcf3bc289de735b68` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453074) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453726) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453515) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557238091/job/122376454019) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237934/job/122376453689) |
| [#41](https://github.com/RobCZart82/yaman1x/pull/41) | `387c5e64c0ee9759b3d41dfa58bcda1b35594731` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943734) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943358) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943754) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011629/job/122407456482) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566012882/job/122408084911) |
| [#42](https://github.com/RobCZart82/yaman1x/pull/42) | `a67b599f880f0981d68e04b276b0d904ea3f7d29` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580997) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580920) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580479) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072700/job/122432444681) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072611/job/122432149653) |
| [#43](https://github.com/RobCZart82/yaman1x/pull/43) | `83caf332df3539037187888c60bc35ae422dc2a3` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457473798) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474477) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474285) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107561/job/122456981421) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107642/job/122457680585) |
| [#44](https://github.com/RobCZart82/yaman1x/pull/44) | `1bed694bdc066a66bcebb747ed74a052a4650c25` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074548) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074297) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074723) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879809/job/122480613181) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879880/job/122480584462) |

## A terv folyamatos vezetése

Ez a dokumentum a fejlesztés során folyamatosan karbantartandó terv.
Minden, a tervet érintő fejlesztési PR ugyanabban a változtatásban frissítse
az érintett mérföldkövet és az alábbi előrehaladási naplót. Rögzíteni kell:

- a dátumot, a konkrét eredményt és a kapcsolódó PR-t vagy commitot;
- az állapotot: tervezett, folyamatban, részben teljesült vagy igazoltan kész;
- a végrehajtott ellenőrzéseket és eredményüket, a platformot és a vizsgált
  revíziót; a nem futtatott és környezeti okból akadályozott próbákat külön;
- a megmaradó feltételeket, függőségeket és a következő konkrét lépést.

A mérföldkő csak a saját elfogadási feltételeinek bizonyítása után jelölhető
késznek. A ROM-mentes CI sikere nem helyettesíti a hardver-, firmware-, audio-
vagy REAPER-próbát. Tervváltozásnál az indokot és az új sorrendet is rögzíteni
kell. A történeti eredményeket megőrizzük dátumozott bizonyítékként;
az aktuális állapotot az új bejegyzés és az érintett mérföldkő együtt mutatja.

A PR készítésekor a végső CI még nem ismert. Beolvasztás után a következő
nyilvántartási frissítésben kötelező a merge SHA, végső head és tényleges CI-eredmény
lezárása; az előző jelölt nem maradhat aktuális aktív fejlesztésként. A strukturált
nyilvántartás motoralapja az utolsó ellenőrzött motorcommit, nem szükségszerűen a
legújabb dokumentációs commit; `active_change: null` azt jelenti, nincs nyilvántartott
nyitott motorjelölt. A dokumentációs PR saját CI-jét a PR lezárása rögzíti.

ROM, bank és privát trace helyett csak publikálható azonosító és összefoglaló
kerülhet a tervbe. A részletes mérési jegyzőkönyvekre hivatkozás mutasson.

Az új szerkezet vezetési szabálya: minden új fejlesztésnél az aktuális
állapottáblát és az érintett részletes mérföldkövet együtt kell frissíteni.
A munkanapló új sora kapjon dátumot, PR-hivatkozást és „Korabeli állapot” jelölést.
A végső ellenőrzések lezárásakor a PR-lezárási táblába a valóban tesztelt
PR-revízió és annak saját futásai kerüljenek; más revízió CI-je nem helyettesíti.
A korabeli mérési számokat és korlátokat megőrizzük, a már teljesült előfeltételt
pedig az aktuális feladatlistából átvezetjük az elkészült eredmények közé.

## Előrehaladási napló

### Repository-vizsgálat és központi változásjegyzék — 2026-10-27

Az `AUD-2026-10-27` vizsgálat a #37 merge állapotáról indult. Elkészült a
[repository-audit](REPOSITORY_AUDIT_2026-10-27.md), a PR-eket indexelő
[fejlesztési napló](DEVELOPMENT_LOG.md) és a hozzá tartozó strukturált leltár.
A korabeli bejegyzések és a fenti végső PR-ellenőrzési táblázat megmaradnak.
Javítás készült a bridge-auditor mai register-only fejlécének kezelésére;
ez nem változtatja a DSP-modellt vagy M0/M1 készültségét. A vizsgálat és a
vezetési szabály a naplóban szerepel, a jelölt végső ellenőrzései a saját PR-ben.

### Dokumentációs egységesítés — 2026-10-27

[#37](https://github.com/RobCZart82/yaman1x/pull/37): az M0/M1 részletes állapota,
a lezárt PR-ek bizonyítékai és a korabeli pillanatképek értelmezése egységesítve.
Az ellenőrzés élő GitHub PR-/workflow-/jobadatokra és a fenti kódbázisra épült.
A módosítás csak dokumentáció; M0/M1 készültségét és az elfogadási feltételeket
nem változtatja meg. A dokumentációs PR saját CI-je a saját checkjeinél követhető.

### Ellenőrzött PR-lezárások — legutóbbi elöl

A #16–#36 PR-ek mind **beolvadtak**. Az alábbi SHA a végső tesztelt PR-head,
nem a squash után keletkezett `main` commit; a beolvasztás a PR hivatkozásánál
ellenőrizhető. Minden felsorolt alap-CI mindhárom platformjobja sikeres.
`—`: az adott PR végső revízióján nem volt ilyen PR-triggerelt workflow-futás;
nem FAIL és nem más revízió eredményének átvétele. A korábbi hibakereső vagy
köztes reprodukciós revíziók a korabeli naplóban megmaradnak.

| Beolvadt PR / eredmény | Végső tesztelt PR-revízió | Lezárt ROM-mentes ellenőrzések |
|---|---|---|
| [#36](https://github.com/RobCZart82/yaman1x/pull/36) — Közös aszinkron SCI-csatorna | `6f3fbf6c22b9f3bfc28f78d0fc1bad55ed98ed8c` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706565); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706683); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706500) |
| [#35](https://github.com/RobCZart82/yaman1x/pull/35) — Aszinkron RX | `eeceb8282e920e163c5ff649068b90952d753671` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506274); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506107); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506093) |
| [#34](https://github.com/RobCZart82/yaman1x/pull/34) — Aszinkron TX | `da5fcd3dd9301f06e68c489c879989661a3b3428` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811757); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811799); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811994) |
| [#33](https://github.com/RobCZart82/yaman1x/pull/33) — LICENSE szövegtördelése; státusz változatlan | `22cb57c8f1a617b6a9ddca90bad6fc8962671327` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40489564815); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40489564814); MAME: — |
| [#32](https://github.com/RobCZart82/yaman1x/pull/32) — Licenclefedettség-ellenőrzés | `fc6f4ed47774ba2b1a17394255a1427e2e7249fb` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40488326442); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40488326433); MAME: — |
| [#31](https://github.com/RobCZart82/yaman1x/pull/31) — Cserélhető PKS-megvalósítás követelménye | `41ff1d08c834632417efb4c6bbdca2a5c3754c24` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484817843); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484817846); MAME: — |
| [#30](https://github.com/RobCZart82/yaman1x/pull/30) — PKS ötletfeljegyzés | `955dd73afd3760df1535de60a94503c016161925` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484348640); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484348645); MAME: — |
| [#29](https://github.com/RobCZart82/yaman1x/pull/29) — SCI SSR-latch | `b169bbe9b3e995328b75d6190f7458138c154722` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540061); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540084); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540083) |
| [#28](https://github.com/RobCZart82/yaman1x/pull/28) — SCI ciklusóra | `747cd98fac4ae300216f0bd5021dbd0cc2eae6ea` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835302); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835203); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835254) |
| [#27](https://github.com/RobCZart82/yaman1x/pull/27) — IRQ-regiszter/láb adapter | `d33a27bdef963ddc8b1385d87fa324878af51f89` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786571); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786580); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786569) |
| [#26](https://github.com/RobCZart82/yaman1x/pull/26) — Belső IRQ-választó | `d5d80e49f4664ce00f5df33d2d3408e74c2aa64a` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767512); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767565); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767326) |
| [#25](https://github.com/RobCZart82/yaman1x/pull/25) — H8 CI-bizonyíték dokumentálása | `a917c9779f64ce5afacae5fa2fc615d194c8abaa` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40469158573); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40469158626); MAME: — |
| [#24](https://github.com/RobCZart82/yaman1x/pull/24) — H8-leltár, generátor és GPIO | `896bd7882309bb03868c5c3816f4f9b7b068b858` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100477); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100462); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100442) |
| [#23](https://github.com/RobCZart82/yaman1x/pull/23) — Mac párosított próba előkészítése | `e0ad4df4884af8d0926cd68e451ebcbc280ed4d0` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40463469779); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40463469732); MAME: — |
| [#22](https://github.com/RobCZart82/yaman1x/pull/22) — VOP3 konstruktorjavítás | `d363d6a530f03aef5e6167759357874d7b4650b0` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40461780030); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40461779887); MAME: — |
| [#21](https://github.com/RobCZart82/yaman1x/pull/21) — Kutatási buildbizonyíték dokumentálása | `cfb87b521f601daff01c15befb09af88066239e4` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40459057121); MSan: —; MAME: — |
| [#20](https://github.com/RobCZart82/yaman1x/pull/20) — Rögzített kutatási forrás és build | `31664c74f2726091eb9f403c1621ef949479ddbb` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40457998062); MSan: —; [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40457998036) |
| [#19](https://github.com/RobCZart82/yaman1x/pull/19) — SCI-hozzáférés-megfigyelő | `748d8495b1e75ed43bd69d456bfd519ffca85557` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40455618012); MSan: —; MAME: — |
| [#18](https://github.com/RobCZart82/yaman1x/pull/18) — Tervvezetési szabályok | `e503321ad31b2d0a36e0ae419215034cbf82b74f` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454887762); MSan: —; MAME: — |
| [#17](https://github.com/RobCZart82/yaman1x/pull/17) — ADR és függőségalap | `bd2262fd7c06c91f7e40ea2aaa3ea52dceecedc1` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454539768); MSan: —; MAME: — |
| [#16](https://github.com/RobCZart82/yaman1x/pull/16) — Python -O / AsmJit javítások | `ac56d8c7144ef8cb478f3a714578203c440b841e` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792); MSan: —; MAME: — |

A zöld CI a forrás-/komponenstesztek és a jelzett kutatási build eredménye.
Nem igazol teljes H8-portot, friss firmware-es MIDI-vételt, hardverazonosságot,
audio- vagy REAPER-elfogadást, és nem jelent jogi/licenc- vagy bináris kiadási
jóváhagyást. A csak dokumentációs PR-ek sem új működési bizonyítékok.

### Korabeli munkanapló — PR-sorrendben

Az alábbi sorok az adott fejlesztési lépés **akkori** állapotát és akkori
következő feladatát rögzítik. A `NOT RUN`, „PR megnyitva”, „platform-CI a PR-ben”
és a még külön TX/RX modellekre utaló mondatok itt történeti állapotok.
Platformlezáráshoz a fenti táblát, jelenlegi feladathoz az aktuális M0/M1 részt
kell olvasni. A firmware-/hardver-/audio-elfogadás hiányát nem írja felül az,
hogy egy korábban még nem futott ROM-mentes platformteszt azóta sikeres lett.
A helyi szintetikus és upstream-összehasonlító mérések eredeti számai megmaradnak.

| Dátum | Korabeli változás és állapot | Akkori ellenőrzés / bizonyíték | Akkori következő lépés |
|---|---|---|---|
| 2026-10-27 | **Korabeli állapot:** [#16](https://github.com/RobCZart82/yaman1x/pull/16), beolvadt: Python `-O` mellett is aktív ABI-ellenőrzések, AsmJit tesztforrások kizárása, harmadik fél patch-darabszám javítása | `ac56d8c7144ef8cb478f3a714578203c440b841e`: Linux x64, macOS arm64, Windows x64 CI 11/11 tesztcsoport PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792). Helyi Windows: build PASS, CTest 10/11; egy meglévő symlinkteszt WinError 1314 miatt akadályozott. [Auditkövetés](AUDIT_FOLLOWUP_2026-10-27.md). | A VOP3 inicializálás/`memcmp` kockázat célzott vizsgálata; EG target-hold változtatás csak hardveres bizonyíték után. |
| 2026-10-27 | **Korabeli állapot:** [#17](https://github.com/RobCZart82/yaman1x/pull/17), beolvadt: [ADR-0001](ADR-0001-engine-boundary.md), függőségazonosítók és [buildalap](BUILD_BASELINE.md). **M0 részben teljesült.** | `bd2262fd7c06c91f7e40ea2aaa3ea52dceecedc1`: mindhárom platform CI PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454539768). Helyi CMake-konfiguráció és forrásaudit PASS, 201 változatlan upstream fájl ellenőrizve. | H8-port függőség- és licenclezárása, példányizolációs/időléptetési próbák, teljes referencia-toolchain rögzítése; saját terjesztési licenc döntése. M1 a külön MAME-kutatási úton folytatható. |
| 2026-10-27 | **Korabeli állapot:** [#19](https://github.com/RobCZart82/yaman1x/pull/19): **M1 folyamatban:** időbélyeges SCI-hozzáférés-megfigyelő és naplóellenőrző elkészült; firmware-es elfogadás még nem futott. [Részletek](m1-sci-access-milestone.md). | Natív megfigyelő és célzott Python-tesztek Windows x64-en PASS. A teljes Python-csomag meglévő symlinktesztjét WinError 1314 akadályozza. Tizenegy patch alkalmazása a rögzített forrásra PASS; teljes új MAME-build és friss firmware-próba NOT RUN. | Külön macOS kutatási build, SCI-beállítások és rövid engedélyezési ablakok rögzítése, majd a PC-k követése privát disassemblyvel. PKS-válasz továbbra sem feltételezhető. |
| 2026-10-27 | **Korabeli állapot:** [#20](https://github.com/RobCZart82/yaman1x/pull/20), beolvadt: **M0/M1 folyamatban.** Upstream forrásazonosítók visszanyerése, ismételhető patch-előellenőrző és teljes macOS MAME-build CI. [Bizonyíték és határok](source-identity-recovery.md#verified-result). | `31664c74f2726091eb9f403c1621ef949479ddbb`: [Linux/macOS/Windows alapcsomag](https://github.com/RobCZart82/yaman1x/actions/runs/40457998062) 12/12 csoport PASS platformonként; [teljes MAME-build és arm64/regisztrációs próba](https://github.com/RobCZart82/yaman1x/actions/runs/40457998036) PASS Xcode 16.2-vel. 11 patch, 4 egyező projektfejléc; firmware/SCI/MIDI-elfogadás NOT RUN. A kezdeti fordító- és Windows-sorvéghiba dokumentálva. | Friss, helyi firmware-felvétel és SCI-vezérlésiút-elemzés. A teljes kutatási build előfeltétele teljesült; H8-port és példányizoláció továbbra is nyitott. |
| 2026-10-27 | **Korabeli állapot:** [#22](https://github.com/RobCZart82/yaman1x/pull/22): **M0 alapstabilitás, bizonyított konstruktorhiba javítása.** VOP3 `ops` inicializálása az első dekódolás előtt; [bizonyíték és visszafordítható provenance](vop3-initial-decode-fix.md). | `44d5e8c8d2708e6736ce88e8cbe48be67a7b314c`: az eredeti fejléc inicializálatlan `memcmp` olvasását a [MemorySanitizer reprodukálta](https://github.com/RobCZart82/yaman1x/actions/runs/40461084899), az inicializált jelölt tiszta. Natív konstrukció/dekódolás és provenance-regressziók hozzáadva; végső platform- és sanitizer-eredmények a PR checkjeinél. | H8-port és friss helyi firmware/SCI/MIDI-elfogadás továbbra is nyitott. Az EG target-hold változatlan, hardveres bizonyíték szükséges. |
| 2026-10-27 | **Korabeli állapot:** [#23](https://github.com/RobCZart82/yaman1x/pull/23): **M1 Mac-folytatás előkészítve.** Generált két másodperces MIDI-hang és két friss, egyetlen státuszprofilban eltérő firmware-megfigyelés futtatója; [Mac M1 átadás](MAC_M1_HANDOFF.md). | Szintetikus MIDI-időzítés, párosítási és hibakezelési próbák helyben PASS; forrásaudit PASS. A végső platform- és sanitizer-eredmények a PR checkjeinél. Valódi firmware/SCI/MIDI-futás NOT RUN. | A Macen eszköz- és forrásellenőrzés, build, majd a helyi párosított próba. A befejezett suite nem jelent automatikusan igazolt MIDI-feldolgozást vagy bootot. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 H8-port előkészítés:** 30 fájlos, hash-elt include/implementáció leltár és generált/keretrendszer-határok. [Részletek](h8-port-source-boundary.md). | A rögzített archívum read-only feldolgozása PASS; ciklus-, párosítás-, hiányzó forrás-, hash- és útvonal-regressziók PASS. Ez nem teljes fordítói függőségzárás vagy licenclezárás; beágyazott H8-build NOT RUN. | Generátor/lista/buildszabály rögzítése, framework-szolgáltatások feltárása és minimális hostadapter; a Macen külön friss firmware-próba. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 H8-generátor előkészítés:** három buildbemenet és négy generált fájl hash-rögzítése; [ismételt generálási próba](h8-port-source-boundary.md#repeated-generation-experiment). | Windows Python 3.13.3: négy kimenet két független generálása PASS; bemenetazonosság- és kimenettartalom-regressziók PASS; forrásaudit PASS. Ág: `feat/h8-source-inventory`; PR megnyitva; végső platform-CI a #24 checkjeinél. | Keretrendszer-szolgáltatások és generált kód függőségeinek feltárása, minimális hostadapter és fordítás. CPU/firmware-végrehajtás nem történt. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 első önálló H8-periféria:** MAME-független digitális portmodell; [forrásazonosság, határok és teszt](h8-digital-port.md). | MSVC 19.44 Release build és natív teszt PASS: 786432 DDR/adat/maszk/forced-input kombináció négy bemeneti mintával; értesítéssorrend, reset és példányizoláció PASS. Platform-CI, firmware-integráció és hardverpróba NOT RUN; ág: `feat/h8-source-inventory`. | PR és háromplatformos ellenőrzés; utána a SCI/IRQ/timer időzítési és hostszolgáltatás-határának kialakítása. A CPU-port még nincs kész. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): digitális port helyi upstream összehasonlítása. | A rögzített MAME 11 eredeti regiszter/reset metódustörzsével 600000 determinisztikus művelet: **0 eltérés**, MSVC Release. [Próba határai](h8-digital-port.md#validation). | Végső platform-CI és teljes kutatási build ellenőrzése. Teljes MAME-eszköz-, CPU- és hardvervalidáció továbbra sem történt. |
| 2026-10-27 | **Korabeli állapot:** **#24 beolvadt, H8-periféria platformellenőrzés lezárva.** [Végső bizonyíték](h8-digital-port.md). | `896bd7882309bb03868c5c3816f4f9b7b068b858`: [Linux/macOS/Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40468100477), [MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40468100462) és [teljes kutatási MAME-build](https://github.com/RobCZart82/yaman1x/actions/runs/40468100442) PASS. A korábbi NOT RUN platformbejegyzések történeti állapotok. | SCI/IRQ/timer hostadapter és CPU-leválasztás. Az új GPIO firmware-integrációja és a friss Mac-firmware-próba továbbra is NOT RUN. M0/M1 még nem teljesült. |
| 2026-10-27 | **Korabeli állapot:** [#26](https://github.com/RobCZart82/yaman1x/pull/26): **M0 H8H belső IRQ-választó elkészült külön komponensként.** [Szemantika és hiányzó adapter](h8-internal-irqs.md). | Helyi MSVC Release build/teszt PASS: 11 prioritáscsoport és 44 belső vektor, 990 vektorpár, DMA-elfogyasztás, szűrők, NMI-latch, reset és izoláció. Platform-CI a külön PR-ben; CPU-/SCI-/firmware-integráció NOT RUN. | Teljes IRQ-adapter, külső lábak és CPU-szűrők bekötése; időzített SCI-modell és friss Mac-firmware-próba. Az NMI-élt és időfeltételt a tulajdonosnak kell igazolnia. |
| 2026-10-27 | **Korabeli állapot:** [#27](https://github.com/RobCZart82/yaman1x/pull/27): **M0 H8H IRQ-regiszter/láb adapter elkészült.** [Szemantika, ICR-kockázat és inbox-értékelés](h8h-irq-controller.md). | Helyi MSVC Release tesztek PASS; az eredeti upstream metódustörzsekkel 300000 determinisztikus művelet, **0 eltérés**. A PIN/NMI/IER/ISR/ISCR/ICR és publikációs határok megőrizve; CPU/SCI/firmware-bekötés NOT RUN. | Platform-CI és kutatási build; utána időzített SCI- és CPU-adapter. Az új PKS-HLE/dummy-ROM jegyzet nem tekinthető protokollbizonyítéknak; feltételezett választ nem vezetünk be. |
| 2026-10-27 | **Korabeli állapot:** [#28](https://github.com/RobCZart82/yaman1x/pull/28): **M0 SCI belső ciklusóra-adapter.** [Részletes szerződés, tesztek és következő lépés](h8-sci-cycle-clock.md). | Helyi MSVC Release próba: 1024 osztó, TX/RX ciklusnézet, blokkfelosztás, standby és hibahatárok PASS. Eredeti upstream ütemezési metódusokkal 200000 determinisztikus művelet, **0 eltérés**. Végső platform-CI a külön PR-ben; bájtállapotgép és CPU-/firmware-integráció NOT RUN. | SCI TX/RX és SSR/RDR mellékhatások, IRQ-kapcsolat, majd közös gépütemező és tényleges H8-végrehajtás. Külső SCI-órás módok még nem támogatottak. |
| 2026-10-27 | **Korabeli állapot:** [#29](https://github.com/RobCZart82/yaman1x/pull/29): **M0 részben teljesült:** önálló [SCI SSR státusz-latch](h8-sci-status.md), olvasás utáni törlés, debugger/DMA mellékhatások és upstream reset-viselkedés. A teljes UART még nincs bekötve. | Windows MSVC Release: 33 554 432 átmenet független bitenkénti orákulummal PASS; külön szekvenciatesztek PASS. A végső revízió és platformeredmények a kapcsolódó PR checkjeinél. Firmware/MIDI/hardver: NOT RUN. | SCI TX/RX bájtállapotgép, TDR/RDR és megszakítások; az SSR-write utáni tx_start/sync_rx_start tulajdonosi lépések és ciklusóra összekötése, majd CPU-bekötés. |
| 2026-10-27 | **Korabeli állapot:** [#30](https://github.com/RobCZart82/yaman1x/pull/30): **Ötletfeljegyzés:** PKS HLE, fokozatos protokollbizonyítás, dummy ROM korlátai és távlati hardveres helyettesítés lehetőségei rögzítve az M1 alatt. | Dokumentációs értékelés; új működési vagy hardveres bizonyíték nincs, M0/M1 állapota változatlan. | Firmware-elvárások és tényleges kommunikáció bizonyítása; csak utána döntés egy külön tesztelhető PKS-modellről. |
| 2026-10-27 | **Korabeli állapot:** [#31](https://github.com/RobCZart82/yaman1x/pull/31): **Architekturális követelmény:** cserélhető PKS HLE/firmware megvalósítás, közös időzített perifériafelület és verziózott mentett állapot; áttérési út az M1 alatt és ADR-0001-ben. | Dokumentációs döntés; nincs megvalósított backend vagy új dump, M0/M1 változatlan. | A bizonyított protokoll alapján interfész és szintetikus tesztek; dump esetén MCU-integráció és összehasonlító elfogadás. |
| 2026-10-27 | **Korabeli állapot:** [#32](https://github.com/RobCZart82/yaman1x/pull/32): **M0 licencnyilvántartás megerősítve:** javított harmadikfél-összesítő; 219 fájl, 5 licencszöveg nyilvántartása és CTest-ellenőrzése. [Hatókör és nyitott feltételek](../THIRD_PARTY.md#reviewed-sourcenotice-coverage). | Helyi Windows: 7 célzott pozitív/negatív teszt PASS; forrás-/értesítés-lefedettség PASS. A végső revízió és háromplatformos eredmények a kapcsolódó PR checkjeinél. Ez nem jogi vagy bináris kiadási jóváhagyás. | Új átvételnél kötelező nyilvántartás és licencvizsgálat; saját projektlicenc döntése, teljes H8/SDK függőségzár és tényleges kiadási csomag értesítéseinek ellenőrzése továbbra is nyitott. |
| 2026-10-27 | **Korabeli állapot:** [#34](https://github.com/RobCZart82/yaman1x/pull/34): **M0 részben teljesült:** [aszinkron SCI TX](h8-sci-async-tx.md), TDR/puffer, SSR/ciklusóra összekötése, adási vonal és TXI/TEI események. RX/CPU még nincs bekötve. | Windows MSVC Release: 4096 adat/keretezés kombináció és célzott tulajdonosi szekvenciák PASS; eredeti upstream metódusokkal 200000 művelet, 82931 TX-tick, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. Firmware/hardver/audio NOT RUN. | Aszinkron RX/RDR és hibák, közös TX/RX óra/státusz, IRQ/CPU MMIO; utána friss firmware-es MIDI-elfogadás. Paritás upstream szerint megőrizve, nem igazolt hardvertény. |
| 2026-10-27 | **Korabeli állapot:** [#35](https://github.com/RobCZart82/yaman1x/pull/35): **M0 részben teljesült:** [aszinkron SCI RX](h8-sci-async-rx.md), vonalmintavétel, RDR és RXI/ERI kérések, túlcsordulás/paritás/keretezési hibák. Külön RX-tulajdonos; közös TX/RX és CPU még nincs bekötve. | Windows MSVC Release: 4096 adat/keretezés kombináció és célzott hibás szekvenciák PASS; eredeti upstream metódusokkal 896623 összehasonlítás, 663805 RX-tick, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. Firmware/hardver/audio NOT RUN. | Közös TX/RX regiszter/óra/státusz és azonos idejű események; IRQ/CPU MMIO; utána friss firmware-es MIDI-elfogadás. A külön TX/RX osztályok együtt még nem teljes SCI-csatorna. |
| 2026-10-27 | **Korabeli állapot:** [#36](https://github.com/RobCZart82/yaman1x/pull/36): **M0 részben teljesült:** [közös aszinkron SCI-csatorna](h8-sci-async-channel.md), egy regiszter/óra/SSR-tulajdonos TX/RX állapotgépekkel. Régi felületek kompatibilitási rétegként megmaradtak, a bitlogika közös. | Windows MSVC Release: 4096 teljes duplex adat/keretezés kombináció és célzott sorrend-/státusz-/izolációs tesztek PASS; eredeti upstream óra/TX/RX metódusokkal 1153468 összehasonlítás, 804316 közös él, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. CPU/firmware/hardver/audio NOT RUN. | Négy SCI IRQ-kérés útvonalazása és hatregiszteres MMIO-adapter, két SCI-csatorna tesztje; majd CPU/periféria-ütemező és friss firmware-es MIDI-elfogadás. |

## A cél pontos meghatározása

Az első cél egy macOS Apple Siliconon futó **VST3 hangszer**, amely REAPER-ben
betöltve, MIDI note-on hatására egy AN1x-hangszínt megszólaltat, note-off után
pedig annak elengedését végrehajtja. A hangot az eredeti, változatlan helyi
firmware által vezérelt DSP-program állítja elő. Egy scene és egy egyszerre
megszólaló hang elegendő az első bemutatóhoz. A szükséges chipek számát a
feltárt hangút határozza meg; a kétchipes működést nem tekintjük késznek.

A demonstrációhoz rögzített firmware-, hangszín- és konfigurációazonosító,
ismételhető indítás és helyi REAPER-hangfelvétel tartozik. A kísérleti státusz-,
EG- és órajelprofil minden eredmény mellett szerepel. Egy sikeres megszólalás
önmagában nem igazolja az AN1x teljes bootját vagy hangzásazonosságát.

Az első változathoz minimális beállítási lehetőség kell a helyi ROM-fájlok
kiválasztására és az indítás állapotának jelzésére. Az egyedi grafikus skin,
AU, Windows, teljes polifónia, bankböngésző és MEG-effektek későbbi feladatok.

## Jelenlegi helyzet és kutatási előzmények

A ROM-mentes motoralap aktuális állapota a dokumentum eleji #136 motorlezárásban
és az alábbi M0-szakaszban szerepel. A korábbi táblázat és a #36 lezárása történeti adat. Az alábbi firmware-/DSP-megfigyelések a hivatkozott kutatási
jegyzőkönyvek korábbi felvételei; a dokumentáció egységesítésekor nem futtattunk
új firmware-t vagy hardvermérést. Nem tekinthetők a közös SCI-adapter friss
firmware-integrációs bizonyítékának.

| Terület | Bizonyított eredmény | Nyitott feladat |
|---|---|---|
| H8 és firmware | A kutatási MAME-target futtatja a helyi 1.04 ROM-ot; kísérleti státuszprofil mellett mindkét DSP programját feltölti | Megismételhető indulás a hangszín- és MIDI-kezeléshez szükséges állapotig |
| MIDI és PKS | RXD0 a MIDI bemenet; a jel eléri a modellt. SCI1-en 30 TDR-írás és nulla RDR-olvasás látszik a párosított próbákban | A PKS-protokoll és az SCI0 bekapcsolásának vezérlési útja; tényleges MIDI-bájtvétel |
| DSP és időzítés | Két VOP3-modell és mintaléptetés működik; a feltöltött mezők auditálhatók | Órajel, EG-viselkedés, feltöltés alatti futás, táblarom-hozzáférés és soros hangút |
| Táblarom | Külső 128 KiB-os kép egyes táblái egyeznek a CPU-ROM-ban lévő másolattal | Indexskála, modellbe kötés és a második panel ROM-jának azonossága |
| Hang és plugin | Szintetikus DSP-soros próba és ROM-mentes CI van | AN1x offline hang, termékmotor, VST3 és REAPER-próba |

Részletek: [STATUS](STATUS.md), [SCI-megfigyelések](sci-state-milestone.md),
[táblarom és órajel](an1x-clock-and-table-rom.md),
[időzített DSP-kísérlet](timed-vop3-experiment.md).

## Mérföldkövek és elfogadási feltételek

### M0 — Rögzített fejlesztési alap és motorarchitektúra

2026-11-10: **részben teljesült**. A következő alapok elkészültek:

- Az [ADR-0001](ADR-0001-engine-boundary.md) kiválasztotta a motorhatárt, a
  külön MAME kutatási utat és a tervezett VST3 SDK-t. Az architekturális választás
  rögzített; a CPU és több periféria szintetikus integrációja elkészült,
  a teljes gép és a firmware-es indulás elfogadása még hátravan.
- A [függőségazonosítók](engine-dependencies.json), a forráslockok, a H8-leltár
  és a generátor bemenetei/kimenetei rögzítettek. A teljes fordítói/licenc-
  függőségzár ezzel még nincs lezárva.
- A [buildalap](BUILD_BASELINE.md) dokumentálja a helyi Windows referencia-
  környezetet és a külön teljes macOS arm64 MAME-build rögzített adatait.
  A kutatási build előfeltétele a #20 óta teljesült, a #36-nál is PASS.
  A hosztolt CI-runner verziói ettől nem válnak állandó toolchain-lockká.
- Önálló GPIO, IRQ-adapter és [közös aszinkron SCI TX/RX-csatorna](h8-sci-async-channel.md)
  áll rendelkezésre. Komponensszintű időléptetési/izolációs tesztjeik sikeresek;
  ez még nem teljes CPU-/gépizolációs vagy firmware-elfogadás.
- A forráseredet és a [licenc-/értesítés-lefedettség](../THIRD_PARTY.md#reviewed-sourcenotice-coverage)
  ellenőrzése automatizált. A saját projektlicenc és a tényleges kiadási csomag
  megfelelőségének ellenőrzése továbbra is nyitott.

A [kétcsatornás SCI MMIO/IRQ-adapter](h83002-sci-mmio-irq.md) szintén elkészült: bájt-/szóhozzáférés, DMA-elsőbbség és csatornánkénti vektorkiosztás.

Az önálló SCI-idővonal (#41), CPU-hoszthatár (#42), buszsegédek (#43),
cikluskeret (#44) és [végrehajtási sorrend](h8-execution-runner.md) elkészült.
A [beolvadt H8H-részhalmaz](h8-first-instructions.md) már fordul és eredeti
full/partial kezelőket hajt végre. A #57-kori 366 kezelő támogatott műveletei (történeti részhalmaz):
memória és prefixek, rel8/rel16 feltételek, kiválasztott aritmetika/logika,
egybites shift/rotate, regiszteres/indirekt/abszolút bitműveletek, normál
[veremkezelés és vezérlésátadás](h8-stack-control-flow.md). Ez részhalmaz,
nem teljes CPU. A korábbi bővítések és mérések az előrehaladási naplóban maradnak.

Nyitott feladatok, végrehajtási sorrendben:

1. A 264 kiválasztott H8H-kódolássor kezelői már bekerültek, a kiválasztott
   leltárban 0 hiányzó sor van (135 kizárt sor külön hatókör). Ez nem teljes
   CPU- vagy hardverelfogadás. A közös ROM/RAM/SCI/GPIO/IRQ-busz szintetikus
   integrációja elkészült. A közös korlátozott SYSCR-módok és a támogatott részreset
   már teszteltek. Az aktív belső órás ITU gépütemező/MMIO- és CPU IRQ-bekötése
   #215–222-ben elkészült; a korlátozott ADC-konverzió/scan/restart #229–242-ben
   kapcsolódott a buszhoz. További perifériaprofilok, teljes hardver-reset,
   címtér/időzítés és gépszintű elfogadás továbbra is szükséges. A friss
   PKS-operandusok érték-/utasítási eredetének vizsgálata az M1 kutatási iránya.
2. A teljes gép CPU-/RAM-/periféria-/DSP-/MIDI-állapotának példányizolációját,
   resetjét és blokkfelosztástól független időléptetését igazolni.
3. Az újonnan ténylegesen beépített függőségek licenceit lezárni; a saját kód
   licencéről a bináris terjesztés előtt dönteni. A kiválasztott SDK tényleges
   integrációját és összekapcsolt részeit akkor ellenőrizni; az SDK még tervezett.
4. Megőrizni a ROM-mentes alapbuildet és a külön kutatási utat; a beágyazott
   motorhoz saját, revízióhoz kötött referencia-buildet rögzíteni.

**Kész, ha:** van rövid architektúradöntés, függőséglista és megismételhető
buildutasítás. A kiválasztott motor CPU-, periféria-, DSP- és MIDI-állapota
példányonként elkülöníthető, időléptetése külső audiofolyamhoz illeszthető.

### M1 — Indulási út, PKS és valódi MIDI-vétel

2026-10-27: **előkészítve, nem teljesült**. Elkészült az
[időbélyeges SCI-hozzáférés-megfigyelő](m1-sci-access-milestone.md), a naplóellenőrzés
és a [Mac M1 párosított helyi próba futtatója](MAC_M1_HANDOFF.md). Ezek szintetikus
szervezési/hibakezelési tesztjei sikeresek; nem friss firmware-es elfogadások.

**Teljesült előfeltétel:** a [rögzített forrású teljes kutatási build](source-identity-recovery.md)
#20 óta sikeres, és a #36 külön macOS buildje/regisztrációs próbája is PASS.
A build sikere önmagában nem bizonyít tényleges SCI-bájtvételt.
A közös SCI-modell szintetikus CPU-/buszintegrációja már az M0 része. Ez nem
bizonyítja a valódi firmware MIDI-bájtfogadását; az M1 elfogadása továbbra is nyitott.

**Következő elfogadási lépés:** a Mac M1 átadási útmutató szerint friss helyi
firmware-futást és párosított megfigyelést készíteni, majd a tényleges SCI0
bájtfogadást és a note-on/note-off feldolgozását bizonyítani. A friss elfogadáshoz
nincs rögzített eredmény; dokumentált státusza **NOT RUN**. A PKS-kézfogás és az
SCI0 engedélyezésének okozati útja továbbra is kutatási feladat; a ROM-mentes CI
nem váltja ki.

- Visszakövetni a firmware SCI1/PKS-inicializálását és az SCI0 engedélyezését.
  A mostani megfigyelés alapján a PKS-válasz hiánya lehetséges akadály, de
  okozati összefüggését még igazolni kell.
- Megállapítani a szükséges PKS-kézfogást firmware-elemzéssel és elérhető
  hardveres bizonyítékokkal. Ha a hiányzó ROM helyett HLE szükséges, annak
  minden válaszát, állapotát és időzítési feltételezését dokumentálni.
- Ellenőrizni az SCI0 konfigurációját, a beérkezett bájtokat és a firmware
  eseménykezelőjéig vezető utat; az RXD-vonalátmenetek önmagukban nem elegendők.
- Rögzíteni egy determinisztikus induló hangszínt és rendszerállapotot.
  Az üres NVRAM, gyári inicializálás és normál indulás útját külön tisztázni.

**Kész, ha:** friss indulás után a firmware bizonyíthatóan fogad egy ismert
note-on/note-off bájtsorozatot és annak megfelelő voice/EG/regiszterváltozást
végez. A próba megismételhető, a használt perifériaprofilok rögzítettek.

**Ha elakad:** a hiányzó protokollt vagy hardvermérést konkrét feladatként
rögzíteni. A további offline DSP-kutatás folytatható, de egy közvetlenül
feltöltött DSP-kép megszólaltatása még nem teljesíti a firmware-es MIDI-célt.

#### Kutatási ötletek: a hiányzó PKS-program lehetséges helyettesítése

Feljegyezve: 2026-10-27, az `AN1x_DUMP-hiánypótlás.txt` ötleteinek
értékelése alapján. **Lehetőségek és hipotézisek; nem igazolt működés,
nem megvalósított funkciók és nem új elfogadási bizonyítékok.** M1 állapota
és elfogadási feltételei ettől nem változnak.

- **Minimális PKS HLE-modell lehetősége.** Elképzelhető, hogy az eredeti
  HD63B05-program futtatása helyett a fő CPU által megfigyelhető kommunikáció
  modellezése elegendő. Előbb azonosítani kell a kéréseket, válaszbájtokat,
  állapotátmeneteket, reset- és időzítési szabályokat. A PKS-válasz hiánya
  jelenleg csak lehetséges indulási akadály; az okozati kapcsolat nem igazolt.
- **Fokozatos bizonyítás.** Először firmware-elemzéssel követni a fő CPU
  elvárásait; lehetőség szerint működő hangszer privát buszmegfigyelésével
  ellenőrizni. Ezután kis, külön kapcsolható protokollmodellt és pozitív/negatív
  teszteket készíteni. Meg kell különböztetni a minimális indulási kézfogást
  a teljes kezelőpanel-viselkedéstől. A siker mércéje továbbra is a tényleges
  firmware-es bájtfogadás és eseményfeldolgozás, nem egy kényszerített státusz.
- **Dummy ROM csak betöltési kísérletként merülhet fel.** Egy 4096 bájtos
  üres fájl nem helyettesíti a programot vagy a protokollt. Csak elkülönített,
  egyértelműen szintetikus tesztadatként lenne értelme, ha konkrét betöltési
  kérdést vizsgálunk; nem nevezhető valódi dumpnak és nem bizonyít bootot.
  Előnyösebb lehet egy kifejezetten ROM nélküli HLE-mód, ha a protokoll már
  bizonyított. Jelenleg egyik megoldás bevezetéséről sincs döntés.
- **Fizikai mikrokontrolleres helyettesítés: külön távlati irány.** Csak
  bizonyított protokoll, elektromos illesztés és időzítés alapján mérlegelhető.
  A szervizrajz önmagában nem tárja fel a program működését; a javasolt Hitachi
  változatok láb-, programozási és működési kompatibilitása nincs igazolva.
  Ez nem előfeltétele a jelenlegi szoftveres fejlesztésnek.

**Továbbfejleszthetőségi követelmény — 2026-10-27.** Ha bizonyított HLE
áthidalást vezetünk be, azt cserélhető PKS-megvalósításként kell tervezni.
A későbbi eredeti dumpot futtató MCU-modell ugyanahhoz az időbélyeges
perifériafelülethez kapcsolódjon; a plugin és a fő H8 firmware ne függjön a
HLE belső megoldásaitól. Ez tervezési követelmény, nem elkészült funkció.
Részletek: [ADR-0001: cserélhető PKS-megvalósítás](ADR-0001-engine-boundary.md#replaceable-pks-implementation-and-future-dump-integration).

Tervezett áttérési út és elfogadási feltételek:

1. Bizonyított protokoll alapján rögzíteni a közös bemenet/kimenet, reset és
   időléptetés szerződését; szintetikus tesztekkel ellenőrizni az izolációt.
2. A HLE-t külön azonosított kísérleti profilként megtartani. A mentett állapot
   tartalmazza a megvalósítás/profil azonosítóját, sémaverzióját és a szükséges
   ROM tartalmi azonosítóját; Yamaha bájtokat vagy helyi abszolút utat ne.
3. Ha elérhető lesz a dump, ellenőrizni az eredetét/azonosítóját, a pontos MCU-t,
   memóriatérképet és perifériákat; csak ezután készíteni firmware-t futtató
   modellt. Az eredeti program megléte önmagában nem bizonyít hardverhűséget.
4. Azonos időzített ingerekkel összevetni a HLE-t és az új modellt; indulás,
   kezelőpanel, reset, firmware-es MIDI és audio elfogadását újrafuttatni.
   A bizonyíték alapján tudatosan kijelölni az új alapértelmezett profilt.
5. Váltáskor ellenőrzött újrainicializálást használni; futó audio közbeni csere
   nem követelmény. Régi HLE-állapotot tilos észrevétlenül MCU-állapotként
   betölteni. Hiányzó ROM/profil esetén egyértelmű jelzés szükséges; migráció
   csak külön dokumentált és tesztelt támogatásként ígérhető.

Korlátok: a jelenlegi LCD-buszmodell és SysEx-eszközök nem helyettesítik a
PKS-kézfogást. A PKS HLE ötlete a külön VOP3 ROM-ok hiányát nem oldja meg.
Nem vezetünk be feltételezett sikeres válaszokat vagy firmware-módosítást
pusztán az ötletjegyzet alapján. A hardveres bizonyítékhoz kötött EG target-hold
szabály változatlan. A feljegyzés nem teszi feladattá ROM, bank vagy privát
trace repository-ba másolását.

### M2 — Egy hangszínhez elegendő DSP- és táblarom-modell

- Külső bemenetként betölteni és azonosítani a szükséges táblaromot; rögzíteni
  az előjel-, szó- és címezési szabályokat. A DMS ROM azonosságát külön kezelni.
- Feltárni a választott hangszínhez szükséges indexskálát és táblahasználatot.
  A táblák tartalmi egyezése nem helyettesíti a címezés ellenőrzését.
- Megvizsgálni az EG indítását, célkeresztezését, tartását és elengedését,
  valamint a firmware által használt 0x21/0x22 visszaolvasásokat.
- Rendezni az upload alatti megállítás/újraindítás és a CPU–DSP eseménysorrend
  modelljét. A regiszterfeltöltés sikere nem jelenti ezek helyességét.
- Meghatározni az első demonstráció motoróráját. Ha csak feltételezett érték
  áll rendelkezésre, névvel jelölt kísérleti profilt használni, és ebből a
  megfelelő hangmagasságra vagy hardveres időzítésre nem következtetni.

**Kész, ha:** a kiválasztott voice determinisztikusan fut, az érintett DSP-állapotok
végesek, note-on és note-off hatására a szintek változnak. Minden még feltételezett
órajel-, EG- és címzési szabály szerepel a mérési jegyzőkönyvben.

### M3 — Első száraz hang offline rendereléssel

- Követni a voice kimeneti soros slotjait és az esetleges chipközi kapcsolatot.
  A szükséges útvonalat a rajz, a firmware és a DSP-program alapján azonosítani.
- Kialakítani egy megfigyelhető száraz kimeneti pontot. A MEG elhagyása csak
  dokumentált kutatási száraz út lehet; nem állítjuk róla, hogy a teljes gyári
  kimenet másolata. Ha a hanghoz további chip kötelező, azt be kell kötni.
- Meghatározni a szóformátumot, skálázást, jelkésést és a csatornák jelentését.
  Az upstream FS1R DAC-segédfüggvény AN1x-re való alkalmazását külön igazolni.
- Ugyanazon firmware/MIDI útvonalból helyi WAV-fájlt renderelni: kezdeti csend,
  note-on, tartott hang, note-off és lecsengés. Diagnosztikához hibás számértéknél
  némítani; kezdetben alacsony kimeneti szintet használni.

**Kész, ha:** a WAV-ban hallható, nemnulla, véges jel van; a hang indulása és
elengedése a MIDI-eseményekhez kötődik. Nincs tartós túlvezérlés vagy váratlan
egyenkomponens. Azonos bemenet ismételt renderelése egyezik az előre rögzített
numerikus tolerancián belül. A felvételhez profil-, ROM- és buildazonosító tartozik.

### M4 — Beágyazható, audio-blokkokkal működő motor

- Kialakítani az init, reset, időbélyeges MIDI-bemenet, render és állapotkezelés
  felületeit. A firmware indítása és a fájlbetöltés az audiofeldolgozáson kívül történjen.
- Egy idővonalra illeszteni a CPU-t, perifériákat és DSP-ket. A DAW-események
  blokkon belüli helyét megőrizni; a modellezett soros átvitel késleltetését is kezelni.
- A belső motoróra és a hoszt mintavételi frekvenciája közé szükség esetén
  mintavétel-konverziót építeni; a hoszt frekvenciája ne írja át észrevétlenül a DSP-modellt.
- Előre lefoglalni a szükséges memóriát. Az audioútból eltávolítani a fájlműveletet,
  dinamikus foglalást, blokkoló várakozást és korlátlan naplózást.
- Kezelni az all-notes-off, reset, leállítás és újraindítás eseteit, valamint a
  MIDI-eseménysor túlcsordulását dokumentált hibaviselkedéssel.

**Kész, ha:** a beágyazott motor az M3 referenciafolyamát különböző blokkfelosztásokkal
is reprodukálja. Az első célbeállításon, például 48 kHz/256 mintás hosztblokkal,
tartósan az audiohatáridőn belül fut. A mért idő és tartalék dokumentált;
44,1 kHz és más blokkméretek támogatását külön ellenőrizzük.

### M5 — Minimális VST3 hangszer

- A motorhoz VST3 adaptert készíteni a rögzített SDK/keretrendszer-verzióval;
  audio-kimenetet, MIDI-eseményfogadást és példányonkénti állapotot biztosítani.
- GUI nélkül, hosztparaméterrel vagy helyi konfigurációval biztosítani a ROM-kiválasztást, indítási állapotot és hangerőt.
  A hiányzó/ismeretlen ROM kezelése legyen egyértelmű, a plugin maradjon némítható.
- A hoszt pluginkeresése ne indítson hosszan futó firmware-próbát. A motor
  elkészültéig a feldolgozás determinisztikus csendet adjon.
- Projektmentéshez tárolni a szükséges profilokat, hangszínállapotot és
  ROM-azonosítókat. Visszatöltéskor egyező helyi fájlokat keresni/kérni;
  a ROM-tartalom ne kerüljön a pluginba vagy a projektállapotba.
- Megépíteni a REAPER-rel egyező architektúrájú helyi `.vst3` csomagot,
  ellenőrizni a formátum validátorával, és előkészíteni a helyi telepítést.

**Kész, ha:** a VST3 betölthető, MIDI-t fogad és ugyanazt a referenciahangot adja,
mint a motor önálló renderelése. A projekt mentése/újranyitása megőrzi a szükséges
beállításokat; hiányzó ROM esetén érthető hibajelzés és csend az eredmény.

### M6 — Az első REAPER-ben megszólaló hang igazolása

1. Rögzíteni a macOS-, gép-, REAPER-, pluginbuild- és architektúra-adatokat,
   az audioeszközt, frekvenciát, blokkméretet, ROM-hasheket és a választott voice-t.
2. A helyi VST3-at felismertetni, üres projekt hangszer-sávjára betölteni,
   a helyi ROM-okat kiválasztani és megvárni az induláskész állapotot.
3. Beállítani a sáv audioútját a master kimenetre. Rövid MIDI-itemmel indítani
   egy ismert hangot, például a 60-as MIDI-hangszámot, majd note-offot küldeni.
4. Ellenőrizni a hangot lehallgatással és mérővel: indulás, tartás, elengedés,
   ismétlés és leállítás. A note-off/all-notes-off ne hagyjon beragadt hangot.
5. A REAPER-kimenetet helyi WAV-ba menteni, összevetni az offline referenciával,
   majd a projektet újranyitva megismételni a próbát. A különbségekhez rögzített
   késleltetés- és numerikus tolerancia tartozzon.

**Kész, ha:** a fenti jegyzőkönyv és felvétel alapján a REAPER-ben a YamAN1x
firmware-vezérelt DSP-hangja hallható és reprodukálható. Rögzíteni kell, hogy
mely tulajdonságok kísérletiek. A ROM-mentes forrás és a helyi kutatási adatok
publikálási határa továbbra is megmarad.

## Végrehajtási sorrend és döntési pontok

Fő függőségi lánc: **M0 → M1 → M2 → M3 → M4 → M5 → M6**.
M1 közben az M2 táblarom-/órajel-kutatása önállóan haladhat. Egy ROM-mentes
VST3 szintetikus próbája később segítheti a hosztadapter ellenőrzését, de a
végcélt a firmware-vezérelt jelút teljesíti.

Minden mérföldkőhöz kis, önálló PR, reprodukciós parancsok, elvégzett próbák és
frissített státusz tartozik. A repository CI a ROM-mentes kódot ellenőrzi;
a helyi firmware-, audio- és REAPER-próbát külön jegyzőkönyv igazolja.

A legnagyobb bizonytalanság M1–M3: PKS-protokoll, DSP-modell, táblarom-címezés
és hangút. Ha ezekhez új hardveradat kell, annak pontos kérdését és beszerzési
lépését rögzíteni kell. Naptári kiadási dátumot az offline referenciahang
megszületése előtt nem vállalunk; M3 után a motor- és plugincsomagolás becsülhető.

## 2026-10-27 — Korlátozott ciklus-eseményfuttató

PR: [#40](https://github.com/RobCZart82/yaman1x/pull/40), ág: `feat/bounded-cycle-dispatcher`, alap: main #38; a nyitott #39-től független. [Komponensszerződés és tesztek](cycle-dispatcher.md): monoton ciklusidő, befoglaló határidő, folytatható lépéskeret, hibás tulajdonos elkülönítése. 1000 eseménysor blokk-/keretfüggetlensége és határesetei helyben PASS. Ez az ütemező alapja; CPU/periféria-tulajdonos, hardveres eseménysorrend és firmware-végrehajtás továbbra is nyitott. M0 részben teljesült.

## 2026-10-27 — SCI busz- és IRQ-integráció fejlesztési bejegyzés

PR: [#39](https://github.com/RobCZart82/yaman1x/pull/39), ág: `feat/h8-sci-mmio-irq`, alap: main #37. Két SCI-csatorna hatregiszteres MMIO-ja és 52–59 IRQ-vektorai bekötve a különálló IRQ-vezérlőbe. Helyi macOS arm64 Release: 23/23 tesztcsoport PASS; 512 RX bájt/csatorna eset, TX, DMA, hibák, reset és izoláció. ASan/UBSan PASS. Részletes [komponensjegyzőkönyv](h83002-sci-mmio-irq.md). Platform-CI a kapcsolódó PR-ben ellenőrizendő. M0 részben teljesült; CPU-/firmware-/MIDI-/audioelfogadás nincs. Következő lépés: közös CPU/periféria-ütemező és tényleges CPU-port.

### #40 szinkronizálása a beolvadt #39-cel

2026-10-27: a közös build-/naplófájlok ütközése feloldva, mindkét komponens és tesztcsoport megtartva. Az egyesített csomag 24 CTest-csoportot és 60 Python-esetet tartalmaz. A korábbi 23-as eredmények történeti bejegyzések; új PR-head CI szükséges. A CPU/periféria-tulajdonos és M0 lezárása továbbra is nyitott.

## 2026-10-27 — Közös kétcsatornás SCI-idővonal

PR: [#41](https://github.com/RobCZart82/yaman1x/pull/41), ág: `feat/sci-cycle-timeline`, alap: beolvadt #40. [Komponensszerződés](sci-cycle-timeline.md): időbélyegzett RX-bemenet és két SCI-ciklusóra, korlátozott és folytatható eseményfeldolgozás. 256 kétcsatornás teljes duplex minta naplója azonos egyben, ciklusokra bontva és egyesével engedélyezett eseményekkel; 512 ismert RX-bájt ellenőrizve. Azonos ciklusban bemenetek forrássorrendben, majd SCI0/SCI1; szoftveres konvenció, nem hardveres bizonyíték. CPU/periféria-ütemezésből a CPU-rész és buszidőzítés továbbra is hiányzik; M0 részben teljesült.

Helyi ellenőrzés: macOS arm64 Release, 25/25 CTest-csoport és 60 Python-eset PASS; az új natív teszt ASan/UBSan alatt is PASS. Végső platform-CI a kapcsolódó PR-headhez kötve ellenőrizendő.

## 2026-10-27 — H8 CPU gazdaszolgáltatás-határ

PR: [#42](https://github.com/RobCZart82/yaman1x/pull/42), ág: `feat/h8-host-services-boundary`, alap: beolvadt #41. [Részletes határ és végrehajtási sorrend](h8-host-services-boundary.md). Hash-ellenőrzött leltár 30 forrásból és négy kétszer generált kimenetből; két teljes futás bájtazonos. Memóriabusz, szóigazítás, ciklustartozás/részutasítás, IRQ/DMA, idő, életciklus, standby/mentés és diagnosztika követelményei rögzítve. Ez előkészítés: a CPU még nincs lefordítva vagy végrehajtva, M0 részben teljesült.

Helyi ellenőrzés: macOS arm64 Release; 25/25 CTest-csoport és 68 Python-eset sikeres; a forrás- és licencértesítési audit sikeres. A végleges PR-fej platformellenőrzése még függőben volt.

## 2026-10-27 — H8H CPU buszhozzáférési réteg

PR: [#43](https://github.com/RobCZart82/yaman1x/pull/43), ág: `feat/h8h-bus-access`, alap: beolvadt #42. [Komponensszerződés](h8h-bus-access.md): öt H8H buszsegéd, utasítás/adat út, szóigazítás és hozzáférés előtti kétciklusos levonás. Eredeti upstream metódustörzsekkel 200000 vegyes művelet, 0 eltérés; minden 16 bites szóérték és hiba-/izolációs határ tesztelve. Ez nem CPU-utasításvégrehajtás: a részutasítás-/keretkezelés, valódi címtér és IRQ/SCI CPU-bekötés nyitott. M0 részben teljesült.

Helyi ellenőrzés: macOS arm64 Release; 26/26 CTest-csoport és 72 Python-eset sikeres; az új natív ASan/UBSan-vizsgálat sikeres. A végleges PR-fej platformellenőrzése külön bizonyíték.

## 2026-10-27 — H8 végrehajtási cikluskeret

PR: [#44](https://github.com/RobCZart82/yaman1x/pull/44), ág: `feat/h8-execution-budget`, alap: beolvadt #43. [Komponensszerződés](h8-execution-budget.md): ciklustartozás továbbvitele, perifériahatáridő és explicit szeletfázisok. Eredeti execute_run módszerrel 200000 szintetikus, esemény nélküli szelet; recompute_bcount módszerrel 200000 határidő, 0 eltérés. Natív buszkapcsolat 1000 felosztási történettel. Ez számtani komponens; a valódi full/partial utasításkezelők, CPU-állapot és IRQ/SCI ütemezési kapcsolata még nyitott. M0 részben teljesült.

Helyi ellenőrzés: macOS arm64 Release; 27/27 CTest-csoport és 74 Python-eset sikeres; az új natív ASan/UBSan-, forrás- és licencértesítési audit sikeres. A végleges PR-fej ellenőrzése még függőben volt.

2026-10-27 dokumentációs lezárás: #44 beolvadt, végső head mind az öt CI-je PASS. A motor legutóbbi helyi eredménye 27/27 CTest-csoport és 74 Python-eset PASS. CPU-opcode végrehajtás, M0 teljes lezárása és M1/audio továbbra is nyitott.

## 2026-10-28 — H8 teljes/részutasítás-futtatási sorrend

PR: [#46](https://github.com/RobCZart82/yaman1x/pull/46). Ág: `feat/h8-execution-order`, alap: beolvadt #45. A [futtatókeret](h8-execution-runner.md) összeköti a ciklustartozást, perifériahatáridőket és full/partial callback-sorrendet. 200000 eseményes referencia-szelet, nulla eltérés az eredeti execute_run metódussal szintetikus kezelők mellett. Natív sorrend-/hiba-/1000 felosztási teszt. M0 részben teljesült; valódi CPU-utasítások, IRQ-belépés, firmware/MIDI/audio nem igazolt. A jelölt saját platform-CI-je külön ellenőrizendő.

## 2026-10-28 — Első valódi generált H8-kezelők

PR: [#47](https://github.com/RobCZart82/yaman1x/pull/47). Ág: `feat/h8-first-generated-handlers`, alap: beolvadt #46. [Részletes bizonyíték és korlátok](h8-first-instructions.md). Saját regiszter-/prefetch-/részutasítás-állapot, eredeti generált reset/NOP/MOV.B/BRA kód, explicit nem támogatott opcode-hiba. 200000 szintetikus szakasz összevetve az eredeti H8H-dekóderrel és futtatóval, 899400 olvasás, nulla eltérés; szándékos hibát a kontroll észleli. M0 részben teljesült. Következő: adatmemória-műveletek, feltételes elágazások, IRQ-belépés és CPU/SCI-sorrend. A teljes CPU-/firmware-/MIDI-/audioelfogadás továbbra is nyitott.

Előző lépcső lezárása: [#46](https://github.com/RobCZart82/yaman1x/pull/46): merge `00e16ce805d542e4a654923ddde2d462ebac78bb`, végső head `478294b0570b0140b0ba2c5f78b65fcc99fecaa4`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467656), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467504), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467588).

## 2026-10-28 — H8H memória és rel8 feltételek

PR: [#48](https://github.com/RobCZart82/yaman1x/pull/48). Ág: `feat/h8-memory-conditional-branches`, alap: beolvadt #47. [Részletes tesztek és korlátok](h8-memory-branches.md). 44 eredeti generált full/partial metódus, háromszavas IR, teljes ER-címek és kétciklusos adatbusz-hozzáférés. 200000 eredeti referencia-szakasz, 844255 utasításolvasás, 27571 adatolvasás, 27574 adatírás: nulla eltérés. M0 részben teljesült. Következő: szó-/hosszúszó-műveletek, aritmetikai/összehasonlítási jelzőbitek, majd IRQ és CPU/SCI-sorrend. Teljes CPU-/firmware-/MIDI-/audioelfogadás továbbra is nyitott.

Előző lépcső lezárása: [#47](https://github.com/RobCZart82/yaman1x/pull/47): merge `343f4c32c81229e89ec092b0d8e324f2a0305f8a`, végső head `6f8107ec35e511c6aa9d347295e67a882a7826d3`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640131062), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130940), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130959).

## 2026-10-28 — H8H szó-/hosszúszó és prefixállapot

PR: [#49](https://github.com/RobCZart82/yaman1x/pull/49). Ág: `feat/h8-word-long-memory`, alap: beolvadt #48. [Részletes bizonyíték és korlátok](h8-word-long-memory.md). Eredeti 0100 prefix, külön belső dekódállapot, igazított word busz és kétlépcsős long átvitel. 200000 referencia-szakaszon nulla eltérés; mid-transfer folytatás/hiba és unsigned címkörbefordulás tesztelve. M0 részben teljesült. Következő: aritmetikai/összehasonlítási jelzőbitek, szélesebb utasításkészlet, majd IRQ és CPU/SCI-sorrend. Firmware/MIDI/audio-elfogadás változatlanul nyitott.

Előző lépcső lezárása: [#48](https://github.com/RobCZart82/yaman1x/pull/48): merge `800f8b4eabfe484b54e9918a652096c0acf50986`, végső head `a2da3e66797ec5d9f617e3e85b1e6505ccf26beb`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345253), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345183), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345179). Az automatikus review a beolvasztáskor még futott; nem lezárt eredményként szerepel. A beolvasztás a kézi forrásellenőrzés, eredeti kódos kontrollok és mind az öt sikeres CI alapján történt.

## 2026-10-28 — H8H aritmetika és összehasonlítás

PR: [#50](https://github.com/RobCZart82/yaman1x/pull/50). Ág: `feat/h8-arithmetic-flags`, alap: beolvadt #49. [Részletes bizonyíték és korlátok](h8-arithmetic-flags.md). Byte ADD/CMP immediate, byte ADD/SUB/CMP register, word/long ADD/SUB/CMP immediate; H/N/Z/V/C és CMP nem-írás ellenőrizve. 200000 eredeti referencia-szakasz, nulla eltérés; független signed-range/maradékos orákulum és half-carry negatív kontroll. M0 részben teljesült. Következő: további regiszteres/carry-input aritmetika, logika/shift, stack/IRQ és CPU/SCI-sorrend. Firmware/MIDI/audio-elfogadás változatlanul nyitott.

Előző lépcső lezárása: [#49](https://github.com/RobCZart82/yaman1x/pull/49): merge `4567d7bfe852100f5483a8104bcba81b50ae26bb`, végső head `f69503487de2fbbf3c575d81f54ca5a91e965f90`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971856), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971826), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971835). Az automatikus review a `5aff015d6157f166d9f26d0591e7b13d067cd171` forrásrevízión hibát nem jelzett; a követő commit csak PR-metaadatot rögzített a dokumentációban.

## 2026-10-28 — Carry, regiszteres aritmetika és logika

Ág: `feat/h8-carry-register-logic`, alap: beolvadt #50. [Részletes bizonyíték és korlátok](h8-carry-register-logic.md). ADDX/SUBX bemenő carry és sticky Z; wide register ADD/SUB/CMP; byte és wide immediate logika; LDC CCR. 200000 eredeti referencia-szakasz: nulla eltérés; több mint egymillió carry/Z és 20000 többbyte-os lánc matematikai kontrollal. M0 részben teljesült. Következő: wide register logika, shift/rotate és bitműveletek, majd stack/IRQ és CPU/SCI. Firmware/MIDI/audio-elfogadás továbbra is nyitott.

Előző lépcső lezárása: [#50](https://github.com/RobCZart82/yaman1x/pull/50): merge `0f8d3ae96eb32dcabb3a039f40e5c0fd0315a1b9`, végső head `4bbea3449f5d73f15031332b537919b493a5defc`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119562), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119555), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119545). Automatikus review a `ddc8d51221838e64c9e849101756c1aeca260dce` forrásrevízión: kész, nincs találat; a követő commit csak PR-metaadatot rögzített.

Korabeli jelöltbejegyzés: carry/regiszter/logika fejlesztés: [PR #51](https://github.com/RobCZart82/yaman1x/pull/51), `feat/h8-carry-register-logic`. Helyben 32 CTest-csoport és 83 Python-eset PASS; egy symlink-jogosultsági eset kihagyva. A végső head teljes CI-je külön ellenőrzendő; M0 részben teljesült.

## H8-WIDE-LOGIC-2026-10-28 — széles regiszteres logika

Előző lépcső lezárása: [#51](https://github.com/RobCZart82/yaman1x/pull/51), merge `ba2c91d8a156972bcf3c7b9adab98e36f7279b0c`, végső head `931a5a50dfe57c575d45d9d8b6b817889501a5a0`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821739), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821601), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821621). A forrásrevízió (`df6ce5574346e32b57a81dffb5c5b5f0e7929bbf`) automatikus review-ja kész, nincs találat; a követő commit dokumentációs PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-wide-register-logic`, alap: beolvadt #51. 138 eredeti generált full/partial kezelő: word/long regiszteres OR/XOR/AND és eredeti 01f0 prefix. 196608 word-eset, 245760 regiszter/CCR-kombináció, 65536 prefixkiterjesztés és 1000 felosztás PASS. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; XOR helyett OR hibát a kontroll észleli. [Szerződés és korlátok](h8-wide-register-logic.md). Helyben 33 natív CTest-csoport PASS; a teljes 34 csoport / 84 Python-eset végső CI-je külön ellenőrizendő. M0 részben teljesült; shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott.

Korabeli jelöltbejegyzés: széles regiszteres logika: [PR #52](https://github.com/RobCZart82/yaman1x/pull/52), `feat/h8-wide-register-logic`. Helyi 33 natív csoport / 83 Python-eset PASS; a teljes végső head CI-je külön ellenőrizendő.

## H8-SHIFT-ROTATE-2026-10-28 — egybites eltolás és forgatás

Előző lépcső lezárása: [#52](https://github.com/RobCZart82/yaman1x/pull/52), merge `88e88bd8551f78bc312230d459ea3c92d7b66f01`, végső head `eac6be6a95e22f1266730c6941efa0c766658913`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124816), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124527), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124813). A `c726582b72c43bc87cc917f1ff60c10f762a768a` forrásrevízió review-ja kész, nincs találat; a későbbi dekódersorrend-védelmet célzott és eredeti kódos teszt igazolja. Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-shift-rotate`, alap: beolvadt #52. 186 eredeti generált full/partial kezelő; SHLL/SHAL/SHLR/SHAR/ROTXL/ROTL/ROTXR/ROTR 8/16/32 biten. Független bitenkénti orákulum, signed-range overflow, 1744896 operandus/CCR/regisztersáv-eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; a megerősített explicit carry-program a kihagyott ROTXR carry-t seed 2/slice 8 helyen észleli. [Szerződés és korlátok](h8-shift-rotate.md). M0 részben teljesült; kétbites változatok, bitműveletek, stack/IRQ és CPU/SCI-integráció nyitott. A jelölt teljes CI-je külön ellenőrizendő.

Korabeli jelöltbejegyzés: egybites shift/rotate fejlesztés: [PR #53](https://github.com/RobCZart82/yaman1x/pull/53), `feat/h8-shift-rotate`. Helyi 34 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-REGISTER-BITS-2026-10-28 — regiszteres bitműveletek

Előző lépcső lezárása: [#53](https://github.com/RobCZart82/yaman1x/pull/53), merge `806ddf86a6373731f7b138c985a55ed41c583d6a`, végső head `b234b460a19d63d9a6c80e2086a4172a9a313a26`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288385), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288495), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288416). A `8ea376e0ba91bbc83ff94b521c5f0082bb093811` forrás review-ja kész, nincs találat; követő commit csak PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-register-bit-operations`, alap: beolvadt #53. 222 eredeti generált full/partial kezelő; regiszteres/immediate BSET/BNOT/BCLR/BTST, BST/BIST és carry logikai/load formák. 8126464 független operandus/bitindex/CCR/átfedési eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a BNOT helyett BSET hibát seed 3/slice 51 helyen felismeri a kontroll. [Szerződés és korlátok](h8-register-bit-operations.md). **Tervpontosítás:** a kétbites shift/rotate az eredeti listában `s20` (H8S/2000), a H8H-dekóderben nincs; nem H83002-fejlesztési feladat. Memóriás bitműveletek, stack/control flow és IRQ/SCI-integráció nyitott; M0 részben teljesült. Végső jelölt-CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: regiszteres bitműveleti fejlesztés: [PR #54](https://github.com/RobCZart82/yaman1x/pull/54), `feat/h8-register-bit-operations`. Helyi 35 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-MEMORY-BITS-2026-10-28 — indirekt memóriás bitműveletek

Előző lépcső lezárása: [#54](https://github.com/RobCZart82/yaman1x/pull/54), merge `efdb6df59ba53e8497d313fb33c7b40dcd9d76c4`, végső head `f9cab7c831a6aa2e880e2131b52a3623c049347c`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059555), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059577), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059727). Automatikus review-eredmény a lezáráskor nem volt; helyi forrás-/független/eredeti kódos kontroll ellenőrizve. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-memory-bit-operations`, alap: beolvadt #54. 262 eredeti generált full/partial kezelő: 7c/7d prefix és 18 indirekt bájtos bitművelet. 7340032 immediate eset, 131072 cím/indexátfedés, 131072 prefixkiterjesztés, 31 megszakítási pont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; read/prefetch/write sorrend és pontosan egyszeri mellékhatás ellenőrizve. [Szerződés és korlátok](h8-memory-bit-operations.md). M0 részben teljesült; abszolút memóriás formák, stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: indirekt memóriás bitműveleti fejlesztés: [PR #55](https://github.com/RobCZart82/yaman1x/pull/55), `feat/h8-memory-bit-operations`. Helyi 36 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-ABSOLUTE-BITS-2026-10-28 — abszolút memóriás bitműveletek

Előző lépcső lezárása: [#55](https://github.com/RobCZart82/yaman1x/pull/55), merge `e53b6fe7e2906b472e898c57d59ccfe1b791fa96`, végső head `452f9585afe1e67ceb57cd9cc8ac258202cd3cf7`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897203), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897191), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897202). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-absolute-bit-operations`, alap: beolvadt #55. 302 eredeti generált full/partial kezelő; 7e/7f prefix és 18 abs8 bitművelet. 7340032 immediate eset, 262144 cím/regisztersáv eset, 131072 prefixkiterjesztés, 31 futtatási vágáspont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés. [Szerződés és korlátok](h8-absolute-bit-operations.md). M0 részben teljesült; stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: abszolút memóriás bitműveleti fejlesztés: [PR #56](https://github.com/RobCZart82/yaman1x/pull/56), `feat/h8-absolute-bit-operations`. Helyi 37 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-STACK-CONTROL-2026-10-28 — veremkezelés és vezérlésátadás

Előző lépcső lezárása: [#56](https://github.com/RobCZart82/yaman1x/pull/56), merge `a3c2eb8ba25fc40eaa5057cc388260e4fc58b9df`, végső head `dee08721b3c2d3ccb16e1dfa4b4475625889c962`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502703), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502638), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502838). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-stack-control-flow`, alap: beolvadt #56. 366 eredeti full/partial kezelő; predecrement/postincrement MOV byte/word/long, JMP/JSR mindhárom címzése, BSR rel8/rel16, mind a 16 rel16 feltétel, RTS és szintetikus RTE-veremkép-visszaállítás. Független előjeles eltolás-/alias-/címhatár-/CCR-próbák, 201 beágyazott és 388 további hívás/RTE vágáspont, buszhiba-kontroll. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a hibás RTS SP+2 értéket seed 6/slice 5 helyen felismeri a kontroll. [Hatókör, bizonyíték és fennmaradó korlátok](h8-stack-control-flow.md). A normál verem/vezérlés utasításcsaládok ebben a rögzített H8H-profilban megvalósítva; IRQ/TRAPA-belépés, teljes CPU és CPU/SCI-integráció nyitott, M0 részben teljesült. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: verem/vezérlésátadási fejlesztés: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), `feat/h8-stack-control-flow`. Helyi 38 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő. Normál utasításcsaládok készültek el; IRQ/TRAPA-belépés továbbra is nyitott.

## DOC-RECONCILE-2026-10-28 — aktuális és történeti állapot egységesítése

A #57 lezárása a motoralaphoz rögzítve, a strukturált nyilvántartásban nincs nyitott motorjelölt. STATUS és roadmap egységesítve; korábbi jelöltbejegyzések megőrizve és történetiként jelölve. Az M0 következő feltétele már a hiányzó lefedettségre és gépintegrációra vonatkozik, nem az első CPU-fordításra. Dokumentációs javítás, új motor-/firmware-/audioelfogadás nélkül. Ellenőrzés: élő #57 merge/head és mind az öt CI-feladat, JSON-lezárás és egyedi PR-azonosítók, UTF-8 és helyi fájlhivatkozások, forrás- és értesítéslefedettségi audit.

A dokumentációs egységesítés PR-je: [#58](https://github.com/RobCZart82/yaman1x/pull/58). A motoralap továbbra is a lezárt #57; a dokumentációs PR saját aktuális CI-/merge-állapotát a hivatkozott PR mutatja.

## 2026-10-28 — repository-audit a #58 főágon

[Auditjelentés](REPOSITORY_AUDIT_2026-10-28.md): a 39 csoportos/84 Python-esetes csomag Macen teljesen PASS, 200000 eredetimetódus-szelet eltérés nélkül, célzott CPU ASan/UBSan PASS. Az ellenőrzés nem igazol firmware-, hardver- vagy audioelfogadást; M0/M1 állapota változatlan. A következő feladatok a megmaradt utasítás/címzés-lefedettség, IRQ/TRAPA és a közös gépütemezés.

## 2026-10-28 — lefedettség és CCR

[Jegyzőkönyv](h8-coverage-and-ccr.md): forrásazonosított kódolásleltár (181 kiválasztott / 83 hiányzó / 135 másprofilú sor), STC/LDC regiszteres és ORC/XORC/ANDC immediát család. Ez nem ISA-készültségi százalék és nem IRQ-belépési bizonyíték. Következő a fennmaradó utasítás/címzéscsaládok, majd IRQ/TRAPA és gépütemezés.

## 2026-10-28 — regiszterközi MOV

[Jegyzőkönyv](h8-register-transfers.md): mindhárom szélesség, teljes lane/alias megőrzés és N/Z/V szemantika. A forrásleltárban 80 hiányzó H8H-sor marad. CPU IRQ/TRAPA és gépütemezés továbbra is nyitott.

## 2026-10-28 — INC/DEC és kis cím-aritmetika

[Jegyzőkönyv](h8-small-arithmetic.md): 16 csoport, flagmegőrzés és részutasítás-felosztás tesztelve. A leltárban 64 hiányzó H8H-kódolási sor marad. M0/M1 készültsége ettől nem teljes.

## 2026-10-28 — unáris műveletek és kiterjesztés

[Jegyzőkönyv](h8-unary-extension.md): tíz új csoport, 434 full/partial kezelő; 54 hiányzó H8H-sor marad. Az eredetimetódus-próbák új családjai elérhető ciklusban és bejárási őrfeltétellel futnak. A mérföldkő nem IRQ-, firmware- vagy hangelfogadás.

## 2026-10-28 — szinkron TRAPA-belépés

[Jegyzőkönyv](h8-trapa-entry.md): default SYSCR-profil, négy vektor, eredeti stack/busz/időzítés. Külső IRQ/NMI, szűrők és gépütemezés továbbra is nyitott; hardveres trap-visszatérés nincs igazolva.

## H8-DISPLACEMENT16-2026-10-29 — 16 bites eltolású memóriaátvitel

Ág: `feat/h8-displacement16-memory`, alap: beolvadt #77. Hat eredeti MOV byte/word/long
load/store csoport, 12 változatlan full/partial metódus: 478 összesen;
231 kiválasztott / 33 hiányzó / 135 kizárt kódolássor. Mind a 65536 eltolás,
regiszterátfedések, teljes ER-címkörbefordulás, CCR és szóigazítás tesztelve.
201 felosztás és kétlépcsős store-hiba; eredeti dekóderrel 200000 szakasz nulla eltéréssel.
Mind a hat új forma és a korábbi referencia-családok elérésellenőrzése megmarad.
[Részletes bizonyíték és korlátok](h8-displacement16-memory.md). A jelölt teljes csomagja
56 CTest / 87 Python; firmware/audio-elfogadás továbbra sincs. Végső PR-CI külön bizonyíték.

Az eltolásos memóriautasítások PR-je: [#78](https://github.com/RobCZart82/yaman1x/pull/78). A készítéskori jelöltállapot és a végső CI-/merge-bizonyíték külön kezelendő.

## 2026-10-29 — Előjel nélküli bájtszorzás jelölt

Az eredeti MULXU.B full/partial kezelők kiválasztva; fix H8H MAC nélküli időzítés.
65536 operanduspár,19200 alias/CCR-határeset és161 szétvágott futás PASS;
200000 eredeti referencia-szelet nulla eltéréssel; szándékos product+1 hiba felismerve.
Helyi56 CTest/86 Python PASS, ismert Windows-symlink eset kizárva; távoli CI függőben.
Jelölt:57 CTest/87 Python,480 eredeti módszer,232 kiválasztott/32 hiányzó/135 kizárt sor.
A szó-szorzás külön nyitott: az eredeti u16 operandusok int-promóciója nagy szorzatnál
C++-túlcsordulási kockázatot jelent. Bekötés előtt független nagyértékű teszt,
UBSan és dokumentált unsigned-szélesítés felülvizsgálata szükséges.
Ez lehetőség és igazolandó javítási feladat; nem hardveres hibamegállapítás.
Részletek: [szerződés és bizonyíték](h8-byte-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-szorzás jelölt

MULXU.W: két eredeti eredetű full/partial kezelő minimális, dokumentált javítással:
szorzás előtt u32-szélesítés, hogy a u16 operandusok int-promóciója ne csorduljon túl.
A korábbi480 kezelő változatlan;482 eredeti eredetű kezelőből2 adaptált.
327680 operandus-söprés,9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS.
200000 eredeti referenciaszelet nulla eltéréssel, csak biztonságos szorzatokkal;
a nagy szorzatokat független64 bites oracle ellenőrzi. Product+1 kontroll:seed9/slice9.
Helyi57 CTest/86 Python PASS, ismert Windows-symlink eset kizárva. Jelölt:58 CTest/87 Python,
233 kiválasztott/31 hiányzó/135 kizárt kódolássor. Linux UBSan és külön eredeti-hibakontroll
bekötve; tényleges távoli eredmény még függőben. A szó-szorzás nyitott kockázata
csak ezek sikeres lezárása után tekinthető rendezettnek; ez nem hardverelfogadás.
[Szerződés, pontos adaptáció és bizonyítékok](h8-word-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjeles bájt- és szó-szorzás jelölt

Eredeti01c0 prefix és MULXS.B/MULXS.W full/partial kezelők; külön őrzött folytatás.
Az eredeti set_nz16/set_nz32 csak N/Z-t módosít. Független64 bites számítás:
65536 bájtpár,327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár PASS;
241 szétvágott futás és65536 prefixfolytatás ellenőrzött. Eredeti referenciaszeletek:
200000,nulla eltérés; mindkét forma elérése kötelező. Product+1 kontroll seed9/slice14.
Jelölt:59 CTest/87 Python,488 upstreamből származó kezelő (486 változatlan,2 korábbi
unsigned-adaptáció);235 kiválasztott/29 hiányzó/135 kizárt kódolássor. Linux UBSan
mindkét új formát is teszteli; távoli CI függőben, M0 továbbra is részleges.
[Szerződés és részletes bizonyítékok](h8-signed-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli bájtos osztás jelölt

Eredeti DIVXU.B full/partial kezelők, külön5100/ff00 dekóderág. Nullás osztónál
célregiszter-megőrzés; hányados alacsony8 bitje és maradék külön bájtban; N/Z
az eredeti osztófüggő szabályt követi. Ez forrásegyezés, nem hardveres bizonyíték.
393216 osztandó-söprés,2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár
és241 szétvágás PASS;200000 referenciaszelet nulla eltéréssel. Hányados+1 kontroll
seed9/slice18 felismerve. Jelölt:60 CTest/87 Python,490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott/28 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve osztásra is; távoli CI függőben. M0 továbbra is részleges.
[Szerződés és ellenőrzési hatókör](h8-byte-divide.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-osztás jelölt

DIVXU.W, külön5300/ff08 dekóderág. A hányados/maradék u32 típusa explicit;
csomagolás unsigned32 marad. Két dokumentált típusadaptáció, nem C++20 UB-mérés.
Az eredeti N az osztó7. bitjét követi, nem a15.-et; hardvermérés nélkül ezt megőrizzük.
524288 osztó-söprés,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS;
200000 biztonságosan korlátozott referenciaszelet nulla eltéréssel. Nagy értékek
független64 bites számítással ellenőrizve; hányados+1 kontroll seed9/slice26.
Jelölt:61 CTest/87 Python,492 upstreamből származó kezelő (488 változatlan,4 adaptált);
237 kiválasztott/27 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve szó-osztásra;
távoli CI függőben, M0 továbbra is részleges. [Szerződés](h8-word-divide.md).
Yamaha-adat nem került be; firmware/MIDI/hang/plugin elfogadás nincs.

## 2026-10-29 — M0 decimális korrekciós részlépés: DAA/DAS jelölt

Négy változatlan eredeti full/partial kezelő, külön0f00/fff0 és1f00/fff0 dekóderág.
Független tízes számrendszerű számítás: minden0–99 operanduspár összeadás/kivonás,
két CCR-minta (40000 eset);2048 regiszterhatáreset és241 szétvágott futás PASS.
Érvénytelen BCD/flag kombinációkra nem állítunk teljes független helyességbizonyítást.
200000 eredeti referenciaszelet nulla eltéréssel, mindkét új forma és korábbi családok
elérése kötelező; DAA korrekció+1 kontroll seed7/slice0 felismerve. Jelölt:62 CTest/87 Python,
496 upstreamből származó kezelő (492 változatlan,4 korábbi adaptáció);239 kiválasztott/
25 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve. M0 továbbra is részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. [Szerződés](h8-decimal-adjust.md).
Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles bájtos osztás jelölt

Változatlan eredeti01d0 prefix és DIVXS.B kezelők, őrzött0xf0000 folytatás.
Független64 bites számítás:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás és65536 prefixfolytatás PASS. A hányados nullához
csonkolódik,maradék az osztandó előjelét követi; N a teljes hányados előjele,Z a nullás
osztó. Eredeti kommentek helyett a matematikát és futó kódot ellenőrizzük; hardverigazolás nincs.
200000 referenciaszelet nulla eltéréssel; hányados+1 kontroll seed7/slice3 felismerve.
Jelölt:63 CTest/87 Python,500 upstream-kezelő (496 változatlan,4 korábbi adaptáció),
240 kiválasztott/24 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve; CI függőben.
Szó-osztás INT32_MIN/-1 és csomagolás külön nyitott vizsgálat. [Szerződés](h8-signed-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles szó-osztás és szélső hányados jelölt

Két változatlan eredeti DIVXS.W kezelő, őrzött meglévő01d0 folytatás;32 bites int
fordítási feltétel. Az eredeti unsigned32 segédértékek elkerülik a közvetlen signed
INT_MIN/-1 osztást. A matematikai +2147483648 hányadost az eredeti int32-re szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres hibajavítás vagy mérés.
524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,1024 szélső/CCR-eset,321
szétvágás és128 tiltott ER-szomszéd PASS. Független64 bites számítás és külön
szűkítési flagmodell.200000 eredeti referenciaszelet nulla eltéréssel; fordított
N kontroll seed7/slice7 felismerve. Jelölt:64 CTest/87 Python,502 upstream-kezelő
(498 változatlan,4 korábbi adaptáció),241 kiválasztott/23 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve; CI függőben. [Hatókör és szélső eredmény](h8-signed-word-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nincs.

## 2026-10-29 — M0 32 bites eltolásos bájt/szó-memóriamozgatás jelölt

Eredeti7800 prefix és négy MOV-csoport,tíz változatlan full/partial kezelő.
Meglévő négyszavas IR,őrzött160000 folytatás,full32 cím és külön24 bites fizikai
leképezés.1048576 eltolásfél-söprés,55296 cím/regiszter/CCR-határeset,241 szétvágás,
65536 prefixfolytatás és memóriahibás fail-stop tesztek PASS. Nem teljes2^32 keresztszorzat.
A prefixkontroll nyolc hibás belső-állapot újradekódolást fogott; az elsőszó-maszk
szűkítése rendezte.200000 eredeti referenciaszelet nulla eltéréssel; felső eltolásszó
elhagyása kontroll seed6/slice5 felismerve. Jelölt:65 CTest/87 Python,512 upstream-kezelő
(508 változatlan,4 korábbi adaptáció),245 kiválasztott/19 hiányzó/135 kizárt sor.
Hosszú szó32-eltolással továbbra is nyitott. Linux UBSan kiterjesztve; CI függőben.
[Szerződés és hatókör](h8-displacement32-memory.md). M0 részleges; firmware/MIDI/hang/plugin
elfogadás nincs. Yamaha-adat nem került be.

## MAC_M1_HANDOFF

# Mac M1: az M1 firmware/SCI/MIDI-próba folytatása

Állapot: 2026-10-27. A teljes arm64 MAME kutatási build már igazolt CI-ben;
a friss firmware-es MIDI-próba még **nem futott**. Az alábbi lépéseket a Macen
folytató fejlesztő/Codex hajtsa végre a helyi környezet ellenőrzése után.
Ez nem elkészült H8-port és nem igazolt AN1x boot.

## Szükséges helyi anyagok

- A repository aktuális `main` ága; a munka elején ellenőrizni a Git-állapotot.
- A felhasználó beérkező Yamaha-mappája a Macen, ténylegesen letöltött fájlokkal.
  A Windowsos abszolút útvonalat nem szabad Mac-útvonalként használni.
- A változatlan, ismert AN1x 1.04 firmware BIN vagy ZIP formában. Többfájlos
  ZIP-nél az eredeti tag neve a `--member` kapcsolóval adható meg.
- A kipróbált kutatási fordítókörnyezet: Xcode 16.2; Python, Git, CMake és Make.
  A telepített verziókat és az arm64 architektúrát előbb helyben ellenőrizni.
  Xcode 15.4 a rögzített MAME-forrással már elbukott; ne ezt használjuk vakon.

A fizikai AN1x nem kell ehhez a szoftveres próbához. Az EG-, órajel- és
hangzásazonosság igazolása továbbra is külön hardveres/referenciamérési feladat.
ROM, bank és nyers napló nem kerülhet Gitbe vagy CI-be. A OneDrive tartalmát
helyi bemenetként kell kezelni; újabb archívumra ne cseréljük észrevétlenül
a [rögzített buildalapot](source-identity-recovery.md).

## Ellenőrzés és build

A repository gyökerében, a forrásállapot megőrzésével:

Másik gépről átmásolt `build` mappát ne használjunk. Ha ilyen már van,
válasszunk új buildkönyvtárat; ne töröljünk felhasználói munkát.
Az Xcode útvonalát csak akkor állítsuk így, ha a csomag ott valóban telepítve van.

A teljes kutatási programhoz külön, tiszta MAME-checkout szükséges:

Ha `work/mame` már létezik, előbb megvizsgálni: az előkészítő szándékosan
elutasítja a módosított vagy már patchelt checkoutot. Ilyenkor használjunk
új könyvtárat, vagy igazoljuk a meglévő build eredetét; ne alkalmazzuk újra
a patch-láncot. A ROM-mentes regisztrációs próba még nem firmware-futtatás.

## Egy parancs a párosított helyi kísérlethez

A firmware útvonalát a Macen található eredeti fájlra kell cserélni.
Az eredménykönyvtárnak újnak kell lennie:

A futtató generált SMF formátumú MIDI-t készít: explicit 500000 µs/negyed
tempó, 480 tick/negyed, 60-as hang, 64-es velocity, két másodperc után note-off.
A MAME fájllejátszása tíz másodperc után indul; a két futás 14-14 emulált
másodpercig tart. Külső hangszínbankot nem használ.

Mindkét friss processzusban DMS hiányzik, bridge nincs, EG-readback nulla,
DSP-mintaléptetés kikapcsolva. Csak a `passive` és `firmware-contract`
státuszprofil különbözik. Az örökölt `YAMANIX_*` és `VOP3_INTERP` környezeti
beállításokat a futtató törli, majd a gyermekpróba explicit kapcsolókat kap.
Ez szándékosan nem a több profilt egyszerre változtató régi összehasonlítás.

## Mit ellenőrzünk az eredményben?

A helyi `suite.json` tartalmazza a projekt revízióját/módosítottságát, a közös
firmware-, végrehajtható- és MIDI-azonosítókat, valamint a két SCI-csatorna
megfigyeléseit. Hibánál a suite nem lesz sikeres; a már létrejött futások és
indítási naplók megmaradnak helyben. Régi, SCI-hozzáférés-megfigyelő nélküli
MAME-programmal a suite elutasítja a hiányzó bizonyítékot.
Az eseményszámoknak egyezniük kell a külön SCI-zárószámlálókkal; a MIDI
vonalátmenetek zárószámlálója is kötelező, de nulla érték is megfigyelési eredmény.

`suite_complete=true` kizárólag a párosított megfigyelés befejezését jelenti.
A `firmware_note_handling_verified`, `complete_an1x_boot`, `hardware_validated`
és `pks_handshake_verified` mezők ettől még hamisak maradnak. RDR-olvasás
lehet elavult adat olvasása; önmagában sem ez, sem az RXD-vonal mozgása nem
bizonyítja a note-on/note-off feldolgozását.

Következő elemzés: az SCI1 inicializáló írásai és az SCI0 SCR-engedélyezések
PC-jeinek követése privát firmware-disassemblyben; szükség esetén SSR/RDR
összefüggések és ismert bájtsorozat igazolása. PKS-választ csak bizonyított
protokoll alapján szabad hozzáadni. A futtató jelenlegi szintetikus tesztjei
a szervezést és az elutasítási feltételeket vizsgálják; valódi Mac-firmware
eredménynek nem számítanak. Az új helyi eredményeket dátummal, revízióval és
tesztstátusszal fel kell vezetni a [fejlesztési tervbe](FIRST_REAPER_SOUND_PLAN.md).

## REPOSITORY_AUDIT_2026-10-27

# Repository-vizsgálat — 2026-10-27

Vizsgált `main`: `276399870400bd81272c21889257984651c816ed`, beolvadt #37.
Az auditot követő jelölt javítások külön PR-ben kerülnek ellenőrzésre.
Az induló leltár 354 követett fájlt tartalmazott.

## Eredmény és az igazolás határa

A jelenlegi kódbázis működő, szintetikus tesztekkel ellenőrzött kutatási alap.
A tiszta helyi macOS arm64 build minden 22 CTest-csoportja és 56 Python-esete
sikeres volt. A tényleges termék továbbra sem kész H8-motor vagy hangszer:
M0 részben teljesült, M1 előkészített, M2–M6 nyitott. AN1x-hang, VST3/AU és
REAPER-elfogadás nincs igazolva.

A repository egészének leltárát, saját kódját, tesztfelületeit, build-/CI- és
kutatási határait vizsgáltuk. A külső 201 fájlon tartalomazonosságot és az egy
regisztrált VOP3-javítás visszafordíthatóságát ellenőriztük; ez nem a teljes
AsmJit/MAME forráscsalád új biztonsági vagy hardverhelyességi auditja.

## Átvizsgált területek

| Terület | Ellenőrzés és megállapítás |
|---|---|
| CMake és alap-CI | 22 tesztcsoport; három platformmátrix; az ABI-próbák normál és optimalizált Python alatt is futnak. Az alap-CI nem firmware-/audio-elfogadás. |
| Saját DSP-kód | Bridge ABI, argumentum-/maszkhatárok, egész mintaóra, kétchip-állapot, szintetikus soros próba és feltöltés-visszajátszás áttekintve, tesztelve. Táblarom-bekötés és audioút még hiányzik. |
| H8-segédek | GPIO, IRQ-választó és láb-/regiszteradapter, SCI ciklusóra/SSR és közös TX/RX tulajdonos áttekintve. A natív byte/format, hibajelzés, sorrend és izolációs tesztek PASS; teljes CPU-integráció nincs. |
| Python bemenet/trace | ROM/SYX/table/upload ellenőrzők, SCI/DSP stream és kísérletfuttatók áttekintve. A bridge audit formátumhibája reprodukálva és javítva, lásd F-01. |
| MAME-kutatás | A rögzített archívumból 11 patch előkészítése sikeres; 8 előállított fájl és injektált saját helper egyezik. A H8-generátor négy kimenete két független futásban egyezik a lockkal. |
| Forrás és licenc | 201 vendorizált fájl, egy visszafordítható helyi módosítás, 222 lefedett fájl és öt licencszöveg ellenőrzése PASS. Ez forrás-/notice-ellenőrzés, nem bináristerjesztési jóváhagyás. |
| Dokumentáció | Aktuális és történeti állítások összevetve a CMake-mel, patch-lánccal és PR-ekkel; elavult állítások javítva. A helyi Markdown-fájlhivatkozások feloldhatók. |
| Fejlesztési előzmény | A #6–#37 közötti 32 merge metaadata és commitja ellenőrizve. A részletes terv már tartalmazott dátumozott naplót és CI-lezárásokat; ezekhez külön központi index és vezetési szabály készült. |

## Konkrét hibák és javításuk

### F-01 — P2: a jelenlegi register-only trace téves elutasítása — javítva

Az előállított MAME-driver `BRIDGE state=live-register-only abi=2
hardware_validated=0` fejlécet ír, a kikapcsolt órajelet pedig külön
`DSP_SAMPLE_CLOCK profile=disabled rate_hz=0` sorban rögzíti. Az auditor
kizárólag a régi, `dsp_samples=0` mezőt tartalmazó formátumot fogadta el.
Így egy helyesen befejezett, mai register-only futás is elbukott a fejlécnél.

A szintetikus teljes capture-teszt a javítás előtt reprodukálta a hibát.
A javítás elfogadja az új formátumot a kikapcsolt órajel igazolásával, megőrzi
a régi ABI 1/2 felvételeket, és elutasítja a hiányzó/ellentmondó órajelet vagy
a register-only manifestben állított nemnulla mintaszámot. Négy regressziós
eset ellenőrzi az aktuális, régi, timed és hibás felvételeket normál és `-O`
módban. A DSP-modell és a MAME-patchek viselkedése ettől nem változik.

### F-02 — P2: több jelenidejű státuszállítás elavult — javítva

- README: tévesen azt állította, hogy a H8-futtató nem hív mintaléptetést.
  Most az alapértelmezett register-only és az opt-in timed profil külön szerepel.
- STATUS/HISTORY: a kilenc tesztcsoportos kezdeti snapshot aktuális állapotként
  jelent meg; a jelenlegi 22-es szám és a későbbi H8-komponensek bekerültek.
- STATUS: változatlan VOP3-snapshotot állított, miközben #22 óta egy dokumentált,
  visszafordítható konstruktorjavítás van benne. A leírás most ezt rögzíti.
- ROADMAP/research README: a már elkészült keresztplatformos CI és teljes
  macOS MAME-build még jövőbeli/hiányzó eredményként is szerepelt. Javítva;
  a friss firmware-/MIDI-elfogadás továbbra is nyitott.

A korabeli mérési jegyzőkönyveket és számokat nem írtuk át új eredménynek.

## Nyitott fejlesztési és bizonyítéki feladatok

| Azonosító | Sorrend | Következő konkrét eredmény |
|---|---|---|
| N-01 | M0 következő lépés | Két SCI-csatorna IRQ- és MMIO-bekötése a H8H-vezérlőhöz; közös eseményütemező, majd tényleges CPU-port és végrehajtási/izolációs próbák. |
| N-02 | M1 párhuzamos kutatás | A már elkészült Mac-kísérletfuttató friss helyi firmware-felvétele. SCI0 bájtfogadás és note-on/off feldolgozás igazolása, PKS-kézfogás bizonyítása. A 21 RXD-átmenet nem sikeres MIDI-elfogadás. |
| N-03 | M2–M3 | AN1x órajel, EG célon tartás, upload stop/run, táblacímzés és DMS-ROM-azonosság, soros száraz hangút. A 48 kHz továbbra is modellfeltevés. |
| N-04 | Reprodukálhatóság | A dokumentált nagy, ideiglenes upstream-összehasonlító fixture-ök nem mind részei a repository-nak. Ezek korábbi eredményeit az audit nem futtatta újra; tartós, licencelt, ROM-mentes reprodukciójuk későbbi erősítés. |
| N-05 | Naplóegyeztetés | A régi nyitott #2/#3 issue-leírások a timed profil előtti állapotot is tükrözik. A jelenlegi státusz a tervben/STATUS-ban van; issue-frissítéskor az eredeti hardveres elfogadási feltételeket meg kell tartani. |
| N-06 | M4–M6 és terjesztés | Offline referenciahang, határidőn belüli render, VST3/REAPER-próba és a ténylegesen terjesztett függőségek felülvizsgálata. A saját kód nyilvános terjesztési licence továbbra sincs kiválasztva. |

Az ismert upstream ICR-maszk- és EG-viselkedést nem módosítottuk pusztán
feltételezés alapján. A szintetikus tesztben megőrzött upstream-egyezés
nem azonos az AN1x hardverének validálásával.

## Friss ellenőrzési jegyzőkönyv

Platform: macOS arm64; AppleClang 21.0.0 (`clang-2100.3.34.2`), CMake 4.4.3,
Python 3.9.6; Release, explicit arm64 cél, új buildkönyvtár.

| Ellenőrzés | Eredmény |
|---|---|
| Vizsgált alapcommit tiszta configure/build/CTest | 22/22 PASS |
| Alapcommit Python unittest | 56/56 PASS |
| F-01 regresszió javítás előtt | Az aktuális register-only formátum tesztje hibázott a várt fejléc-elutasítással |
| F-01 regresszió javítás után, normál és `python -O` | Módonként 4/4 PASS |
| Végső javításjelölt teljes Python unittest | 60/60 PASS |
| Végső javításjelölt CTest | 22/22 PASS; a négy új eset a meglévő Python-csoport része |
| Lockolt archívum patch-preflight / H8 generálás | 11 patch, 8 kimeneti fájl; 3 generátorbemenet, 4 kimenet, 2 futás PASS |
| Forrás-/notice-audit és helyi dokumentumlinkek | PASS |
| Új teljes MAME-fordítás / új firmware-, audio- vagy REAPER-próba ebben az auditban | NOT RUN; a patch-lánc nem változott, korábbi MAME CI-t ellenőriztünk |
| Új helyi Linux MSan / Windows futás ebben az auditban | NOT RUN; a megjelölt CI-adatok külön bizonyítékok |

Az auditált alapcommit saját [ROM-mentes CI-je](https://github.com/RobCZart82/yaman1x/actions/runs/40504511349)
és [MSan-futása](https://github.com/RobCZart82/yaman1x/actions/runs/40504511264)
sikeres. A változatlan kutatási cél legutóbbi main-ági
[MAME-futása](https://github.com/RobCZart82/yaman1x/actions/runs/40501704765)
a #36 merge `33dc7e97c174c7b4992ff838380b2ae9de01a579` revíziójához tartozik;
nem a #37 vagy az auditjelölt újrafordításának állítjuk be.

A végső jelölt [#38 PR](https://github.com/RobCZart82/yaman1x/pull/38) saját
ellenőrzései a PR checkjeinél követhetők. A helyben tesztelt fa megegyezik a
`de892c3ea278611d1a60e68ccbad8991e174b768` közzétett revízió fájlfájával;
a követő commit csak a napló/ellenőrzési hivatkozásokat egészíti ki. A fenti
helyi jelölttesztek a kódjavítást és regresszióit tartalmazó munkafát vizsgálják;
a merge SHA csak beolvasztás után ismert. A nyilvános jegyzőkönyv nem tárol
személyes elérési utat, Yamaha-adatot vagy privát nyers logot.

## A fejlesztési napló további vezetése

[DEVELOPMENT_LOG.md](DEVELOPMENT_LOG.md) és
[development-ledger.json](development-ledger.json): PR, dátum, reviewed head,
merge commit, változásfajta és aktív auditbejegyzés. A részletes elfogadási
bizonyíték a REAPER-tervben marad. [CONTRIBUTING](../CONTRIBUTING.md) és a PR-sablon
rögzíti, mikor kell frissíteni ezeket. A sablon és a kézi nyilvántartás nem
garantál automatikus jövőbeli naplófrissítést; ezt a PR-review során ellenőrizni kell.

## REPOSITORY_AUDIT_2026-10-28

Átvizsgált főági revízió: `863d21abcde448d746b3b44c39a1eb61be455a15` (#58).
Motoralap: `f8b6aa20834f209931e3e9bb9463cc47b5b7a540` (#57).
Ez ellenőrzési pillanatkép, nem teljes CPU-, firmware-, hardver- vagy hangelfogadás.

## Eredmény

A vizsgált tesztekben és összehasonlításokban nem találtam reprodukálható
motorregressziót. A főági dokumentáció aktuális összefoglalója helyesen 39
CTest-csoportot és 84 Python-esetet jelez. Egy későbbi STATUS-bekezdés viszont
még „current CMake suite” megjelöléssel 27 csoportot írt; ezt 39-re javítottam,
a korábbi 22/56 és 60-as eredményeket kifejezetten történeti bizonyítékként hagyva.
A friss audit hivatkozásait README/STATUS/fejlesztési napló alatt egységesítettem.
Működő motorkódot nem kellett módosítani.

## Hatókör

- Build és három workflow: platformtesztek, VOP3 kezdeti dekódolási MSan,
  külön teljes MAME-kutatási build; ROM-mentes és helyi kutatási határok.
- Saját CPU futtató, költségkeret, busz, kézi dekóder/segédek; GPIO/IRQ/SCI,
  ciklus-/bemeneti idővonal; DSP bridge, mintavételi óra és upload-szerződés.
- Python bemenet-/napló-/bankellenőrzők és generátor-/referenciaeszközök;
  teszt-fixture-ek, független flag/bit orákulumok és hiba-/izolációs próbák.
- A #44 óta beolvadt fejlesztések (#45–#58), napló, státusz, elsőhang-terv,
  rögzített forráseredet, licencek, privát adatok kizárása és helyi hivatkozások.
- A 201 változatlan upstream fájl tartalmát hash-audit ellenőrizte, nem új,
  sorról sorra végzett AsmJit/VOP3-kódreview. Az egyetlen áttekintett vendor-
  javítás továbbra is a visszafordítható első-dekódolási inicializálás.

## Friss helyi bizonyíték — macOS arm64, Release

| Ellenőrzés | Eredmény és határ |
|---|---|
| Tiszta CMake-konfiguráció/build/CTest | 39/39 PASS; új buildkönyvtár, Yamaha-fájl nélkül |
| Python unittest discovery | 84/84 PASS; itt nincs Windows symlink-jogosultsági kizárás |
| Két rögzített generált kiválasztás | Eredeti generátorból kétszer előállítva; 366 full/partial metódus egyezik a kiválasztással |
| Eredeti H8H dekóder/host/execute_run összevetés | 200000 szelet, 668426 utasításolvasás, 46258 bájtolvasás, 25800 bájtírás, 67331 szóolvasás, 41382 szóírás; 0 eltérés |
| Külön dekóderkiválasztási próba | 917504 vizsgált állapot/fázis; eltérő vagy túl tág elfogadás nem jelentkezett a vizsgált csoportokban |
| ASan/UBSan | Verem/vezérlés, aritmetika, carry/logika, shift/rotate: 4/4 célzott natív csoport PASS |
| Forrás-/feltöltési audit | 425 fájl; 201 upstream azonosság és 1 áttekintett vendor-javítás, 0 probléma |
| Értesítés-/licenclefedettség | 229 fájl, 5 licencszöveg; 0 lefedettségi probléma. Nem bináris terjesztési/jogi jóváhagyás |
| Napló és dokumentumok | 52 bejegyzett merge commit elérhető; 0 hiányzó helyi Markdown-fájlhivatkozás |

A fájlszámok az átvizsgált alapra vonatkoznak; maga ez a jelentés és naplófrissítése
növeli a jelölt fájlszámát. Motor-tesztcsoportot vagy Python-esetet nem ad hozzá.

A külön dekóderpróba az összes 16 bites alsó állapotot ellenőrizte full és partial
fázisban a 0, 2, 0x11, 0x19, 0x1a, 0x1b, 0x1c felső állapotcsoportokban.
A módszer a kezelők végrehajtása helyett a kiválasztott metódus nevét hasonlította
az eredeti generált diszpécserhez, a jelenleg kiválasztott metódushalmaz határán.
Ez egyszeri helyi auditpróba, nem új CI-teszt vagy teljes H8 opcode-elfogadás.
A teljes állapot/busz eredetimetódus-próbát a repository eszköze reprodukálja:

Az opcionális referenciafuttatás új work könyvtárat és a meglévő hash-elt külső
forrásarchívumot igényli. Nem módosítja a letöltött archívumot vagy ROM-okat.

## GitHub-bizonyíték

A #58 dokumentációs headje `a331d76f4d6a9dd4ab5e6d4b61de0b5145055f1d`:
[három platform PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40690053399),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40690053451).
A dokumentációs útvonalszűrő miatt ezen nem kellett külön MAME-jobnak indulnia.
A #57 motorhead öt lezárt jobját a [STATUS](STATUS.md) és a
[verem/vezérlés komponensjegyzőkönyve](h8-stack-control-flow.md) őrzi.
Ez az audit nem indított új teljes MAME-buildet és nem végzett friss firmware-futást.

## Következő fejlesztési és ellenőrzési feladatok

1. A megvalósított H8H-részhalmaz bővítése előtt maradjon meg a szigorú profil-
   és dekóderhatár. IRQ/TRAPA-belépés, IRQ-szűrők és közös CPU/periféria-ütemezés
   még hiányzik; szintetikus RTE-visszaállítás nem igazol megszakításbelépést.
2. Érdemes a két kiválasztott generált fájl valódi újragenerálását és az
   eredeti dekóderrel való összevetést külön CI-kapuvá tenni. Jelenleg ezek
   opcionális helyi referenciaeszközök; a normál CI fixture-/hash-próbái és a
   teljes MAME-build nem helyettesítik a saját kiválasztás ilyen ellenőrzését.
3. A CPU-családok ASan/UBSan futtatását érdemes automatizálni. A meglévő MSan-job
   a VOP3 kezdeti dekódolási hibáját vizsgálja; nem a teljes CPU sanitizerfutása.
4. M1 firmware/PKS/MIDI-elfogadás, tényleges AN1x órajel/EG/táblacímzés és
   offline hang továbbra is nyitott. A VST3/REAPER szakasz ezek után következik.

A 2–3. pont teszt-infrastruktúra fejlesztési javaslat, nem most reprodukált
motorhiba. A vizsgálat nem indokolja a jelenlegi dokumentációs PR több
mérföldkövön át történő visszatartását. Sikeres saját ellenőrzések és review
után önállóan beolvasztható.

## STATUS

## Sikertelen írások értéke — #276 beolvadt — 2026-11-12

A `research/boot-probe-failed-write-context` ág a bounded firmware probe
`fault_access` mezőjében a sikertelen bájt-/szóírás megkísérelt értékét is
rögzíti; a hibás hozzáférés továbbra is fail-closed. A célzott tesztek pontosan
ellenőrzik a címet, szélességet és értéket: köztük bájt `0x5A`, szó `0x1234`,
és ADC-regiszter `0xA5`.

Négy friss, csak olvasó firmware-futtatás AN1x 1.04 ROM-mal: idle-panel
profil, `swap16`, 100,000,000 ciklus, 20,000,000 callback, AN3=0/1023 és
4,096/1,000,000 ciklusos szeletek. Mind a négy egy mintát adott a
`95,706,795` ciklusnál, majd megállt az `FFFFE8` byte-írásnál; az új mező
mindegyiknél `0x33`, post-fetch PC `1B84`, perifériaciklus `95,707,121`, hiba
`Unsupported ADC control mode`. Az egy mintán túli CPU-idő nem bizonyít
mintabemenet-függő firmware-hatást: a releváns nem-szeletfüggő számlálók
megegyeztek. A ROM hash `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`;
nem változott és nem került a repóba.

A lezárt MAME hivatkozás `706dbb73281320c54d7c2f16262c1c3786f2be01`; az ADC
forrása az `H8_ADC_3337` `FFFFE8` vezérlőírás `0x33` értékét szoftveres
AN0–AN3 pásztázó módként dekódolja. Ez kizárólag MAME-forrásértelmezés,
nem Yamaha-hardvervalidáció; új MAME build/futtatás nem volt. A helyi Release
build és teljes 138/138 CTest sikeres, a repository-audit és whitespace-check
sikeres. A PR változásai beolvadtak; sem ADC-pásztázás, sem boot, MIDI, hang vagy
REAPER-elfogadás nem került hozzáadásra.

PR [#276](https://github.com/RobCZart82/YamAN1x/pull/276) beolvadt
2026-11-12 15:50:27 UTC-kor, a végső `27fc4b0a0677b340a279439db7d54466c5f23f8d`
headről a `74f0dc545c79a89790ad6eef42370b06f73bd65a` main commitba.
A PR-head mind az öt ellenőrzése sikeres: Linux `131281742752`, macOS
`131281743186`, Windows `131281743094`, MSan `131281739988`, MAME research
`131281740553`. A Codex review a pontos headen észrevétel nélkül zárult
(issue-comment `6827690290`). GitHub review-objektum és review-szál nem volt;
a ruleset nulla jóváhagyást ír elő és rendezetlen szál nem volt. Kvótahiba
nincs jelezve, merge-bypass nem történt.
Az új main mindhárom kötelező platformellenőrzése, továbbá az MSan és MAME
research futása is sikeres: `131300475364`, `131300474485`, `131300475237`,
`131300473882`, `131300476046` (workflow runok `43252136161`,
`43252135897`, `43252136454`).

## PKS forrás-RAM íráselőzmény — jelölt — 2026-11-10

A következő, még be nem olvadt auditorváltozás a rögzített PKS-forrásolvasások
értékét a legutóbbi, ugyanarra az igazított RAM-szóra és bájtlane-re eső korábbi
írással is összeveti. Az ER0 indexének ki kell választania a megfigyelt szót és
a bájtlane-et; a byte-write és full-word write eseteket külön kezeli. Az
ellenőrzés a meglévő FA92 olvasás → FA94 célírás cím/érték/R0L/ER4/idő kapcsolat
előtt fut. Ez megfigyelt írás-olvasás eredetlánc, nem PKS/ADC-szemantika.

A 12 célzott forráskontextus-teszt és a teljes Python-tesztkészlet (125/125)
helyben sikerült. A 138 CTest-csoportot ez a Python-only változás nem módosítja;
teljes CTest, MAME-fordítás és új firmware-futtatás ehhez a jelölthöz még nincs
rögzítve. ROM-ot ez a lépés nem olvas, nem módosít és nem tesz a repóba. A
távoli ellenőrzések és a pontos PR-head review a jelölt PR kapui; ez a rész még
nincs beolvasztva, hardver/boot/MIDI/hang-elfogadást nem jelent.

PR [#269](https://github.com/RobCZart82/YamAN1x/pull/269) az eredeti
headről (`0afd0e86fd4845514c2e969d809d25880b9d7d64`) nyílt, a
`947d67950345c7e03362d40f25174fcbe3b1f268` alapra. Ezen a pontos headen
mindhárom kötelező platformellenőrzés sikeres (Linux `130053106191`, macOS
`130053106085`, Windows `130053105515`); az MSan is sikeres
(`130053107228`). GitHub review-objektum, review-szál és PR-komment egyaránt
0; kvótahibára nincs bizonyíték. A szabályzat 0 jóváhagyást ír elő, a review-
szálak rendezését követeli. A naplófrissítés új PR-headet készít, ezért az új
pontos head ellenőrzései még függőben vannak. A PR még nincs beolvasztva.

## an1x-boot-probe

A #186 utáni main mind5 CI-ellenőrzése sikeres. Változatlan1.04 ROM,privát
swap16,10000000 kért ciklus,1000000 callback-limit mellett a próba482160.
perifériaciklusnál megáll:byte data_readFFFFD6 (Port B),PC0x3F666,
instruction_state1864224/substate0. Szeletméret1/64/257 esetén azonos fault,
PC és perifériaidő;teljes visszatért szeletciklus rendre482158/482112/482132,
callback739440/116288/104976.9 GPIO,0 TX/clock él. A hibás szelet munkája nem
szerepel a completed_slice számlálóban,ezért ezek eltérése önmagában nem hiba.
Ez diagnosztikai részprofil,nem firmware-boot-,hardver- vagy hangelfogadás.

Rögzített eredeti MAME706dbb73281320c54d7c2f16262c1c3786f2be01,
h83002.cpp132. sor:FFFFD6 portb port_r/dr_w;159. sor utáni konfiguráció
PORT_B,resetDDR00,mask00. Ellenőrzött privát archívumpillanatképből olvasva.
A chipcímtér nem bizonyítja az AN1x külső huzalozását vagy bemeneti értékét.
Következő:Port B külső eszköz/protokoll szerződés vizsgálata meglévő kutatási
forrásokból,majd explicit bemeneti modell és független tesztek. A hibajelzést
nem oldjuk fel bizonyítatlan fix bemenettel. ROM SHA előtte/utána azonos:
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.

## Explicit MEG write observation — 2026-11-03

## Separate original MEG status-stub experiment — 2026-11-03

## an1x-p9-input

Helyi változatlan1.04 ROM,swap16,lcd-16mhz,500000 kért ciklus és1000000
callback-limit:szelet1/64/257 mind cycle_limit hiba nélkül,periféria500000,
PC270856,state22640/substate2,22 GPIO,0 TX/clock él. Callbackszám rendre
766218/119835/108101;ez ütemezési szeletfüggő,nem állapothűségi mérőszám.
10000000 ciklusra nyújtott kísérletek a callback-korlátnál álltak meg különböző
ciklusnál;ez nem következő eszközhiba és nem teljes boot. Az eredeti ROM SHA
előtte/utána azonos:4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe.
A memóriaoracle kezdetben esetenként új buszt épített;felesleges tömeges
memóriamásolást megszüntetve ugyanaz65536 eset egy leállított buszon fut.

## bus-map

# AN1x busztérkép – bizonyítékok és nyitott kérdések

Frissítés: az LC7985-mérföldkőben a firmware mindkét DSP-báziscímet (`0x440000`, `0x480000`) dinamikusan is elérte, 16 bites tranzakciókkal. Az alábbi korábbi statikus megállapításokat ez kiegészíti; teljes címtükrözési térképet még nem bizonyít. A bekötött LCD-portok leírása a `lcd-milestone.md` dokumentumban található.

2026-10-26. Az alábbi statikus alaptérképet a későbbi futtatási mérések egészítik ki. LCD-modell és választható VOP3-regiszterkapcsolat már van; a teljes perifériakészlet és a fizikai hardvervalidálás nincs kész.

| Tartomány / elem | Bizonyíték | Státusz |
|---|---|---|
| 0x000000–0x0FFFFF fő-ROM | Letöltött MAME yman1x.cpp és a helyi 1.04 kép | Bizonyított betöltési tartomány |
| 0x200000–0x23FFFF NVRAM | MAME driver, két HM628128 SRAM | MAME szerinti leképezés |
| 0xFFFD10–0xFFFF0F belső RAM 24 bites módban | MAME h83002_device::map | CPU-modell része; nem külső RAM |
| 0x440000 és 0x480000 | Firmware-mérés és rajz IC6 V31CS/V32CS | 16 bites elérések, mindkét DSP teljes programfeltöltése; teljes címtükrözés még nincs igazolva |
| H8 P92/RXD0, IC1 pin 16 | DM CN11 pin 1 → DMS CN1 pin 1 → DG | Aktív alacsony panelérzékelés; firmware Port 9 bit 2 → 0x23CBC3 jelenlétflag |
| DSP 0x22 (base + 0x44) | Firmware 0x0545E8 / 0x0515EE és trace | Kiválasztott csatorna előjeles szintje; külön kísérleti readback, fizikai hardvervalidálás nélkül |
| 0x400001 és közeli címek | Firmware-immediát minták, IC6 MEGCS | Effekt-interfész jelöltek; még nem megvalósított térkép |

## Kapcsolási rajz vizsgálata

A felhasználó AN1x-ServiceManual.pdf fájljának 51. PDF-oldalán teljes DM/DMS kapcsolási rajz található. Külön nagyítva ellenőriztük az IC6 74AC139 dekódert: az 1A/1B bemeneteken A18/A19, az 1Y0/1Y1/1Y2 kimeneteken MEGCS/V31CS/V32CS jelölések szerepelnek. A chipkiválasztás engedélyezését és minden tükrözött címtartományt még külön végig kell követni.

A főprocesszor H8/3002, a főpanelen és a DMS panelen is van VOP3. A fizikai billentyűzetvezérlő továbbra is külön, hiányzó belső ROM-mal szerepel.

## Firmware-indítás

A word-swap utáni resetvektor 0x000009D8. A resetcím környezetében a veremmutató 0x00FFFEFE értékét betöltő MOV.L mintája található. Ez a MAME 24 bites H83002 belső RAM-tartományába esik. A statikus mintakeresés önmagában nem bizonyít végrehajtott vezérlési utat.

A DSP-báziscímek statikus azonosításához 16 bites határokon 7A 00..07 opkód utáni 32 bites értékeket vizsgáltunk. Adattáblákban is lehet ilyen minta, ezért ezek csak címjelöltek. A H8-kutatási futtató dinamikus megfigyelése szükséges a megerősítéshez.

## Korábbi mérési terv

Az első LCD nélküli megfigyelést később LC7985-bekötés, teljes DSP-regiszternapló, külön kísérleti feltöltésvezérlés és élő VOP3-regiszterkapcsolat követte. Az alapbeállítás továbbra is passzív; a kísérleti válaszok csak explicit profilban aktívak. Aktuális eredmények és következő mérés: [élő regiszterkapcsolat](live-register-milestone.md).

Legfrissebb vizsgálat: [EG-szint és mintaszámláló](eg-readback-milestone.md).

## Önálló ROM/RAM-térkép beolvadt #116 — 2026-10-29

[Új projektkód és szintetikus CPU-próba](an1x-memory-map.md):a fenti három
memóriatartomány összekötve,MMIO és ismeretlen címek fail-closed. Nem teljes gép,
nem firmware-indítás és nem fizikai reset/open-bus bizonyíték.

## MEG kanonikus írási adapter — 2026-11-03

IC10 A0..A4=H8 A1..A5 és alsó adatbájt ellenőrizve az51.PDF-oldal nagyított
rajzán. Szűk kutatási címzés400001+2*offset,széles tükrözés nélkül.
Önálló adapter,nem bekötött CPU-periféria;logging-only vezérlőmegfigyelés csak
explicit módban. [Részletes bizonyíték és korlátok](meg-host-write-adapter.md).

## dsp-capture-milestone

# DSP-regiszternapló és ellenőrzött feltöltés-visszajátszás

Újabb mérföldkő: `upload-control-milestone.md`. A maszk és a várt státuszbit már kiolvasott, és külön kísérleti profilban az első DSP teljes programfeltöltése ellenőrizhető. A passzív eredmény változatlan; a kísérleti profil nem hardvervalidálás.

2026. október 26. – YamANIx

**Elkészült a veszteségellenőrzött DSP-regiszternapló és a VOP3 könyvtárra épülő offline feltöltési audit.** Az eredeti firmware öt másodperces futásának 6441 eseményét rögzítettük; a 2343 írás visszajátszott feltöltési állapota bájtra pontosan egyezik egy külön Python-rekonstrukcióval. A mérés egy konkrét hiányt is feltárt: a DSP feltöltésvezérlő regiszterének válasza még nincs modellezve, ezért teljes programfeltöltést nem igazoltunk.

## Mit rögzítünk?

A H8 által végrehajtott minden olvasást és írást a `0x440000–0x4400FF` és `0x480000–0x4800FF` tartományokban. A napló megőrzi az ismételt hozzáféréseket, a sorrendet, a nanomásodpercben megadott szimulált gépidőt, az adatot, a memóriamaszkot és a programcímet. Ez az adott időablakra és címtartományokra teljes felvétel; más címeken megjelenő esetleges aliasokat nem zár ki.

Sorszámfolytonosság, időrend, fejléc, zárósor és eseményszám ellenőrzött. A 200 000-es korlátból nem veszett esemény. Hiányzó zárósor, számeltérés, eseményvesztés vagy részleges szóhozzáférés esetén az ellenőrző hibával leáll. Az exportált eseményfájlnak is van kötelező zárószámlálója, így egy teljes sor végén csonkolt exportot is elutasít a visszajátszó.

## Mért forgalom

| | DSP 0, 0x440000 | DSP 1, 0x480000 |
|---|---:|---:|
| Olvasás | 4097 | 1 |
| Írás | 1176 | 1167 |
| Programszó-írási tranzakció | 130 | 130 |
| Különböző érintett programszóhely | 60 | 60 |

A 512 lépéses programtérben 2560 programszóhely van chipenként. A felvételben csak a 0x06 és 0x07 programregiszterek írásai látszanak; a 0x08–0x0A regiszterek, a teljes kezdeti mikroprogram és a slotok feltöltése nem igazolt. A lefedettségi szám önmagában nem jelenti, hogy egy működő programnak minden nullás szót is fel kell töltenie; a hiányos betöltésre a vezérlési út megfigyelése is utal.

## A következő akadály pontosabb helye

Az első rögzített tranzakció a DSP 0 `0x01` regiszterébe írt `0x0001`. Ezt ugyanennek a regiszternek **4096 darab, nullát visszaadó olvasása** követi. A passzív felvételben ezek a még be nem kötött MAME-címtartomány alapértékei, nem valódi YSS236-válaszok.

A firmware saját H8 disassemblyjében a `0x0567AA` környékén `0x1000` számláló inicializálása, majd maszkolt státusz-összehasonlítás látszik. A számláló elfogyásához tartozó ágon `0x007F` visszatérési értéket állít elő. A firmware tehát nem egyszerűen díszítésként olvassa a címet; feltételes, időkorlátos várakozás történik.

A VOP3 kutatási dokumentuma a `0x01` regisztert feltöltésvezérlésként írja le, de a bitjeit nyitott kérdésként kezeli. A könyvtár jelenleg nem modellezi ezt a kézfogást. Nem állítottunk be találomra echo-/„kész” választ a teljes feltöltés kikényszerítésére. Az AN1x-ből látott olvasási feltétel pontos jelentését és az elvárt chipviselkedést külön kell tisztázni.

## Mit ellenőriz a visszajátszás?

Az új `yamanix_dsp_replay` program két valódi VOP3 könyvtárpéldány `write()` interfészére adja az írásokat. A külön Python-rekonstrukció a leírt regiszterszabályokból számolja ki a következőket:

- SELECT-latch, beleértve az automatikus növelést és az 511→0 körbefordulást;
- az öt programszó minden lépésen;
- együtthatók, tag bájtok és slot-offsetpárok.

Az ellenőrzött, 14 340 bájtos kétchipes feltöltési állapotkép SHA-256 értéke mindkét útvonalon: `62f2939a81db9b28b200033f59944e5a851eb20c891843d958f53389b739e598`.

**Ez feltöltési állapotegyezés, nem teljes DSP-állapot vagy hangazonosság.** A visszajátszó nem futtat DSP-mintákat, nem lépteti az LFO-/EG-időzítést, nem modellezi a hosttal közös órázást vagy a külső táblaromot. A 4098 passzív olvasási értéket nem táplálja helyes hardverválaszként a magba. A VOP3 által még figyelmen kívül hagyott konfigurációs regiszterek jelentését az egyező állapotkép sem igazolja.

## Validálás és megőrzés

- A saját négy CTest bejegyzés sikeres: DSP soros próba, LCD-busz, új feltöltési keresztellenőrzés és a Python-eszközök tesztcsoportja.
- A Python-csoport most 12 esetet tartalmaz; a C++ feltöltési próba külön ellenőrzi a két chipet, a select-wrapet, a tagcsonkolást, a slotokat és a sérült bemenetek elutasítását.
- A zárószámlálós export bevezetése után az érintett teszteket újrafuttattuk és a tényleges felvételt ismét összevetettük.
- A végleges MAME-kutatási build friss munkamappában, öt másodperces próbával szabályosan befejeződött; az állapotegyezés ezen a felvételen is fennáll.
- A kutatási patch ellenőrzött; a leltározott 22 eredeti letöltött fájl hashértéke változatlan.

Reprodukció: `research/README_HUN.md`. Elemző: `tools/dsp_trace.py`; VOP3-visszajátszó: `src/dsp_replay.cpp`; módosítás: `research/mame-dsp-capture.patch`. A `docs/dsp-capture-milestone/` könyvtár összefoglalókat, teszteredményeket és hash-azonosítókat tartalmaz. A nyers regiszterfelvétel, az export és a firmware-ből származó állapotképek a `work/YamANIx/dsp-milestone-final/` alatt maradnak; nem csomagoltuk őket a saját forrás mellé.

## Következő lépés

A `0x01` feltöltésvezérlő állapotregiszterének működése és a firmware sikeres/fallback útvonalainak összevetése. Ezután következhet az igazolt teljes mikroprogram-feltöltés és a CPU-val időben összekötött két VOP3 mag. Hangkimenet, teljes hardverazonosság és VST3/AU kiadás továbbra sincs igazolva. GUI nem készült, eredeti ROM nem módosult.

## eg-readback-milestone

# EG-szint visszaolvasása és a DSP-időalap előkészítése

2026. október 26. – YamANIx

**Visszakövettük a 0x22-es regiszter AN1x-firmware-beli használatát, elkészült egy külön kapcsolható szint-visszaolvasási kísérlet és a két VOP3 időalapja.** Szintetikus próba chipenként 2400 tényleges `Vop3::run()` hívással ellenőrizte a blokkfelosztástól független működést. A firmware-es futás továbbra is regiszterpróba: az új mintafuttatás még nincs bekötve hozzá, AN1x-hangkimenet nincs.

## Mit olvas a firmware a 0x22-es regiszterből?

A korábbi felvétel 898 ilyen olvasását két rutinba azonosítottuk az eredeti 1.04 ROM disassemblyjével és a teljes busznaplóval.

| Útvonal | Megfigyelt működés |
|---|---|
| `0x0545E8` rutin, olvasás `0x054674`, naplózott következő PC `0x054676` | A SELECT-be 0..15 csatornaszámot ír, mindegyikhez külön 16 bites értéket olvas, és a `0x2382EE + 2 × csatorna` címen tárolja |
| Ugyanott `0x05468E–0x0546B2` | Előjeles összehasonlítás: 16384 fölött, illetve −256 alatt külön eljárást hív; a 14. csatornán az alsó korlátot kihagyja |
| `0x0515EE` rutin | A 14. csatornát választja; a két chip olvasását `0x051668` és `0x0516C6` naplózott PC mellett 0xC000 értékhez hasonlítja |
| `0x0514A6–0x0514C4` | A RAM-ból vett szintet 64-gyel szorozza, és a kiolvasott szóhoz hasonlítja |

Ezek **kiválasztott csatornához tartozó számszerű, előjeles szintérték** használatát mutatják. Nem indokolják a VOP3 mag 0x21-es, `0x10 | csatornaszám` alakú EG-befejezési státuszának egyszerű átirányítását a 0x22 címre.

Az öt másodperces felvételben chipenként 28 teljes, 16 csatornás pásztázás van, összesen 448 olvasással. Ezen felül egy 14. csatornás összehasonlítás történik. A nullás válasz a pásztázás tartományellenőrzésén átmegy, de a külön 0xC000 összehasonlításon **nem**: ezért a korábbi nullás válasz nem tekinthető teljesen megfelelő perifériaviselkedésnek.

Az új `tools/eg_readback_audit.py` a SELECT állapotát a trace-ből követi, ellenőrzi a csatornasorrendet és külön mutatja a kétféle feltétel eredményét. Hiányos pásztázás és részleges szóolvasás hiba. A firmware-címek az ismert 1.04 ROM-ra vonatkoznak.

## Külön kísérleti modell

A bridge ABI 2 új `an1x-level-experiment` profilja a kiválasztott VOP3 EG-csatorna modellbeli `level` értékét szorozza 64-gyel. A nem egész eredményt nulla felé csonkolja, majd előjeles 16 bitre telíti. A szorzó firmware-beli bizonyítékon alapul; **a pontos lekerekítés, telítés és a modell szintjének fizikai hardverhez való megfelelése még nincs igazolva**. Az index 0..15 lehet; más indexet elutasítunk, nem maszkoljuk önkényesen egy létező csatornára.

A kapcsoló `--eg-readback an1x-level-experiment`, csak explicit bridge és kísérleti feltöltésprofil mellett használható. Az alapértelmezett `zero` megőrzi az előző nullás viselkedést. A napló és a manifeszt rögzíti a profilt. A 0x21 státusz továbbra is külön a külső VOP3 `read()` függvényét használja. Az auditor a régi ABI 1 felvételeit is kezeli, de nem fogad el hozzájuk új modellre utaló jelölést.

A kísérleti profil 898 olvasását az audit külön **kísérleti szintolvasásként** jelöli. A `unsupported_reads=0` ebben a profilban a szoftveres kezelés meglétét jelenti, nem hardveres helyességet. A firmware-es próbában a mintafuttatás hiánya miatt a modell szintjei továbbra is nullák; a külön 0xC000 feltétel nem teljesült. Nem állítottuk mesterségesen a várt értékre.

## Időalap és tényleges szintetikus mintafuttatás

A saját `SampleClock` abszolút, egész nanomásodpercekből számol mintaszámot. A minta határára eső hozzáférés előtt esedékes az adott minta; az idő nem léphet vissza. Nem kerekített mintaperiódusok összeadásával dolgozik, ezért nincs blokkfelosztástól függő sodródás. Az egy hívásra jutó korlát túllépését állapotmódosítás előtt elutasítja.

Az ABI 2 `yamanix_vop3_advance()` függvénye mindkét tényleges VOP3-objektumon meghívja a `run()` függvényt a kiszámolt darabszámban, legfeljebb 4096 mintát chipenként és hívásonként. A könyvtár jelenlegi modelljének **48 kHz** értékét használja. Ez **nem az AN1x hardverórajelének igazolása**; a firmware-es futtató nem hívja az új függvényt. A fizikai órajel, a CPU–DSP hozzáférések mintahatárhoz viszonyított sorrendje, az upload alatti leállítás és az audioút még külön bekötést és mérést igényel.

A szintetikus teszt Yamaha programkép nélkül:

- külön chipet és külön csatornát ellenőriz, például −256 → 0xC000 és +256 → 0x4000;
- egy 1 ms-os, még célszint-keresztezés előtti változást független exponenciális képlettel vet össze;
- az 50 ms-ot egyben és több részre bontva futtatja: chipenként **2400 minta**, egyező visszaolvasás;
- ellenőrzi az előjeles ábrázolást, a választott telítési szabályt, az érvénytelen indexet, az idővisszalépést és a mintakeretet;
- az időszámláló külön próbája 44,1/48/192 kHz-en vizsgálja a mintahatárokat, a blokkfelosztást és a nagy időértékeket. Ez általános számlálóteszt, nem három hardveres AN1x-mód igazolása.

## Feltárt eltérés a külső EG-modellben

A próba a VOP3 forráskód leírása és a tényleges működés között eltérést talált. A `Vop3::run()` célszint-keresztezéskor beállítja a `done` bitet és lenullázza a `side` mezőt, de a következő mintákban az aktív `rate` miatt tovább közelít az `aim` értékhez. A forrás leírása ezzel szemben a célszinten való megállást mondja.

A konkrét, szintetikus esetben a kezdőszint −256, cél 128, aim 511, rate 64. 50 ms után a visszaolvasott szó **0x7FBF**, a célszint 64-szerese pedig **0x2000** lenne. A blokkfelosztások egyeznek, tehát ez nem az új időszámláló kerekítési hibája. A cél- és aim-írások pontos fizikai szakaszindítási viselkedése is nyitott kérdés az upstream dokumentációban; a megfigyelés önmagában nem bizonyítja, hogy a valódi chip hogyan tartja a szintet.

Ezt reprodukálható mérési eredményként rögzítettük, a külső VOP3-forrás változatlan maradt. A sikeres tesztek a szállítási, profilkezelési, számlálási és célszint előtti matematikai feltételeket igazolják; **nem állítják, hogy a teljes EG-modell hardverhű**. A mérési JSON `upstream_holds_target=false` mezővel őrzi az eltérést.

## Ellenőrzések és folytatás

- Mind a **10 CTest-bejegyzés** sikeres, köztük az új időszámláló, a tényleges VOP3-mintafuttatás és a firmware-feltételek elemzője.
- Az új `zero` profil és a kísérleti szintprofil mind a korábbi **25 310 regiszteres eseményt** változatlanul reprodukálja ebben a mintafuttatás nélküli firmware-próbában. Az élő feltöltési mezők és az előző natív visszajátszás egyeznek.
- A passzív alapbeállítás korábbi **6441 eseménye** is változatlan.
- A profillal ellentmondó trace-jelölést az auditor elutasítja.
- Hét egymás utáni kutatási patch az eredeti ZIP-ből bájtra előállítja a hét módosított/új MAME-forrásfájlt. A VOP3 eredeti munkapéldányának minden meglévő archívumbeli fájlját és a letöltési leltár 27 eredeti fájlját is változatlannak ellenőriztük.

Következő feladat az EG célszint utáni viselkedésének tisztázása/javítása mérési referenciával, az AN1x órajel és táblarom-skála igazolása, majd a most ellenőrzött időalap összekötése a firmware regisztereléréseivel. AN1x-hang, teljes boot, VST3/AU és GUI még nincs.

Mentett eredmények: `docs/eg-readback-milestone/`. Reprodukció: `research/README_HUN.md`. A nyers firmware/disassembly felvételek és az állapotképek a `work/` alatt maradtak.

## h8-execution-budget

Helyi ellenőrzés: macOS arm64 Release; 27/27 CTest-csoport és 74 Python-eset sikeres; az új natív ASan/UBSan-, forrás- és licencértesítési audit sikeres. A végleges PR-fej ellenőrzése még függőben volt.

## h8-host-services-boundary

Helyi ellenőrzés: macOS arm64 Release; 25/25 CTest-csoport és 68 Python-eset sikeres; a forrás- és licencértesítési audit sikeres. A végleges PR-fej platformellenőrzése még függőben volt.

## h8h-bus-access

Helyi ellenőrzés: macOS arm64 Release; 26/26 CTest-csoport és 72 Python-eset sikeres; az új natív ASan/UBSan-vizsgálat sikeres. A végleges PR-fej platformellenőrzése külön bizonyíték.

## lcd-milestone

# Következő mérföldkő: működő LCD-kézfogás, elért DSP-címek

Újabb állapot: a `dsp-capture-milestone.md` már teljes regiszternaplót és offline feltöltési auditot dokumentál; a következő konkrét hiány a DSP 0x01 feltöltésvezérlő regisztere.

2026. október 26. – YamANIx

**Az eredeti 1.04-es firmware túljutott a korábbi LCD-várakozó cikluson.** A külön kutatási futtatóban a MAME meglévő LC7985 regiszter- és időzítésmodelljét kötöttük a H8 portjaihoz. Az öt másodperc szimulált idejű hidegindítás szabályosan befejeződött. Ez még nem igazolt teljes AN1x-boot vagy hangzó emulátor.

## Mit bizonyítottunk a bekötés előtt?

A korábbi naplóban a firmware a `0x03F66A–0x03F67E` ciklus környékén maradt. A MAME H8 disassemblere szerint a rutin:

- a portok irányát olvasásra állítja;
- PB2-n RS=0-t, P90-en R/W=1-et állít;
- P95-ön E-jelet vált;
- a B portot olvassa;
- a negatív/busy bit alapján visszaugrik a ciklus elejére.

Ez megerősíti, hogy a megfigyelt ciklus a kijelző busy-flagjére vár. A teljesebb szervizkönyvek CPU- és LCD-oldalán ellenőrzött bekötés:

Az adatbusz tehát nem egyszerűen egyetlen port. Az írási adat `(PA & 0x3D) | (PB & 0xC2)`. A saját, kis C++ bekötési modell ezt és a jeléleket kezeli; a vezérlőregiszterek, DDRAM/CGRAM és a busy-állapot továbbra is a MAME LC7985 eszközében vannak.

Olvasást az E felfutó élén mintavételezünk; írást a lefutó élén adunk át. Az ismételt, azonos E-szint nem hoz létre új tranzakciót. Az LCD-adatvonalakon kívüli bemeneti bitek korábbi felhúzott szintje megmarad; a portirány és a kimeneti bitek visszaolvasását a H8 modell kezeli.

## Az ellenőrzött öt másodperces futás

| Megfigyelés | Eredmény |
|---|---:|
| Futtató kilépési kódja | 0 |
| PC-minták | 105 |
| Első olvasási címmegfigyelések | 110 |
| Első írási címmegfigyelések | 183 |
| Busy státuszolvasások | 7193 |
| Kész státuszolvasások | 561 |
| LCD parancsbusz-írások | 288 |
| LCD adatbusz-írások | 274 |

A parancs-/adatszámlálók a buszon átadott tranzakciókat számolják, nem minden egyes parancs hardveres elfogadásának független bizonyítékai. A busy- és készválaszok viszont a vezérlőmodell változó státuszából származnak; nem állandó nullát adtunk a firmware-nek.

A naplóban felismerhető inicializáló parancsok jelennek meg (`38`, `0C`, `01`, `06`, `80`), majd adatírások. A megfigyelt adatfolyamban a `YAMAHA` és `1997` bájtjai is szerepelnek. Ezek buszadatok, nem megjelenített képernyőkép.

A firmware a `0x440000` és `0x480000` körüli, 16 bites DSP-regisztereléréseket is végrehajtja; e két korábbi báziscímjelölt így dinamikus megerősítést kapott. A `0x400000` körüli effektinterfész-írások is láthatók. Ettől a DSP még nem működik: ezek jelenleg megfigyelt, a szintézismaghoz még nem kapcsolt elérések. A teljes címtükrözés, az összes regiszter jelentése és a host-válaszok még ellenőrzendők.

## Tesztelés

- A saját LCD-buszteszt mind a 256 bájt leírását és visszaolvasását ellenőrzi az osztott portokon, továbbá RS-választást, E-élirányt, ismételt jelszintet és a tétlen busz felhúzott szintjét.
- A saját soros DSP-próba továbbra is sikeres.
- A Python ROM-/bank-/trace-eszközök 9 tesztesete sikeres. Az új trace-teszt ellenőrzi az LCD-számlálók és a DSP-címek kiolvasását, és nem állít ezekből teljes bootot.
- A `ctest -R yamanix_ --output-on-failure` három saját tesztbejegyzése sikeres.
- A fordított MAME-kutatási programmal friss munkamappában 5 másodperces integrációs próbát futtattunk.
- A kutatási patch visszafelé alkalmazhatóságának ellenőrzése sikeres. A korábban leltározott 22 letöltött fájl SHA-256 értéke változatlan.

## Reprodukálható eredmények

Források és fordítás: `research/README_HUN.md`. Saját bekötés: `src/lcd_bus.h`. Mérési parancs, napló és buildazonosítók: `docs/lcd-milestone/run.json`, `trace.log`, `console.log`, `build-fingerprints.json`. Az előző mérföldkő naplói külön megmaradtak, így összehasonlítható az LCD nélküli és az új állapot.

Az LCD karakter-ROM-ját ebben a kijelzés nélküli kutatási buildben üres grafikus régió helyettesíti; a karakterrajzokat nem használjuk. A firmware és a H8 CPU-logika változatlan. A LC7985 busz- és időzítésmodellje nincs mesterséges „mindig kész” válaszra cserélve. A valódi hardverhez mért LC7985-időzítési pontosságot ebben a körben nem igazoltuk. Soft reset és save-state kezelés még nincs validálva.

## Következő motorfeladat

A DSP-k teljes regiszterfolyamának rögzítése és visszajátszása, majd a két VOP3 példány valós host-interfészen történő bekötése. A jelenlegi „címenként első elérés” trace nem alkalmas teljes mikroprogram rekonstruálására, mert az ismételt feltöltési írásokat nem őrzi meg. Az ismeretlen eszközök alapértelmezett olvasási értékei nem tekinthetők hiteles DSP-válasznak.

Hangszín megszólaltatása, MIDI-vezérlés, MEG effektek és a VST3/AU csomagolás még hátravan. GUI-fejlesztés és távoli publikálás nem történt.

## live-register-milestone

# Két DSP feltöltése és élő H8–VOP3 regiszterkapcsolat

2026. október 26. – YamANIx

**Az eredeti 1.04 firmware mindkét VOP3-példány teljes hangprogramját feltöltötte a futó kutatási folyamatban.** A feltöltési állapot egyezik a független Python-rekonstrukcióval és a külön C++ visszajátszóval. A feltöltésvezérlés továbbra is kísérleti státuszprofilt használ. Ez regiszterkapcsolat: nincs DSP-mintaütemezés, hangkimenet vagy igazolt teljes AN1x-boot.

## A második DSP érzékelése

A firmware `0x06C558` címen a H8 Port 9 regiszterét (`0xFFFFD2`) olvassa, majd a 2. bitet (`0x0004`) vizsgálja. A `0x23CBC3` bájtba magas bemenetnél nullát, alacsonynál egyet ír. Ez a feltétel vezérli a második DSP inicializálását.

A helyi **AN1x-ServiceManual.pdf 51. PDF-oldalán** a kapcsolási rajz útvonala:

A főpanelen felhúzó ellenállás tartozik a bemenethez; a csatlakoztatott DMS panel földre köti. A korábbi kutatási modell a teljes Port 9 bemenetre `0xFF` értéket adott, tehát ennél az érzékelésnél a hiányzó panel állapotát mutatta. Ez magyarázza, miért maradt ki a második chip teljes feltöltése.

Új, explicit futtatási beállítás:

- `--dms-board absent`: a régi `0xFF` bemenet; alapértelmezés a korábbi mérések megismételhetőségéhez;
- `--dms-board installed`: `0xFB`, csak P92 alacsony; a többi bemenet magas marad, a kimeneteket a H8 perifériamodell egyesíti.

A firmware saját utasítása állítja elő a jelenlétflaget. A RAM-flaget és az eredeti ROM-ot nem írjuk át. A rajz és a firmware egyezése erős bizonyíték a bemeneti logikára, de nem helyettesít fizikai műszeres mérést, és nem igazolja a teljes modell többi feltételezését.

## Élő regiszterkapcsolat

A saját `yamanix_vop3_bridge` megosztott könyvtár két változatlan VOP3 objektumot tartalmaz. A külön MAME-kutatási program macOS-en, csak explicit `--bridge` kapcsolóval tölti be. A verziózott C interfész ellenőrzi az argumentumokat; részleges szóhozzáférést és hibás chipcímet elutasít. Az ABI 1 egyetlen szálú, regiszterekre korlátozott felület.

A H8 írásai azonnal, a tényleges végrehajtási sorrendben jutnak a VOP3 `write()` függvényébe. A 0x01 státuszolvasását a külön kísérleti `firmware-contract` profil kezeli, a többi DSP-olvasás a VOP3 `read()` függvényéhez kerül. A napló megőrzi a H8 gépidejét, de **a VOP3 `run()` függvényét még nem hívjuk**, ezért ez nem mintaszinten időzített emuláció.

A library hiánya, eltérő ABI vagy elutasított hozzáférés leállítja a próbát. A futtató friss munkakönyvtárat igényel, kizárja a korábbi NVRAM-ot, a környezetből örökölt bridge-beállítást pedig törli. Kilépéskor a tényleges élő VOP3-példányok feltöltési mezőit és a hozzáférésszámlálókat menti. Az új auditor hiányzó lezárást, sérült állapotképet és a trace-től eltérő számlálót elutasít.

## Öt másodperces mérés

Kísérleti státuszprofil, csatlakoztatott DMS-panel bemeneti modellje, élő regiszterkapcsolat:

| Mérőszám | Első DSP | Második DSP |
|---|---:|---:|
| Élő VOP3-példánynak átadott írás | 12 208 | 12 200 |
| Különböző érintett programszóhely | 2560 / 2560 | 2560 / 2560 |
| Átadott olvasás | 449 | 449 |
| Ebből a VOP3 által nem támogatott olvasás | 449 | 449 |
| Lefuttatott DSP-minta | 0 | 0 |

Összesen **25 310 DSP-esemény**, veszteség nélkül. A 902 olvasásból négy a kísérleti feltöltési státuszhoz tartozik; a többi a 0x22-es regiszter olvasása. Mindkét DSP alapfeltöltése külön-külön egyezik a helyi VOP3 `voice.bin` képével: **2560 programszó, 512 együttható, 512 tag, 128 slot-offsetpár**. A második chip feltöltésének ellenőrzési pontja a 21 676. esemény után van.

A két chip összesen 14 340 bájtos, futásvégi feltöltési állapotának SHA-256 értéke: `62f2939a81db9b28b200033f59944e5a851eb20c891843d958f53389b739e598`.

Három módon azonos: élő natív objektumokból mentve, a teljes trace-ből független Python-kóddal rekonstruálva, és a trace-t külön natív VOP3-példányokra visszajátszva. Ez a SELECT/program/együttható/tag/slot mezőket ellenőrzi; más belső állapotot vagy audioazonosságot nem.

A bridge nélküli, de csatlakoztatott DMS és azonos státuszprofil mellett felvett mind a 25 310 esemény egyezik az élő futással, időbélyeggel és adattal együtt. Ez megmutatja, hogy ezen a futási szakaszon az élő regiszterkezelés nem változtatja meg a megfigyelt CPU-útvonalat. A passzív/hiányzó panel alapbeállítás mind a korábbi **6441 eseményt** változatlanul reprodukálja.

## Nyitott regiszterkérdés és határok

Az AN1x ebben a felvételben a **0x22 regisztert** olvassa. A helyi VOP3 forrásban az EG-státusz a **0x21** címen van implementálva; minden más olvasás nulla. Ezért a 898 átadott olvasás mind **nem támogatott, nullás helyettesítő válasz**, amit a bridge külön számlál. Nem vezetünk be bizonyítatlan 0x22→0x21 címátirányítást. Előbb az AN1x firmware olvasási rutinját és a regiszter jelentését kell tisztázni.

A VOP3 egyes konfigurációs és host-RAM írásokat sem valósít meg; az írásszámláló az átadást jelenti, nem minden regiszter teljes modellezését. A feltöltésvezérlő 0x01 időzítése és további státuszbitjei feltételezések maradtak. A táblarom skálázása/bekötése, a chipenkénti mintafuttatás, az összekötött soros audioút, a MEG-effektek és a hiányzó PKS-ROM kezelése továbbra is nyitott. Nincs VST3/AU csomag vagy GUI.

## Ellenőrzések és folytatás

- Mind a **8 CTest-bejegyzés** sikeres: hat saját tesztcsoport és két külső VOP3-teszt.
- A C ABI tesztje két chip külön állapotát, SELECT-léptetést, körbefordulást, hibás argumentumokat és a valós olvasás-/mintaszámlálókat ellenőrzi.
- Valódi felvétel másolatán a sérült élő állapot és a hiányzó chipzárósor elutasítását is ellenőriztük.
- Hat egymás utáni kutatási patch az eredeti MAME ZIP-ből bájtra újra előállítja a hét módosított/új MAME-forrásfájlt.
- A letöltési leltár mind a **27 eredeti fájlja** változatlan SHA-256 alapján.

Reprodukció: `research/README_HUN.md`. Mentett, ROM-adatot nem tartalmazó eredmények: `docs/live-register-milestone/`. A firmware, DSP-állapotképek, nyers trace-ek és fordított programok a helyi `work/` alatt maradtak. Következő motorfeladat a 0x22 státuszolvasás értelmezése, majd az igazolt táblarom-formátumra épített DSP-mintaütemezés.

## Parancsok a Mac M1 átadási útmutatójához

Az alábbi, nyelvfüggetlen parancsblokkok a fenti magyar útmutatóhoz tartoznak; a firmware útvonalát a helyi, változatlan fájlra kell beállítani.

```sh
git status --short
uname -m
export DEVELOPER_DIR=/Applications/Xcode_16.2.app/Contents/Developer
xcodebuild -version
clang++ --version
python3 --version
cmake --version
make --version
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

```sh
git clone --config core.autocrlf=false https://github.com/mamedev/mame.git work/mame
git -C work/mame checkout 706dbb73281320c54d7c2f16262c1c3786f2be01
python3 tools/prepare_mame_research.py --source work/mame
make -C work/mame SUBTARGET=yamanix SOURCES=src/mame/yamaha/yman1x.cpp OSD=mac NO_USE_MIDI=1 NO_USE_PORTAUDIO=1 USE_QTDEBUG=0 USE_TAPTUN=0 USE_PCAP=0 REGENIE=1 OPTIMIZE=0 SYMBOLS=0 -j2
lipo -archs work/mame/yamanix
work/mame/yamanix -listfull an1x
```

```sh
python3 tools/run_m1_serial_experiment.py \
  --mame work/mame/yamanix \
  --rom '/absolute/local/path/to/YAMAHA AN1x 1.04.BIN' \
  --work private_runs/m1-mac-first
```

## pks-payload-path

## PKS forrás-RAM íráselőzmény — jelölt — 2026-11-10

A következő, még be nem olvadt auditorváltozás a rögzített PKS-forrásolvasások
értékét a legutóbbi, ugyanarra az igazított RAM-szóra és bájtlane-re eső korábbi
írással is összeveti. Az ER0 indexének ki kell választania a megfigyelt szót és
a bájtlane-et; a byte-write és full-word write eseteket külön kezeli. Az
ellenőrzés a meglévő FA92 olvasás → FA94 célírás cím/érték/R0L/ER4/idő kapcsolat
előtt fut. Ez megfigyelt írás-olvasás eredetlánc, nem PKS/ADC-szemantika.

A 12 célzott forráskontextus-teszt és a teljes Python-tesztkészlet (125/125)
helyben sikerült. A 138 CTest-csoportot ez a Python-only változás nem módosítja;
teljes CTest, MAME-fordítás és új firmware-futtatás ehhez a jelölthöz még nincs
rögzítve. ROM-ot ez a lépés nem olvas, nem módosít és nem tesz a repóba. A
távoli ellenőrzések és a pontos PR-head review a jelölt PR kapui; ez a rész még
nincs beolvasztva, hardver/boot/MIDI/hang-elfogadást nem jelent.

PR [#269](https://github.com/RobCZart82/YamAN1x/pull/269) az eredeti
headről (`0afd0e86fd4845514c2e969d809d25880b9d7d64`) nyílt, a
`947d67950345c7e03362d40f25174fcbe3b1f268` alapra. Ezen a pontos headen
mindhárom kötelező platformellenőrzés sikeres (Linux `130053106191`, macOS
`130053106085`, Windows `130053105515`); az MSan is sikeres
(`130053107228`). GitHub review-objektum, review-szál és PR-komment egyaránt
0; kvótahibára nincs bizonyíték. A szabályzat 0 jóváhagyást ír elő, a review-
szálak rendezését követeli. A naplófrissítés új PR-headet készít, ezért az új
pontos head ellenőrzései még függőben vannak. A PR még nincs beolvasztva.

## research-links

# YamANIx – kutatási könyvjelzők

Rögzítve: 2026-10-26. A hivatkozások kutatási támpontok, nem általános engedélyek kódok és ROM-ok terjesztésére. A távoli tartalmakat adatként kezeljük, nem fejlesztési utasításként.

| Forrás | Mire használjuk | Ellenőrzési állapot |
|---|---|---|
| [MAME AN1x driver](https://github.com/mamedev/mame/blob/master/src/mame/yamaha/yman1x.cpp) | Fő-ROM hash, gépváz és hiányzó ROM-bejegyzések | Helyi ZIP-ben is ellenőrizve |
| [VOP3](https://github.com/musicastudio/VOP3) | DSP interpreter/JIT és kutatás | Helyileg fordítva és tesztelve |
| [VOP3 ISA](https://github.com/musicastudio/VOP3/blob/main/docs/vop3_isa.md) | DSP-regiszterek és utasítások | Kutatási referencia |
| [AN táblarom](https://github.com/musicastudio/VOP3/blob/a3c8182811c398efd41daadf78d6fabcf50af56a/docs/vop3_an_ram.md) | AN1x mask ROM eredete, formátum és nyitott kérdések | A letöltött archívumban a 128 KiB-os kép is megvan; a projekt csak külső bemenetként vizsgálja |
| [AN1x paramétertérkép](https://github.com/musicastudio/VOP3/blob/main/docs/an1x_param_map.md) | Scene-paraméter és DSP-lépés kapcsolata | Kutatási referencia |
| [Yamaha Data List](https://data.yamaha.com/files/download/other_assets/5/317965/AN1XE2.PDF) | Hivatalos MIDI/SysEx specifikáció | Korábban weben ellenőrizve; helyi PDF megvan |
| AN1x Service Manual, p. 11 (local user copy) | H8 pin table: RXD0 = MIDI IN, RXD1 = keyboard receive | MAME MIDI IN route checked against pin labels; source PDF is not redistributed |
| [Yamaha kézikönyv](https://data.yamaha.com/files/download/other_assets/3/333543/AN1xE1.PDF) | Működés és adatmentés | Hivatalos referencia |
| [QAN1xEditor](https://github.com/thefinalcutbg/QAN1xEditor) | Hangbank- és MIDI-adatmodell összevetése | Webes vizsgálat, kódátvétel nélkül |
| [QAN1xEditor AN1 olvasó](https://github.com/thefinalcutbg/QAN1xEditor/blob/master/src/Model/An1File.cpp) | AN1 fájlformátum | Részben megvizsgálva; fejléckonstansok még nem hitelesítve |
| [QAN1xEditor MIDI](https://github.com/thefinalcutbg/QAN1xEditor/blob/master/src/Model/MidiMaster.cpp) | Hardverkommunikáció referenciája | Részletes audit még hátravan |
| [rgwan FS1R RE](https://github.com/rgwan/fs1r_firmware_RE) | PLG150-AN/VOP3 összehasonlítás, hardveres kutatás | README és néhány könyvtárlista ellenőrizve; nem AN1x CPU-emulátor |
| [VSE firmware-téma](https://forum.vintagesynth.com/viewtopic.php?t=96664) | AN1x 1.04 előzmények | Fő-ROM-ról szól; PKS dumpot nem igazoltunk |

A külső kódok teljes licenc- és függőségvizsgálata minden tényleges átvétel előtt szükséges. A QAN1xEditor README MIT licencet jelöl, a teljes licencfájl közvetlen webes lekérése eddig nem sikerült. A jelenlegi saját SYX-ellenőrző nem tartalmaz onnan átvett kódot.

LCD-mérföldkő forrásai: [Sanyo LC7985 adatlap, Crystalfontz tükör](https://www.crystalfontz.com/controllers/uploaded/Sanyo_LC7985.pdf), [MAME LC7985 modell](https://github.com/mamedev/mame/blob/master/src/devices/video/lc7985.cpp), [MU5 meglévő bekötési példa](https://github.com/mamedev/mame/blob/master/src/mame/yamaha/ymmu5.cpp). A tényleges AN1x pin-hozzárendelést a két helyi teljes szervizkönyv rajzai és a firmware saját disassemblyje alapján ellenőriztük; a MU5 kiosztását nem vettük át az AN1x-re.

## timed-vop3-experiment

# Kísérleti H8–VOP3 mintaléptetés

Dátum: 2026-10-26. Ez a nyolcadik, külön alkalmazandó MAME-kutatási patch
leírása. A korábbi hét patch változatlan marad. A kísérlet nem része a CMake
termékmotornak, és nem állít AN1x-hardverazonosságot.

## Mit köt össze

A bridge ABI 2 már biztosít egy abszolút nanomásodperces `advance()` hívást,
amely mindkét VOP3-példányt lépteti. A patch ezt a MAME szimulációs idejéhez
köti. A firmware DSP-regiszter olvasása vagy írása előtt a vezérlő a MAME
aktuális idejére léptet. A `SampleClock` szerződése szerint az adott időpontban
esedékes minta megelőzi az ugyanarra az időpontra eső regiszterműveletet.

A bridge hívási korlátja 4096 minta. Ezért a patch legfeljebb 80 ms-os
időszakaszokra bontja a nagyobb időugrást: 48 kHz-es modell mellett egy ilyen
szakasz legfeljebb 3840 mintát futtat. A bridge létrehozásakor rögzíti az időalapot;
az MAME abszolút idejét ehhez képest adja át. A futás lezárásakor az utolsó
DSP-hozzáféréstől a kilépésig esedékes mintákat is lefuttatja, mielőtt
elmentené az állapotot és a mintaszámlálókat.

## Biztonságos opt-in

Alapértelmezett módban nincs mintaléptetés. Csak a következő explicit kapcsoló
engedi meg, élő VOP3 bridge mellett:

A futtató törli a gyermekfolyamatból az örökölt órajel-profilt, majd csak a
kifejezett kapcsoló esetén állítja be. A futásmanifestje rögzíti a profil
nevét, a használt 48 kHz-es modellfrekvenciát és mindkét chip `sample_passes`
számlálóját. Nulla számlálóval vagy hiányzó bridge-visszajelzéssel a futás
hibának minősül.

## Időzített firmware-trace audit

A trace parser és az élő bridge auditor külön kezeli a timed kísérlet órajel-
és bridge-fejlécsorait a DSP-regiszter-eseményektől. Az auditor ellenőrzi a
kifejezett `assumed-48k` profilt, a két chip mintaszámlálóját, a teljes
regisztertrace-t és az élő feltöltési állapot független rekonstrukcióját. A
számlálókat nem tekinti hardveres órajelbizonyítéknak.

Az öt másodperces timed futás újraelemzése 25 310 regisztereseményt adott; a
feltöltött állapot egyezett a Python rekonstrukcióval. Mindkét chipen 28 teljes
16 csatornás 0x22-es pásztázás volt (448 olvasás), tartományhibát nem jelzett a
firmware. Az összes pásztázott érték nulla volt, a külön 14. csatornás olvasás
pedig nem egyezett a firmware által várt 0xC000-rel. A 48 kHz-es kísérleti
futtatás tehát javítja a vizsgálatot, de nem bizonyítja az EG-visszaolvasási
modell helyességét; nincs MIDI-note indítás, hangkimenet vagy hardveres
összehasonlítás.

## Korlátok

48 kHz a vendorizált VOP3-modell értéke, nem az AN1x VOP3 chipjének bizonyított
mintavételezési frekvenciája. Az idő nanomásodpercre kvantált. A patch nem
tölt be táblaromot, nem vezeti át a VOP3 soros kimenetét a másik chipen vagy a
MEG-en, nem hoz létre audiofájlt, és nem módosítja a firmware-olvasásokat.
Az EG/readback és upload státuszprofil továbbra is kísérleti.

A nyolc patchből felépített helyi MAME-target lefordult. A `run_h8_probe.py`
eszköz 1 és 5 MAME-másodperces firmware-próbát futtatott, mindkettő 0 kilépési
kóddal. A két chip számlálója rendre 48 799 és 240 799 volt; a két futás
közötti pontos 192 000 növekmény négy modellmásodpercnek felel meg. A 799-es
többlet a MAME frame-határig futtatásából adódik, nem AN1x-órajelmérés. A trace
mindkét chipnél regiszterforgalmat mutat, de a teljes AN1x boot továbbra sincs
igazolva. A ROM-mentes CI nem fordítja ezt a külön MAME-kutatási targetet.

Következő lépés: auditálni a futó magok által visszaadott értékeket a firmware
elvárásaival, majd táblaromot és soros audioútvonalat kapcsolni be külön,
explicit kísérleti profilban. Külön AN1x hardvermérés kell az órajel, a
mintahatár és a regiszterek sorrendjének igazolásához.

## upload-control-milestone

# Feltöltésvezérlés: bizonyított feltétel és kísérleti sikeres útvonal

2026. október 26. – YamANIx

**A firmware pontos státuszfeltételét kiolvastuk, és külön kísérleti profilban elértük az első DSP teljes programfeltöltését.** A betöltött program, együtthatók, tagok és slot-offsetek egyeznek a helyi VOP3 csomag AN1x `voice.bin` képével. Az alapértelmezett passzív mód változatlan. A státuszprofil nem hardveren mért YSS236-modell; audio és teljes gépazonosság nincs igazolva.

## Letöltési mappa

Újonnan átnézett anyagok: `mame0289-arm64.zip`, `HD44780Renderer.cpp`, `HD44780LcdDisplayJUCE.h`. Az előző leltárhoz képest az `an1x.zip` és `mulcd.zip` is új bejegyzés, de ezeket már az előző ellenőrzésben külön megvizsgáltuk.

- A MAME ARM64-csomag összes ZIP-tagjának CRC-olvasása sikeres. Ez kész programokat, köztük unidasm eszközt tartalmaz; a roms könyvtárában csak dir.txt van. Új PKS-ROM-ot nem találtunk benne. A csomag programjait nem futtattuk; a saját fordított MAME maradt a kutatási futtató.
- A két új C++ fájl HD44780-karakterrajzolási példa. Az egyik JUCE-komponenst ír le; nem tartalmaznak LC7985 kézfogást vagy az AN1x osztott adatbuszának bekötését. Nem emeltük be őket és nem kezdtünk GUI-fejlesztést. A bennük állított hardverpontosság nincs igazolva.
- A teljes aktuális leltár 27 fájlja változatlan maradt a vizsgálat és a fejlesztés alatt. Részletes fájlazonosítók a projekt melletti `YamANIx-letoltes-frissites-2026-10-26.json` fájlban vannak.

## A firmware-ből bizonyított logikai feltétel

A polling-rutinban a CPU-regiszterek közvetlen megfigyelése:

| Fázis | Maszk | Várt érték |
|---|---:|---:|
| Feltöltés megkezdése, 0x01 regiszterbe írt 1 után | 0x8000 | 0x8000 |
| Feltöltés elengedése, ugyanoda írt 0 után | 0x8000 | 0x0000 |

A `0x056B5C` környéki firmware-rutin az első várakozás időkorlátjának hibáján kihagyja a következő inicializáló blokkot. A sikeres ág beállítja a chip konfigurációját, feltölt egy indítóprogramot, majd a hangprogramot. A korábbi passzív futás ezért csak későbbi paraméterfrissítéseket mutatott, teljes alapfeltöltést nem.

## Külön kísérleti profil

Az új `UploadControlContract` egy korábban kiadott vezérlőparancsból állít elő logikai státuszt:

- 1: a státusz 15. bitje beáll;
- 0: a státusz 15. bitje törlődik;
- egyéb parancs vagy részleges írás: nem támogatott, a kísérleti futás hibával leáll.

A válasz nem a CPU várt értékéből készül; a CPU-regisztereket csak a megfigyelő naplózza. **A válasz késleltetése és a többi státuszbit nullája viszont feltételezés.** Ezeket valódi chipen még ellenőrizni kell. Ezért a profil neve `firmware-contract`, alapból kikapcsolt, és a naplókban `hardware_validated=0` jelölésű. A fő-ROM és a CPU-kód nem módosult.

## Öt másodperces összehasonlító mérés

| Mérőszám | Passzív mód | Kísérleti profil |
|---|---:|---:|
| Rögzített DSP-esemény | 6441 | 13827 |
| Első DSP különböző érintett programszóhelyei | 60 | 2560 |
| Első DSP programszó-írásai, ismétlésekkel | 130 | 5915 |
| Második DSP különböző érintett programszóhelyei | 60 | 60 |
| Elveszett esemény | 0 | 0 |

A friss passzív felvétel minden eseménye egyezik a korábbi ötmásodperces felvétellel, beleértve az időt, adatot és programcímet. Az új modell alapból nem változtatta meg az addigi eredményt.

A kísérleti felvételben az első DSP teljes feltöltése látszik. A firmware `0x23CBC3` című feltételflagje mindvégig nulla; a disassembly szerint emiatt a második DSP teljes inicializáló hívását kihagyja. Ennek a flagnek a hardveres eredete még külön vizsgálandó. Nem írtuk át a flaget a második DSP mesterséges bekapcsolásához.

## A programkép összevetése

Az ellenőrzés az első DSP alapfeltöltése és a slotok beírása utáni, de a későbbi paramétermódosítások előtti pontot választja ki. Mindegyik egyezik:

- 512 × 5 = **2560 programszó**;
- **512 együttható**;
- **512 tag**;
- **128 slot-offsetpár**.

A korábban kétértelműnek tűnő slotmezők is tisztázódtak ennél a képnél: egy négyes lépéscsoportban legfeljebb egy különböző nemnulla offset van. A firmware által a slotba írt érték ezzel egyezik mind a 128 csoportban. Az új adapter konfliktus esetén hibát ad, nem választ önkényesen.

A vizsgált `voice.bin` SHA-256 értéke: `041ca9414c563158e5f84928c159e1e23b4c46fa43a807bd9713b6bf7f8fffc7`.

Az első chip 7170 bájtos feltöltési állapotképének SHA-256 értéke: `20b036c5c234412a1501d0ad643bca1e9d1727dc86e9fdf32037143817facd0c`.

Ez a digest megegyezik a firmware-naplóból rekonstruált és a VOP3 C++ könyvtárra visszajátszott statikus feltöltéssel is. A két adatforrás közös firmware-eredetű lehet; ez nem független fizikai ROM-kiolvasás vagy hangmérés.

## Elkészült kód és tesztek

- `src/upload_control_contract.h`: alapból kikapcsolt, logikai státuszmodell.
- `tools/run_h8_probe.py --dsp-profile`: megkülönböztetett passzív és kísérleti futtatás, mentett profilazonosítóval.
- `tools/dsp_trace.py`: profiljelölés és kevert/érvénytelen profilok elutasítása.
- `tools/an1x_upload.py`: 8192 bájtos AN1x-kép dekódolása, ellenőrzött slotképzés, statikus regiszterírás-lista és összevetés a rögzített feltöltéssel.
- `research/mame-upload-control-experiment.patch`: a megfigyelő és a választható kísérleti profil a MAME-munkapéldányhoz.

Mind az öt saját CTest-bejegyzés sikeres; az új képadapter hozzáadása után a Python-csoport mind a 15 esete sikeres. A natív VOP3-visszajátszó a valódi felvétel végállapotát és a képadapter által előállított programot is betöltötte; a vizsgált állapotok egyeznek. A teljes patchlánc az eredeti MAME ZIP-ből újra előállította a munkapéldány hat módosított/új fájlját.

## Határok és következő feladat

A hangmag élő, időben összekötött H8–VOP3 működése, a táblarom megfelelő skálájú bekötése, a második DSP érzékelése és teljes inicializálása még nincs kész. A statikus képadapter íráslistája időzítés nélküli referencia, nem valós idejű hardverfeltöltés. Audio nem készült, a kísérleti státuszmodell nincs fizikai hardveren validálva.

Következő lépés a második DSP feltételflagjének eredete és az élő VOP3 host-interfész előkészítése; a most rögzített, teljes első program ad hozzá ellenőrzött betöltési referenciát. Mérési összefoglalók: `docs/upload-control-milestone/`. A nyers felvételek és firmware-eredetű állapotképek a `work/` alatt maradtak.
