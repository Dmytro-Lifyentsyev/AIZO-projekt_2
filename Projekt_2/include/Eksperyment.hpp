#pragma once
#include <iostream>
#include <string>
#include "Parameters.h"
#include "Reader.hpp"
#include "Graph.hpp"
#include "Generator.hpp"
#include "Tablica.hpp"
#include "Prim.hpp"
#include "Utils.hpp"

using namespace std;

// Uruchamia wybrany algorytm na danym grafie, sprawdzajac zgodnosc typu grafu z wymaganiami algorytmu
void uruchomAlgorytm(Graph* graf, Parameters::Algorithms alg) {
    cout << "\nAlgorytm: " << Utils::tlumaczAlgorytm(alg) << "\n";

    switch (alg) {
    case Parameters::Algorithms::prim:
        if (graf->getIsDirected()) 
            cerr << "Blad: Prim wymaga grafu nieskierowanego\n";
        else 
			Prim::run(graf);
        break;

    case Parameters::Algorithms::kruskal:
        if (graf->getIsDirected()) 
            cerr << "Blad: Kruskal wymaga grafu nieskierowanego\n";
        else {/* TODO: Kruskal::run(graf); */}
        break;

    case Parameters::Algorithms::dijkstra:
        if (!graf->getIsDirected()) 
            cerr << "Blad: Dijkstra wymaga grafu skierowanego\n";
        else {/* TODO: Dijkstra::run(graf); */}
        break;

    case Parameters::Algorithms::bellmanFord:
        if (!graf->getIsDirected()) 
            cerr << "Blad: Bellman-Ford wymaga grafu skierowanego\n";
        else {/* TODO: BellmanFord::run(graf); */ }
        break;

    case Parameters::Algorithms::fordFulkerson:
        if (!graf->getIsDirected()) 
            cerr << "Blad: Ford-Fulkerson wymaga grafu skierowanego\n";
        else {/* TODO: FordFulkerson::run(graf); */ }
        break;

    default:
        cerr << "Blad: Niepoprawny parametr algorytmu\n";
        break;
    }

}

// Główna funkcja przeprowadzająca badanie, w zależności od trybu uruchomienia (pojedynczy test lub benchmark) 
void przeprowadzEksperyment() {

    if (Parameters::problem == Parameters::Problems::undefined || Parameters::problem == Parameters::Problems::count) {
        cerr << "BLAD: Nie zdefiniowano poprawnie problemu (flaga -p).\n";
        return;
    }

	// Określenie, czy graf jest skierowany (SP/MF) czy nieskierowany (MST)
    bool isDirected = (Parameters::problem == Parameters::Problems::sp || Parameters::problem == Parameters::Problems::mf);

	Tablica<Parameters::Structures> testowaneStruktury = Utils::pobierzStrukturyDoUruchomienia(); // Pobranie tablicy struktur do uruchomienia
	Tablica<Parameters::Algorithms> testowaneAlgorytmy = Utils::pobierzAlgorytmyDoUruchomienia(); // Pobranie tablicy algorytmow do uruchomienia

	// Tryb pojedynczego testu
    if (Parameters::runMode == Parameters::RunModes::singleFile) {
        if (Parameters::inputFile.empty()) {
            cerr << "BLAD: Tryb pojedynczego testu wymaga podania pliku wejsciowego (flaga -i).\n";
            return;
        }

        for (size_t s = 0; s < testowaneStruktury.getSize(); ++s) {
            Parameters::Structures obecnaStruktura = testowaneStruktury[s];
            bool useMatrix = (obecnaStruktura == Parameters::Structures::incidenceMatrix);

            cout << "\nStruktura: " << Utils::tlumaczStrukture(obecnaStruktura) << "\n";

			Graph* graf = Reader::loadFromFile(Parameters::inputFile, isDirected, useMatrix); // Wczytanie grafu z pliku, z uwzględnieniem typu grafu i struktury
            if (graf == nullptr) continue;

			graf->display(); // Wyświetlenie grafu

            for (size_t a = 0; a < testowaneAlgorytmy.getSize(); ++a) {
                uruchomAlgorytm(graf, testowaneAlgorytmy[a]);
            }

			delete graf; // Czyszczenie pamięci po zakończeniu testu dla danego grafu
        }
    }

	// Tryb benchmarku
    else if (Parameters::runMode == Parameters::RunModes::benchmark) {
        if (Parameters::vertexCount <= 0 || Parameters::density <= 0 || Parameters::iterations <= 0) {
            cerr << "BLAD: Tryb badan wymaga poprawnych wartosci dla wierzcholkow (-l), gestosci (-d) i iteracji (-n).\n";
            return;
        }

        cout << "Problem: " << (isDirected ? "Skierowany (SP/MF)" : "Nieskierowany (MST)") << "\n";
        cout << "V: " << Parameters::vertexCount << " | Gestosc: " << Parameters::density << "%\n";
        cout << "Iteracje (-n): " << Parameters::iterations << "\n\n";

        for (size_t s = 0; s < testowaneStruktury.getSize(); ++s) {
            Parameters::Structures obecnaStruktura = testowaneStruktury[s];
            bool useMatrix = (obecnaStruktura == Parameters::Structures::incidenceMatrix);

            cout << "struktura: " << Utils::tlumaczStrukture(obecnaStruktura) << "\n";

            for (size_t a = 0; a < testowaneAlgorytmy.getSize(); ++a) {
                Parameters::Algorithms obecnyAlgorytm = testowaneAlgorytmy[a];
                cout << "Testowanie algorytmu: " << Utils::tlumaczAlgorytm(obecnyAlgorytm) << "\n";

                for (int iteracja = 0; iteracja < Parameters::iterations; ++iteracja) {

                    Graph* graf = GraphGenerator::generate(
                        Parameters::vertexCount,
                        Parameters::density,
                        isDirected,
                        useMatrix
                    );

                    if (graf == nullptr) {
                        cerr << "Blad generowania grafu w iteracji " << iteracja << "\n";
                        continue;
                    }

                    if (Parameters::vertexCount <= 10) 
                        graf->display();

                    uruchomAlgorytm(graf, testowaneAlgorytmy[a]);

                    delete graf;
                }
            }
        }
    }
}