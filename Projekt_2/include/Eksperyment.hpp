#pragma once
#include <iostream>
#include <string>
#include "Parameters.h"
#include "Reader_Writer.hpp"
#include "Graph.hpp"
#include "Generator.hpp"
#include "Tablica.hpp"
#include "Prim.hpp"
#include "Kruskal.hpp"
#include "Dijkstra.hpp"
#include "BellmanFord.hpp"
#include "FordFulkerson.hpp"
#include "Utils.hpp"
#include "Stoper.hpp"
#include "ZapisCSV.hpp"

using namespace std;

// Uruchamia wybrany algorytm na danym grafie, sprawdzajac zgodnosc typu grafu z wymaganiami algorytmu
void uruchomAlgorytm(Graph* graf, Parameters::Algorithms alg, std::ostream& out = std::cout) {
    if (Parameters::runMode == Parameters::RunModes::singleFile) {
        out << "\nAlgorytm: " << Utils::tlumaczAlgorytm(alg) << "\n";
    }

    switch (alg) {
    case Parameters::Algorithms::prim:
			Prim::run(graf, out);
        break;

    case Parameters::Algorithms::kruskal:
    	Kruskal::run(graf, out);
        break;

    case Parameters::Algorithms::dijkstra:
    	Dijkstra::run(graf, Parameters::vertexStart, Parameters::vertexEnd, out);
        break;

    case Parameters::Algorithms::bellmanFord:
    	BellmanFord::run(graf, Parameters::vertexStart, Parameters::vertexEnd, out);
        break;

    case Parameters::Algorithms::fordFulkerson:
    	FordFulkerson::run(graf, Parameters::vertexStart, Parameters::vertexEnd, out);
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

	// Sprawdzenie zgodności wybranych algorytmów z problemem
    for (size_t i = 0; i < testowaneAlgorytmy.getSize(); ++i) {
        if (!Utils::czyAlgorytmPasujeDoProblemu(testowaneAlgorytmy[i], Parameters::problem)) {
            cerr << "Bląd: Wybrany algorytm (" << Utils::tlumaczAlgorytm(testowaneAlgorytmy[i])
                << ") nie moze byc uzyty do problemu " << Utils::tlumaczProblem(Parameters::problem) << "\n";
            cerr << "Popraw flage -a lub -p\n";
            return; 
        }
    }

	// Tryb pojedynczego testu
    if (Parameters::runMode == Parameters::RunModes::singleFile) {
        if (Parameters::inputFile.empty()) {
            cerr << "BLAD: Tryb pojedynczego testu wymaga podania pliku wejsciowego (flaga -i).\n";
            return;
        }

        string plikWynikowy = Parameters::outputFile;
        if (plikWynikowy.empty()) {
            plikWynikowy = "wynik.txt";
            cout << "Uwaga: Nie podano flagi -o. Zapisuje wynik do domyslnego pliku: " << plikWynikowy << "\n";
        }
        else
            cout << "Plik wynikowy txt: " << plikWynikowy << "\n";

        // Inicjalizacja writera
        Writer writer(plikWynikowy);
        std::ostream& out = writer.getStream();

        out << "problem: " << Utils::tlumaczProblem(Parameters::problem) << "\n";

        for (size_t s = 0; s < testowaneStruktury.getSize(); ++s) {
            Parameters::Structures obecnaStruktura = testowaneStruktury[s];
            bool useMatrix = (obecnaStruktura == Parameters::Structures::incidenceMatrix);

            out << "\nStruktura: " << Utils::tlumaczStrukture(obecnaStruktura) << "\n";

			Graph* graf = Reader::loadFromFile(Parameters::inputFile, isDirected, useMatrix); // Wczytanie grafu z pliku, z uwzględnieniem typu grafu i struktury
            if (graf == nullptr) continue;

			//graf->display(out); // Wyświetlenie grafu

            for (size_t a = 0; a < testowaneAlgorytmy.getSize(); ++a) {
                string czasStart = Stoper().pobierzAktualnyCzas(); // Pobieramy aktualny czas przed rozpoczęciem sortowania
                Stoper stoper;
                stoper.start(); // Uruchamiamy stoper

                uruchomAlgorytm(graf, testowaneAlgorytmy[a], out);

                stoper.stop(); // Zatrzymujemy stoper po zakończeniu sortowania
                string czasKoniec = Stoper().pobierzAktualnyCzas(); // Pobieramy aktualny czas po zakończeniu sortowania

				out << "Czas wykonania algorytmu: " << stoper.getCzasMikrosekundy() << " mikrosekundy | "
					<< stoper.getCzasMilisekundy() << " milisekundy | "
					<< stoper.getCzasSekundy() << " sekundy\n"
            		<<czasStart << " -> " << czasKoniec << "\n";
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

        string plikWynikowy = Parameters::resultsFile;
        if (plikWynikowy.empty()) {
            plikWynikowy = "wyniki_grafy.csv";
            cout << "Uwaga: Nie podano flagi -r! Zapisuje domyslnie do: " << plikWynikowy << "\n";
        }
        else {
            cout << "Plik wynikowy CSV: " << plikWynikowy << "\n";
        }

        string nazwaProblemu = Utils::tlumaczProblem(Parameters::problem);

        cout << "Problem: " << nazwaProblemu << "\n";
        cout << "V: " << Parameters::vertexCount << " | Gestosc: " << Parameters::density << "%\n";
        cout << "Iteracje (-n): " << Parameters::iterations << "\n";
        long long maxEdges = isDirected ? (long long)Parameters::vertexCount * (Parameters::vertexCount - 1)
            : (long long)Parameters::vertexCount * (Parameters::vertexCount - 1) / 2;
        long long requiredEdges = (maxEdges * Parameters::density) / 100;
        long long minEdges = isDirected ? Parameters::vertexCount : Parameters::vertexCount - 1;

        if (requiredEdges < minEdges) {
            std::cout << "Uwaga: Przy podanej liczbie wierzcholkow i gestosci, graf nie bedzie spojny."
                " Ustawiam liczbe krawedzi na minimalna potrzebna do spojnosci: " << minEdges << "\n";
        }
        cout << "\n";

		// Iteracja po strukturach 
        for (size_t s = 0; s < testowaneStruktury.getSize(); ++s) {
			Parameters::Structures obecnaStruktura = testowaneStruktury[s]; // Pobranie obecnej struktury z tablicy struktur
            bool useMatrix = (obecnaStruktura == Parameters::Structures::incidenceMatrix);
            string nazwaStruktury = Utils::tlumaczStrukture(obecnaStruktura);
            cout << "struktura: " << nazwaStruktury << "\n";

			// Iteracja po algorytmach
            for (size_t a = 0; a < testowaneAlgorytmy.getSize(); ++a) {
				Parameters::Algorithms obecnyAlgorytm = testowaneAlgorytmy[a]; // Pobranie obecnego algorytmu z tablicy algorytmów

                string nazwaAlgorytmu = Utils::tlumaczAlgorytm(obecnyAlgorytm);
                cout << "Testowanie algorytmu: " << nazwaAlgorytmu << "\n";

				// Iteracja po liczbie iteracji dla danego algorytmu i struktury
                for (int iteracja = 0; iteracja < Parameters::iterations; ++iteracja) {
					
                	// Generowanie grafu na podstawie parametrów wierzchołków, gęstości, typu grafu i struktury
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

                	//graf->display();

                    string czasStart = Stoper().pobierzAktualnyCzas(); // Pobieramy aktualny czas przed rozpoczęciem sortowania
                    Stoper stoper;
                    stoper.start(); // Uruchamiamy stoper

                    uruchomAlgorytm(graf, testowaneAlgorytmy[a]);

                    stoper.stop(); // Zatrzymujemy stoper po zakończeniu sortowania
                    string czasKoniec = Stoper().pobierzAktualnyCzas(); // Pobieramy aktualny czas po zakończeniu sortowania

                    // Zapisy do pliku csv
                    zapiszWynikDoCSV(plikWynikowy, nazwaProblemu, Parameters::vertexCount, Parameters::density, 
                        graf->getEdgesCount(),nazwaStruktury, nazwaAlgorytmu,
                        stoper.getCzasMikrosekundy(), stoper.getCzasMilisekundy(), stoper.getCzasSekundy(),
                        czasStart, czasKoniec
                    );

                    delete graf;
                }
            }
        }
    }
}