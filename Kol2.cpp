#include "Wybor.h"

using namespace std;

/*
* Uwaga: Zadania kod rozwiazania zaania umieszczać pomiędzy #ifdef a #endif.
*/

#ifdef Kol_2
#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

enum class Stan {
    dopuszczony, niedopuszczony
};

class Wlasciciel {
    
};

class Samochod {
    char nr_rejestracyjny[10];  
    int stan_licznika;
    Stan stan_dopuszczenia;
    Wlasciciel* owner;
    static Samochod* wzorcowy_samochod;
    static constexpr double WAR_POCZ = 50000.0; 

public:
    Samochod() {
        if (wzorcowy_samochod == nullptr) {
            throw runtime_error("Brak instancji wzorcowej");
        }
        strncpy(nr_rejestracyjny, wzorcowy_samochod->nr_rejestracyjny, 9);
        nr_rejestracyjny[9] = '\0';
        setStanLicznika(wzorcowy_samochod->stan_licznika);
        stan_dopuszczenia = wzorcowy_samochod->stan_dopuszczenia;
        owner = wzorcowy_samochod->owner;
    }

    Samochod(const char* nr_rej, int licznik, Stan dop, Wlasciciel* wl) {
        strncpy(nr_rejestracyjny, nr_rej, 9);
        nr_rejestracyjny[9] = '\0';
        setStanLicznika(licznik);
        stan_dopuszczenia = dop;
        owner = wl;
    }

    virtual ~Samochod() = default;

    void setNrRejestracyjny(const char* nr_rej) {
        strncpy(nr_rejestracyjny, nr_rej, 9);
        nr_rejestracyjny[9] = '\0';
    }

    void setStanLicznika(int licznik) {
        if (licznik < 0) {
            throw invalid_argument("Stan licznika nie może być ujemny");
        }
        stan_licznika = licznik;
    }

    const char* getNrRejestracyjny() const {
        return nr_rejestracyjny;
    }

    int getStanLicznika() const {
        return stan_licznika;
    }

    Stan getStanDopuszczenia() const {
        return stan_dopuszczenia;
    }

    Wlasciciel* getOwner() const {
        return owner;
    }

    virtual double obliczZasieg() const {
        if (stan_dopuszczenia == Stan::niedopuszczony) {
            throw runtime_error("Samochod niedopuszczony do jazdy");
        }
        return 800.0;
    }

    virtual double obliczWartosc() const {
        double wsp_sparw = (stan_dopuszczenia == Stan::dopuszczony) ? 1.0 : 0.2;
        double wartosc = (WAR_POCZ - 0.2 * stan_licznika) * wsp_sparw;
        return max(wartosc, 400.0);
    }

    void pokazDane() const {
        cout << "Numer rejestracyjny: " << nr_rejestracyjny << endl;
        cout << "Stan licznika: " << stan_licznika << " km" << endl;
        cout << "Stan dopuszczenia: " << (stan_dopuszczenia == Stan::dopuszczony ? "Dopuszczony" : "Niedopuszczony") << endl;
        if (owner != nullptr) {
            cout << "Właściciel: Obiekt klasy Wlasciciel" << endl;
        }
        else {
            cout << "Właściciel: Brak danych" << endl;
        }
        try {
            cout << "Zasięg: " << obliczZasieg() << " km" << endl;
        }
        catch (const runtime_error& e) {
            cout << "Zasięg: " << e.what() << endl;
        }
        cout << "Wartość: " << obliczWartosc() << " PLN" << endl;
    }

    static void setWzorcowySamochod(Samochod* samochod) {
        wzorcowy_samochod = samochod;
    }

    static Samochod* getWzorcowySamochod() {
        return wzorcowy_samochod;
    }

    bool operator!=(const Samochod& other) const {
        return (abs(stan_licznika - other.stan_licznika) > 20) || (stan_dopuszczenia != other.stan_dopuszczenia);
    }
};

Samochod* Samochod::wzorcowy_samochod = nullptr;

class SamochodElektryczny : public Samochod {
    int stan_baterii;  

public:
    SamochodElektryczny(const Samochod& samochod, int bateria) : Samochod(samochod) {
        setStanBaterii(bateria);
    }

    void setStanBaterii(int bateria) {
        if (bateria < 0 || bateria > 100) {
            throw invalid_argument("Stan baterii musi być w zakresie 0-100%");
        }
        stan_baterii = bateria;
    }

    int getStanBaterii() const {
        return stan_baterii;
    }

    double obliczZasieg() const override {
        if (getStanDopuszczenia() == Stan::niedopuszczony) {
            throw runtime_error("Samochod niedopuszczony do jazdy");
        }
        return 2.5 * stan_baterii;
    }

    double obliczWartosc() const override {
        double wartosc = Samochod::obliczWartosc() * 0.7;  
        return max(wartosc, 400.0);
    }

    void pokazDane() const {
        Samochod::pokazDane();
        cout << "Stan baterii: " << stan_baterii << "%" << endl;
    }
};

int main() {
    Wlasciciel wlasciciel1;
    Samochod samochod1("XYZ1234", 150000, Stan::dopuszczony, &wlasciciel1);

    Samochod::setWzorcowySamochod(&samochod1);
    samochod1.pokazDane();

    try {
        Samochod samochod2;
        cout << "\nDane nowego samochodu (kopii wzorcowego):" << endl;
        samochod2.pokazDane();
    }
    catch (const runtime_error& e) {
        cout << "Błąd: " << e.what() << endl;
    }

    try {
        samochod1.setStanLicznika(-5000);
    }
    catch (const invalid_argument& e) {
        cout << "Błąd: " << e.what() << endl;
    }

    try {
        SamochodElektryczny elektrycznySamochod(samochod1, 80);
        cout << "\nDane samochodu elektrycznego:" << endl;
        elektrycznySamochod.pokazDane();
    }
    catch (const invalid_argument& e) {
        cout << "Błąd: " << e.what() << endl;
    }

    Samochod samochod3("ABC9876", 149980, Stan::dopuszczony, &wlasciciel1);
    cout << "\nPorównanie samochodów 1 i 3: " << (samochod1 != samochod3 ? "Samochody są różne" : "Samochody są takie same") << endl;

    Samochod samochod4("XYZ1234", 150000, Stan::niedopuszczony, &wlasciciel1);
    cout << "\nPorównanie samochodów 1 i 4: " << (samochod1 != samochod4 ? "Samochody są różne" : "Samochody są takie same") << endl;

    return 0;
}
#endif
