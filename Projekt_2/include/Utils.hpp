#pragma once
#include <string>
#include <iostream>
#include <climits>
#include "Parameters.h"
#include "Tablica.hpp"

using namespace std;
/* Klasa Utils zawiera funkcje pomocnicze do tłumaczenia enumów na czytelne stringi 
 * oraz do pobierania listy algorytmów i struktur do uruchomienia na podstawie ustawień w Parameters. 
 */
class Utils {
public:
    static string tlumaczAlgorytm(Parameters::Algorithms alg) {
        switch (alg) {
        case Parameters::Algorithms::prim: return "Prim (MST)";
        case Parameters::Algorithms::kruskal: return "Kruskal (MST)";
        case Parameters::Algorithms::dijkstra: return "Dijkstra (SP)";
        case Parameters::Algorithms::bellmanFord: return "Bellman-Ford (SP)";
        case Parameters::Algorithms::fordFulkerson: return "Ford-Fulkerson (MF)";
        default: return "Nieznany";
        }
    }

    static string tlumaczStrukture(Parameters::Structures struc) {
        switch (struc) {
        case Parameters::Structures::incidenceMatrix: return "Macierz Incydencji";
        case Parameters::Structures::adjacencyList: return "Lista Sasiadow";
        default: return "Nieznana";
        }
    }

    static Tablica<Parameters::Algorithms> pobierzAlgorytmyDoUruchomienia() {
        Tablica<Parameters::Algorithms> algorytmy;
        if (Parameters::algorithm != Parameters::Algorithms::allAlgorithms) {
            algorytmy.push_back(Parameters::algorithm);
        }
        else {
            if (Parameters::problem == Parameters::Problems::mst) {
                algorytmy.push_back(Parameters::Algorithms::prim);
                algorytmy.push_back(Parameters::Algorithms::kruskal);
            }
            else if (Parameters::problem == Parameters::Problems::sp) {
                algorytmy.push_back(Parameters::Algorithms::dijkstra);
                algorytmy.push_back(Parameters::Algorithms::bellmanFord);
            }
            else if (Parameters::problem == Parameters::Problems::mf) {
                algorytmy.push_back(Parameters::Algorithms::fordFulkerson);
            }
        }
        return algorytmy;
    }

    static Tablica<Parameters::Structures> pobierzStrukturyDoUruchomienia() {
        Tablica<Parameters::Structures> struktury;
        if (Parameters::structure != Parameters::Structures::allStructures) {
            struktury.push_back(Parameters::structure);
        }
        else {
            struktury.push_back(Parameters::Structures::incidenceMatrix);
            struktury.push_back(Parameters::Structures::adjacencyList);
        }
        return struktury;
    }

    // Funkcja do wypisania scieżki 
    static void printSinglePath(int startNode, int endNode, const Tablica<int>& dist, const Tablica<int>& prev) {
        std::cout << "Najkrotsza sciezka V" << startNode << " -> V" << endNode << " | Koszt: ";

        // Jeśli dystans wynosi INT_MAX, oznacza to, że algorytm nigdy tam nie dotarł
        if (dist[endNode] == INT_MAX) {
            std::cout << "brak trasy\n";
        }
        else {
            std::cout << dist[endNode] << " | Trasa: ";

            // Zbieranie trasy: Zaczynamy od końca (endNode) i cofamy się używając tablicy prev
            Tablica<int> path;
            int current = endNode;
            while (current != -1) {
                path.push_back(current); // Dodajemy aktualny wierzchołek do trasy
                current = prev[current]; // Cofamy się do poprzedniego wierzchołka na trasie, aż dotrzemy do startu 
            }

            // Wyswietlamy trasę we właściwej kolejności od startu do celu 
            for (int j = path.getSize() - 1; j >= 0; --j) {
                std::cout << "V" << path[j];
                if (j > 0) std::cout << " -> ";
            }
            std::cout << "\n";
        }
    }
};