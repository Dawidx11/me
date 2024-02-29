#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

const double CENA_SERWISU = 200.0;
const double CENA_ZA_100G = 40.0;
const double CENA_OZONOWANIE = 50.0;

// Funkcja do obliczania sumy zużytego czynnika w miesiącu i roku
void obliczZuzycie(int &zuzycieMiesiac, int &zuzycieRok, int currentMonth, int currentYear) {
    ifstream file("podsumowanie.txt");
    if (!file) {
        cerr << "Nie mozna otworzyć pliku 'podsumowanie.txt'" << endl;
        zuzycieMiesiac = 0;
        zuzycieRok = 0;
        return;
    }

    string line;
    zuzycieMiesiac = 0;
    zuzycieRok = 0;

    while (getline(file, line)) {
        if (line.find("Suma zuzytego czynnika w miesiacu") != string::npos) {
            int miesiac, ilosc;
            sscanf(line.c_str(), "Suma zuzytego czynnika w miesiacu %d: %d", &miesiac, &ilosc);
            if (miesiac == currentMonth) {
                zuzycieMiesiac = ilosc;
            }
        } else if (line.find("Suma zuzytego czynnika w roku") != string::npos) {
            int rok, ilosc;
            sscanf(line.c_str(), "Suma zuzytego czynnika w roku %d: %d", &rok, &ilosc);
            if (rok == currentYear) {
                zuzycieRok = ilosc;
            }
        }
    }

    file.close();
}

int main() {
    string marka, model, rocznik;
    int gramy_chlodniczego;
    char ozonowanie;
    double cena = CENA_SERWISU;


    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentMonth = now->tm_mon + 1;
    int currentYear = now->tm_year + 1900;

    // Otwórz plik w trybie dopisywania
    ofstream file("podsumowanie.txt", ios::app);
    if (!file) {
        cerr << "Nie mozna otworzyc pliku 'podsumowanie.txt'" << endl;
        return 1;
    }

    int zuzycieMiesiac, zuzycieRok;
    obliczZuzycie(zuzycieMiesiac, zuzycieRok, currentMonth, currentYear);

    cout << "Podaj marke samochodu: ";
    getline(cin, marka);
    cout << "Podaj model samochodu: ";
    getline(cin, model);
    cout << "Podaj rocznik samochodu: ";
    getline(cin, rocznik);

    cout << "Ile gramow czynnika chlodniczego zostalo napelnione? ";
    cin >> gramy_chlodniczego;

    int ilosc_100g = gramy_chlodniczego / 100;
    int reszta_100g = gramy_chlodniczego % 100;

    if (reszta_100g > 30) {
        ilosc_100g++;
    }

    double cena_nabicia = ilosc_100g * CENA_ZA_100G;

    cout << "Czy ozonowanie zostalo wykonane? (t/n) ";
    cin >> ozonowanie;

    if (ozonowanie == 't' || ozonowanie == 'T') {
        cena += CENA_OZONOWANIE;
    }

    // Wyswietl podsumowanie na ekranie
    int width = 43;

    cout << "+----------------------------------------------+" << endl;
    cout << "|               PODSUMOWANIE                   |" << endl;
    cout << "+----------------------------------------------+" << endl;
    cout << "| Marka:              " << left << setw(width - 25) << marka << "       |" << endl;
    cout << "| Model:              " << left << setw(width - 25) << model << "       |" << endl;
    cout << "| Rocznik:            " << left << setw(width - 25) << rocznik << "       |" << endl;
    cout << "| Ilosc czynnika:     " << setw(width - 46) << gramy_chlodniczego << "g                     |" << endl;

    if (ozonowanie == 't') {
        cout << "| Ozonowanie:         Tak      " << setw(width - 36) << "                |" << endl;
    } else {
        cout << "| Ozonowanie:         Nie      " << setw(width - 36) << "                |" << endl;
    }

    cout << "+----------------------------------------------+" << endl;
    cout << "| Cena za serwis:                        " << setw(width - 36) << "200 zl|" << endl;

    if (ilosc_100g > 0) {
        cout << "| Cena za nabicie czynnika(" << setw(6) << ilosc_100g * 100 << "g):     " << setw(width - 47) << cena_nabicia << " zl|" << endl;
        cena += cena_nabicia;
    }

    cout << "| Cena za ozonowanie:                     " << setw(width - 36) << (ozonowanie == 't' ? "50 zl|" : "0 zl|") << endl;
    cout << "+----------------------------------------------+" << endl;
    cout << "| Calkowita cena:              " << setw(width - 36) << cena << "zl       |" << endl;
    cout << "+----------------------------------------------+" << endl;

    // Zapisz podsumowanie do pliku
    file << "+----------------------------------------------+" << endl;
    file << "|               PODSUMOWANIE                   |" << endl;
    file << "+----------------------------------------------+" << endl;
    file << "| Marka: " << marka << endl;
    file << "| Model: " << model << endl;
    file << "| Rocznik: " << rocznik << endl;
    file << "| Ilosc czynnika: " << gramy_chlodniczego << "g" << endl;
    file << "| Ozonowanie: " << (ozonowanie == 't' ? "Tak" : "Nie") << endl;
    file << "| Cena za serwis: 200 zl" << endl;
    if (ilosc_100g > 0) {
        file << "| Cena za nabicie czynnika (" << ilosc_100g * 100 << " g): " << cena_nabicia << " zl" << endl;
    }
    file << "| Cena za ozonowanie: " << (ozonowanie == 't' ? "50 zl" : "0 zl") << endl;
    file << "| Calkowita cena: " << cena << " zl" << endl;
    file << "+----------------------------------------------+" << endl;
    file << endl;

    // Zapisz sume zuzytego czynnika ze wszystkich samochodow
    file << "Suma zuzytego czynnika w miesiacu " << currentMonth << ": " << zuzycieMiesiac + gramy_chlodniczego << "g" << endl;
    file << "Suma zuzytego czynnika w roku " << currentYear << ": " << zuzycieRok + gramy_chlodniczego << "g" << endl;
    file << endl << endl;

    file.close();

    return 0;
}
