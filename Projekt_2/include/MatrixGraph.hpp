#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"
#include <iostream>

class MatrixGraph : public Graph {
private:
    Tablica<Tablica<int>> matrix; // Macierz V (wiersze) x E (kolumny)
    int currentEdgeIndex;         // Indeks aktualnie dodawanej krawedzi (kolumny)

public:
	MatrixGraph(int v, int e, bool directed) // Konstruktor inicjujacy podstawowe dane i macierz
        : Graph(v, e, directed), currentEdgeIndex(0) {

        // Inicjalizacja macierzy samymi zerami (V wierszy, E kolumn)
        for (int i = 0; i < verticesCount; ++i) {
			Tablica<int> row(edgesCount); // Tworzymy wiersz o rozmiarze równym liczbie krawędzi, wypełniony zerami
            for (int j = 0; j < edgesCount; ++j) {
                row.push_back(0); 
            }
			matrix.push_back(row); // Dodajemy wiersz do macierzy
        }
    }

    void addEdge(int startNode, int endNode, int weight) override {
		if (currentEdgeIndex >= edgesCount) return; // Zabezpieczenie przed dodaniem zbyt wielu krawędzi

        if (isDirected) {
            matrix[startNode][currentEdgeIndex] = weight;  // Wychodzi z wierzcholka startowego
            matrix[endNode][currentEdgeIndex] = -weight;   // Wchodzi do koncowego
        }
        else {
            // W grafie nieskierowanym traktujemy oba końce równo
            matrix[startNode][currentEdgeIndex] = weight;
            matrix[endNode][currentEdgeIndex] = weight;
        }

        currentEdgeIndex++; // Przechodzimy do nastepnej kolumny (krawędzi)
    }

	void display() const override { // Wyświetlanie macierzy incydencji
        for (int i = 0; i < verticesCount; ++i) {
            for (int j = 0; j < edgesCount; ++j) {
                std::cout << matrix[i][j] << "\t";
            }
            std::cout << "\n";
        }
    }

	// Implementacja metody getNeighbors, ktora zwraca tablice sasiadow dla danego wierzcholka w formacie Edge (z klasy bazowej)
    Tablica<Edge> getNeighbors(int vertex) const override {
		Tablica<Edge> neighbors; // Tablica do przechowywania sasiadow w formacie Edge (z klasy bazowej)
        if (vertex < 0 || vertex >= verticesCount) return neighbors;

		for (int c = 0; c < edgesCount; ++c) { // iterujemy po kolumnach (krawędziach) dla danego wierzcholka (wiersza)
            bool isOutgoing = (matrix[vertex][c] > 0); 

			if (isOutgoing) { // jeśli jest krawędź wychodząca z tego wierzchołka 
				for (int r = 0; r < verticesCount; ++r) { // szukamy w tej kolumnie drugiego wierzchołka, który jest połączony tą krawędzią
                    if (r != vertex && matrix[r][c] != 0) {
						neighbors.push_back(Edge(vertex, r, matrix[vertex][c])); // Dodajemy sasiada do tablicy sasiadow w formacie Edge
                        break;
                    }
                }
            }
        }
		return neighbors; // Zwracamy tablice sasiadow dla danego wierzcholka
    }
};