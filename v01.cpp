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
    srand( static_cast<unsigned int>(time(nullptr)));
   vector<Vartotojas> vartotojai = generuotiVartotojus(1000);
    for(int i = 0; i < 10; i++)
    {
        cout << vartotojai[i].getVar() << " " << vartotojai[i].getpKey() << " " << vartotojai[i].getBalance() << endl;
    }
    return 0;
}
void naudojimosiInstrukcija()
{
    
    int rinktis;    // komandos parinkimas
    do{
        cout << "Pasirinkite:" << endl;
        cout << "1 - ivestis ranka\n";
        cout << "2 - ivestis is failo\n";
        cout << "3 - generuoti failus\n";
        cout << "4 - avalanche testavimas\n";
        cout << "5 - atsparumas kolizijai\n";
        cout << "6 - konstitucijos testavimas\n";
        cout << "7 - atlikti mano hash ir sha-256 palyginima\n";
        cout << "8 - atlikti hiding testavima hash su druska\n";
        cout << "9 - atlikti puzzle-friendliness testavima hash su druska\n";
        cout << "10 - baigti darbą\n";
        cin >> rinktis;
        while(!cin>>rinktis || rinktis < 1 || rinktis > 10)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Klaida! Turite pasirinkti nuo 1 iki 10: \n";
            cin >> rinktis;
            
        }
        
        switch (rinktis)
        {
            case 1:{
                string tekstas;
                cout << "Irasykite norima teksta: \n";
                //getline(cin,tekstas);
                cin >> tekstas;
                cout << "Ivestas tekstas: \n" << tekstas << endl;
                cout << "Sio teksto hash:\n" << hashFunkcija(tekstas) << endl;
                break;}
            case 2:{
//                string tekstas = skaityti();
//                cout << hashFunkcija(tekstas) << endl;
                break;
            }
            case 3:{
//                failuGeneravimas();
                break;
            }
            case 4:{
//                 AvalancheTestavimas();
                break;
            }
            case 5:{
//                kolizijosTestavimas();
                break;
            }
            case 6:{
//                konstitucijosTestavimas();
                break;
            }
            case 7:{
//                manoHashVS256Hash();
                break;
            }
            case 8:{
//                hidingTyrimas();
                break;
            }
            case 9:{
//                puzzleFriendliness();
                break;
            }
        }
    }while(rinktis!=10);


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


// Transakcijos struktūra
struct Transaction {
    string transaction_id;
    string sender;
    string receiver;
    long long amount;
};

// Bloko struktūra
struct Block {
    string previous_hash;
    vector<Transaction> transactions;
    string block_hash;
    long long nonce;
};
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
string druskosGeneravimas(int ilgis)
{
    string druska;
    for(int i = 0; i < ilgis; i++)
    {
        char randSimb = ' ' + rand()%95;
        druska+=randSimb;
    }
    return druska;
}
string hashFunkcijaSuDruska(string input)
{
    string druska = druskosGeneravimas(16);
    //cout << druska << endl;
    string ivestis =input + druska;
    const unsigned long long sk1 = 0x100000001b3; //1099511628211 pirminis
    const unsigned long long sk2 = 0xab5351bc652b4e61;
    //12345300873145699937 pirminis sk
    vector<unsigned long long> outputHash(4, 0);
    for(int i = 0; i < ivestis.length(); i++)
    {
        char dabSimb = ivestis[i];
        unsigned long long reiksme = static_cast<unsigned long long>(dabSimb);
        for(int j = 0; j < 4; j++)
        {
//            cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j] ^= reiksme;
//           cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j]*= sk1;
           // cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j] = leftRotate(outputHash[j], 13);
           // cout << (std::bitset<64>) outputHash[j] << endl;
            outputHash[j]^=rightRotate(outputHash[(j+1)%4], 17);
//            cout << (std::bitset<64>) outputHash[j] << endl;
//            cout << endl;
            reiksme *= sk2;
        }
    }
    std::stringstream ss;
    for (const auto& val : outputHash) {
            ss << std::hex << std::setfill('0') << std::setw(16) << val;
        }
    
        return ss.str();
    
}
