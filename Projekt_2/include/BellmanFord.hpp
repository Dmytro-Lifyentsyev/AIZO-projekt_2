#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"
#include "Parameters.h"
#include "Utils.hpp"
#include <iostream>
#include <climits>

class BellmanFord {
public:
    static void run(Graph* graph, int startNode, int endNode = -1) {
        int V = graph->getVerticesCount(); // Pobiera liczbę wierzchołków w grafie
        if (V == 0) return;

        int Start = startNode; // wierzchołek startowy

        if (Parameters::runMode == Parameters::RunModes::singleFile) {
            // Sprawdzenie wierzchołka startowego (-c)
            if (startNode < 0 || startNode >= V) {
                std::cout << "Ostrzezenie: Nie podano poprawnego wierzcholka startowego. Domyslnie startuje od 0\n";
                Start = 0;
            }

            // Sprawdzenie wierzchołka końcowego (-e)
            if (endNode < 0 || endNode >= V) { 
                std::cout << "Informacja: Nie podano wierzcholka koncowego. Zostana wyznaczone sciezki do wszystkich wierzcholkow\n";
            }
        }
        else {
            Start = 0; // W trybie badan (benchmark) startowy to zawsze 0
        }

        Tablica<int> dist(V); // dist[i]: najkrótszy znany dotąd czas przejazdu do wierzchołka i
        Tablica<int> prev(V); // prev[i]: z jakiego wierzchołka najszybciej przyjsć do wierzchołka i

        for (int i = 0; i < V; ++i) {
            dist.push_back(INT_MAX); // Na początku dystans wszędzie to nieskończoność
            prev.push_back(-1); // Brak poprzednika
        }

        dist[Start] = 0; // Punkt startowy nie kosztuje nic, dystans do siebie samego to 0

        // Algorytm musi wykonać dokładnie V - 1 iteracji
        for (int i = 0; i < V - 1; ++i) {
			bool anyUpdate = false; // Flaga do sprawdzenia, czy w tej iteracji zaktualizowano jakąkolwiek odległość

            // Przechodzimy przez wszystkie wierzchołki i ich sąsiadów
            for (int u = 0; u < V; ++u) {
                if (dist[u] == INT_MAX) continue; // Jeśli do u nie da się dojść, nie sprawdzamy jego sąsiadów

                Tablica<Edge> neighbors = graph->getNeighbors(u); // Pobieramy wszystkich sąsiadów wierzchołka u
                for (size_t j = 0; j < neighbors.getSize(); ++j) {
                    int v = neighbors[j].dest; 
                    int weight = neighbors[j].weight;

                    // relaksacja: jeśli znaleziono krótszą trasę do v przez u, aktualizujemy dystans i poprzednika
                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        prev[v] = u;
						anyUpdate = true; // Zaznaczamy, że dokonano aktualizacji w tej iteracji
                    }
                }
            }
            if (!anyUpdate) break; // Jeśli nie zaktualizowano żadnej odległości w tym kroku, znaczy, że znaleziono optymalną ścieżkę szybciej
        }

        // Weryfikacja cykli o ujemnej wadze, jeden dodatkowy przebieg (w naszym przypadku jest bezużyteczny, bo nie mamy ujemnych wag)
        bool hasNegativeCycle = false;
        for (int u = 0; u < V; ++u) {
            if (dist[u] == INT_MAX) continue;

            Tablica<Edge> neighbors = graph->getNeighbors(u);
            for (size_t j = 0; j < neighbors.getSize(); ++j) {
                int v = neighbors[j].dest;
                int weight = neighbors[j].weight;

                if (dist[u] + weight < dist[v]) {
                    hasNegativeCycle = true;
                    break;
                }
            }
            if (hasNegativeCycle) break;
        }

        // wyswietlanie: Działa tylko w trybie SingleFile 
        if (Parameters::runMode == Parameters::RunModes::singleFile) {
            if (hasNegativeCycle) {
                std::cerr << "Bład: Graf zawiera cykl o ujemnej wadze. Najkrotsza sciezka nie istnieje\n";
                return;
            }
            if (endNode >= 0 && endNode < V) {
                Utils::printSinglePath(Start, endNode, dist, prev);
            }
            else {
                std::cout << "Sciezki od V" << Start << " do wszystkich wierzcholkow:\n";
                for (int i = 0; i < V; ++i) {
                    if (i != Start) Utils::printSinglePath(Start, i, dist, prev);
                }
            }
        }
    }
};