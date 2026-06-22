#pragma once
#include "Graph.hpp"
#include "MatrixGraph.hpp"
#include "ListGraph.hpp"
#include "Tablica.hpp"
#include <cstdlib>

class GraphGenerator {
public:
    static Graph* generate(int V, int density, bool isDirected, bool useMatrix) {
        if (V <= 0) return nullptr;

		long long maxEdges = isDirected ? V * (V - 1) : V * (V - 1) / 2; // Liczba krawedzi w grafie pelnym
		long long requiredEdges = (maxEdges * density) / 100;  // Obliczamy wymagana liczbe krawedzi na podstawie gestosci

        long long minEdges = isDirected ? V : V - 1; // minimalna liczba krawedzi dla spojnosci grafu
        if (V <= 1) minEdges = 0;

        if (requiredEdges < minEdges) requiredEdges = minEdges; // Zapewniamy, ze graf bedzie spojny, nawet przy niskiej gestosci

		Graph* graph = nullptr; // Wskaznik na graf, ktory zostanie wygenerowany
        if (useMatrix) graph = new MatrixGraph(V, requiredEdges, isDirected);
        else graph = new ListGraph(V, requiredEdges, isDirected);

        if (V <= 1) return graph;

		Tablica<bool> existingEdges(V * V); // Tablica do sledzenia, czy dana krawedz juz istnieje 
		for (int i = 0; i < V * V; ++i) existingEdges.push_back(false); // Inicjalizacja tablicy na false (nie ma zadnych krawedzi)

		int currentEdges = 0; // Licznik aktualnie dodanych krawedzi
		int maxWeightRange = (requiredEdges * 4) / 5; // Ustalamy zakres wag krawedzi 
		if (maxWeightRange < 1) maxWeightRange = 1; // Zapewniamy, ze zakres wag bedzie przynajmniej 1

        // Tworzy krawedz miedzy kolejnymi wierzcholkami, zeby zapewnic spojnosc grafu
        for (int i = 0; i < V - 1; ++i) {
			int u = i; 
            int v = i + 1;
			int weight = (rand() % maxWeightRange) + 1; // Losowanie wagi krawedzi z zakresu od 1 do maxWeightRange

			graph->addEdge(u, v, weight); // Dodajemy krawedz miedzy kolejnymi wierzcholkami
			existingEdges[u * V + v] = true; // Oznaczamy, ze ta krawedz juz istnieje
			if (!isDirected) existingEdges[v * V + u] = true; // W grafie nieskierowanym oznaczamy krawedz w obie strony
			currentEdges++; // Zwiekszamy licznik aktualnie dodanych krawedzi
        }

		// Jesli graf jest skierowany, dodajemy krawedz miedzy ostatnim a pierwszym wierzcholkiem
        if (isDirected) { 
            int u = V - 1;
            int v = 0;
            int weight = (rand() % maxWeightRange) + 1;

            graph->addEdge(u, v, weight);
            existingEdges[u * V + v] = true;
            currentEdges++;
        }

		// Dodajemy losowe krawedzie, az osiagnie wymagana liczbe krawedzi
        while (currentEdges < requiredEdges) {
			int u = rand() % V; // Losujemy dwa wierzcholki u i v, miedzy ktorymi bedziemy probowac dodac krawedz
            int v = rand() % V;

			if (u == v) continue; // Nie dodajemy krawedzi miedzy tym samym wierzcholkiem
			if (existingEdges[u * V + v]) continue; // Nie dodajemy krawedzi, ktora juz istnieje

			int weight = (rand() % maxWeightRange) + 1; // Losujemy wage krawedzi z zakresu od 1 do maxWeightRange
			graph->addEdge(u, v, weight);

            existingEdges[u * V + v] = true;
            if (!isDirected) existingEdges[v * V + u] = true;

            currentEdges++;
        }

		return graph; // Zwracamy wygenerowany graf
    }
};