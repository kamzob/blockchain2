//
//  funkcijos.h
//  v01
//
//  Created by Kamilė Zobėlaitė on 2024-10-19.
//

#ifndef funkcijos_h
#define funkcijos_h

#include "biblioteka.h"

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
    string getVar() const { return vardas_;}
    string getpKey() const { return viesRaktas_;}
    double getBalance() { return valiutosBal_;}
    
};
class Transakcija {
    
};

void naudojimosiInstrukcija();
string hashFunkcija(string input);
unsigned long long int rightRotate (unsigned long long int reiksme, unsigned long long int d);
unsigned long long int leftRotate (unsigned long long int reiksme, unsigned long long int d);
string druskosGeneravimas(int ilgis);
string hashFunkcijaSuDruska(string input);
vector<Vartotojas> generuotiVartotojus(int n);
double randomDouble ();
#endif /* funkcijos_h */
