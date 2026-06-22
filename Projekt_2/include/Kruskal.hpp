#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"
#include <iostream>

class Kruskal {
private:
	// Struktura danych do zarządzania zbiorami rozłącznymi (Union-Find) 
    class UnionFind {
    private:
		Tablica<int> parent; // Przechowuje korzeń dla każdego wierzchołka
        Tablica<int> rank; // Przechowuje głębokość drzewa, żeby optymalizować łączenie

    public:
        // Konstruktor: Na początku każdy wierzchołek jest korzeniem
        UnionFind(int n) {
            for (int i = 0; i < n; ++i) {
                parent.push_back(i); 
                rank.push_back(0); // Głębokość każdego drzewa to na początku 0
            }
        }

        // Funkcja szuka korzenia dla wierzchołka i
        int find(int i) {
			if (parent[i] == i) { // Jeśli i jest korzeniem, zwróć i
                return i;
            }
			parent[i] = find(parent[i]); // bezpośrednio łączymy i z jego korzeniem, co przyspiesza przyszłe wyszukiwania
			return parent[i]; // Zwraca korzeń dla i
        }

        // Funkcja łączy dwa zbiory w jeden
        bool unionSets(int i, int j) {
            int root_i = find(i); // Znajdź korzeń pierwszego wierzchołka
            int root_j = find(j); // Znajdź korzeń drugiego wierzchołka

            // Jeśli mają różne korzenie, to znaczy, że nie są jeszcze połączeni
            if (root_i != root_j) {
                if (rank[root_i] < rank[root_j]) {
                    parent[root_i] = root_j;
                }
                else if (rank[root_i] > rank[root_j]) {
                    parent[root_j] = root_i;
                }
                else {
                    parent[root_j] = root_i;
                    rank[root_i]++;
                }
                return true; // udało się połaczyć
            }
            return false; // nie udało się połączyć (cykl)
        }
    };

	// Quick sort do sortowania krawędzi według wagi
    static int partition(Tablica<Edge>& arr, int low, int high) {
        int pivot = arr[high].weight;
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (arr[j].weight < pivot) {
                i++;
				std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i+1], arr[high]);
        return (i + 1);
    }

    static void quickSort(Tablica<Edge>& arr, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(arr, low, high);
            quickSort(arr, low, pivotIndex - 1);
            quickSort(arr, pivotIndex + 1, high);
        }
    }

public:
    static void run(Graph* graph, std::ostream& out = std::cout) {
		int V = graph->getVerticesCount(); // Pobiera liczbę wierzchołków w grafie
        if (V == 0) return;

		Tablica<Edge> allEdges; // Tablica do przechowywania wszystkich krawędzi grafu
        for (int i = 0; i < V; ++i) {
			Tablica<Edge> neighbors = graph->getNeighbors(i); // Pobiera sąsiadów wierzchołka i 
            for (size_t j = 0; j < neighbors.getSize(); ++j) {
				if (i < neighbors[j].dest) { // Dodajemy krawędź tylko raz (i < dest) dla grafu nieskierowanego
                    allEdges.push_back(neighbors[j]);
                }
            }
        }

		// Sortowanie krawędzi według wagi za pomocą quicksorta
        if (allEdges.getSize() > 0) {
            quickSort(allEdges, 0, allEdges.getSize() - 1);
        }

		UnionFind uf(V); // Inicjalizacja struktury Union-Find dla V wierzchołków

		int mstWeight = 0; // Zmienna do przechowywania całkowitej wagi MST
		int edgesInMST = 0; // Licznik krawędzi dodanych do MST

        if (Parameters::runMode == Parameters::RunModes::singleFile)
			out << "Krawedzie wchodzace w sklad MST:\n";

        for (size_t i = 0; i < allEdges.getSize(); ++i) {
			int u = allEdges[i].src; // Pobiera wierzchołek startowy krawędzi
			int v = allEdges[i].dest; // Pobiera wierzchołek końcowy krawędzi
			int weight = allEdges[i].weight; // Pobiera wagę krawędzi

            if (uf.unionSets(u, v)) {
                mstWeight += weight;
                edgesInMST++;

                if (Parameters::runMode == Parameters::RunModes::singleFile)
					out << "Dodano krawedz: " << u << " - " << v << " (waga: " << weight << ")\n";

                if (edgesInMST == V - 1) {
					break; // kooniec jeżeli mamy już V-1 krawędzi w MST
                }
            }
        }

        if (edgesInMST != V - 1)
            std::cout << "Ostrzezenie: Nie udalo sie zbudowac MST. Graf prawdopodobnie nie jest spojny!\n";
        else if (Parameters::runMode == Parameters::RunModes::singleFile)
            out << "Calkowity koszt MST: " << mstWeight << "\n";
    }
};