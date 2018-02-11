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
        Jednostka(const Jednostka&)=delete;
        Jednostka& operator=(const Jednostka&)=delete;

        virtual void bonus()=0; //Przydziela bonus w zależności od rodzaju jednostki
        virtual void bonus(Pole_bitwy*)=0; //Jak wyzej
        virtual void znies_bonus()=0; 
        virtual Jednostka* celuj(Pole_bitwy*)=0;
        virtual double atakuj(Jednostka*)=0;

        bool zywa(){return (liczeb>0);};
        void przesun(int,int);

        virtual bool czy_wsparcie(){return false;};
        virtual string status();
        virtual void otrzymaj_obrazenia(int);
        virtual void otrzymaj_wsparcie(double,double,int);
        virtual void znies_wsparcie(double,int);
    };

//Grupy

    class Atak: public Jednostka{

    public:
    //technikalia
        ~Atak()=default;
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
        ~Wsparcie()=default;
        Wsparcie(int, double, int,  int, int,  double,  int,  int ,  char);
        Wsparcie(const Wsparcie&)=delete;
        Wsparcie& operator=(const Wsparcie&)=delete;

        //API
        Jednostka* celuj(Pole_bitwy*);
        bool czy_wsparcie(){return true;};
        double atakuj(Jednostka*){return 0;};
    };

//Wsparcie

    class Tarczownik: public Wsparcie{
    public:
        ~Tarczownik()=default;
        Tarczownik(int,int);      
        Tarczownik(const Tarczownik&)=delete;
        Tarczownik& operator=(const Tarczownik&)=delete;

        void bonus(){};
        void bonus(Pole_bitwy*){};
        void znies_bonus(){};

    };

    class Bebniarz: public Wsparcie{
    public:
        ~Bebniarz()=default;
        Bebniarz(int,int);   
        Bebniarz(const Bebniarz&)=delete;
        Bebniarz& operator=(const Bebniarz&)=delete;

        void bonus(){};
        void bonus(Pole_bitwy*){};
        void znies_bonus(){};
    };

//Atak

    class Miecznik: public Atak{
        friend class Pole_bitwy::Bebniarz;
        friend class Pole_bitwy::Tarczownik;
    public:
        ~Miecznik()=default;
        Miecznik(int,int);      
        Miecznik(const Miecznik&)=delete;
        Miecznik& operator=(const Miecznik&)=delete;

        void bonus(){};
        void bonus(Pole_bitwy*){};
        void znies_bonus(){};
    };

    class Konny: public Atak{
        friend class Bebniarz;
        friend class Tarczownik;
    public:
        ~Konny()=default;
        Konny(int,int);  
        Konny(const Konny&)=delete;
        Konny& operator=(const Konny&)=delete;

        void bonus(){};
        void bonus(Pole_bitwy*);
        void znies_bonus();
    };

    class Halabardzista: public Atak{
        friend class Bebniarz;
        friend class Tarczownik;
    public:
        ~Halabardzista()=default;
        Halabardzista(int,int);    
        Halabardzista(const Halabardzista&)=delete;
        Halabardzista& operator=(const Halabardzista&)=delete;

        void bonus(){};
        void bonus(Pole_bitwy*);
        void znies_bonus();
    };

//Dystansowe

    class Lucznik: public Atak{
        friend class Bebniarz;
        friend class Tarczownik;
    public:
        ~Lucznik()=default;
        Lucznik(int,int);  
        Lucznik(const Lucznik&)=delete;
        Lucznik& operator=(const Lucznik&)=delete;

        void bonus();
        void znies_bonus();
        void bonus(Pole_bitwy*){};
    };

    class Kusznik: public Atak{
        friend class Bebniarz;
        friend class Tarczownik;
    public:
        ~Kusznik()=default;
        Kusznik(int,int);   
        Kusznik(const Kusznik&)=delete;
        Kusznik& operator=(const Kusznik&)=delete;

        void bonus();
        void znies_bonus();
        void bonus(Pole_bitwy*){};
    };

private:
    int szerokosc;
    Pole_bitwy::Jednostka*** pole;  
    
public:
    ~Pole_bitwy();
    Pole_bitwy(int, string*);
    Pole_bitwy(const Pole_bitwy&)=delete;
    Pole_bitwy& operator=(const Pole_bitwy&)=delete;
    
    Pole_bitwy& konsolidacja(); //Konsoliduje rzedy i linie na polu bitwy
    void tura(); //wykonuje ture
    int szer(){return szerokosc;};
    Jednostka*** pol(){return pole;};

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
void Pole_bitwy::Jednostka::otrzymaj_wsparcie(double mor,double obr, int bebniarz){
    morale=(1+bebniarz*0.5)*morale; //za kazdego bebniarza ktory wspiera powieksz mnoznik o 0.5
    obrona=(1+obr)*obrona;
};

void Pole_bitwy::Jednostka::znies_wsparcie(double obr, int bebniarz){
    morale=morale/(1+bebniarz*0.5); //za kazdego bebniarza ktory wspiera powieksz mnoznik o 0.5
    obrona=obrona/(1+obr);
};
 //Atak

//Konstruktor
Pole_bitwy::Atak::Atak(int sila, double obrona, int wytrz,  int zasieg, int liczeb,  double morale,  int x,  int y,  char id):
Jednostka(sila,obrona,wytrz,zasieg,liczeb,morale,x,y,id){};

Pole_bitwy::Jednostka* Pole_bitwy::Atak::celuj(Pole_bitwy* plansza){
    int cel_x,cel_y,i; //dwie wspolrzedne celu i iterator
    i=1;
        //Jeśli łucznik to najpierw na 2 linie na przeciwko.
        if (id=='L'){
            if(x<3){ //jesli 1. armia
                cel_x=4;
                cel_y=y;
            }
            else {      //jesli 2. armia
                cel_x=1;
                cel_y=y;
            };
                //zmieniaj rzędy dopoki w zasiegu i wybrana do tej pory jednostka jest martwa
                while(i<=zasieg && not(((*plansza).pole[cel_x][cel_y])->zywa())){
                //za pierwszym razem odejmij rzad(jesli ma to sens), a potem na zmiane
                    if(i%2==1 && y-i>=0){
                        cel_y=y-i;
                    }
                    else if(y+i<(*plansza).szerokosc-1){
                        cel_y=y+i;
                    };
                        i++;
                };
                //Koncze petle, jesli nadal nie znalezlismy to przejdz do 1 rzedu i powtorz szukanie celu
                if(not(((*plansza).pole[cel_x][cel_y])->zywa())){
                    if(x<3){
                        cel_x=3;
                        cel_y=y;
                    }
                    else {
                        cel_x=2;
                        cel_y=y;
                    };
                    //zresetuj licznik przed nowa petla
                    i=1;
                    while(i<=zasieg && not(((*plansza).pole[cel_x][cel_y])->zywa())){
                    //za pierwszym razem odejmij rzad, a potem na zmiane
                        if(i%2==1 && y-i>=0){
                            cel_y=y-i;
                        }
                        else if(y+i<(*plansza).szerokosc-1){
                            cel_y=y+i;
                        };
                            i++;
                    };
                };
                //jesli znalezlismy to zwroc wskaznik do wycelowanej jednostki.
                if((*plansza).pole[cel_x][cel_y]->zywa()){
                    return (*plansza).pole[cel_x][cel_y]; 
                };
        }
        //Jednostki inne niz lucznik
        else {
            if(x<3){
                cel_x=3;
                cel_y=y;
            }
            else {
                cel_x=2;
                cel_y=y;
            };
            while(i<=zasieg && not((*plansza).pole[cel_x][cel_y]->zywa())){
             //za pierwszym razem odejmij rzad, a potem na zmiane
                if(i%2==1 && y-i>=0){
                    cel_y=y-i;
                }
                else if(y+i<(*plansza).szerokosc-1){
                    cel_y=y+i;
                };
                i++;
            };
            return (*plansza).pole[cel_x][cel_y];
        };
        //Jesli nie znaleziono celu to zwroc siebie
    return (*plansza).pole[x][y];
};

double Pole_bitwy::Atak::atakuj(Jednostka* cel){
    return (1+sila)*(liczeb);    
};

//Wsparcie

//Konstruktor
Pole_bitwy::Wsparcie::Wsparcie(int sila, double obrona, int wytrz,  int zasieg, int liczeb,  double morale,  int x,  int y,  char id):
Jednostka(sila,obrona,wytrz,zasieg,liczeb,morale,x,y,id){};

Pole_bitwy::Jednostka* Pole_bitwy::Wsparcie::celuj(Pole_bitwy* plansza){
    int cel_x,cel_y,i;
        if(x>=3){
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
        i=2;
        while(i<=zasieg && not(((*plansza).pole[cel_x][cel_y]->zywa()))){
             //za pierwszym razem odejmij rzad, a potem na zmiane
            if(i%2==1 and y-i>=0){
                cel_y=y-i;
            }
            else if(y+i<(*plansza).szerokosc-1){
                cel_y=y+i;
            };
        i++;
        };
    return (*plansza).pole[cel_x][cel_y];
        //Jesli nie znaleziono celu to daj siebie
return (*plansza).pole[x][y];

};


//Tarczownik
Pole_bitwy::Tarczownik::Tarczownik(int a, int b): Wsparcie(0, 20, 100, 2, 100, 0, a, b , 'T'){};


//Bebniarz
Pole_bitwy::Bebniarz::Bebniarz(int a, int b): Wsparcie(0, 1, 50, 4, 40, 0, a, b , 'B'){};


//Miecznik
Pole_bitwy::Miecznik::Miecznik(int a, int b): Atak(30, 20, 50, 1, 200, 0, a, b , 'M'){};


//Konny

Pole_bitwy::Konny::Konny(int a, int b): Atak(50, 10, 70, 2, 100, 0, a, b, 'K'){};

void Pole_bitwy::Konny::bonus(Pole_bitwy* plansza){
    if(this->celuj(plansza)->id=='L'||this->celuj(plansza)->id=='C'){
      sila*=1.25;  
    };
};

void Pole_bitwy::Konny::znies_bonus(){
    sila=50;
};


//Halabardzista

Pole_bitwy::Halabardzista::Halabardzista(int a, int b): Atak(40, 20, 40, 2, 150, 0, a, b, 'H'){};

void Pole_bitwy::Halabardzista::bonus(Pole_bitwy* plansza){
    if(this->celuj(plansza)->id=='K'){
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

//destruktor
Pole_bitwy::~Pole_bitwy(){
    for (int i=1;i<=5;i++){
        for(int j=0;j<szerokosc;j++){
            delete pole[i][j];
        };
        delete[] pole[i];
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
            }
            else if (tablica[i][2*j]=='T'){
                pole[i][j]=new Tarczownik(i,j);
            }
            else if (tablica[i][2*j]=='B'){
                pole[i][j]=new Bebniarz(i,j);
            };
        };
    };   
};

//strumien
ostream& operator<<(ostream& os,Pole_bitwy& plansza){
        for(int j=0;j<7*plansza.szer();j++){
            os<<"=";
        };
        os<<endl;
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
            
    for(int j=0;j<7*plansza.szer();j++){
            os<<"=";
        };
    os<<endl;
    return os;
};


//przejscie przez jedna ture, celowanie, atak jednostek, itp.
void Pole_bitwy::tura(){
    //deklaracja i inicjalizacja zmiennych
    int x=0;
    int y=0;
    int** wsparcie_bebniarza= new int*[6]; //licznik ile bebniarzy wspiera
    int** tablica_ataku=new int*[6];
    double** tablica_morali=new double*[6];
    double** tablica_obrony=new double*[6];
    int**  tablica_zgonow=new int*[6];
    for(int i=0;i<6;i++){
        wsparcie_bebniarza[i]=new int[szerokosc];
        tablica_ataku[i]=new int[szerokosc];
        tablica_morali[i]=new double[szerokosc];
        tablica_obrony[i]=new double[szerokosc];
        tablica_zgonow[i]=new int[szerokosc];
        for(int j=0; j<szerokosc;j++){
            wsparcie_bebniarza[i][j]=0;
            tablica_ataku[i][j]=0;
            tablica_morali[i][j]=1; //bedzie mnozone, wiec 1
            tablica_obrony[i][j]=1; //bedzie mnozone, wiec 1
            tablica_zgonow[i][j]=pole[i][j]->zywa();
        };
    };
    
    //petla po polach, ktore biora udzial w walce
    for(int i=1;i<5;i++){
        for(int j=0; j<szerokosc;j++){
            if(pole[i][j]->zywa()){
                //wycelowanie w jednostke
                x=pole[i][j]->celuj(this)->x;
                y=pole[i][j]->celuj(this)->y;
                
                //odpalenie bonusow
                pole[i][j]->bonus();
                pole[i][j]->bonus(this);
                
                //jesli wycelowanie bylo poprawne- nie zwrocilo jednostki, ktora celowala to zaczynamy atak/wsparcie
                if ((pole[i][j]->celuj(this))!=pole[i][j]){
                    //Jesli wsparcie to jesli bebniarz to dopisz wsparcie
                    if(pole[i][j]->czy_wsparcie()){
                        if(pole[i][j]->id=='B'){
                            wsparcie_bebniarza[x][y]+=1;                        
                        }
                        else if(pole[i][j]->id=='T'){
                            tablica_obrony[x][y]+=0.5;
                        };
                    }
                    else{
                        tablica_ataku[x][y]+=(pole[i][j]->atakuj(pole[i][j]->celuj(this)));
                        tablica_morali[x][y]-=(pole[i][j]->atakuj(pole[i][j]->celuj(this)))/(pole[x][y]->liczeb);
                    }
                };
                //zniesienie bonusow
                pole[i][j]->znies_bonus();
            };
        };
    };
    //Zadanie obrazen
    for(int i=1;i<5;i++){
        for(int j=0; j<szerokosc;j++){
            pole[i][j]->otrzymaj_wsparcie(tablica_morali[i][j],tablica_obrony[i][j],wsparcie_bebniarza[i][j]);
            pole[i][j]->otrzymaj_obrazenia(tablica_ataku[i][j]); //otrzymanie obrazen
            
            pole[i][j]->morale+=min(0.0,(1-wsparcie_bebniarza[i][j])*tablica_morali[i][j]); //zmiana morali wynikajaca z ataku
            pole[i][j]->znies_wsparcie(tablica_obrony[i][j],wsparcie_bebniarza[i][j]);
            
            tablica_zgonow[i][j]=(tablica_zgonow[i][j]-pole[i][j]->zywa())%2;
            if(tablica_zgonow[i][j]){
                //zmiana morali sasiednich jednostek po zgonie jednostki- maximum(...<-tak jak w tresci zadania) i do tego mnoznik wsparcia bebniarzy. Jesli bebniarzy>=2 to nie ma zmiany morali, dlatego minimum.
                if (i==2){
                    if(j==szerokosc-1){
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i-1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j]->morale))); //linia nad
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i-1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j]->morale))); //linia nad
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                    }
                    else if(j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i-1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j]->morale))); //linia nad
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    };
                }
                else if (i==3){
                    if(j==szerokosc-1){
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i+1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j]->morale))); //linia pod
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i+1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j]->morale))); //linia pod
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                    }
                    else if(j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i+1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j]->morale))); //linia pod
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    };
                }
                else if (i==4){
                        if(j==szerokosc-1){
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i-1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j]->morale))); //linia nad
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i-1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j]->morale))); //linia nad
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                    }
                    else if (j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i-1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j]->morale))); //linia nad
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    };
                }
                else if (i==1){
                    if(j==szerokosc-1){
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i+1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j]->morale))); //linia pod
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    }
                    else if(j==0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i+1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j]->morale))); //linia pod
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                    }
                    else if(j<szerokosc-1 && j>0){
                        pole[i][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j+1]->morale))); //rzad po prawej
                        pole[i+1][j]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j]->morale))); //linia pod
                        pole[i+1][j+1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i+1][j+1]->morale))); //skos na prawo
                        pole[i][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i][j-1]->morale))); //rzad po lewej
                        pole[i-1][j-1]->morale-=min(0.0,(1-wsparcie_bebniarza[i][j]*0.5)*max(0.25,0.25*abs(pole[i-1][j-1]->morale))); //skos na lewo
                    }; 
                };
            };
        };
    };
    //niszczenie tablic, ktore utworzylem
    for (int i=0;i<6;i++){
        delete[] tablica_ataku[i];
        delete[] tablica_obrony[i];
        delete[] tablica_zgonow[i];
        delete[] tablica_morali[i];
        delete[] wsparcie_bebniarza[i];
    };
    delete[] tablica_ataku;
    delete[] tablica_obrony;
    delete[] tablica_zgonow;
    delete[] tablica_morali;
    delete[] wsparcie_bebniarza;
};

//konsolidacja
Pole_bitwy& Pole_bitwy::konsolidacja(){
//w liniach
    Jednostka* pom;
    //przegladaj po kolei jednostki, jeli ktoras jest martwa to zamien miejscami z ta ktora stoi za nia
    for(int j=0; j<szerokosc;j++){
        for(int i=2;i>0;i--){
            if(not(pole[i][j]->zywa())){
                if((not(pole[1][j]->zywa()) && not(pole[2][j]->zywa()) && not(pole[0][j]->zywa()))){ 
                }
                else{
                    if(((pole[0][j]->id)=='T' && not(pole[2][j]->zywa()) && pole[0][j]->zywa())  || (not(pole[1][j]->zywa()) && not(pole[2][j]->zywa()) && pole[0][j]->zywa())){
                        pom=pole[i][j];
                        pole[i][j]=pole[i-2][j];
                        pole[i][j]->przesun(i,j);

                        pole[i-2][j]=pom;
                        pole[i-2][j]->przesun(i-2,j);    
                    }
                    else{
                    pom=pole[i][j];
                        pole[i][j]=pole[i-1][j];
                        pole[i][j]->przesun(i,j);

                        pole[i-1][j]=pom;
                        pole[i-1][j]->przesun(i-1,j);
                    };
                    //jesli nie ma dobrego ulozenia to zresetuj petle, jak jest to kontynuuj
                    if(not(pole[0][j]->zywa()<=pole[1][j]->zywa() && pole[1][j]->zywa()<=pole[2][j]->zywa())){
                    i=3;
                    };
                };
            }; 
        };
    };
    for(int j=0; j<szerokosc;j++){
        for(int i=3;i<5;i++){
            if(not(pole[i][j]->zywa())){
                if((not(pole[3][j]->zywa()) && not(pole[4][j]->zywa())) && not(pole[5][j]->zywa())){
                    
                }
                else{
                    if(((pole[5][j]->id)=='T' && not(pole[3][j]->zywa()) && pole[5][j]->zywa()) || ((not(pole[3][j]->zywa()) && not(pole[4][j]->zywa())) && pole[5][j]->zywa())){
                        pom=pole[i][j];
                        pole[i][j]=pole[i+2][j];
                        pole[i][j]->przesun(i,j);

                        pole[i+2][j]=pom;   
                        pole[i+2][j]->przesun(i+2,j);
                    }
                    else{
    //                    cout<<i<<j<<endl;
                        pom=pole[i][j];
                        pole[i][j]=pole[i+1][j];
                        pole[i][j]->przesun(i,j);

                        pole[i+1][j]=pom;   
                        pole[i+1][j]->przesun(i+1,j);
                    };
                    //jesli nie ma dobrego ulozenia to zresetuj petle, jak jest to kontynuuj
                    if(not(pole[5][j]->zywa()<=pole[4][j]->zywa() && pole[4][j]->zywa()<=pole[3][j]->zywa())){
                        i=3;
                    };
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
            delete[] tab;
        };
};    
return *this;   
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
        cout<<plansza;   
        
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
        
        //przeprowadzenie tury
        plansza.tura();
        plansza.konsolidacja();
        
        delete[] check;
        delete[] check1;
        
        while (getchar() != '\n');
    };
    delete[] tab;    
    
/*TEST
26 100
T T T T T T T T B T T B T M T M T T B T K T T T M T
L L L H L H L L L L H L L L L L L H L L L L L L L L
H K M M H M K M H M K K K M H M M K H M K H M K H K
L L K M H M K L K K M M M H H H B B B B K L H M K K
K M H K K L L L L L L L L M M M M M T T T T H B K L
K K K K L L L T T T T T M K M L T K M H H H M L K M
*/
    
}

