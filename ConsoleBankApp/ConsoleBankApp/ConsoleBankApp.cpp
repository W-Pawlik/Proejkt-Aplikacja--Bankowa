#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Enum reprezentuj�cy stany maszyny
enum class Stan {
    MENU_GLOWNE,
    LOGOWANIE,
    KONTO_PRACOWNICZE,
    KONTO_KLIENTA,
    KONTO_OSZCZEDNOSCIOWE,
    KONTO_BIEZACE
};

//klasa reprezentuj�ca konto, okre�laj�ca pocz�tkowe saldo. zawiera metody wirtualne, kt�re nadpisywane s� w klasach dziedzicz�cych
class Konto {
protected:
    double saldo;

public:
    //konstrukotr bezparametrowy, kt�ry inicjalizuje saldo z warto�ci� 0.0
    Konto() : saldo(0.0) {}

    double pokazSaldo() const {
        return saldo;
    }

    virtual void wplac(double kwota) {
        saldo += kwota;
        cout << "Wplacono " << kwota << " PLN." << endl;
        cout << "Aktualne saldo: " << saldo << " PLN." << endl;
    }

    bool wyplac(double kwota) {
        if (saldo >= kwota) {
            saldo -= kwota;
            cout << "Wyplacono " << kwota << " PLN." << endl;
            cout << "Aktualne saldo: " << saldo << " PLN." << endl;
            return true;
        }
        else {
            cout << "Brak wystarczaj�cych �rodk�w na koncie." << endl;
            return false;
        }
    }
};

//Klasa reprezntuj�ca konto klienta z kontkretnym numerem. Dziedziczy po klasie Konto
class KontoKlienta : public Konto {
private:
    string numerKonta;

public:
    KontoKlienta(string numer) : numerKonta(numer) {}

    string getNumerKonta() const {
        return numerKonta;
    }

    void setNumerKonta(string nowyNumer) {
        numerKonta = nowyNumer;
    }
};

//Klasa reprezenutj�ca konto pracownika. Posiada list� obiekt�w KontoKlienta w celu manipluacji numerem.
class KontoPracownicze : public Konto {
private:
    vector<KontoKlienta> kontaKlientow;

public:
    bool utworzKontoKlienta(string numerKonta) {
        //Sprawdzenie istnienia konta z takim samym numerem.
        for (const auto& konto : kontaKlientow) {
            if (konto.getNumerKonta() == numerKonta) {
                cout << "Konto klienta o podanym numerze ju� istnieje." << endl;
                return false;
            }
        }

        KontoKlienta noweKonto(numerKonta);
        kontaKlientow.push_back(noweKonto);
        cout << "Konto klienta o numerze " << numerKonta << " zostalo utworzone." << endl;
        return true;
    }

    bool zmienNumerKonta(string nowyNumer, string staryNumer) {
        for (auto& konto : kontaKlientow) {
            if (konto.getNumerKonta() == staryNumer) {
                konto.setNumerKonta(nowyNumer);
                return true;
            }
        }
        return false;
    }

    void zamknijKontoKlienta(string numerKonta) {
        for (auto it = kontaKlientow.begin(); it != kontaKlientow.end(); ++it) {
            if (it->getNumerKonta() == numerKonta) {
                kontaKlientow.erase(it);
                cout << "Konto klienta o numerze " << numerKonta << " zosta�o zamkniete." << endl;
                return;
            }
        }
        cout << "Nie znaleziono konta klienta o podanym numerze." << endl;
    }

    friend class KontoOszczednosciowe;
    friend class KontoBiezace;
};

//Klasy reprezentuj�ca konta oszcz�dno�ciowe i bie��ce, kt�re dziedzicz� po klasie Konto. Nadpisuj� metod� wplac
class KontoOszczednosciowe : public Konto {
public:
    void wplac(double kwota) override {
        saldo += kwota;
        cout << "Wplacono " << kwota << " PLN na konto oszczednosciowe." << endl;
        cout << "Aktualne saldo: " << saldo << " PLN." << endl;
    }
};

class KontoBiezace : public Konto {
public:
    void wplac(double kwota) override {
        saldo += kwota;
        cout << "Wplacono " << kwota << " PLN na konto biezace." << endl;
        cout << "Aktualne saldo: " << saldo << " PLN." << endl;
    }
};

//klasa reprezentuj�ca Maszyn� Stan�w. Zawiera metody s�u��ce do wy�wietlania Menu i wszytskich operacji.
class MaszynaStanow {
private:
    Stan obecnyStan;
    KontoPracownicze kontoPracownicze;
    KontoKlienta* kontoKlienta;

public:
    //konstruktor
    MaszynaStanow() {
        obecnyStan = Stan::MENU_GLOWNE;
        kontoKlienta = nullptr;
    }

    //destruktor
    ~MaszynaStanow() {
        delete kontoKlienta;
    }

    //Pocz�tkowy widok aplikacji
    void uruchom() {
        while (true) {
            switch (obecnyStan) {
            case Stan::MENU_GLOWNE:
                wyswietlMenuGlowne();
                break;
            case Stan::LOGOWANIE:
                logowanie();
                break;
            case Stan::KONTO_PRACOWNICZE:
                obslugaKontaPracowniczego();
                break;
            case Stan::KONTO_KLIENTA:
                obslugaKontaKlienta();
                break;
            case Stan::KONTO_OSZCZEDNOSCIOWE:
                obslugaKontaOszczednosciowego();
                break;
            case Stan::KONTO_BIEZACE:
                obslugaKontaBiezacego();
                break;
            }
        }
    }

    //widok Menu g��wnego
    void wyswietlMenuGlowne() {
        cout << "Witaj w aplikacji bankowej!" << endl;
        cout << "1. Zaloguj sie na konto pracownicze" << endl;
        cout << "2. Zaloguj sie na konto klienta" << endl;
        cout << "0. Wyjscie" << endl;

        int wybor;
        cin >> wybor;

        switch (wybor) {
        case 1:
            obecnyStan = Stan::KONTO_PRACOWNICZE;
            break;
        case 2:
            obecnyStan = Stan::LOGOWANIE;
            break;
        case 0:
            exit(0);
            break;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
            break;
        }
    }

    //Symulacja logowania do konta klienta
    void logowanie() {
        cout << "Logowanie..." << endl;
        // Kod logowania
        bool zalogowany = true;

        if (zalogowany) {
            obecnyStan = Stan::KONTO_KLIENTA;
            kontoKlienta = new KontoKlienta("1");
        }
        else {
            cout << "Nieprawidlowe dane logowania. Sprobuj ponownie." << endl;
            obecnyStan = Stan::MENU_GLOWNE;
        }
    }

    //Widok ob�usgi konta pracowniczego
    void obslugaKontaPracowniczego() {
        cout << "Obsluga konta pracowniczego..." << endl;
        cout << "1. Utworz konto klienta" << endl;
        cout << "2. Zmien numer konta klienta" << endl;
        cout << "3. Zamknij konto klienta" << endl;
        cout << "0. Powrot do menu glownego" << endl;

        int wybor;
        cin >> wybor;

        switch (wybor) {
        case 1: {
            string numerKonta;
            cout << "Podaj numer konta klienta do utworzenia: ";
            cin >> numerKonta;

            if (kontoPracownicze.utworzKontoKlienta(numerKonta)) {
                cout << "Konto klienta o numerze " << numerKonta << " zostalo utworzone." << endl;
            }
            break;
        }
        case 2: {
            string staryNumer, nowyNumer;
            cout << "Podaj numer konta klienta do zmiany: ";
            cin >> staryNumer;
            cout << "Podaj nowy numer konta: ";
            cin >> nowyNumer;

            if (kontoPracownicze.zmienNumerKonta(nowyNumer, staryNumer)) {
                cout << "Numer konta klienta zostal zmieniony." << endl;
            }
            else {
                cout << "Nie znaleziono konta klienta o podanym numerze." << endl;
            }
            break;
        }
        case 3: {
            string numerKonta;
            cout << "Podaj numer konta klienta do zamkniecia: ";
            cin >> numerKonta;

            kontoPracownicze.zamknijKontoKlienta(numerKonta);
            break;
        }
        case 0:
            obecnyStan = Stan::MENU_GLOWNE;
            break;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
            break;
        }
    }

    //Widok ob�ugi konta Klienta
    void obslugaKontaKlienta() {
        cout << "Obsluga konta klienta..." << endl;
        cout << "1. Konto oszczednosciowe" << endl;
        cout << "2. Konto biezace" << endl;
        cout << "0. Wyloguj sie" << endl;

        int wybor;
        cin >> wybor;

        switch (wybor) {
        case 1: {
            
            obecnyStan = Stan::KONTO_OSZCZEDNOSCIOWE;
            break;
        }
        case 2: {
            
            obecnyStan = Stan::KONTO_BIEZACE;
            break;
        }
        case 0:
            obecnyStan = Stan::MENU_GLOWNE;
            break;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
            break;
        }
    }

    //Widok obs�ugi konta oszcz�dnmo�ciowego
    void obslugaKontaOszczednosciowego() {
        cout << "Obsluga konta oszczednosciowego..." << endl;
        cout << "1. Wplac srodki" << endl;
        cout << "2. Wyplac srodki" << endl;
        cout << "3. Pokaz saldo" << endl;
        cout << "0. Wyloguj sie" << endl;

        int wybor;
        cin >> wybor;

        switch (wybor) {
        case 1: {
            double kwota;
            cout << "Podaj kwote do wplaty: ";
            cin >> kwota;

            kontoKlienta->wplac(kwota);
            break;
        }
        case 2: {
            double kwota;
            cout << "Podaj kwote do wyplaty: ";
            cin >> kwota;

            kontoKlienta->wyplac(kwota);
            break;
        }
        case 3: {
            cout << "Aktualne saldo konta: " << kontoKlienta->pokazSaldo() << " PLN" << endl;
            break;
        }
        case 0:
            obecnyStan = Stan::MENU_GLOWNE;
            break;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
            break;
        }
    }

    //Widok obs�ugi konta bie��cego
    void obslugaKontaBiezacego() {
        cout << "Obsluga konta biezacego..." << endl;
        cout << "1. Wplac srodki" << endl;
        cout << "2. Wyplac srodki" << endl;
        cout << "3. Pokaz saldo" << endl;
        cout << "0. Wyloguj sie" << endl;

        int wybor;
        cin >> wybor;

        switch (wybor) {
        case 1: {
            double kwota;
            cout << "Podaj kwote do wplaty: ";
            cin >> kwota;

            kontoKlienta->wplac(kwota);
            break;
        }
        case 2: {
            double kwota;
            cout << "Podaj kwote do wyplaty: ";
            cin >> kwota;

            kontoKlienta->wyplac(kwota);
            break;
        }
        case 3: {
            cout << "Aktualne saldo konta: " << kontoKlienta->pokazSaldo() << " PLN" << endl;
            break;
        }
        case 0:
            obecnyStan = Stan::MENU_GLOWNE;
            break;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
            break;
        }
    }
};

int main() {
    MaszynaStanow maszyna;
    maszyna.uruchom();

    return 0;
}
