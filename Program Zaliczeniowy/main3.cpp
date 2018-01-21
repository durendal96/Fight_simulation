/* 
 * File:   main.cpp
 * Author: Bartosz Kluczka
 *
 * Created on 27 grudnia 2017, 10:29
 */

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;


class Jednostka{
protected:
    int sila;
    int obrona;
    int wytrz;
    int zasieg;
    int liczeb;
    double morale;
    int x;
    int y;
    
//metody 
public:
    virtual string status()=0;
    virtual void atakuj(Jednostka*)=0;
    virtual Jednostka* celuj()=0;
    virtual bool bonus()=0;
    virtual ~Jednostka();
    int dajx(){return this->x;};
    int dajy(){return this->y;};
    virtual bool zywa(){return (this->liczeb)>0;};
};


class Atak: public Jednostka{
protected:
    int sila;
    int obrona;
    int wytrz;
    int zasieg;
    int liczeb;
    double morale;
    int x;
    int y;
public:
//technikalia
    ~Atak()=0;
    string status()=0;
    void atakuj(Jednostka*)=0;
    bool bonus()=0;
    Jednostka* celuj();
};

class Miecznik: public Atak{
    int sila;
    int obrona;
    int wytrz;
    int zasieg;
    int liczeb;
    double morale;
    int x;
    int y;
public:
    Miecznik(const Miecznik&)=default;
    ~Miecznik()=default;
    Miecznik(int,int);      
    Miecznik& operator=(const Miecznik&)=default;
    
    bool zywa(){return (this->liczeb)>0;};
    void atakuj(Jednostka*);
    bool bonus();
    string status();
};

class Konny: public Atak{
    int sila;
    int obrona;
    int wytrz;
    int zasieg;
    int liczeb;
    double morale;
    int x;
    int y;
public:
    Konny(const Konny&)=default;
    ~Konny()=default;
    Konny(int,int);       
    Konny& operator=(const Konny&)=default;
    
    void atakuj(Jednostka*);
    bool bonus();
    string status();
};

class Lucznik: public Atak{
    int sila;
    int obrona;
    int wytrz;
    int zasieg;
    int liczeb;
    double morale;
    int x;
    int y;
public:
    Lucznik(const Lucznik&)=default;
    ~Lucznik()=default;
    Lucznik(int,int);       
    Lucznik& operator=(const Lucznik&)=default;
    
    void atakuj(Jednostka*);
    bool bonus();
    string status();
};

class Pole_bitwy{
    
private:
    int szerokosc;
    Jednostka*** pole;
public:
    ~Pole_bitwy();
    Pole_bitwy(int, string*);
    Pole_bitwy(const Pole_bitwy&)=default;
    Pole_bitwy& operator=(const Pole_bitwy&)=default;
    
    int szer(){return szerokosc;};
    Jednostka*** pol(){return pole;};
    Pole_bitwy& tura();
    Jednostka* inicjuj(char,int,int);
};
//Implementacja konstruktorow i destruktorow
 //Zwarcie

Jednostka::~Jednostka(){};
Atak::~Atak(){};
Jednostka* Atak::celuj(){};    

//Miecznik
Miecznik::Miecznik(int a, int b): sila(30), obrona(20), wytrz(50), zasieg(1), liczeb(200), morale(0), x(a), y(b){};

string Miecznik::status(){
    string stat;
    stat="M:";
    if(this->liczeb<200){
        stat+=(this->liczeb)/200;
    }
    else if(this->liczeb==0){
        stat+="X";
    }
    else{
        stat+="00";  
    };
    stat+=" ";
    return stat;
};

bool Miecznik::bonus(){ return true;};

void Miecznik::atakuj(Jednostka*){};

//Konny

Konny::Konny(int a, int b): sila(50), obrona(10), wytrz(70), zasieg(2), liczeb(100), morale(0), x(a), y(b){};

string Konny::status(){
    string stat;
    stat="K:";
    if(this->liczeb<100){
        stat+=(this->liczeb)/100;
    }
    else if(this->liczeb==0){
        stat+="X";
    }
    else{
        stat+="00";  
    };
    stat+=" ";
    return stat;
};

bool Konny::bonus(){return true;};

void Konny::atakuj(Jednostka*){};

//Lucznik
Lucznik::Lucznik(int a, int b): sila(30), obrona(5), wytrz(20), zasieg(5), liczeb(200), morale(0), x(a), y(b){};

string Lucznik::status(){
    string stat;
    stat="L:";
    if(this->liczeb<200){
        stat+=(this->liczeb)/200;
    }
    else if(this->liczeb==0){
        stat+="X";
    }
    else{
        stat+="00";  
    };
    stat+=" ";
    return stat;
};

bool Lucznik::bonus(){return true;};

void Lucznik::atakuj(Jednostka*){};

//Pole Bitwy implementacja

Pole_bitwy& Pole_bitwy::tura(){
    for(int i=0;i<6;i++){
        for(int j=0; j<szerokosc;j++){
            pole[i][j]->atakuj(pole[i][j]->celuj());
        };
    };
    return *this;
};

Pole_bitwy::~Pole_bitwy(){
    for (int i=1;i<=5;i++){
        for(int j=0;j<szerokosc;j++){
            delete pole[i][j];
        };
        delete pole[i];
    };
    delete[] pole;
};

/*ostream& operator<<(ostream& os,Pole_bitwy& plansza){
    for(int i=0;i<6;i++){
        for(int j=1;j<plansza.szer();j++){
            os << *(plansza.pol()[i][j]);
        };
        os<<endl;
    };
    return os;
};
*/


    
Pole_bitwy::Pole_bitwy(int x, string* tablica){
    //TABLICA DWUWYMIAROWA ZE WSKAZNIKAMI DO JEDNOSTEK
    szerokosc=x;
    pole=new Jednostka**[6];
    //Zapelnianie tablicy
    for (int i=0;i<6;i++){
        pole[i]=new Jednostka*[x];
        for (int j=0;j<x;j++){
                pole[i][j]=inicjuj(tablica[i][2*j],i,j);
        };
    };
    
};

ostream& operator<<(ostream& os,Pole_bitwy& plansza){
    for(int i=0;i<6;i++){
        for(int j=0;j<plansza.szer();j++){
            os<< plansza.pol()[i][j]->status();
        };
        if(i==2){
            os<<endl;
        };
        os<< endl;
    };
    return os;
}

Jednostka* Pole_bitwy::inicjuj(char znak,int x,int y){
    if (znak=='M'){
        Jednostka* Miecz;
        Miecz=new Miecznik(x,y);
        return Miecz;
    }
    else if (znak=='K'){
        Konny* Kon;
        Kon=new Konny(x,y);
        return Kon;
    }
    else if (znak=='L'){
        Jednostka* Lucz;
        Lucz=new Lucznik(x,y);
        return Lucz;
    };
    /*
    else if (znak=='M'){
        Jednostka* Miecz;
        Miecz=new Miecznik(x,y);
        return Miecz;
    }
    else if (znak=='M'){
        Jednostka* Miecz;
        Miecz=new Miecznik(x,y);
        return Miecz;
    }
    else if (znak=='M'){
        Jednostka* Miecz;
        Miecz=new Miecznik(x,y);
        return Miecz;
    }
*/
return 0;};
int main(int argc, char** argv) {   
    //deklaracja zmiennych
    bool dead; //zmienna sprawdzajaca czy jedna z armii nie jest martwa
    string* tab;
    string Dane;
    int Rozmiar, Tury;
    tab= new string[6];
    dead=false;
    //Zebranie danych od uzytkownika
    cout << "Prosze podac Dane:" <<endl;
    getline(cin,Dane);
    Rozmiar= atoi(Dane.substr(0,Dane.find(" ")).c_str());
    Tury= atoi(Dane.substr(Dane.find(" ")+1).c_str()); 
    for (int i=0;i<6;i++){
        getline(cin,tab[i]);
    };
    
    //Stworzenie pola bitwy
    Pole_bitwy plansza(Rozmiar,tab);
    //Wyswietlanie pola
    for(int licz_tury=1;licz_tury<=Tury;licz_tury++){
        for(int j=0;j<(Rozmiar*4+Rozmiar-1)/2;j++){
            cout<<" ";
        };
        cout<<licz_tury<<endl;
        cout<<plansza<<endl;;
        
        for (int i=0;i<6;i++){
            for (int j=0;j<Rozmiar;j++){
                cout<<(*plansza.pol()[i][j]).dajx()<<",";
                cout<<plansza.pol()[i][j]->dajy()<<endl;;
            };
        };
        if(dead){
            break;
        }
    };
}

