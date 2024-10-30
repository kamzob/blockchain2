# blockchain2
# Supaprastintos blokų grandinės kūrimas
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
Paleidus programą, bus sugeneruota 1000 vartotojų, 10000 transakcijų. Tuomet bus sukurta blokų grandinė, inicializuojamas Genesis blokas, kuris bus pridėtas pirmas į blokų grandinę. Tuomet bus vykdomas blokų kūrimas, kuris tęsis tol, kol dar yra nepanaudotų transakcijų mempool'e. Atsitiktinai parenkama 100 transakcijų vienam blokui, jis sukuriamas, tada vyksta jo kasimas. Jei blokas sėkmingai iškastas, transakcijos ištrinamos iš mempoolo, jos įvykdomos, atnaujinami vartotojų balansai. Tuomet blokas pridedamas prie blokų grandinės ir vėl viskas kartojasi kol yra laisvų transakcijų. 
Visą blokų grandinės kūrimo procesą galima matyti konsolėje:

<img width="844" alt="Screenshot 2024-10-30 at 22 55 49" src="https://github.com/user-attachments/assets/35992797-5341-4f9a-b1eb-60161344a1ac">

Sukūrus blokų grandinę, Vartotojas gaus pranešimą, kad sukurta blokų grandinė iš tiek ir tiek blokų. Tada jam iššoks meniu, kur jis galės pasirinkti, ką toliau daryti:

<img width="352" alt="Screenshot 2024-10-30 at 22 57 46" src="https://github.com/user-attachments/assets/4bf7487f-7aef-4b45-b858-4128cd49e45c">
1. Pasirinkus išvesti bloką, reikės nurodyti bloko hash(header'į):
<img width="830" alt="Screenshot 2024-10-30 at 23 05 20" src="https://github.com/user-attachments/assets/f9394c02-3430-4528-9d31-6c274eebce7f">
2. Pasirinkus išvesti
 transakciją, reikės nurodyti transakcijos ID (siuntėjo pK, gavėjo pK, sumos hash reikšmę):
 <img width="718" alt="Screenshot 2024-10-30 at 23 02 46" src="https://github.com/user-attachments/assets/5c7092f8-f8ae-4cc0-b5ab-f348e3df0c4a">
3. Pasirinkus išvesti informaciją apie vartotoją, reikės nurodyti jo viešąjį raktą (public key):

<img width="815" alt="Screenshot 2024-10-30 at 22 59 47" src="https://github.com/user-attachments/assets/a9a38e8d-53b7-4ed0-8c18-e462f9e73081">





