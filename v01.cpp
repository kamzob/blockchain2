//
//  main.cpp
//  v01
//
//  Created by Kamilė Zobėlaitė on 2024-10-19.
//

#include "funkcijos.h"
#include "vartotojas.h"

//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}
int main()
{
    int vartSk = 1000;
    int transSk = 10000;
    int difTrgt = 3;
    srand( static_cast<unsigned int>(time(nullptr)));
   vector<Vartotojas> vartotojai = generuotiVartotojus(vartSk);
    for(int i = 0; i < 10; i++)
    {
        cout << vartotojai[i].getVar() << " " << vartotojai[i].getpKey() << " " << vartotojai[i].getBalance() << endl;
    }
    vector<Transakcija> transakcijosMemPool = generuotiTransakcijas(vartotojai, transSk);
    vector<Transakcija> tuscia;
    vector<Blokas> blockchain;
    Blokas genesis("0000000000000000000000000000000000000000000000000000000000000000", tuscia, difTrgt, "1");
    cout << genesis.mineBlock();
    blockchain.push_back(genesis);
    blockchain[0].printBlock();
    // Kol yra neapdorotų transakcijų mempoole
    while (!transakcijosMemPool.empty()) {
        
        // 1. Atsitiktinai pasirinkti 100 transakcijų
        std::vector<Transakcija> naujoBlokoTransakcijos;
        if (transakcijosMemPool.size() >= 100) {
            std::sample(transakcijosMemPool.begin(), transakcijosMemPool.end(), std::back_inserter(naujoBlokoTransakcijos), 100, std::mt19937{std::random_device{}()});
        } else {
            naujoBlokoTransakcijos = transakcijosMemPool; // jei likę mažiau nei 100
        }

        // 2. Sukurti naują bloką su 100 transakcijų
        Blokas naujasBlokas(blockchain.back().getBlokoHash(), naujoBlokoTransakcijos, difTrgt, "1.0");

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

        // - Atnaujinti vartotojų balansus
        for (const auto& transakcija : naujoBlokoTransakcijos) {
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

        // - Pridėti bloką prie blokų grandinės
        blockchain.push_back(naujasBlokas);

        // Spausdina bloką (patikrinimui)
        naujasBlokas.printBlock();
    }
    
    ofstream fr("vartojaipovisko.txt");
    for(const auto& vart: vartotojai)
    {
        fr << vart.getVar() << " " << vart.getpKey() << " " << vart.getBalance() << endl;
    }
    cout << "Likusios trans" << endl;
    for (const auto& transakcija : transakcijosMemPool)
    {
        transakcija.spausdintiTransakcija();
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
        balansas = randomDouble();
        vart.setVar(vardas);
        vart.setpKey(pKey);
        vart.setBal(balansas);
        fr << vardas << " " << pKey << " " << balansas << endl;
        vartotojai.push_back(vart);
        
        
    }
    fr.close();
    return vartotojai;
}
double randomDouble ()
{
    
    const double min = 100.0;
    const int max = 1000000.0;
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution(min,max);
    double random_double = distribution(generator);
    return random_double;
}

vector<Transakcija> generuotiTransakcijas(vector<Vartotojas>& vartotojai, int transakcijuSk){
    vector<Transakcija> transakcijos;
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
        double suma = (rand()%static_cast<int>(siuntejas.getBalance())+1);
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

