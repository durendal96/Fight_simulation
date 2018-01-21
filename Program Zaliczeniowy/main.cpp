/* 
 * File:   main.cpp
 * Author: Bartosz Kluczka
 *
 * Created on 27 grudnia 2017, 10:29
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>
#include <cmath>

using namespace std;
class Pole_bitwy{

class Jednostka{
    friend Pole_bitwy;
protected:
    int sila;
    double obrona;
    int wytrz;
    int zasieg;
    int liczeb;
    double morale;
    int max_liczeb;
    int x;
    int y;
    char id;
//metody 
public:
    virtual ~Jednostka()=default;
    Jednostka(int,int,int,int,int,double,int,int,char);
    Jednostka(const Jednostka&);
    Jednostka& operator=(const Jednostka&);
    
    virtual void bonus()=0; 
    virtual void bonus(Pole_bitwy*)=0;
    virtual void znies_bonus()=0;
    virtual Jednostka* celuj(Pole_bitwy*)=0;
    virtual double atakuj(Jednostka*)=0;
    
    int dajlicznosc(){return liczeb;};
    int dajx(){return x;};
    int dajy(){return y;};
    char dajid(){return id;};
    bool zywa(){return (liczeb>0);};
    void przesun(int,int);
    
    virtual bool czy_wsparcie(){return false;};
    virtual string status();
    virtual void otrzymaj_obrazenia(int);
    virtual void otrzymaj_wsparcie(double,double);
};

class Atak: public Jednostka{

public:
//technikalia
    ~Atak()=0;
    Atak(int, double, int,  int, int,  double,  int,  int ,  char);
    Atak(const Atak&)=delete;
    Atak& operator=(const Atak&)=delete;
    
    //API
    Jednostka* celuj(Pole_bitwy*);
    double atakuj(Jednostka*);
};

class Wsparcie: public Jednostka{

public:
//technikalia
    ~Wsparcie()=0;
    Wsparcie(int, double, int,  int, int,  double,  int,  int ,  char);
    Wsparcie(const Wsparcie&)=delete;
    Wsparcie& operator=(const Wsparcie&)=delete;
    
    //API
    Jednostka* celuj(Pole_bitwy*);
    bool czy_wsparcie(){return true;};
    double atakuj(Jednostka*);
};

class Tarczownik: public Atak{
public:
    Tarczownik(const Tarczownik&)=default;
    ~Tarczownik()=default;
    Tarczownik(int,int);      
    Tarczownik& operator=(const Tarczownik&)=default;
    
    void bonus(){};
    void bonus(Pole_bitwy*){};
    void znies_bonus(){};
    
};

class Bebniarz: public Atak{
public:
    Bebniarz(const Bebniarz&)=default;
    ~Bebniarz()=default;
    Bebniarz(int,int);      
    Bebniarz& operator=(const Bebniarz&)=default;
    
    void bonus(){};
    void bonus(Pole_bitwy*){};
    void znies_bonus(){};
};

class Miecznik: public Atak{
    friend class Pole_bitwy::Bebniarz;
    friend class Pole_bitwy::Tarczownik;
public:
    Miecznik(const Miecznik&)=default;
    ~Miecznik()=default;
    Miecznik(int,int);      
    Miecznik& operator=(const Miecznik&)=default;
    
    void bonus(){};
    void bonus(Pole_bitwy*){};
    void znies_bonus(){};
};

class Konny: public Atak{
    friend class Bebniarz;
    friend class Tarczownik;
public:
    Konny(const Konny&)=default;
    ~Konny()=default;
    Konny(int,int);       
    Konny& operator=(const Konny&)=default;

    void bonus(){};
    void bonus(Pole_bitwy*);
    void znies_bonus();
};

class Halabardzista: public Atak{
    friend class Bebniarz;
    friend class Tarczownik;
public:
    Halabardzista(const Halabardzista&)=default;
    ~Halabardzista()=default;
    Halabardzista(int,int);      
    Halabardzista& operator=(const Halabardzista&)=default;
    
    void bonus(){};
    void bonus(Pole_bitwy*);
    void znies_bonus();
};

class Lucznik: public Atak{
    friend class Bebniarz;
    friend class Tarczownik;
public:
    Lucznik(const Lucznik&)=default;
    ~Lucznik()=default;
    Lucznik(int,int);       
    Lucznik& operator=(const Lucznik&)=default;
    
    void bonus();
    void znies_bonus();
    void bonus(Pole_bitwy*){};
};

class Kusznik: public Atak{
    friend class Bebniarz;
    friend class Tarczownik;
public:
    Kusznik(const Kusznik&)=default;
    ~Kusznik()=default;
    Kusznik(int,int);       
    Kusznik& operator=(const Kusznik&)=default;
    
    void bonus();
    void znies_bonus();
    void bonus(Pole_bitwy*){};
};

public:
    ~Pole_bitwy();
    Pole_bitwy(int, string*);
    Pole_bitwy(const Pole_bitwy&);
    Pole_bitwy& operator=(const Pole_bitwy&);
    
    Pole_bitwy& konsolidacja();
    void tura();
    int szer(){return szerokosc;};
    Jednostka*** pol(){return pole;};
    
private:
    int szerokosc;
    Pole_bitwy::Jednostka*** pole;  
};


//Jednostka

//Konstruktor
Pole_bitwy::Jednostka::Jednostka(int sila, int obrona, int wytrz,  int zasieg, int liczeb,  double morale,  int x,  int y,  char id):
sila(sila),obrona(obrona), wytrz(wytrz),zasieg(zasieg),liczeb(liczeb),morale(morale),max_liczeb(liczeb),x(x),y(y),id(id){};

//Status
string Pole_bitwy::Jednostka::status(){
    string stat;
    if(liczeb<=0){
        stat="  X  ";
    }
    else if(liczeb<max_liczeb){
        if(int(liczeb/double(max_liczeb)*100)<10){
            stat=id;
            stat+=": 0"+to_string(int(liczeb/double(max_liczeb)*100));
        }
        else{
            stat=id;
            stat+=": "+to_string(int(liczeb/double(max_liczeb)*100));
        };
    }
    else{
        stat=id;
        stat+=": 00";  
    };
    stat+="  ";
    return stat;
};

//przesuniecie
void Pole_bitwy::Jednostka::przesun(int a,int b){
    x=a;
    y=b;
}

//otrzymanie obrazen
void Pole_bitwy::Jednostka::otrzymaj_obrazenia(int obrazenia){
    liczeb-=max(0, int(floor(obrazenia*(1-morale/(1-morale))/(wytrz*(1+obrona)))));
    liczeb=max(0,liczeb);
};
void Pole_bitwy::Jednostka::otrzymaj_wsparcie(double mor,double obr){
    morale*=abs(mor);
    obrona*=obr;
};
 //Atak
Pole_bitwy::Atak::~Atak(){};
//Konstruktor
Pole_bitwy::Atak::Atak(int sila, double obrona, int wytrz,  int zasieg, int liczeb,  double morale,  int x,  int y,  char id):
Jednostka(sila,obrona,wytrz,zasieg,liczeb,morale,x,y,id){};

Pole_bitwy::Jednostka* Pole_bitwy::Atak::celuj(Pole_bitwy* plansza){
    int cel_x,cel_y,i;
    i=1;
        //Jeśli łucznik to najpierw na 2 linie na przeciwko.
        if (id=='L'){
            if(x<3){
                cel_x=4;
                cel_y=y;
            }
            else {
                cel_x=1;
                cel_y=y;
            };
            //dopoki nie dobierzemy zywego celu to zmieniaj cel
            while(not((*plansza).pole[cel_x][cel_y])->zywa()){
                //zmieniaj rzędy dopoki w zasiegu
                while(i<=zasieg and not(((*plansza).pole[cel_x][cel_y])->zywa())){
                //za pierwszym razem odejmij rzad, a potem na zmiane
                    if(i%2==1 and cel_y-1>=0){
                        cel_y=cel_y-i;
                    }
                    else if(cel_y+1<(*plansza).szerokosc){
                        cel_y=cel_y+i;
                    };
                        i++;
                };
                //jesli nadal nie znalezlismy to przejdz do 1 rzedu.
                if(not(((*plansza).pole[cel_x][cel_y])->zywa())){
                    if((*this).x<3){
                        cel_x=3;
                        cel_y=y;
                    }
                    else {
                        cel_x=2;
                        cel_y=y;
                    };
                };
            };
        return (*plansza).pole[cel_x][cel_y]; 
        }
        else {
            if(x<3){
                cel_x=3;
                cel_y=y;
            }
            else {
                cel_x=2;
                cel_y=y;
            };
            while(i<=zasieg and not(((*plansza).pole[cel_x][cel_y]->zywa()))){
             //za pierwszym razem odejmij rzad, a potem na zmiane
                if(i%2==1 and cel_y-1>=0){
                    cel_y=cel_y-i;
                }
                else if(cel_y+1<=(*plansza).szerokosc){
                    cel_y=cel_y+i;
                };
                i++;
            };
            //cout<<"Jednostka na polu ("<<x<<","<<y<<") wycelowala w jednostke na polu ("<<cel_x<<","<<cel_y<<")"<<endl;
            return (*plansza).pole[cel_x][cel_y];
        };
        
        //tutaj jednostka nie ma w kogo celować
return (*plansza).pole[x][y];};

double Pole_bitwy::Atak::atakuj(Jednostka* cel){
    return (1+sila)*(liczeb);    
};

//Wsparcie
Pole_bitwy::Wsparcie::~Wsparcie(){};
//Konstruktor
Pole_bitwy::Wsparcie::Wsparcie(int sila, double obrona, int wytrz,  int zasieg, int liczeb,  double morale,  int x,  int y,  char id):
Jednostka(sila,obrona,wytrz,zasieg,liczeb,morale,x,y,id){};

Pole_bitwy::Jednostka* Pole_bitwy::Wsparcie::celuj(Pole_bitwy* plansza){
    int cel_x,cel_y,i=1;
    
            if(x>3){
                cel_x=3;
                if(y>0){
                    cel_y=y-1;
                }
                else
                    cel_y=y+1;
            }
            else {
                cel_x=2;
                if(y>0){
                    cel_y=y-1;
                }
                else
                    cel_y=y+1;
            };
            while(i<=zasieg and not(((*plansza).pole[cel_x][cel_y]->zywa()))){
             //za pierwszym razem odejmij rzad, a potem na zmiane
                if(i%2==1 and cel_y-1>=0){
                    cel_y=cel_y-i;
                }
                else if(cel_y+1<=(*plansza).szerokosc){
                    cel_y=cel_y+i;
                };
                i++;
            };
            //cout<<"Jednostka na polu ("<<x<<","<<y<<") wycelowala w jednostke na polu ("<<cel_x<<","<<cel_y<<")"<<endl;
            return (*plansza).pole[cel_x][cel_y];
        //tutaj jednostka nie ma w kogo celować
return (*plansza).pole[x][y];

};


//Tarczownik
Pole_bitwy::Tarczownik::Tarczownik(int a, int b): Atak(0, 20, 100, 2, 100, 0, a, b , 'T'){};

double Pole_bitwy::Wsparcie::atakuj(Jednostka* cel){
    return 0.5;
};

//Bebniarz
Pole_bitwy::Bebniarz::Bebniarz(int a, int b): Atak(0, 1, 50, 4, 40, 0, a, b , 'B'){};


//Miecznik
Pole_bitwy::Miecznik::Miecznik(int a, int b): Atak(30, 20, 50, 1, 200, 0, a, b , 'M'){};

//Konny

Pole_bitwy::Konny::Konny(int a, int b): Atak(50, 10, 70, 2, 100, 0, a, b, 'K'){};


void Pole_bitwy::Konny::bonus(Pole_bitwy* plansza){
    if(this->celuj(plansza)->dajid()=='L'||this->celuj(plansza)->dajid()=='C'){
      sila*=1.25;  
    };
};

void Pole_bitwy::Konny::znies_bonus(){
    sila=50;
};
//Halabardzista

Pole_bitwy::Halabardzista::Halabardzista(int a, int b): Atak(40, 20, 40, 2, 150, 0, a, b, 'H'){};

void Pole_bitwy::Halabardzista::bonus(Pole_bitwy* plansza){
    if(this->celuj(plansza)->dajid()=='K'){
        sila*=1.5;  
    };
};

void Pole_bitwy::Halabardzista::znies_bonus(){
    sila=40;
};

//Kusznik
Pole_bitwy::Kusznik::Kusznik(int a, int b): Atak(50, 8, 30, 3, 150, 0, a, b, 'C'){};

void Pole_bitwy::Kusznik::bonus(){
    if(x==2||x==3){
        obrona*=0.7;
    };
};

void Pole_bitwy::Kusznik::znies_bonus(){
        obrona=8;
};
//Lucznik
Pole_bitwy::Lucznik::Lucznik(int a, int b): Atak(30, 5, 20, 5, 200, 0, a, b, 'L'){};

void Pole_bitwy::Lucznik::bonus(){
    if(x==2||x==3){
        obrona*=0.5;
        sila*=0.5;
    };
};

void Pole_bitwy::Lucznik::znies_bonus(){
        obrona=5;
        sila=30;
};

//Pole Bitwy implementacja

//przejscie przez jedna ture, celowanie, atak jednostek, itp.
void Pole_bitwy::tura(){
    //Obliczenie obrazen dla kazdej jednostki
    int x;
    int y;
    int** tablica_ataku=new int*[6];
    double** tablica_morali=new double*[6];
    double** tablica_obrony=new double*[6];
    int**  tablica_zgonow=new int*[6];
    for(int i=0;i<6;i++){
        tablica_ataku[i]=new int[szerokosc];
        tablica_morali[i]=new double[szerokosc];
        tablica_obrony[i]=new double[szerokosc];
        tablica_zgonow[i]=new int[szerokosc];
        for(int j=0; j<szerokosc;j++){
            tablica_ataku[i][j]=0;
            tablica_morali[i][j]=1;
            tablica_obrony[i][j]=1;
            tablica_zgonow[i][j]=pole[i][j]->zywa();
        };
    };
    for(int i=1;i<5;i++){
        for(int j=0; j<szerokosc;j++){
            if(pole[i][j]->zywa()){
                x=pole[i][j]->celuj(this)->x;
                y=pole[i][j]->celuj(this)->y;
                
                //odpalenie bonusow
                pole[i][j]->bonus();
                pole[i][j]->bonus(this);
                if ((pole[i][j]->celuj(this))!=pole[i][j]){
                    if(pole[i][j]->czy_wsparcie()){
                        tablica_morali[x][y]+=(pole[i][j]->atakuj(pole[i][j]->celuj(this)));
                        tablica_obrony[x][y]+=(pole[i][j]->atakuj(pole[i][j]->celuj(this)));
                    }
                    else{
                        tablica_ataku[x][y]+=(pole[i][j]->atakuj(pole[i][j]->celuj(this)));
                        tablica_morali[x][y]+=-(pole[i][j]->atakuj(pole[i][j]->celuj(this)))/(pole[x][y]->dajlicznosc());
                    }
//                    cout<<"Jednostka na polu ("<<i<<","<<j<<") wycelowala w jednostke na polu ("<<x<<","<<y<<")"<<endl;
                };
                //zniesienie bonusow
                pole[i][j]->znies_bonus();
            };
        };
    };
    //Zadanie obrazen

    for(int i=1;i<5;i++){
        for(int j=0; j<szerokosc;j++){
            pole[i][j]->otrzymaj_wsparcie(tablica_morali[i][j],tablica_obrony[i][j]);
            pole[i][j]->otrzymaj_obrazenia(tablica_ataku[i][j]);
            tablica_zgonow[i][j]=(tablica_zgonow[i][j]-pole[i][j]->zywa())%2;
            if(tablica_zgonow[i][j]){
//                cout<<i<<','<<j<<endl;
                if (i==2){
                    if(j==szerokosc-1){
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i-1][j]->morale-=max(0.25,0.25*abs(pole[i-1][j]->morale)); //linia nad
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i-1][j]->morale-=max(0.25,0.25*abs(pole[i-1][j]->morale)); //linia nad
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                    }
                    else if(j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i-1][j]->morale-=max(0.25,0.25*abs(pole[i-1][j]->morale)); //linia nad
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    }
                }
                else if (i==3){
                    if(j==szerokosc-1){
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i+1][j]->morale-=max(0.25,0.25*abs(pole[i+1][j]->morale)); //linia pod
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i+1][j]->morale-=max(0.25,0.25*abs(pole[i+1][j]->morale)); //linia pod
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                    }
                    else if(j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i+1][j]->morale-=max(0.25,0.25*abs(pole[i+1][j]->morale)); //linia pod
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    }
                }
                else if (i==4){
                        if(j==szerokosc-1){
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i-1][j]->morale-=max(0.25,0.25*abs(pole[i-1][j]->morale)); //linia nad
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i-1][j]->morale-=max(0.25,0.25*abs(pole[i-1][j]->morale)); //linia nad
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                    }
                    else if (j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i-1][j]->morale-=max(0.25,0.25*abs(pole[i-1][j]->morale)); //linia nad
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    };                    
                }
                else if (i==1){
                    if(j==szerokosc-1){
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i+1][j]->morale-=max(0.25,0.25*abs(pole[i+1][j]->morale)); //linia pod
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i+1][j]->morale-=max(0.25,0.25*abs(pole[i+1][j]->morale)); //linia pod
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                    }
                    else if(j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=max(0.25,0.25*abs(pole[i][j+1]->morale)); //rzad po prawej
                        pole[i+1][j]->morale-=max(0.25,0.25*abs(pole[i+1][j]->morale)); //linia pod
                        pole[i+1][j+1]->morale-=max(0.25,0.25*abs(pole[i+1][j+1]->morale)); //skos na prawo
                        pole[i][j-1]->morale-=max(0.25,0.25*abs(pole[i][j-1]->morale)); //rzad po lewej
                        pole[i-1][j-1]->morale-=max(0.25,0.25*abs(pole[i-1][j-1]->morale)); //skos na lewo
                    };                    
                };
            };
        };
    };
    //niszczenie tablicy ataku
    for (int i=0;i<6;i++){
        delete tablica_ataku[i];
    };
    delete[] tablica_ataku;
};
//konsolidacja
Pole_bitwy& Pole_bitwy::konsolidacja(){
//w liniach
    Jednostka* pom;
    //przegladaj po kolei jednostki, jeli ktoras jest martwa to zamien miejscami z ta ktora stoi za nia
    for(int j=0; j<szerokosc;j++){
        for(int i=2;i>0;i--){
            if(not(pole[i][j]->zywa())){
                pom=pole[i][j];
                pole[i][j]=pole[i-1][j];
                pole[i][j]->przesun(i,j);
                
                pole[i-1][j]=pom;
                pole[i-1][j]->przesun(i-1,j);
                //jesli nie ma dobrego ulozenia to zresetuj petle, jak jest to kontynuuj
                if(not(pole[0][j]->zywa()<=pole[1][j]->zywa() and pole[1][j]->zywa()<=pole[2][j]->zywa())){
                i=3;
                };
            };            
        };
    };
    for(int j=0; j<szerokosc;j++){
        for(int i=3;i<5;i++){
            if(not(pole[i][j]->zywa())){
                pom=pole[i][j];
                pole[i][j]=pole[i+1][j];
                pole[i][j]->przesun(i,j);
                
                pole[i+1][j]=pom;   
                pole[i+1][j]->przesun(i+1,j);
                //jesli nie ma dobrego ulozenia to zresetuj petle, jak jest to kontynuuj
                if(not(pole[5][j]->zywa()<=pole[4][j]->zywa() and pole[4][j]->zywa()<=pole[3][j]->zywa())){
                i=2;
                };
            };
        };
    };

//konsolidacja rzedu armia1
Jednostka** tab=new Jednostka*[6];
int licznik2=0;
int licznik=0;
//zlicza martwe kolumny w pierwszej i drugiej polowie.
//na poczatku sprawdzam cala tablice, zamieniam miejscami, az martwe beda na skraju, dodaje do licznika i zaczynam od nowa.
for(int j=licznik;j<szerokosc-licznik2;j++){
    bool warunek=(pole[0][j]->zywa()==false) && (pole[1][j]->zywa()==false) && (pole[2][j]->zywa()==false);
    if(warunek){
        if (j<szerokosc/2){
            for (int i=j;i>0;i--){
                tab[0]=pole[0][i];
                tab[1]=pole[1][i];
                tab[2]=pole[2][i];
                
                pole[0][i]=pole[0][i-1];
                pole[1][i]=pole[1][i-1];
                pole[2][i]=pole[2][i-1];
                
                pole[0][i]->przesun(0,i);
                pole[1][i]->przesun(1,i);
                pole[2][i]->przesun(2,i);
                
                pole[0][i-1]=tab[0];
                pole[1][i-1]=tab[1];
                pole[2][i-1]=tab[2];
                
                pole[0][i-1]->przesun(0,i-1);
                pole[1][i-1]->przesun(1,i-1);
                pole[2][i-1]->przesun(2,i-1);

                };
                licznik++;
                
        }
        else if (j>=szerokosc/2){
                for (int i=j;i<szerokosc-1;i++){
                    tab[0]=pole[0][i];
                    tab[1]=pole[1][i];
                    tab[2]=pole[2][i];

                    
                    pole[0][i]=pole[0][i+1];
                    pole[1][i]=pole[1][i+1];
                    pole[2][i]=pole[2][i+1];
                    
                    pole[0][i]->przesun(0,i);
                    pole[1][i]->przesun(1,i);
                    pole[2][i]->przesun(2,i);
                
                   
                    pole[0][i+1]=tab[0];
                    pole[1][i+1]=tab[1];
                    pole[2][i+1]=tab[2];
                    
                    
                    pole[0][i+1]->przesun(0,i+1);
                    pole[1][i+1]->przesun(1,i+1);
                    pole[2][i+1]->przesun(2,i+1);
                };
                licznik2++;
            }; 
            
    };
};

//armia2

licznik2=0;
licznik=0;
//zlicza martwe kolumny w pierwszej i drugiej polowie.
//na poczatku sprawdzam cala tablice, zamieniam miejscami, az martwe beda na skraju, dodaje do licznika i zaczynam od nowa.
for(int j=licznik;j<szerokosc-licznik2;j++){ 
    bool warunek=((pole[3][j]->zywa()==false) && (pole[4][j]->zywa()==false) && (pole[5][j]->zywa()==false));
    if(warunek){
        Jednostka** tab=new Jednostka*[6];
        if (j<szerokosc/2){
            for (int i=j;i>0;i--){
                tab[3]=pole[3][i];
                tab[4]=pole[4][i];
                tab[5]=pole[5][i];
                
                pole[3][i]=pole[3][i-1];
                pole[4][i]=pole[4][i-1];
                pole[5][i]=pole[5][i-1];
                
                pole[3][i]->przesun(3,i);
                pole[4][i]->przesun(4,i);
                pole[5][i]->przesun(5,i);
                
                pole[3][i-1]=tab[3];
                pole[4][i-1]=tab[4];
                pole[5][i-1]=tab[5];

                pole[3][i-1]->przesun(3,i-1);
                pole[4][i-1]->przesun(4,i-1);
                pole[5][i-1]->przesun(5,i-1);
                };
                licznik++;
        }
        else if (j>=szerokosc/2){
                for (int i=j;i<szerokosc-1;i++){

                    tab[3]=pole[3][i];
                    tab[4]=pole[4][i];
                    tab[5]=pole[5][i];


                    pole[3][i]=pole[3][i+1];
                    pole[4][i]=pole[4][i+1];
                    pole[5][i]=pole[5][i+1];
                    

                    pole[3][i]->przesun(3,i);
                    pole[4][i]->przesun(4,i);
                    pole[5][i]->przesun(5,i);
                

                    pole[3][i+1]=tab[3];
                    pole[4][i+1]=tab[4];
                    pole[5][i+1]=tab[5];
                    

                    pole[3][i+1]->przesun(3,i+1);
                    pole[4][i+1]->przesun(4,i+1);
                    pole[5][i+1]->przesun(5,i+1);
                };
                licznik2++;
            };
            delete tab;
        };
};    
return *this;   
};

//destruktor
Pole_bitwy::~Pole_bitwy(){
    for (int i=1;i<=5;i++){
        for(int j=0;j<szerokosc;j++){
            delete pole[i][j];
        };
        delete pole[i];
    };
    delete[] pole;
};

//konstruktor   
Pole_bitwy::Pole_bitwy(int x, string* tablica){
    //TABLICA DWUWYMIAROWA ZE WSKAZNIKAMI DO JEDNOSTEK
    szerokosc=x;
    pole=new Jednostka**[6];
    //Zapelnianie tablicy
    for (int i=0;i<6;i++){
        pole[i]=new Jednostka*[x];
        for (int j=0;j<x;j++){
            if (tablica[i][2*j]=='M'){
                pole[i][j]=new Miecznik(i,j);
            }
            else if (tablica[i][2*j]=='K'){
                pole[i][j]=new Konny(i,j);
            }
            else if (tablica[i][2*j]=='L'){
                pole[i][j]=new Lucznik(i,j);
            }
            else if (tablica[i][2*j]=='H'){
                pole[i][j]=new Halabardzista(i,j);
            }
            else if (tablica[i][2*j]=='C'){
                pole[i][j]=new Kusznik(i,j);
            };
            /*
            else if (tablica[i][2*j]=='T'){
                pole[i][j]=new Tarczownik(i,j);
            }
            else if (tablica[i][2*j]=='B'){
                pole[i][j]=new Bebniarz(i,j);
            };
            */
        };
    };
    
};


ostream& operator<<(ostream& os,Pole_bitwy& plansza){
    for(int i=0;i<6;i++){
        for(int j=0;j<plansza.szer();j++){
            os<< (*plansza.pol()[i][j]).status();
        };
        if(i==2){
            os<<endl;
            for(int i=0;i<7*(plansza.szer());i++){
                os<<"-";
            };
        };
        os<< endl;
    };
    return os;
};

int main(int argc, char** argv) {   
    //deklaracja zmiennych
    string* tab;
    string Dane;
    int Rozmiar, Tury=0,test=0,test1=0;
    tab= new string[6];
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
        cout<<"Tura "<<licz_tury<<" z "<<Tury<<endl;
        for(int j=0;j<7*plansza.szer();j++){
            cout<<"=";
        };
        cout<<endl;
        cout<<plansza;
        
        for(int j=0;j<7*plansza.szer();j++){
            cout<<"=";
        };
        cout<<endl;
        
        plansza.konsolidacja();
        plansza.konsolidacja();
        
        //sprawdzenie czy ktoras z armii jest martwa
        bool* check=new bool[Rozmiar];
        bool* check1=new bool[Rozmiar];
        for (int i=0;i<plansza.szer();i++){
            check[i]=plansza.pol()[2][i]->zywa();
            check1[i]=plansza.pol()[3][i]->zywa();
        };
        for (int i=0;i<plansza.szer();i++){
            test+=check[i];
            test1+=check1[i];
        }
        if(test==0 || test1==0){
            break;
        };
        test=0;
        test1=0;
        plansza.tura();
    };
    
/*TEST
10 30
K L L M K K K H H K
M M M L L L H H K K
H H H M M K M H K M
L L K H M K M H K K
L L K L L M L H L K
M K H M H L K K H H
*/
    
}

