#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

inline void zapiszWynikDoCSV(const string& nazwaPliku, const string& problem, size_t V, int gestosc, int E,
        const string& struktura, const string& algorytm, double czasus, double czasms, double czass,
        const string& czasStart, const string& czasKoniec) {

        ifstream plikTest(nazwaPliku); // Sprawdzamy, czy plik juz istnieje
        bool plikIstnieje = plikTest.good(); // Jesli plik istnieje, nie zapisujemy naglowkow kolumn
        plikTest.close(); // Zamykamy plik testowy

        ofstream plik(nazwaPliku, ios::app); // Otwieramy plik w trybie dopisywania (append)

        if (!plik.is_open()) {
            cerr << "Blad: Nie udalo sie otworzyc pliku " << nazwaPliku << " do zapisu!\n";
            return;
        }

        // Jesli plik nie istnieje, zapisuje naglowki kolumn
        if (!plikIstnieje) {
            plik << "Problem;Liczba Wierzcholkow (V);Gestosc (%);Rzeczywista Liczba Krawedzi (E);Struktura;Algorytm;"
                << "Czas [us];Czas [ms];Czas [s];Start Badania;Koniec Badania\n";
        }

        // Zapisujemy dane do pliku
        plik << problem << ";"
            << V << ";"
            << gestosc << ";"
            << E << ";"
            << struktura << ";"
            << algorytm << ";"
            << czasus << ";"
            << czasms << ";"
            << czass << ";"
            << czasStart << ";"
            << czasKoniec << "\n";

        plik.close();
}