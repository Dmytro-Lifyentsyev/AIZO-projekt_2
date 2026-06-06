#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "MatrixGraph.hpp"
#include "ListGraph.hpp" 

class Reader {
public:
	// Metoda statyczna do wczytywania grafu z pliku
    static Graph* loadFromFile(const std::string& filename, bool isDirected, bool useMatrix) { 
		std::ifstream file(filename); // Otwieramy plik do odczytu

		if (!file.is_open()) { // Sprawdzamy, czy plik został poprawnie otwarty
            std::cerr << "Nie mozna otworzyc pliku " << filename << "!\n";
            return nullptr;
        }

		int v, e; // Zmienna do przechowywania liczby wierzcholkow i krawedzi
		if (!(file >> v >> e)) { // Wczytujemy liczbe wierzcholkow i krawedzi, sprawdzamy czy operacja sie powiodla
            std::cerr << "Niepoprawny format naglowka pliku!\n";
            return nullptr;
        }

		Graph* graph = nullptr; // Wskaznik do grafu, ktory zostanie utworzony na podstawie danych z pliku
        if (useMatrix) { 
			graph = new MatrixGraph(v, e, isDirected); // Tworzymy graf reprezentowany macierza incydencji
        }
        else {
			graph = new ListGraph(v, e, isDirected); // Tworzymy graf reprezentowany lista sasiedztwa
        }

		int startNode, endNode, weight; // Zmienne do przechowywania danych o krawedziach wczytywanych z pliku
		int loadedEdges = 0; // Licznik wczytanych krawedzi

        while (file >> startNode >> endNode >> weight) {
            if (startNode >= v || endNode >= v || startNode < 0 || endNode < 0) {
                std::cerr << "Wierzcholek poza zakresem w pliku!\n";
                delete graph;
                return nullptr;
            }
			graph->addEdge(startNode, endNode, weight); // Dodajemy krawedz do grafu na podstawie danych z pliku
			loadedEdges++; // Zwiekszamy licznik wczytanych krawedzi
        }

		if (loadedEdges != e) { // Sprawdzamy, czy liczba wczytanych krawedzi zgadza sie z zadeklarowana w naglowku pliku
            std::cerr << "Liczba wczytanych krawedzi (" << loadedEdges << ") nie zgadza sie z zadeklarowana (" << e << ")\n";
        }

        file.close();
		return graph; // Zwracamy wskaznik do utworzonego grafu, ktory zostal wczytany z pliku
    }
};