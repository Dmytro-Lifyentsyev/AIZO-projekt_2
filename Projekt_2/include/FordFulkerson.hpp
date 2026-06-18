#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"
#include "Parameters.h"
#include <iostream>
#include <climits>

class FordFulkerson {
private:
    // Wewnętrzna funkcja DFS (szuka ścieżki powiększającej)
    static bool dfs(int start, int target, const Tablica<Tablica<int>>& rGraph, Tablica<int>& parent) {
		int V = rGraph.getSize(); // Liczba wierzchołków

		Tablica<bool> visited(V); // visited[i] == true oznacza, że wierzchołek i został odwiedzony podczas DFS
        for (int i = 0; i < V; ++i) visited.push_back(false); // Nikt nie jest odwiedzony

		Tablica<int> stack; // Stos do przechowywania wierzchołków do odwiedzenia (LIFO)
		stack.push_back(start); // Zaczynamy od wierzchołka startowego
		visited[start] = true; // Oznaczamy wierzchołek startowy jako odwiedzony

        while (stack.getSize() > 0) {
            int u = stack[stack.getSize() - 1]; // Zdejmujemy wierzchołek ze szczytu stosu
            stack.pop_back();

            // Przeszukuje wszystkich sąsiadów w grafie rezydualnym (rGraph)
            for (int v = 0; v < V; ++v) {
                if (!visited[v] && rGraph[u][v] > 0) { // jeżeli wierzchołek nie był odłiedzony i krawędz wciąż może coś przepuścić (rGraph[u][v] > 0)
                    parent[v] = u; // Zapisuje, skąd przyszedł (do rekonstrukcji ścieżki)
                    visited[v] = true; // Zaznaczamy, jako odzwiedzony

                    if (v == target) {
                        return true; // Sukces, znaleźiona droga do ujścia
                    }

                    stack.push_back(v); // Wrzucamy sąsiada na stos do dalszego zwiedzania
                } 
            }
        }
        return false; // Jeśli stos opustoszeje i nie znajdziemy "target", oznacza to, że nie ma już żadnej drogi
    }

public:
    static void run(Graph* graph, int startNode, int endNode) {
        int V = graph->getVerticesCount(); // Pobiera liczbę wierzchołków w grafie
        if (V < 2) return; // Potrzebuje min. 2 wierzchołka (źródło i ujście)

        int Start = startNode; // wierzchołek startowy (źródło)
		int Sink = endNode; // wierzchołek końcowy (ujście)

        if (Parameters::runMode == Parameters::RunModes::singleFile) {
            // Sprawdzenie wierzchołka startowego (-c)
            if (Start < 0 || Start >= V) {
                std::cout << "Ostrzezenie: Niepoprawne zrodlo (-c). Domyslnie uzywam 0\n";
                Start = 0;
            }
            // Sprawdzenie wierzchołka końcowego (-e)
            if (Sink < 0 || Sink >= V || Sink == Start) {
                std::cout << "Ostrzezenie: Niepoprawne ujscie (-e). Domyslnie uzywam " << (V - 1) << "\n";
                Sink = V - 1;
            }
        }
        else {
            // W badaniach przepływ z wierzchołka 0 do ostatniego
            Start = 0;
            Sink = V - 1;
        }

        //BUDOWA GRAFU REZYDUALNEGO
        Tablica<Tablica<int>> rGraph(V); // Inicjalizacja dwuwymiarowej Tablicy V x V wypełnioną zerami
        for (int i = 0; i < V; ++i) {
            Tablica<int> row(V);
            for (int j = 0; j < V; ++j) row.push_back(0);
            rGraph.push_back(row);
        }

        // Przepisuje pojemności z oryginalnego grafu
        for (int u = 0; u < V; ++u) {
            Tablica<Edge> neighbors = graph->getNeighbors(u);
            for (size_t i = 0; i < neighbors.getSize(); ++i) {
                int v = neighbors[i].dest;
                int weight = neighbors[i].weight;
                if (weight > 0) {
                    rGraph[u][v] += weight; 
                }
            }
        }

		Tablica<int> parent(V); // parent[i] będzie przechowywać poprzednika w ścieżce powiększającej
		for (int i = 0; i < V; ++i) parent.push_back(-1); // Inicjalizacja parentów na -1 (brak poprzednika)

		int maxFlow = 0; // Zmienna do przechowywania sumy przepływu

        // Powtarza, dopóki DFS potrafi dojść ze Źródła do Ujścia
        while (dfs(Start, Sink, rGraph, parent)) {

            // Znajdowanie wąskiego gardła konkretnej ścieżki
			int pathFlow = INT_MAX; // Na początku zakładamy, że wąskie gardło jest nieskończonością
            for (int v = Sink; v != Start; v = parent[v]) {
                int u = parent[v];
                if (rGraph[u][v] < pathFlow) {
                    pathFlow = rGraph[u][v]; // Ustawiamy wąskie gardło na mniejszą pojemność
                }
            }

			// Wypisanie znalezionej ścieżki powiększającej i jej przepływ
            if (Parameters::runMode == Parameters::RunModes::singleFile) {
                Tablica<int> path;
                int curr = Sink;
                while (curr != Start) {
                    path.push_back(curr);
                    curr = parent[curr];
                }
                path.push_back(Start);

                std::cout << "Sciezka powiekszajaca: ";
                for (int j = path.getSize() - 1; j >= 0; --j) {
                    std::cout << "V" << path[j];
                    if (j > 0) std::cout << " -> ";
                }
                std::cout << " | Przepchano: " << pathFlow << "\n";
            }

            // Aktualizuje przepustowości resztkowe (odejmij w przód, dodaj w tył)
            for (int v = Sink; v != Start; v = parent[v]) {
                int u = parent[v];
                rGraph[u][v] -= pathFlow; // Wykorzystuje pojemność krawędzi
                rGraph[v][u] += pathFlow; // Tworzy "krawędź powrotną" 
            }

            maxFlow += pathFlow; // Sumowanie przepływu do globalnego wyniku
        }

		// Wypisywanie wyniku końcowego dla SingleFile
        if (Parameters::runMode == Parameters::RunModes::singleFile) {
            std::cout << "\nStan krawedzi (Zajety przeplyw / Pojemnosc)\n";

            for (int u = 0; u < V; ++u) {
                Tablica<Edge> neighbors = graph->getNeighbors(u);
                for (size_t i = 0; i < neighbors.getSize(); ++i) {
					int v = neighbors[i].dest; // Docelowy wierzchołek krawędzi
					int originalCapacity = neighbors[i].weight; // Oryginalna pojemność krawędzi (z grafu wejściowego)

                	int flowUsed = originalCapacity - rGraph[u][v]; // Przepływ to oryginalna pojemność minus to, co zostało w rGraph

                    if (flowUsed > 0) {
                        std::cout << "Krawedz V" << u << " -> V" << v
                            << " | Wykorzystano: " << flowUsed << " / " << originalCapacity << "\n";
                    }
                }
            }
            std::cout << "Maksymalny przeplyw ze zrodla V" << Start << " do ujscia V" << Sink << " wynosi: " << maxFlow << "\n\n";
        }
    }
};