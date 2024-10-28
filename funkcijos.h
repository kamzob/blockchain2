//
//  funkcijos.h
//  v01
//
//  Created by Kamilė Zobėlaitė on 2024-10-19.
//

#ifndef funkcijos_h
#define funkcijos_h

#include "biblioteka.h"
string hashFunkcija(string input);
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
    
};
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
class Blokas{
private:
    string prev_bloko_hash_;
    string merkle_root_hash_;
    string bloko_hash_;
    string versija_;
    time_t laikas_;
    unsigned long long nonce_;
    int difficulty_target_;
    vector<Transakcija> transakcijos_;
    
public:
    Blokas(){};
    Blokas(string prev_blokas,vector<Transakcija> transakcijos, int difficulty_target, string versija)
    : prev_bloko_hash_(prev_blokas), transakcijos_(transakcijos), difficulty_target_(difficulty_target),
    versija_(versija){
        laikas_ = time(nullptr);
        merkle_root_hash_ = skaiciuotiMerkleRoot();
        nonce_ = 0;
        bloko_hash_ = mineBlock();
        
    }
    string getPrBlockHash() const{
        return prev_bloko_hash_;
    }
    string getMerkleRootHash() const{
        return merkle_root_hash_;
    }
    string getBlokoHash() const{
        return bloko_hash_;
    }
    string getVersion() const{
        return versija_;
    }
    time_t getLaikas() const{
        return laikas_;
    }
    unsigned long long getNonce() const{
        return nonce_;
    }
    int getDiffTarget() const{
        return difficulty_target_;
    }
    vector<Transakcija> getTransact() const{
        return transakcijos_;
    }
    
    
    void setPrevBlockHash (string previous_bloko_hash)
    {
        prev_bloko_hash_=previous_bloko_hash;
    }
    void setMerkleRootHash (string merkle_root_h)
    {
        merkle_root_hash_ = merkle_root_h;
    }
    void setBlokoHash(string blokoHash)
    {
        bloko_hash_ = blokoHash;
    }
    void setNonce(unsigned long long nonce){
        nonce_=nonce;
    }
    void setDiffTrgt(int difTarget)
    {
        difficulty_target_=difTarget;
    }
    void setTrans(vector<Transakcija> trans)
    {
        transakcijos_ = trans;
    }
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
    string mineBlock() {
        string target(difficulty_target_, '0');
        string hash;
        do {
            nonce_++;
            hash = hashFunkcija(prev_bloko_hash_+merkle_root_hash_+to_string(laikas_)+to_string(nonce_));
        }while (hash.substr(0, difficulty_target_)!=target);
        return hash;
    }
    void printBlock() {
        cout << "Blokas:" << endl;
        cout << "Versija: " << versija_ << endl;
        cout << "Ankstesnis bloko hash: " << prev_bloko_hash_ << endl;
        cout << "Merkle šaknis: " << merkle_root_hash_ << endl;
        cout << "Timestamp: " << put_time(localtime(&laikas_), "%Y-%m-%d %H:%M:%S") << endl;
        cout << "Nonce: " << nonce_ << endl;
        cout << "Bloko hash: " << bloko_hash_ << endl;
        cout << "Transakcijų kiekis: " << transakcijos_.size() << endl;
//        for (const auto& tx : transakcijos_) {
//            cout << "Transakcija ID: " << tx.getId() << " Siuntėjas: " << tx.getSiuntejas()
//                 << " Gavėjas: " << tx.getGavejas() << " Suma: " << tx.getSuma() << endl;
//        }
        cout << "-----------------------" << endl;
    }
    
};
//class Blockchain {
//private:
//    vector<Blokas> blokai_;
//    vector<Transakcija> nepatvirtintosTransakcijos;
//    int difficultyTarget;
//
//public:
//    Blockchain(int difficulty)
//        : difficultyTarget(difficulty) {
//        // Sukurkite pradžios bloką
//        vector<Transakcija> pradinesTransakcijos;
//        Blokas pirmasBlokas("0", pradinesTransakcijos, difficultyTarget, "1.0");
//        pirmasBlokas.mineBlock();
//        blokai_.push_back(pirmasBlokas);
//    }
//
//    void pridetiTransakcija(const Transakcija& transakcija) {
//        nepatvirtintosTransakcijos.push_back(transakcija);
//    }
////
////    vector<Transakcija> pasirinktiAtsitiktinesTransakcijas(int kiekis) {
////        vector<Transakcija> pasirinktos;
////        std::random_shuffle(nepatvirtintosTransakcijos.begin(), nepatvirtintosTransakcijos.end());
////        int count = min(kiekis, (int)nepatvirtintosTransakcijos.size());
////        pasirinktos.insert(pasirinktos.end(), nepatvirtintosTransakcijos.begin(), nepatvirtintosTransakcijos.begin() + count);
////        return pasirinktos;
////    }
//
//    void atnaujintiBalansus(vector<Transakcija>& transakcijos) {
//        // Atnaujinkite vartotojų balansus pagal įvykdytas transakcijas
//        for (auto& trans : transakcijos) {
//            // pvz., atnaujinkite siuntėjo ir gavėjo balansus čia
//            
//        }
//    }
//
//    void pridetiBloka(Blokas& blokas) {
//        blokai_.push_back(blokas);
//    }
//
//    void kasimasIrAtnaujinimas() {
//        while (!nepatvirtintosTransakcijos.empty()) {
//            vector<Transakcija> transakcijosBlokui = pasirinktiAtsitiktinesTransakcijas(100);
//
//            Blokas naujasBlokas(blokai_.back().getBlokoHash(), transakcijosBlokui, difficultyTarget, "1.0");
//            naujasBlokas.mineBlock();
//            
//            pridetiBloka(naujasBlokas);
//
//            atnaujintiBalansus(transakcijosBlokui);
//
//            // Pašalinkite į bloką įtrauktas transakcijas iš nepatvirtintų sąrašo
//            for (const auto& trans : transakcijosBlokui) {
//                auto itr = find(nepatvirtintosTransakcijos.begin(), nepatvirtintosTransakcijos.end(), trans);
//                if (itr != nepatvirtintosTransakcijos.end()) {
//                    nepatvirtintosTransakcijos.erase(itr);
//                }
//            }
//        }
//    }
//};

void naudojimosiInstrukcija();

unsigned long long int rightRotate (unsigned long long int reiksme, unsigned long long int d);
unsigned long long int leftRotate (unsigned long long int reiksme, unsigned long long int d);
string druskosGeneravimas(int ilgis);
string hashFunkcijaSuDruska(string input);
vector<Vartotojas> generuotiVartotojus(int n);
double randomDouble ();
vector<Transakcija> generuotiTransakcijas(vector<Vartotojas>& vartotojai, int transakcijuSk);
#endif /* funkcijos_h */
