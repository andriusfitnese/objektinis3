NAUDOJIMOSI INSTRUKCIJA:
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Ačiū, kad parsisiuntėte šią programą! Deja, nesu baisiai protingas, todėl ir rašau naudojimosi instrukciją, kadangi paleidimas nėra vien paprasto .exe failo paspaudimas.

Pradedant nuo pradžių, turite turėti veikiantį GCC, kurio versija palaiko C++17 ar naujesnes versijas.
Antra, turite turėti veikiantį MinGW, kuris reikalingas programos .exe failo sukūrimui, kuriam reikia mingw32-make komandos. Tam pridedu šią nuorodą: https://nerdyelectronics.com/install-mingw-on-windows-for-make/

---------------------------------------------------------------------
Reikalingos komandos:

mingw32-make -B     - buildina visue .exe failus

mingw32-make class  - sukuria .exe versijai su deque tipo konteineriu, bet naudojant klasę.

mingw32-make clean  - ištrina bin failą, kuriame laikomi sukurti jūsų .exe failai bei sugeneruoti duomenų failai

Test.exe taip pat pakeistas į catch_tests.exe , naudojantis unit testais, tad jį paleist reiktų atsidaryt komandinę eilutę, naviguoti į šio repo directory, į bin (jau turite būt paleidę mingw32-make bent kartą, kad bin ten būtų), ir jei esate and Windows, tereikia suvesti catch_tests.exe. Žemiau įkelsiu screenshot, kaip tai pas mane ant Windows 11 atrodo.

![image](https://github.com/user-attachments/assets/32dca57e-a640-41e7-be35-c251a82659bb)
--------------------------------------------------------------------------------

Parsisiuntę repo, turite du variantus:

Visual Studio
-
  Atsidaryti .sln failą, kuris atidarys visą projektą per Visual Studio, per kurį galėsite atsidaryti terminalą (CTRL+~) ir suvesti atitinkamas komandas, surašytas viršuje.

CMD
-
  Atsidaryti command line, (ieškoti cmd paspaudus Windows ikoną), nuvesti path iki šitos repo direktorijos naudojant cd komandą, pvz: cd C:\Users\andri\OneDrive\Desktop\oop naujas\bin ir tada vesti atitinkamas komandas pagal Jūsų poreikį.

![image](https://github.com/user-attachments/assets/dca6b442-b0ee-4910-a348-7c9574be7ef3)


Tai atlikus, atsiras bin folder'is, kuriame rasite .exe failus.
Paleidus programą, visi pasirinkimai bus aiškiai jums duoti.

**Rekomendacija** - Jei naudojate atskyrimą išlaikiusių ir neišlaikiusių studentų, rekomenduojama gavus klausimą pasirinkti antrą strategiją - ji patikimiausia bei greičiausiai veikianti.

Gero naudojimo!

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TESTAVIMAS
-

Kompiuterio, naudoto testavimui specifikacijos:
CPU: Ryzen 7 5800U
RAM: 32GB DDR4 3200MHz
SSD: 480GB, ne M.2



v1.1 benchmark'as (O3 optimizacija)
-

![image](https://github.com/user-attachments/assets/e2540cf4-4f2e-4a25-ae8a-bce9edebf667)
![image](https://github.com/user-attachments/assets/2e670fa6-28aa-438e-8988-ef868e04f21f)
![image](https://github.com/user-attachments/assets/d2e97aa2-9634-4f94-8cee-dae7acf24a48)
![image](https://github.com/user-attachments/assets/e3831801-1726-4993-97a2-4f9c13ace2cd)
![image](https://github.com/user-attachments/assets/dcc80e43-4d32-402a-8c6e-f9c2de081b7b)


| Studentų kiekis                    | 1000  | 10000 | 100000 | 1000000 |10000000|
|------------------------------------|-------|-------|--------|---------|--------|
| Skaitymo laikas                    | 0.003 | 0.02  | 0.20   | 1.62   | 14.45 |
| Rūšiavimo laikas                   | 0.0004  | 0.005 | 0.06   | 0.80 | 14.45 |
| Atskyrimo i vektorius laikas       | 0.0003 | 0.002  | 0.02   | 0.21    | 2.78  |
| Irašymo i nerdus laikas            | 0.007  |  0.02 |  0.17  |  1.72 | 18.21 |
| Irašymo i galiorką laikas          |  0.005   |  0.01  |  0.12  |  1.19   |  12.29  |
| Visas veikimo laikas               | 0.02  | 0.06  | 0.56   | 5.54   |  60.00 |


Naujienos
---------------------------------------------------------------------------------------------------------------------------------------------------------------

v1.2 buvo pritaikyta klasei penkių metodų taisyklė - t.y kopijavimo ir kėlimo konstruktoriai bei assign'eriai ir destruktorius.
Taip pat pridėti operatorių išvesties ir įvesties būdai

Ką jie daro?
-----------

![image](https://github.com/user-attachments/assets/9b79e803-85c2-45ec-a025-676f98cf623b)

Šios taisyklės pagerina optimizaciją ir tuo pat programos veikimą, tiksliai parodantys klasei, kaip ką apdoroti, kelti, kopijuoti, bei naikinti.

Su išvesties ir įvesties metodais supaprastintas įvedimas ir išvedimas ranka arba atsitiktinai generuojant, t.y - viskas, kas nesusiję su darbu su failu.
Tuose operatoriuose dabar perkelta faktiškai viskas, kas buvo kitose dalyse kodo ir susiję su rankiniu įvedimu bei išvedimu.
![image](https://github.com/user-attachments/assets/6b508693-94b0-4f46-bac2-bd61bfdcaee6)
(Šios eilutės užėmė daug vietos main.cpp faile, o dabar yra atskirai ir tvarkingai laikomos operatoriuje funkcijų .cpp faile, kur jas daug lengviau iššaukti.)

Su išvedimu tas pats - paprasčiau suprast kodą, lengviau iššaukt.

Prieš -
![image](https://github.com/user-attachments/assets/b47b977b-2fc9-4787-bf77-a89ce36ad4ad)


Po - 
![image](https://github.com/user-attachments/assets/c5c067dc-92ab-48e8-a510-27c32da5dc9e)

Dabar cout<<n ne tik paprastai išveda į ekraną n, o aktyvuoja viską, kas yra išvedimo operatoriuje -
![image](https://github.com/user-attachments/assets/2c9b8263-a832-4ef2-b1c3-833dad52847e)

V1.5
------------------------------------------------------------------------------------------------------------

Versijai 1.5 pritaikyta abstrakti klasė Zmogus, kuri bus naudinga ateityje. Kol kas, tai pakeitė tik studento klasės realizaciją - ji dabar yra išvestinė klasė Zmogus abstrakčiąjai (visi studentai žmonės, lygtais..)

Visas veikimas bus identiškas į V1.2.

![image](https://github.com/user-attachments/assets/ebab23b8-74cd-46b7-9898-5f63ae04085c)


![image](https://github.com/user-attachments/assets/e517dab3-1ac3-409f-8370-b7889e075f3c)

V2.0
------------------------------------------------------------------------------------------------------------

Ši versija atneša kelias naujoves -

**Doxygen dokumentacija** - 📄 [Nuoroda į PDF failą](docs/latex/refman.pdf)

**Unit testus** - Pasinaudota Catch2, kuris leis ir man greičiau išbandyt naujas idėjas ar kodo perrašymą mažiau sukant galvą, ar viskas vistiek veiks taip, kaip noriu, ir tuo pat gali būt reikalinga jums.

Taip pat pastebima (ypač jei sekėt senesnes versijas), kad pagaliau išliko tiktais klasių versija, kadangi kitos yra jau be galo senos ir nebereikalingos tolesnėm realizacijom.



V3.0  
------------------------------------------------------------------------------------------------------------

Šioje versijoje yra daug behind-the-scenes pakeitimų - parašytas nuosavas Vector konteineris. Pagal funkcionalumą jis atitinka arti 100% STL Vector. Apačioje pateiksiu kelis funkcijų pavyzdžius, o pilnas jų sąrašas tiems, kuriems įdomu, bus dokumentacijoje html bei pdf formomis.


Default konstruktorius
![image](https://github.com/user-attachments/assets/0cf36922-55a7-4033-92df-b54e3b83a2bc)

Destruktorius
![image](https://github.com/user-attachments/assets/8d24dcc8-2e0b-4e57-ab85-b31bed2254d9)

Move assign operatorius
![image](https://github.com/user-attachments/assets/bc4b2fab-b7e9-43d4-880d-fef7120a1c9f)
Patikrina, ar ne keliamas į save, jei taip, toliau kodas nevykdomas, jei ne - tikrina, ar ne dealokuoja tuščios rodyklės ir tada dealokuoja atmintį naudotą esamo objekto. Ateinančios keturios eilutės iš tiesų daro move priskyrimą į other objektą, o likusios - išvalo seną. 

Iteratoriai
![image](https://github.com/user-attachments/assets/01d469c3-6815-4d8a-95c0-b3d77a5eabd4)
Kaip ir vector, tiesiog leidžia judėti per vektorių iš priekio arba iš galo.

Dalis capacity funkcijų
![image](https://github.com/user-attachments/assets/efd2f0f5-e57e-488c-ac5e-1ddb556cc7b6)

Modifiers
![image](https://github.com/user-attachments/assets/14d76b74-4635-4233-8d69-afe6cd964d5f)

Įvedimo, lygumo, išvedimo ir kiti operatoriai, kitos non-member funkcijos
![image](https://github.com/user-attachments/assets/fc941e02-cabf-4f16-bd5f-7c972b0785c5)


Testavimas
-

Viskas buvo ištestuota, parašyta daug testo atvejų ir ištaisytos atitinkamos klaidos, tad visos funkcijos veikia taip, kaip ir turi STL vector funkcijos.
![image](https://github.com/user-attachments/assets/cfb77732-afe7-4df0-9779-29754c39dfd2)


Spartos analizė
-
Buvo vykdyta spartos analizė, pagal kurios pirmus rezultatus iš kart matosi 2x pagreitėjimas duomenų pildyme parašius paprastą kodą testavimui- 
![image](https://github.com/user-attachments/assets/9f63b156-e3a4-4041-9cf1-616d984c7857)
Rezultatas - 
![image](https://github.com/user-attachments/assets/d8175a5c-f476-4750-a351-a909052921e0)





