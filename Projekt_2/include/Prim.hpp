#pragma once
#include "Graph.hpp"
#include "MinHeap.hpp"
#include "Tablica.hpp"
#include <iostream>

class Prim{
public:
    static void run(Graph* graph) {
        int V = graph->getVerticesCount();
        if (V == 0) return;

		Tablica<bool> visited(V); // Tablica do śledzenia odwiedzonych wierzchołków, inicjalizowana na false
        for (int i = 0; i < V; ++i) visited.push_back(false);

		MinHeap<Edge> pq; // Kopiec minimalny do przechowywania krawędzi, posortowany według wagi

		int mstWeight = 0; // Zmienna do przechowywania całkowitej wagi MST
		int edgesInMST = 0; // Licznik krawędzi dodanych do MST

        std::cout << "Krawedzie wchodzace w sklad MST:\n";

		int startNode = 0; // wierzchowek początkowy 
		visited[startNode] = true; // Oznacza wierzchołek początkowy jako odwiedzony

		Tablica<Edge> startNeighbors = graph->getNeighbors(startNode); // Pobiera sąsiadów wierzchołka początkowego
		for (size_t i = 0; i < startNeighbors.getSize(); ++i) { // Dodaje krawędzie sąsiadów do kopca
            pq.push(startNeighbors[i]); 
        }

		// Pętla trwa do momentu, gdy kopiec jest pusty lub gdy dodano V-1 krawędzi do MST
        while (!pq.isEmpty() && edgesInMST < V - 1) {

			Edge minEdge = pq.pop(); // Pobiera krawędź o najmniejszej wadze z kopca

			int u = minEdge.src; // Wierzcholek startowy krawędzi
			int v = minEdge.dest; // Wierzcholek koncowy krawędzi

			if (visited[v]) continue; // Jeśli wierzchołek docelowy jest już odwiedzony, pomija tę krawędź

			visited[v] = true; // Oznacza wierzchołek docelowy jako odwiedzony
			mstWeight += minEdge.weight; // Dodaje wagę krawędzi do całkowitej wagi MST
			edgesInMST++; // Zwiększa licznik krawędzi w MST

            std::cout << "Dodano krawedz: " << u << " - " << v << " (waga: " << minEdge.weight << ")\n";

			Tablica<Edge> nextNeighbors = graph->getNeighbors(v); // Pobiera sąsiadów nowo dodanego wierzchołka
			for (size_t i = 0; i < nextNeighbors.getSize(); ++i) { // Dodaje krawędzie sąsiadów do kopca
                if (!visited[nextNeighbors[i].dest]) { // jeśli ich docelowe wierzchołki nie są jeszcze odwiedzone
                    pq.push(nextNeighbors[i]);
                }
            }
        }

        if (edgesInMST != V - 1) {
            std::cout << "Nie udalo sie zbudowac MST. Graf prawdopodobnie nie jest spojny\n";
        }
        else {
            std::cout << "Calkowity koszt MST: " << mstWeight << "\n";
        }
    }
};