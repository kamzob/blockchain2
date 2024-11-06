# blockchain2
# Supaprastintos blokų grandinės kūrimas
<img width="531" alt="Screenshot 2024-10-30 at 23 29 42" src="https://github.com/user-attachments/assets/dc1216ad-2266-4e50-9187-6e054bc74ab3">

## Užduoties formuluotė
Sukurkite "centralizuotą" blokų grandinę (blockchain) ir imituokite jos veikimą kuo realistiškiau. Preliminari veiksmų seka: 
1. Sugeneruokite ~1000 tinklo vartotojų,turinčių šiuos atributus:
vardą,
viešąjį raktą ( public_key ),
atsitiktinį valiutos balansą (pvz., nuo 100 iki 1000000 valiutos vienetų).
2. Sugeneruokite ~10000 naujų transakcijų,kurios dar nėra įtrauktos į jokį bloką.Transakcijos struktūra turi turėti šiuos atributus:
transakcijos ID (kitų transakcijos laukų maišos reikšmė), siuntėjas (jo viešasis raktas),
gavėjas (jo viešasis raktas),
suma.
Transakcijų struktūrą galite tobulinti, pvz., vietoj sąskaitos modelio (angl. account model) galite naudoti UTXO modelį.
3. Atsitiktinai pasirinkite 100 transakcijų iš transakcijų sąrašo ir pabandykite jas įtraukti į naują bloką.
4. Realizuokite naujų blokų kasimo (angl.mining) procesą, naudojant Proof-of-Work(PoW) algoritmą.Tikslas – rasti naujo bloko maišos reikšmę, atitinkančią Difficulty Target reikalavimą (nulių skaičių maišos reikšmės pradžioje). Nulių skaičius priklauso nuo jūsų sukurtos maišos funkcijos.
5. Suradus tinkamą naujo bloko maišos reikšmę:
ištrinkite į bloką įtrauktas transakcijas iš transakcijų sąrašo;
"įvykdykite" transakcijas, t.y. atnaujinkite vartotojų balansus;
 pridėkite naują bloką prie blokų grandinės.
7. Kartokite 3-5 žingsnius, kol yra neįtrauktų transakcijų. Taip pat galite kartoti 1-5 žingsnius, generuodami naujus vartotojus ir transakcijas.

## Hash generatoriaus pseudo-kodas

1. Funkcija priima input'ą bet kokio dydžio
2. Tada du tiesiog pirminiai dideli skaičiai paimami, užrašau juos šešioliktainiu formatu su priekyje 0x
3. Susikuriu masyvą 4 elementų (kiekvieno dydis 64 bitai), kurių pradinės reikšmės 0
4. Suku ciklą per inputo ilgį
5. Skaitomam simboliui priskiriu ASCII reikšmę
6. Suku dar vieną ciklą, kuris pereis per visus 4 masyvo elementus
7. Atlieku XOR operaciją elemento su skaitomo simbolio ASCII reikšme
8. Tada elementą padauginu su vienu iš pirminių skaičių
9. Tada vykdau bitų rotaciją į kairę per 13 elementų
10. Tada vykdau sekančio masyvo elemento bitų rotaciją dešinėn per 17 pozicijų ir atlieku XOR operaciją du dabartiniu masyvo elementu
11. Tuomet skaitomo simbolio reikšmę padauginu iš antrojo pirminio skaičiaus
12. Vidinis ciklas baigiasi
13. Pagrindinis ciklas baigiasi
14. Inicializuoju tuščią eilutę
15. Suku ciklą per kiekvieną masyvo elementą
16. Kiekvieną masyvo elementą pridedu į eilutę kaip 16 simbolių šešioliktainį sk
17. Grąžinu funkcijos rezultatą
18. Funkcija baigia darbą
## v0.1 versija
1. Realizuotas 1000 vartotojų generavimas, kurie saugomi vektoriuje. Tam realizuoti buvo sukurta klasė, naudojant gerąsias OOP praktikas:
```
class Vartotojas{
private:
    string vardas_;
    string viesRaktas_;
    double valiutosBal_;
public:
    Vartotojas() = default;
    Vartotojas(string vardas, string publicKey, double balansas):
    vardas_(vardas), viesRaktas_(publicKey), valiutosBal_(balansas) {}
    
    void setVar(string vard){
        vardas_ = vard;
    }
    void setpKey(string raktas){
        viesRaktas_ = raktas;
    }
    void setBal(double balansas){
        valiutosBal_ = balansas;
    }
    void atnaujintiBalansa(double suma){
        valiutosBal_ += suma;
    }
    
    string getVar() const { return vardas_;}
    string getpKey() const { return viesRaktas_;}
    double getBalance() const{ return valiutosBal_;}
    void spausdintiUseri() const{
        cout << "Vardas: " << vardas_ << endl;
        cout << "Viesasis raktas: " << viesRaktas_ << endl;
        cout << "Valiutos balansas: " << valiutosBal_ << endl;
    }
    
};
```
2. Sukurtas transakcijų generavimas. Tam buvo naudojama klasė Transakcija su gerosiomis OOP praktikomis:
```
class Transakcija {
private:
    string id_;
    string siuntejas_;
    string gavejas_;
    double suma_;
    
public:
    Transakcija()=default;
    Transakcija(string id, string siunt, string gav, double sum):
    id_(id), siuntejas_(siunt), gavejas_(gav), suma_(sum) {}
    //getteriai
    string getId() const { return id_; }
    string getSiuntejas() const { return siuntejas_; }
    string getGavejas() const { return gavejas_; }
    double getSuma() const { return suma_; }
    //setteriai
    void setId(const string& id) { id_ = id; }
    void setSiuntejas(const string& siuntejas) { siuntejas_ = siuntejas; }
    void setGavejas(const string& gavejas) { gavejas_ = gavejas; }
    void setSuma(double suma) { suma_ = suma; }
    void spausdintiTransakcija() const {
        cout << "Transakcijos ID: " << id_ << endl;
        cout << "Siuntėjas: " << siuntejas_ << endl;
        cout << "Gavėjas: " << gavejas_ << endl;
        cout << "Suma: " << suma_ << endl;
    }
    
};
```

3. Blokų grandinės kūrimui buvo sukurta klasė Blokas, kur objektas turi šiuos atributus:
```
string prev_bloko_hash_;
    string merkle_root_hash_;
    string bloko_hash_;
    string versija_;
    time_t laikas_;
    unsigned long long nonce_;
    int difficulty_target_;
    vector<Transakcija> transakcijos_;
```
prev_bloko_hash_ - prieš tai buvusio bloko hash

merkle_root_hash_ - transakcijų merkle root hash

bloko_hash_ - šio bloko hash, susidedantis iš prev_bloko_hash_, merkle_root_hash_, versija_, laikas_, nonce_

versija_ - bloko versija

laikas_ - kada blokas sukurtas (timestamp)

nonce_ - skaičius, kuris rodo attemptų skaičių kol bus sugeneruotas toks hash pagal difficulty_target
transakcijos_ - transakcijų sąrašas įtrauktas į bloką

4. Realizuotas blokų kasimas funkcijoje mineBlock(), kur pagal duotą difficulty_target_ vykdomas do while ciklas, kuriame vis didinamas nonce_ ir generuojamas hash, kol jis pasiekia tą difficulty_target_.
5. Suradus tinkamą bloko reikšmę (t.y. išmine'nus bloką), panaudotos transakcijos išimamos iš bendro transakcijų mempool'o
7. Tuomet atnaujinanami vartotojų balansai (įvykdomos transakcijos)
8. Blokas pridedamas prie grandinės

<img width="817" alt="Screenshot 2024-10-30 at 22 46 05" src="https://github.com/user-attachments/assets/d29a7b8b-3f18-4d65-a159-d244dc6ad337">

## Naudojimosi instrukcija
### Programos paleidimas
```g++ -o mano_programa v01.cpp -std=c++17```

```./mano_programa```

Pavyzdys:

<img width="417" alt="Screenshot 2024-10-30 at 23 49 35" src="https://github.com/user-attachments/assets/86d432d5-bb74-49da-86ea-c7b54384c651">


### Naudojimas

Paleidus programą, bus sugeneruota 1000 vartotojų, 10000 transakcijų. Tuomet bus sukurta blokų grandinė, inicializuojamas Genesis blokas, kuris bus pridėtas pirmas į blokų grandinę. Tuomet bus vykdomas blokų kūrimas, kuris tęsis tol, kol dar yra nepanaudotų transakcijų mempool'e. Atsitiktinai parenkama 100 transakcijų vienam blokui, jis sukuriamas, tada vyksta jo kasimas. Jei blokas sėkmingai iškastas, transakcijos ištrinamos iš mempoolo, jos įvykdomos, atnaujinami vartotojų balansai. Tuomet blokas pridedamas prie blokų grandinės ir vėl viskas kartojasi kol yra laisvų transakcijų. 
Visą blokų grandinės kūrimo procesą galima matyti konsolėje:

<img width="844" alt="Screenshot 2024-10-30 at 22 55 49" src="https://github.com/user-attachments/assets/35992797-5341-4f9a-b1eb-60161344a1ac">

Sukūrus blokų grandinę, Vartotojas gaus pranešimą, kad sukurta blokų grandinė iš tiek ir tiek blokų. Tada jam iššoks meniu, kur jis galės pasirinkti, ką toliau daryti:

<img width="352" alt="Screenshot 2024-10-30 at 22 57 46" src="https://github.com/user-attachments/assets/4bf7487f-7aef-4b45-b858-4128cd49e45c">

1. Pasirinkus išvesti bloką, reikės nurodyti bloko hash(header'į):
<img width="830" alt="Screenshot 2024-10-30 at 23 05 20" src="https://github.com/user-attachments/assets/f9394c02-3430-4528-9d31-6c274eebce7f">

2. Pasirinkus išvesti transakciją, reikės nurodyti transakcijos ID (siuntėjo pK, gavėjo pK, sumos hash reikšmę):
 <img width="718" alt="Screenshot 2024-10-30 at 23 02 46" src="https://github.com/user-attachments/assets/5c7092f8-f8ae-4cc0-b5ab-f348e3df0c4a">
 
3. Pasirinkus išvesti informaciją apie vartotoją, reikės nurodyti jo viešąjį raktą (public key):

<img width="815" alt="Screenshot 2024-10-30 at 22 59 47" src="https://github.com/user-attachments/assets/a9a38e8d-53b7-4ed0-8c18-e462f9e73081">


### Gerosios praktikos
1. Enkapsuliavimas

Privatūs kintamieji: Visų trijų klasių – `Vartotojas`, `Transakcija` ir `Blokas` – duomenų laukai yra apsaugoti kaip privatūs, kad jų nebūtų galima tiesiogiai keisti iš išorės. Tai apsaugo kritinę informaciją (pvz., vartotojų balansus, bloko hash) nuo netyčinio pakeitimo.
Vieši getter ir setter metodai: Kiekviena klasė suteikia prieigą prie duomenų naudodama aiškiai apibrėžtus getter ir setter metodus, kurie kontroliuoja prieigą prie privačių kintamųjų ir leidžia užtikrinti, kad duomenys bus keičiami tik saugiai ir tiksliai.


2. Klasėse naudojami konstruktoriai, RAII užtikrina kad resursai bus atlaisvinti, kai jų nebereikės.
# v0.2
Pridėtas UTXO modelis, transakcijų verifikavimas prieš dedant į bloką bei atliekant transakcijas, pridėta nauja kasimo funkcija su sąlygomis (bandymų skaičiumi ir laiku), įgyvendintas lygiagretus 5 blokų kasimas.
## Veikimas
1. Sugeneruojami vartotojai ir transakcijos
2. Sukuriamas genesis blokas
3. Klausiama ką nori vartotojas daryti toliau: 1 - kasti blokus tol kol yra nepanaudotų transakcijų, 2 - kasti 5 blokus kandidatus.
4. Pasirinkus bus atitinkamai arba sugeneruota 101 bloko grandin4 arba kasami 5 blokai kandidatai ir sudaroma 2 blokų grandinė.
5. Po šių veiksmų bus galima pasirinkti ką daryti toliau: išvesti bloką, išvesti transakciją, išvesti vartotoją.
<img width="887" alt="Screenshot 2024-11-06 at 17 42 16" src="https://github.com/user-attachments/assets/d7566211-5bd3-400c-ad37-de28bbdd4de6">

<img width="972" alt="Screenshot 2024-11-06 at 17 56 57" src="https://github.com/user-attachments/assets/7069ca7f-030e-484d-a493-d3e8b2c9ae19">




### Vartotoju ir transakcijų generavimas
Paleidus programą bus rodoma taip: 

<img width="544" alt="Screenshot 2024-11-06 at 15 38 22" src="https://github.com/user-attachments/assets/8f0e239e-f34d-4011-bb60-e091e40ab1ea">

Bus automatiškai sugeneruojama 1000 vartotojų su vardu (user1, user2...), viešuoju raktu hashFunkcija(user_) bei UTXOs: kiekvienam vartotojui atsitiktinai sugeneruojama nuo 1 iki 10 UTXOs ir kiekvieno UTXOs suma gali būti nuo 100 iki 10000 (tam naudojama funkcija randomSuma). Jie saugomi Vartotojas klasės vartotojai vektoriai.
Toliau seka transakciju generavimas, kur sugeneruojama 10000 transakcijų su gavėju (publicKey), siuntėju (publicKey) ir suma. Sugeneruotos transakcijos saugomos Transakcija klasės transakcijuMemPool vektoriuje.

### Transakcijų įtraukimas į bloką
Visų pirma paimama 100 atsitiktinių transakcijų iš transakcijų MemPool. Kadangi transakcijos parenkamos atsitiktinai, tai gali būti, kad siuntėjas negalės jų vykdyti. Tam tikslui sukurta funkcija validIBloka, kurioje naudojamas <unordered_map> laikiniBalansai. Ten vykdomas transakcijų tikrinimas, ar pakankamas balansas ir ar sutampa transakcijosID su gavėjo, siuntėjo ir sumos maišos rezultatu. Kadangi transakcijos yra įvykdomos tik iškasus bloką, todėl čia svarbų vaidmenį žaidžia laikiniBalansai, nes jie imituoja vartotojo balanso pasikeitimą priklausomai nuo transakcijų, bet *TIKRIEJI balansai ir UTXOs NĖRA NAUDOJAMI.* Kai praeinama 100 transakcijų, funkcija grąžina tinkamas transakcijas ir jos įtraukiamos į bloką.

### Bloko sandara
Klasėje blokas yra konstuktoius: 
```
    Blokas(string prev_blokas,vector<Transakcija> transakcijos, int difficulty_target, string versija, string minerioVardas)
    : prev_bloko_hash_(prev_blokas), transakcijos_(transakcijos), difficulty_target_(difficulty_target),
    versija_(versija), minerioVardas_(minerioVardas){
        laikas_ = time(nullptr);
        merkle_root_hash_ = skaiciuotiMerkleRoot();
        nonce_ = 0;
        
    }
```
Sukuriant bloką, t.y. įtraukiant transakcijas, bloko header'is pasipildo šiais elementais: prev_bloko_hash, timestamp, versija, difficulty target, nonce (kol blokas nekasamas jis 0) ir merkel_root_hash.
### Kaip apskaičiuojamas merkel root?
1. Pradinis maišų generavimas:
Surenkame kiekvieno transakcijos_ objekto ID (jau esamą maišos reikšmę) į hashai vektorių.
2. Iteracijos per maišų sąrašą:
Kol hashai yra didesnio nei 1 dydžio, iteruojame, kad sujungtume po dvi maišų reikšmes ir sugeneruotume jų jungtinę maišą.
Jei hashai dydis yra nelyginis, paskutinė maiša dubliuojama, kad kiekvieną kartą būtų sujungiamos poros (tai padeda išlaikyti pilną dvejetainį Merkle medžio struktūros balansą).
3. Rekursyvinis maišų jungimas:
Kiekvienos iteracijos metu maišų porų sujungimai vėl saugomi naujiHashai vektoriuje, kuris atnaujina hashai ir kartojasi iki tol, kol lieka tik viena maišų reikšmė.
4. Rezultatas:
Paskutinis likęs maišas vektoriuje hashai[0] yra Merkle šaknis, kurią funkcija grąžina.
```
   string skaiciuotiMerkleRoot()
    {
        if(transakcijos_.empty())
        {
            return "";
        }
        vector<string> hashai;
        for(const auto& trans: transakcijos_)
        {
            hashai.push_back(trans.getId());
        }
        while(hashai.size()>1)
        {
            if(hashai.size() % 2 != 0){
                hashai.push_back(hashai.back());
            }
            vector<string> naujiHashai;
            for(size_t i = 0; i < hashai.size(); i+=2)
            {
                string komb = hashai[i] + hashai[i+1];
                naujiHashai.push_back(hashFunkcija(komb));
            }
            hashai = naujiHashai;
        }
        return hashai[0];
    }
```

### Blokų kasimas
Blokas kasamas tol, kol gaunamas bloko_hash_ atitinkantis difficulty_target. Kasimas paremtas nonce_ didinimu, kai randamas toks nonce, kad `bloko_hash_ = hashFunkcija(prev_bloko_hash_+merkle_root_hash_+to_string(laikas_)+to_string(nonce_))` atitiktų diffTrgt.

### Kas vyksta iškasus bloką? 
1. Panaudotos transakcijos (esančios iškastame bloke) ištrinamos iš transakcijosMemPool.
2. Transakcijos įvykdomos, tam naudojama funkcija `atnaujintiBalansus`, kurioje dar kartą patikrinamas vartotojo bendras balansas. Jei siuntėjas turi pakankamą balansą, renkami atskiri UTXO įrašai, kad būtų pasiekta transakcijos suma. 
3. Blokas pridedamas į blokų grandinę.

### UTXO modelio naudojimas (1 papildoma užduotis)
1. Sukurta klasė UTXO:
```
class UTXO{
public:
    string utxoID;
    string vartotojoPK;
    int suma_;
    UTXO(){}
    UTXO(int suma, string useris):
    suma_(suma), vartotojoPK(useris){
        utxoID = hashFunkcija(vartotojoPK+to_string(suma_));
    }
    
};
```
Vartotjas klasei vietoje balansas_ naudojimo, naudojame vektorių su turimais UTXOs:
```
class Vartotojas{
private:
    string vardas_;
    string viesRaktas_;
    vector <UTXO> utxos_;
public:
```
2. Vartotojų generavimo metu kiekvienas vartotojas gauna nuo 1 iki 10 UTXOs, kurių kiekviena vertė gali būti nuo 100 iki 100000:

<img width="561" alt="Screenshot 2024-11-06 at 16 44 01" src="https://github.com/user-attachments/assets/09700191-044d-4e23-8148-1775c7cbcba1">


3. Transakicjų vykdymas realizuotas atnaujintiBalansus funkcijoje:
> UTXO pasirinkimas:
Jei siuntėjas turi pakankamą balansą, renkami atskiri UTXO įrašai, kad būtų pasiekta transakcijos suma.
surinktaSuma yra naudojama tikrinant, ar suma jau pakankama. Jei taip, siuntėjo UTXO pasirinkimas baigiamas.
> UTXO pašalinimas:
Pasirinkti UTXO yra pašalinami iš siuntėjo UTXO sąrašo. Tokiu būdu siuntėjas „išleidžia“ tuos UTXO ir jie nebebus naudojami ateities transakcijose.
> Gavėjo UTXO atnaujinimas:
Sukuriamas naujas UTXO įrašas su pervedama suma ir pridedamas prie gavėjo UTXO sąrašo. Taip užtikrinama, kad gavėjas gaus reikiamą balansą.
> Grąža:
Jei surinkta suma viršija transakcijos sumą (yra „grąža“), likusi suma sukuriama kaip naujas UTXO ir grąžinama atgal siuntėjui.
4. Vartotojai ir jų UTXOs po visko:
<img width="674" alt="Screenshot 2024-11-06 at 16 44 26" src="https://github.com/user-attachments/assets/58370765-ff7b-4ca9-a18b-d56625077cb7">





### Lygiagretus 5 blokų kandidatų kasimas (2 papildoma užduotis)
1. Sudaromi 5 potencialūs blokai kandidatai, į juos įtraukiamos transakcijos (blokuose transakcijos gali kartotis)
2. Kiekvienai gijai priskiriamas blokas - tam naudojami `vector<std::thread> gijos`.
3. Vykdomas blokų kasimas lygiagrečiai: 5 gijos vienu metu kasa 5 blokus, t.y. 1 gija kasa viena bloką.
4. Sukurta nauja kasimo funkcija su laiko ribojimu ir bandymų ribojimu (nonce_<=maxBandymuSk)
5. Pirmasis iškastas blokas pridedamas į blokų grandinę (
6. Jei nei vienas iš 5 blokų neiškastas, tada didinam bandymų skaičių ir laiką

<img width="886" alt="Screenshot 2024-11-06 at 17 10 41" src="https://github.com/user-attachments/assets/e4adce97-2ae8-479c-998e-ba9e6f72a759">

<img width="893" alt="Screenshot 2024-11-06 at 17 13 12" src="https://github.com/user-attachments/assets/3013eaed-5cc7-49a0-bdf6-72d9f5bb5cf2">

Taigi, kadanagi kiekvienas blokas yra sudarytas iš transakcijų kurios gali kartotis ir blokai kasami lygiagrečiai, tai visų jų previous_bloko hash yra genesis bloko hash. Dėl šios priežasties į blokų grandinę įdedamas tik pats pirmasis iškastas blokas kandidatas. 

<img width="345" alt="Screenshot 2024-11-06 at 17 17 27" src="https://github.com/user-attachments/assets/3cb281cc-5b3f-4e40-b0b9-b33d88470e8c">
<img width="873" alt="Screenshot 2024-11-06 at 17 17 50" src="https://github.com/user-attachments/assets/98d75641-35a2-4bef-b0b0-34c89e71631e">



