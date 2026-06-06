#pragma once

// Abstrakcyjna klasa bazowa dla grafu, definiujaca interfejs i podstawowe dane.
class Graph {
protected: 
	int verticesCount; // Liczba wierzcholkow
	int edgesCount; // Liczba krawedzi
	bool isDirected; // Czy graf jest skierowany (true) czy nieskierowany (false)

public:
    // Konstruktor inicjujacy podstawowe dane
    Graph(int v, int e, bool directed)
        : verticesCount(v), edgesCount(e), isDirected(directed) {
    }

	virtual ~Graph() = default; // Wirtualny destruktor, zeby zapewnic poprawne czyszczenie pamieci przy usuwaniu przez wskaznik do klasy bazowej

	virtual void addEdge(int startNode, int endNode, int weight) = 0; // Czysto wirtualna metoda do dodawania krawedzi, musi byc zaimplementowana w klasach pochodnych
	virtual void display() const = 0; // Czysto wirtualna metoda do wyswietlania grafu, musi byc zaimplementowana w klasach pochodnych

    int getVerticesCount() const { return verticesCount; }
    int getEdgesCount() const { return edgesCount; }
    bool getIsDirected() const { return isDirected; }
};