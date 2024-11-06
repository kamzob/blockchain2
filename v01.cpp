//
//  main.cpp
//  v01
//
//  Created by Kamilė Zobėlaitė on 2024-10-19.
//

#include "funkcijos.h"

int main()
{
    int vartSk = 1000;
    int transSk = 10000;
    int difTrgt = 3;
    int maxKasimoLaikas = 5;
    int maxBandymuSk = 100000;
    srand( static_cast<unsigned int>(time(nullptr)));
    cout << "Vykdomas vartotoju generavimas..." << endl;
   vector<Vartotojas> vartotojai = generuotiVartotojus(vartSk);
    if(!vartotojai.empty())
    {
        cout << "Vartotoju generavimas sekmingas! Sugeneruota: " << vartotojai.size() << endl;
    }
    cout << "Vykdomas transakciju generavimas..." << endl;
    vector<Transakcija> transakcijosMemPool = generuotiTransakcijas(vartotojai, transSk);
    if(!transakcijosMemPool.empty())
    {
        cout << "Vartotoju generavimas sekmingas! Sugeneruota: " << transakcijosMemPool.size() << endl;
    }
    

    vector<Transakcija> tuscia;
    vector<Blokas> blockchain;
    Blokas genesis("0000000000000000000000000000000000000000000000000000000000000000", tuscia, difTrgt, "1", "Nezinomas");
    cout << "Sukurtas genesis blokas: " << endl;
    genesis.mineBlock();
    genesis.printBlock();
    blockchain.push_back(genesis);
    
    
    int pasirinkimas;
    cout << "Pasirinkite, kuri veiksma norite atlikti:" << endl;
    cout << "1 - Kasti blokus naudojant visas transakcijas" << endl;
    cout << "2 - Kasti lygiagreciai 5 blokus kandidatus" << endl;
    cin >> pasirinkimas;

    if (pasirinkimas == 1) {
        kolYraTransakciju(blockchain, transakcijosMemPool, vartotojai, difTrgt);
    } else if (pasirinkimas == 2) {
        vykdytiKasima(blockchain, transakcijosMemPool, vartotojai, maxKasimoLaikas, maxBandymuSk);
    } else {
        cout << "Neteisingas pasirinkimas. Baigiama programa." << endl;
        return 0;
    }

    cout << "Sukurta blokų grandinė iš " << blockchain.size() << " blokų." << endl;

    
    ofstream fr("vartojaipovisko.txt");
    for(const auto& vart: vartotojai)
    {
        fr << vart.getVar() << "\n" << vart.getpKey() << "\n" << "Balansas: " << vart.getBalance() << endl;
        fr << "UTXOs: \n";
        for (const auto& ut: vart.GetUtxos())
        {
            fr << "ID: " << ut.utxoID << " Suma: " << ut.suma_ << endl;
            
        }
    }
    
    cout << "Sukurta bloku grandine is " << blockchain.size() << " bloku" << endl;
    int rinktis;
    do{
        cout << "Pasirinkite ka norite daryti toliau:\n";
        cout << "1 - isvesti bloka\n";
        cout << "2 - isvesti transakcija\n";
        cout << "3 - isvesti vartotoja\n";
        cout << "4 - isvesti bloka su transakcijomis\n";
        cout << "5 - baigti darba\n";
        cin >> rinktis;
        while(!cin>>rinktis || rinktis <1 || rinktis > 4)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Klaida! Turite pasirinkti nuo 1 iki 4\n";
            cin >> rinktis;
        }
        switch(rinktis)
        {
            case 1:{
                string hash;
                cout << "Iveskite bloko hash: \n";
                cin >> hash;
                auto it = std::find_if(blockchain.begin(), blockchain.end(), [&](const Blokas& blokas){
                    return blokas.getBlokoHash() == hash;
                });
                if(it != blockchain.end()){
                    it->printBlock();
                }
                else {
                    cout << "Blokas su siuo hash nerastas.\n";
                }
                break;
                
            }
            case 2 :{
                string transId;
                cout << "Iveskite transakcijos ID: \n";
                cin >> transId;
              bool found = false;
              for (const auto& blokas : blockchain) {
                  for (const auto& transakcija : blokas.getTransact()) {
                      if (transakcija.getId() == transId) {
                          transakcija.spausdintiTransakcija();
                          found = true;
                          break;
                      }
                  }
                  if (found) break;
              }
              if (!found) {
                  cout << "Transakcija su šiuo ID nerasta.\n";
              }
              break;
            }
            case 3:{
                string pKey;
                cout << "Iveskite vartotojo viesaji rakta:\n";
                cin >> pKey;
                auto it = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& vart){
                    return vart.getpKey()==pKey;
                });
                if(it!=vartotojai.end()) {
                    it->spausdintiUseri();
                }
                else {
                    cout << "Su siuo viesuoju raktu vartotojas nebuvo rastas" << endl;
                }
                
            }
            case 4:{
                string hash;
                cout << "Iveskite bloko hash: \n";
                cin >> hash;
                auto it = std::find_if(blockchain.begin(), blockchain.end(), [&](const Blokas& blokas){
                    return blokas.getBlokoHash() == hash;
                });
                if(it != blockchain.end()){
                    it->printBlockWithTrans();
                }
                else {
                    cout << "Blokas su siuo hash nerastas.\n";
                }
                break;
            }
        }
    
        
        
    } while(rinktis!=4);
//
    
    return 0;
}

string hashFunkcija(string input)
{
    const unsigned long long sk1 = 0x100000001b3; //1099511628211 pirminis
    const unsigned long long sk2 = 0xab5351bc652b4e61;
    //12345300873145699937 sk
    vector<unsigned long long> outputHash(4, 0);
    for(int i = 0; i < input.length(); i++)
    {
        char dabSimb = input[i];
        unsigned long long reiksme = static_cast<unsigned long long>(dabSimb);
//        cout <<(std::bitset<64>) sk1 << endl;
        for(int j = 0; j < 4; j++)
        {
//            cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j] ^= reiksme;
//           cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j]*= sk1;
//            cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j] = leftRotate(outputHash[j], 13);
//             cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j]^=rightRotate(outputHash[(j+1)%4], 17);
//            cout <<  (std::bitset<64>) outputHash[j] << endl;
//            cout << endl;
            reiksme *= sk2;
        }
    }
//    for(int i = 0; i < 4; i++)
//        cout << outputHash[i] << endl;
    std::stringstream ss;
    for (const auto& val : outputHash) {
            ss << std::hex << std::setfill('0') << std::setw(16) << val;
        }
    
        return ss.str();
    
}


unsigned long long int rightRotate (unsigned long long int reiksme, unsigned long long int d) {
    return (reiksme >> d) | (reiksme << (64 - d));
}
unsigned long long int leftRotate (unsigned long long int reiksme, unsigned long long int d) {
    return (reiksme << d) | (reiksme >> (64 - d));
}



vector<Vartotojas> generuotiVartotojus(int n){
    vector<Vartotojas> vartotojai;
    ofstream fr("vartotojai.txt");
    for(int i = 0; i < n; i++)
    {
        string vardas;
        string pKey;
        vardas = "user"+to_string(i+1);
        pKey = hashFunkcija(vardas);
        Vartotojas vart(vardas, pKey);
        int utxoSk = randomSuma(1, 10);
        vector<UTXO> utxos;
        fr << vardas << "\n" << pKey << "\n" << "UTXOs:" << endl;
        for(int i = 0; i < utxoSk; i++)
        {
            int suma = randomSuma(100, 100000);
            UTXO utx(suma, pKey);
            utxos.push_back(utx);
            fr << "ID:" << utx.utxoID << " Suma: " << utx.suma_ << endl;
            
        }
        vart.setUTXO(utxos);
//        balansas = randomSuma();

//        vart.setBal(balansas);

        vartotojai.push_back(vart);
 
    }
    fr.close();
    return vartotojai;
}
int randomSuma(const int min, const int max) {

    static std::random_device rd;
      static std::mt19937 generator(rd());  // Užtikrina pakankamą atsitiktinumą
      std::uniform_int_distribution<int> distribution(min, max);
      return distribution(generator);
}
vector<Transakcija> generuotiTransakcijas(vector<Vartotojas>& vartotojai, int transakcijuSk){
    vector<Transakcija> transakcijos;
    std::unordered_map<string, double> laikini_balansai;
    // Inicializuojame laikinuosius balansus
    for (const auto& vart : vartotojai) {
        laikini_balansai[vart.getpKey()] = vart.getBalance();
    }
    ofstream fr("transakcijos.txt");
    for(int i = 0; i < transakcijuSk; i++)
    {
        int siuntIndex = rand() % vartotojai.size();
        int gavIndex = rand() % vartotojai.size();
        while(siuntIndex==gavIndex)
        {
            gavIndex = rand()%vartotojai.size();
        }
        Vartotojas siuntejas = vartotojai[siuntIndex];
        Vartotojas gavejas = vartotojai[gavIndex];
//
//        int maksimaliSuma = laikini_balansai[siuntejas.getpKey()];
//        if(maksimaliSuma <= 0) {
//            continue;
//        }
//        int suma = rand()%static_cast<int>(maksimaliSuma)+1;
        // laikinu balansu atnaujinimas
//        laikini_balansai[siuntejas.getpKey()] -= suma;
//        laikini_balansai[gavejas.getpKey()] += suma;
        int suma = randomSuma(1, 50000);
        string transakcijosID = hashFunkcija(siuntejas.getpKey() + gavejas.getpKey() + to_string(suma));
        fr << transakcijosID << " " << "\nSiuntejas: "<< siuntejas.getpKey() << " " << "\n Gavejas: " << gavejas.getpKey() << "\n" << "Suma: " << suma << endl;
        Transakcija trans(transakcijosID, siuntejas.getpKey(), gavejas.getpKey(), suma);
        transakcijos.push_back(trans);
//        vartotojai[siuntIndex].setBal(siuntejas.getBalance() - suma);
//        vartotojai[gavIndex].setBal(gavejas.getBalance() + suma);
        
    }
    fr.close();
    return transakcijos;
}
vector<Transakcija> validIBloka(const vector<Transakcija>& atsitiktinesTransakcijos, const vector<Vartotojas>& vartotojai) {
    std::unordered_map<string, int>laikiniBalansai;
    vector<Transakcija> galiojanciosTransakcijos;
    for(const auto& vart: vartotojai)
    {
        laikiniBalansai[vart.getpKey()] = vart.getBalance();
    }
    for(const auto& tx: atsitiktinesTransakcijos)
    {
        auto siuntejoPK = tx.getSiuntejas();
        auto gavejoPK = tx.getGavejas();
        int suma = tx.getSuma();
        if(laikiniBalansai[siuntejoPK]>=suma && hashFunkcija(siuntejoPK+gavejoPK+to_string(suma))==tx.getId())
        {
            laikiniBalansai[gavejoPK]+=suma;
            laikiniBalansai[siuntejoPK]-=suma;
            galiojanciosTransakcijos.push_back(tx);
        }
        else {
            cout << "Nepavyko itraukti transakcijos i bloka del nepakankamo balanso arba pakeisto siuntejo/gavejo: " << tx.getId() << endl;
        }
        
    }
    return galiojanciosTransakcijos;
}

void atnaujintiBalansus(const vector<Transakcija>& transakcijos, vector<Vartotojas>& vartotojai) {
    for (const auto& transakcija : transakcijos) {
        auto siuntejas = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& v) {
            return v.getpKey() == transakcija.getSiuntejas();
        });
        auto gavejas = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& v) {
            return v.getpKey() == transakcija.getGavejas();
        });
        // ar siuntejas ir gavejas rasti
        if (siuntejas != vartotojai.end() && gavejas != vartotojai.end()) {
            // jei rasti, tikrinama ar siuntejo utxo suma (balansas) yra pakankamas
            if (siuntejas->getBalance() < transakcija.getSuma()) {
                std::cout << "Nepakanka lėšų siuntėjo balanse. Transakcija atmesta." << std::endl;
                continue;
            }
            vector<UTXO> pasirinktiUTXO;
            int surinktaSuma = 0;
            
            for (const auto& utxo : siuntejas->GetUtxos()) {
                pasirinktiUTXO.push_back(utxo);
                surinktaSuma += utxo.suma_;
                if (surinktaSuma >= transakcija.getSuma()) {
                    break;
                }
            }
            
            // Pašaliname pasirinktas UTXO iš siuntėjo
            for (const auto& utxo : pasirinktiUTXO) {
                siuntejas->pasalintiUTXO(utxo.utxoID);
            }
            
            // Sukuriame naują UTXO gavėjui su pervedama suma
            UTXO naujasUTXO(transakcija.getSuma(), gavejas->getpKey());
            gavejas->pridetiUTXO(naujasUTXO);
            
            // Jei yra grąža, sukuriame naują UTXO siuntėjui
            int grazinamojiSuma = surinktaSuma - transakcija.getSuma();
            if (grazinamojiSuma > 0) {
                UTXO grazosUTXO(grazinamojiSuma, siuntejas->getpKey());
                siuntejas->pridetiUTXO(grazosUTXO);
            }
        }
    }
}
        
            
//            siuntejas->atnaujintiBalansa(-transakcija.getSuma());
//            gavejas->atnaujintiBalansa(transakcija.getSuma());
   





void vykdytiKasima(vector<Blokas>& blockchain, vector<Transakcija>& transakcijos, vector<Vartotojas>& vartotojai, int maxKasimoLaikas, int maxBandymuSkaicius) {
    int minerioID = 1;
    vector<Blokas> kandidatai;
    
    
    // Paruošiame 5 kandidatinius blokus
    for (int i = 0; i < 5; i++) {
        vector<Transakcija> bandomosTransakcijos;
        std::sample(transakcijos.begin(), transakcijos.end(), std::back_inserter(bandomosTransakcijos), 100, std::mt19937{std::random_device{}()});
        string minerioVardas = "Kamile_" + std::to_string(minerioID++);
        vector<Transakcija> validTransakcijos = validIBloka(bandomosTransakcijos, vartotojai);
        Blokas kandidatas(blockchain.back().getBlokoHash(), validTransakcijos, 3, "v0.2", minerioVardas);
        kandidatai.push_back(kandidatas);
    }
    
    
    std::atomic<bool> blokasPridetas(false); // žymė, ar pirmas blokas jau pridėtas
    bool blokasIskastas = false; // zymi ar bent vienas blokas iskastas
    
    do{
        std::vector<std::thread> gijos;
        // Lygiagrečiai kasame kiekvieną kandidatą su gijos ID
        for (size_t i = 0; i < kandidatai.size(); i++) {
            gijos.emplace_back([&, i, maxKasimoLaikas, maxBandymuSkaicius]() {
                kandidatai[i].kastiBlokaSuLimitais(maxKasimoLaikas, maxBandymuSkaicius);
                if (kandidatai[i].getIsMined()) {
                    std::cout << "Gija " << i << " sėkmingai iškasė bloką su nonce: " << kandidatai[i].getNonce() << std::endl;
                    kandidatai[i].printBlock();
                    blokasIskastas = true;
                    if(!blokasPridetas.exchange(true))
                    {
                        cout << "^" << endl;
                        cout << "|" << endl;
                        cout << "Šis blokas pridedamas i bloku grandine" << endl;
                        for (const auto& tx : kandidatai[i].getTransact()) {
                            transakcijos.erase(std::remove_if(transakcijos.begin(), transakcijos.end(),
                                                              [&](const Transakcija& t) { return t.getId() == tx.getId(); }),
                                               transakcijos.end());
                        }
                        atnaujintiBalansus(kandidatai[i].getTransact(), vartotojai);
                        
                        
                        blockchain.push_back(kandidatai[i]);
                        
                    }
                }
            });
        }
        
        
        // Laukiame, kol visos gijos baigs kasimą
        for (auto& gija : gijos) {
            if (gija.joinable()) {
                gija.join();
            }
        }
        if (!blokasIskastas) {
            maxKasimoLaikas += 5; // Padidiname laiką, pavyzdžiui, 5 sekundėmis
            maxBandymuSkaicius += 50000; // Padidiname bandymų limitą, pavyzdžiui, 50 000
            std::cout << "Nepavyko iškasti nė vieno bloko per pradinį laiką. Didiname kasimo laiką iki "<< maxKasimoLaikas << " ir bandymų skaičių iki " << maxBandymuSkaicius << std::endl;
            
        }
    } while(!blokasIskastas && !blokasPridetas);
    

}
    
    // paprastas kasimas 5 bloku kandidatu
    void vykdytiKasimaa(vector<Blokas>& blockchain, vector<Transakcija>& transakcijos, vector<Vartotojas>& vartotojai, int maxKasimoLaikas, int maxBandymuSkaicius) {
        int minerioID = 1;
        vector<Blokas> kandidatai;
        // bloku kandidatu paruosimas
        for(int i = 0; i < 5; i++)
        {
            vector<Transakcija> bandomosTransakcijos;
            std::sample(transakcijos.begin(), transakcijos.end(), std::back_inserter(bandomosTransakcijos), 100, std::mt19937{std::random_device{}()});
            string minerioVardas = "Kamile_" + std::to_string(minerioID++);
            vector<Transakcija> validTransakcijos = validIBloka(bandomosTransakcijos, vartotojai);
            Blokas kandidatas(blockchain.back().getBlokoHash(), validTransakcijos, 3, "v0.2", minerioVardas);
            kandidatai.push_back(kandidatas);
            
        }
        for(auto& kandidatas: kandidatai)
        {
            kandidatas.kastiBlokaSuLimitais(maxKasimoLaikas, maxBandymuSkaicius);
            if(kandidatas.getIsMined())
            {
                cout << "Blokas iskastas sekmingai" << endl;
                kandidatas.printBlock();
                for (const auto& tx : kandidatas.getTransact()) {
                    transakcijos.erase(std::remove_if(transakcijos.begin(), transakcijos.end(), [&](const Transakcija& t) { return t.getId() == tx.getId(); }), transakcijos.end());
                    
                }
                atnaujintiBalansus(kandidatas.getTransact(), vartotojai);
                blockchain.push_back(kandidatas);
                
                
            }
            
        }
    }

void kolYraTransakciju(vector<Blokas>& blockchain, vector<Transakcija>& transakcijosMemPool, vector<Vartotojas>& vartotojai, int difTrgt){
        // Kol yra neapdorotų transakcijų mempoole
    int ID = 1;
        while (!transakcijosMemPool.empty()) {
    
            // 1. Atsitiktinai pasirinkti 100 transakcijų
            std::vector<Transakcija> naujoBlokoTransakcijos;
            if (transakcijosMemPool.size() >= 100) {
                std::sample(transakcijosMemPool.begin(), transakcijosMemPool.end(), std::back_inserter(naujoBlokoTransakcijos), 100, std::mt19937{std::random_device{}()});
            } else {
                naujoBlokoTransakcijos = transakcijosMemPool; // jei likę mažiau nei 100
            }
            vector<Transakcija> galimosTransakcijos = validIBloka(naujoBlokoTransakcijos, vartotojai);
    
    
            if (galimosTransakcijos.empty()) {
                std::cout << "Nepavyko rasti tinkamų transakcijų šiame cikle." << std::endl;
                break; // Išėjimas iš ciklo, jei nėra daugiau galiojančių transakcijų
            }
            string miner = "Kamile_" + to_string(ID++);
    
            // 2. Sukurti naują bloką su 100 transakcijų
            Blokas naujasBlokas(blockchain.back().getBlokoHash(), galimosTransakcijos, difTrgt, "1.0", miner);
    
            // 3. Iškasti naują bloką naudojant PoW
            naujasBlokas.mineBlock();
    
            // 4. Jei sėkmingai rastas tinkamas hash:
            // - Ištrinti transakcijas iš mempool
            for (const auto& transakcija : naujoBlokoTransakcijos) {
                auto it = std::find_if(transakcijosMemPool.begin(), transakcijosMemPool.end(), [&](const Transakcija& t) {
                    return t.getId() == transakcija.getId();
                });
                if (it != transakcijosMemPool.end()) {
                    transakcijosMemPool.erase(it);
                }
            }

            atnaujintiBalansus(naujasBlokas.getTransact(), vartotojai);
    
            // - Pridėti bloką prie blokų grandinės
            blockchain.push_back(naujasBlokas);
    
            // Spausdina bloką (patikrinimui)
            naujasBlokas.printBlock();
        }
}
