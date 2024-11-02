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
   vector<Vartotojas> vartotojai = generuotiVartotojus(vartSk);
    vector<Transakcija> transakcijosMemPool = generuotiTransakcijas(vartotojai, transSk);
    cout << "Sugeneruota " << transakcijosMemPool.size() << endl;
    vector<Transakcija> tuscia;
    vector<Blokas> blockchain;
    Blokas genesis("0000000000000000000000000000000000000000000000000000000000000000", tuscia, difTrgt, "1", "Nezinomas");
    cout << genesis.mineBlock();
    blockchain.push_back(genesis);
    vykdytiKasima(blockchain, transakcijosMemPool, vartotojai, maxKasimoLaikas, maxBandymuSk);
    
//    blockchain[0].printBlock();
//    
//    while (!transakcijosMemPool.empty()) {
//        int ctr = 1;
//        std::vector<Blokas> blokaiKandidatai;
//
//        // 4 blokų kandidatų kūrimas
//        for (int i = 0; i < 4; i++) {
//            std::vector<Transakcija> naujoBlokoTransakcijos;
//            if (transakcijosMemPool.size() >= 100) {
//                std::sample(transakcijosMemPool.begin(), transakcijosMemPool.end(), std::back_inserter(naujoBlokoTransakcijos), 100, std::mt19937{std::random_device{}()});
//            } else {
//                naujoBlokoTransakcijos = transakcijosMemPool;
//            }
//
//            // Tinkamų transakcijų surinkimas
//            std::vector<Transakcija> validTransakcijos;
//            for (const auto& transakcija : naujoBlokoTransakcijos) {
//                if (transakcijosTikrinimas(transakcija, vartotojai)) {
//                    validTransakcijos.push_back(transakcija);
//                }
//            }
//
//            if (!validTransakcijos.empty()) {
//                Blokas kandidatas(blockchain.back().getBlokoHash(), validTransakcijos, difTrgt, "1.0");
//                blokaiKandidatai.push_back(kandidatas);
//            }
//        }
//
//        Blokas iškastasBlokas;
//        bool blokasIškastas = false;
////        omp_set_num_threads(4);
//
//        // Kiekvieno bloko kandidato kasimas lygiagrečiai
////        #pragma omp parallel for
//        for (int i = 0; i < blokaiKandidatai.size(); i++) {
//            blokaiKandidatai[i].mineBlock();
////            #pragma omp critical
//            {
//                if (blokaiKandidatai[i].getIsMined() && !blokasIškastas) {
//                    iškastasBlokas = blokaiKandidatai[i];
//                    blokasIškastas = true;
//                }
//            }
//        }
//
//        if (blokasIškastas) {
//            // Patvirtintos transakcijos šalinamos iš mempool
//            for (const auto& transakcija : iškastasBlokas.getTransact()) {
//                auto it = std::find_if(transakcijosMemPool.begin(), transakcijosMemPool.end(), [&](const Transakcija& t) {
//                    return t.getId() == transakcija.getId();
//                });
//                if (it != transakcijosMemPool.end()) {
//                    transakcijosMemPool.erase(it);
//                }
//            }
//
//            // Balanso atnaujinimas
//            atnaujintiBalansus(iškastasBlokas.getTransact(), vartotojai);
//            iškastasBlokas.printBlock();
//            blockchain.push_back(iškastasBlokas);
//            ctr++;
//        } else {
//            std::cout << "Nepavyko iškasti bloko. Pailginamas kasimo laikas arba bandymų skaičius." << std::endl;
//        }
//    }

    
    
    
    // Kol yra neapdorotų transakcijų mempoole
//    while (!transakcijosMemPool.empty()) {
//        // Kandidatinių blokų sukūrimas (5 blokai)
//        vector<Blokas> blokaiKandidatai;
//        for (int i = 0; i < 5; i++) {
//            vector<Transakcija> naujoBlokoTransakcijos;
//            if (transakcijosMemPool.size() >= 100) {
//                std::sample(transakcijosMemPool.begin(), transakcijosMemPool.end(), std::back_inserter(naujoBlokoTransakcijos), 100, std::mt19937{std::random_device{}()});
//            } else {
//                naujoBlokoTransakcijos = transakcijosMemPool;
//            }
//
//            // Filtruojame tinkamas transakcijas
//            vector<Transakcija> validTransakcijos;
//            for (const auto& transakcija : naujoBlokoTransakcijos) {
//                if (transakcijosTikrinimas(transakcija, vartotojai)) {
//                    validTransakcijos.push_back(transakcija);
//                }
//            }
//
//            if (!validTransakcijos.empty()) {
//                Blokas kandidatas(blockchain.back().getBlokoHash(), validTransakcijos, difTrgt, "1.0");
//                blokaiKandidatai.push_back(kandidatas);
//            }
//        }
//
//        bool blokasIškastas = false;
//        for (int pailginti = 0; pailginti < 3 && !blokasIškastas; pailginti++) {
//            int bandymuSkaicius = maxBandymuSk * (pailginti + 1);
//            int kasimoLaikas = maxKasimoLaikas * (pailginti + 1);
//
//            // Atsitiktinai parenkame kandidatą ir bandom "kasti"
//            for (auto& kandidatas : blokaiKandidatai) {
//                if (kandidatoKasimas(kandidatas, bandymuSkaicius, kasimoLaikas)) {
//                    blockchain.push_back(kandidatas);
//                    for (const auto& transakcija : kandidatas.getTransact()) {
//                        auto it = std::find_if(transakcijosMemPool.begin(), transakcijosMemPool.end(), [&](const Transakcija& t) {
//                            return t.getId() == transakcija.getId();
//                        });
//                        if (it != transakcijosMemPool.end()) {
//                            transakcijosMemPool.erase(it);
//                        }
//                    }
//
//                    atnaujintiBalansus(kandidatas.getTransact(), vartotojai);
//                    kandidatas.printBlock();
//                    blokasIškastas = true;
//                    break;
//                }
//            }
//        }
//
//        if (!blokasIškastas) {
//            cout << "Nepavyko iškasti bloko. Pailginamas kasimo laikas arba bandymų skaičius." << endl;
//        }
//    }

    cout << "Sukurta blokų grandinė iš " << blockchain.size() << " blokų." << endl;
//    // Kol yra neapdorotų transakcijų mempoole
//    while (!transakcijosMemPool.empty()) {
//        
//        // 1. Atsitiktinai pasirinkti 100 transakcijų
//        std::vector<Transakcija> naujoBlokoTransakcijos;
//        if (transakcijosMemPool.size() >= 100) {
//            std::sample(transakcijosMemPool.begin(), transakcijosMemPool.end(), std::back_inserter(naujoBlokoTransakcijos), 100, std::mt19937{std::random_device{}()});
//        } else {
//            naujoBlokoTransakcijos = transakcijosMemPool; // jei likę mažiau nei 100
//        }
//        
//        // Filtruojame tinkamas transakcijas
//        std::vector<Transakcija> validTransakcijos;
//        for (const auto& transakcija : naujoBlokoTransakcijos) {
//            if (transakcijosTikrinimas(transakcija, vartotojai)) {
//                validTransakcijos.push_back(transakcija); // Įtraukiame tik patvirtintas transakcijas
//            }
//            else
//            {
//                auto it = std::find_if(transakcijosMemPool.begin(), transakcijosMemPool.end(), [&](const Transakcija& t) {
//                    return t.getId() == transakcija.getId();
//                });
//                if (it != transakcijosMemPool.end()) {
//                    transakcijosMemPool.erase(it);
//                }
//                
//            }
//        }
//
//        if (validTransakcijos.empty()) {
//            std::cout << "Nepavyko rasti tinkamų transakcijų šiame cikle." << std::endl;
//            break; // Išėjimas iš ciklo, jei nėra daugiau galiojančių transakcijų
//        }
//
//        // 2. Sukurti naują bloką su 100 transakcijų
//        Blokas naujasBlokas(blockchain.back().getBlokoHash(), validTransakcijos, difTrgt, "1.0");
//
//        // 3. Iškasti naują bloką naudojant PoW
//        naujasBlokas.mineBlock();
//
//        // 4. Jei sėkmingai rastas tinkamas hash:
//        // - Ištrinti transakcijas iš mempool
//        for (const auto& transakcija : validTransakcijos) {
//            auto it = std::find_if(transakcijosMemPool.begin(), transakcijosMemPool.end(), [&](const Transakcija& t) {
//                return t.getId() == transakcija.getId();
//            });
//            if (it != transakcijosMemPool.end()) {
//                transakcijosMemPool.erase(it);
//            }
//        }
//
//        // - Atnaujinti vartotojų balansus
//        for (const auto& transakcija : validTransakcijos) {
//            auto siuntejas = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& v) {
//                return v.getpKey() == transakcija.getSiuntejas();
//            });
//            auto gavejas = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& v) {
//                return v.getpKey() == transakcija.getGavejas();
//            });
//            if (siuntejas != vartotojai.end() && gavejas != vartotojai.end()) {
//                siuntejas->atnaujintiBalansa(-transakcija.getSuma());
//                gavejas->atnaujintiBalansa(transakcija.getSuma());
//            }
//        }
//
//        // - Pridėti bloką prie blokų grandinės
//        blockchain.push_back(naujasBlokas);
//
//        // Spausdina bloką (patikrinimui)
//        naujasBlokas.printBlock();
//    }
    
    ofstream fr("vartojaipovisko.txt");
    for(const auto& vart: vartotojai)
    {
        fr << vart.getVar() << " " << vart.getpKey() << " " << vart.getBalance() << endl;
    }
    
    cout << "Sukurta bloku grandine is " << blockchain.size() << " bloku" << endl;
    int rinktis;
    do{
        cout << "Pasirinkite ka norite daryti toliau:\n";
        cout << "1 - isvesti bloka\n";
        cout << "2 - isvesti transakcija\n";
        cout << "3 - isvesti vartotoja\n";
        cout << "4 - baigti darba\n";
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
        double balansas;
        Vartotojas vart;
        vardas = "user"+to_string(i+1);
        pKey = hashFunkcija(vardas);
        balansas = randomSuma();
        vart.setVar(vardas);
        vart.setpKey(pKey);
        vart.setBal(balansas);
        fr << vardas << " " << pKey << " " << balansas << endl;
        vartotojai.push_back(vart);
        
        
    }
    fr.close();
    return vartotojai;
}
int randomSuma() {
    const int min = 100;
    const int max = 1000000;
    static std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
    static std::uniform_int_distribution<int> distribution(min, max);
    int random_suma = distribution(generator);
    return random_suma;
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
        int maksimaliSuma = laikini_balansai[siuntejas.getpKey()];
        if(maksimaliSuma <= 0) {
            continue;
        }
        int suma = rand()%static_cast<int>(maksimaliSuma)+1;
        // laikinu balansu atnaujinimas
        laikini_balansai[siuntejas.getpKey()] -= suma;
        laikini_balansai[gavejas.getpKey()] += suma;
        string transakcijosID = hashFunkcija(siuntejas.getpKey() + gavejas.getpKey() + to_string(suma));
        fr << transakcijosID << " " << siuntejas.getpKey() << " " << gavejas.getpKey() << " " << suma << endl;
        Transakcija trans(transakcijosID, siuntejas.getpKey(), gavejas.getpKey(), suma);
        transakcijos.push_back(trans);
//        vartotojai[siuntIndex].setBal(siuntejas.getBalance() - suma);
//        vartotojai[gavIndex].setBal(gavejas.getBalance() + suma);
        
    }
    fr.close();
    return transakcijos;
}
bool transakcijosTikrinimas(const Transakcija& tx, const std::vector<Vartotojas>& vartotojai) {
    auto siuntejas = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& v) {
        return v.getpKey() == tx.getSiuntejas();
    });

    // Balanso tikrinimas
    if (siuntejas == vartotojai.end() || siuntejas->getBalance() < tx.getSuma()) {
//        std::cout << "Transakcija atmesta: nepakankamas siuntėjo balansas." << std::endl;
        return false;
    }

    // Maišos validacija
    string calculatedHash = hashFunkcija(tx.getSiuntejas() + tx.getGavejas() + std::to_string(tx.getSuma()));
//    if (calculatedHash != tx.getId()) {
////        std::cout << "Transakcija atmesta: transakcijos ID nesutampa su apskaičiuota maišos reikšme." << std::endl;
//        return false;
//    }

    return calculatedHash==tx.getId();  // Transakcija yra tinkama
}

bool kandidatoKasimas(Blokas& kandidatas, int maxBandymuSkaicius, int maxKasimoLaikas) {
    auto pradziosLaikas = std::chrono::steady_clock::now();
    int bandymai = 0;

    while (bandymai < maxBandymuSkaicius) {
        kandidatas.mineBlock();
        if (kandidatas.getIsMined()) {
            return true;
        }
        bandymai++;

        auto dabartinisLaikas = std::chrono::steady_clock::now();
        auto laikasPraejo =std::chrono::duration_cast<std::chrono::seconds>(dabartinisLaikas - pradziosLaikas).count();
        if (laikasPraejo >= maxKasimoLaikas) {
            break;
        }
    }
    return false;
}
bool kasimasSuKandidatais(vector<Blokas>& kandidatai, int maxBandymuSkaicius, int maxKasimoLaikas) {
    for (auto& kandidatas : kandidatai) {
        if (kandidatoKasimas(kandidatas, maxBandymuSkaicius, maxKasimoLaikas)) {
            return true;
        }
    }
    return false;  // Nei vienas kandidatas nebuvo iškastas
}
void atnaujintiBalansus(const vector<Transakcija>& transakcijos, vector<Vartotojas>& vartotojai) {
    for (const auto& transakcija : transakcijos) {
        auto siuntejas = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& v) {
            return v.getpKey() == transakcija.getSiuntejas();
        });
        auto gavejas = std::find_if(vartotojai.begin(), vartotojai.end(), [&](const Vartotojas& v) {
            return v.getpKey() == transakcija.getGavejas();
        });
        if (siuntejas != vartotojai.end() && gavejas != vartotojai.end()) {
            siuntejas->atnaujintiBalansa(-transakcija.getSuma());
            gavejas->atnaujintiBalansa(transakcija.getSuma());
        }
    }
}
void vykdytiKasima(vector<Blokas>& blockchain, vector<Transakcija>& transakcijos, vector<Vartotojas>& vartotojai, int maxKasimoLaikas, int maxBandymuSkaicius) {
    int minerioID=1;
    while (!transakcijos.empty()) {
        vector<Blokas> kandidatai;

        // Sukuriame 5 blokų kandidatus
        for (int i = 0; i < 5; ++i) {
            vector<Transakcija> kandidatoTransakcijos;
            std::sample(transakcijos.begin(), transakcijos.end(), std::back_inserter(kandidatoTransakcijos), 100, std::mt19937{std::random_device{}()});
            string minerioVardas = "Kamile_" + std::to_string(minerioID++);
            Blokas kandidatas(blockchain.back().getBlokoHash(), kandidatoTransakcijos, 3, "v0.2", minerioVardas);
            kandidatai.push_back(kandidatas);
        }

        // Bandom iškasti vieną iš kandidatų
        if (kasimasSuKandidatais(kandidatai, maxBandymuSkaicius, maxKasimoLaikas)) {
            // Randame iškastą bloką
            Blokas iskastasBlokas = *std::find_if(kandidatai.begin(), kandidatai.end(), [](const Blokas& b) { return b.getIsMined(); });

            // Pridedame bloką prie blockchain
            blockchain.push_back(iskastasBlokas);
            iskastasBlokas.printBlock();

            // Ištriname transakcijas, kurios buvo panaudotos
            for (const auto& tx : iskastasBlokas.getTransact()) {
                transakcijos.erase(std::remove_if(transakcijos.begin(), transakcijos.end(), [&](const Transakcija& t) { return t.getId() == tx.getId(); }), transakcijos.end());
            }
            atnaujintiBalansus(iskastasBlokas.getTransact(), vartotojai);
        } else {
            // Nepavyko iškasti, padidiname laiką arba bandymų skaičių ir kartojame
            maxKasimoLaikas += 5;
            maxBandymuSkaicius += 50000;
        }
    }
}
