#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"
#include "Double_List.hpp"
#include <iostream>

class ListGraph : public Graph {
private:
    struct ListEdge { // Pomocnicza struktura reprezentujaca krawedz wychodzaca z danego wierzcholka
		int destination; // Wierzcholek docelowy (sasiad)
		int weight; // Waga krawedzi 

		ListEdge(int dest = 0, int w = 0) : destination(dest), weight(w) {} // Konstruktor
    };

    Tablica<Double_List<ListEdge>> adjList; // Indeks tablicy to wierzcholek startowy, a na liscie sa jego sasiedzi.

public:
	ListGraph(int v, int e, bool directed) // Konstruktor inicjujacy podstawowe dane
        : Graph(v, e, directed) {

		for (int i = 0; i < verticesCount; ++i) { // Inicjalizacja tablicy listami sasiedztwa dla kazdego wierzcholka
			Double_List<ListEdge> emptyList; // Tworzymy pusta liste sasiedztwa dla kazdego wierzcholka
			adjList.push_back(emptyList); // Dodajemy pusta liste do tablicy sasiedztwa
        }
    }

    void addEdge(int startNode, int endNode, int weight) override {
		if (startNode >= verticesCount || endNode >= verticesCount) return; // Zabezpieczenie przed dodaniem krawedzi z nieistniejacymi wierzcholkami

        adjList[startNode].push_back(ListEdge(endNode, weight)); // Dodaje krawedz wychodzaca ze startNode do endNode

		if (!isDirected) { // W grafie nieskierowanym dodajemy krawedz w obie strony
            adjList[endNode].push_back(ListEdge(startNode, weight));
        }
    }

    void display(std::ostream& out = std::cout) const override {
        for (int i = 0; i < verticesCount; ++i) {
            out << "V" << i << " -> ";

            for (auto it = adjList[i].begin(); it != adjList[i].end(); ++it) {
                ListEdge e = *it;
                out << "[V" << e.destination << " | " << e.weight << "]  ";
            }
            out << "\n";
        }
    }

	// Implementacja metody getNeighbors, ktora zwraca tablice sasiadow dla danego wierzcholka w formacie Edge (z klasy bazowej)
    Tablica<Edge> getNeighbors(int vertex) const override {
		Tablica<Edge> neighbors; // Tablica do przechowywania sasiadow w formacie Edge (z klasy bazowej)
        if (vertex < 0 || vertex >= verticesCount) return neighbors;

        for (auto it = adjList[vertex].begin(); it != adjList[vertex].end(); ++it) {
			ListEdge e = *it; // wyciagamy sasiedniego wierzcholka i wage krawedzi z listy sasiedztwa
			neighbors.push_back(Edge(vertex, e.destination, e.weight)); // Dodajemy sasiada do tablicy sasiadow w formacie Edge (z klasy bazowej)
        }
		return neighbors; // Zwracamy tablice sasiadow dla danego wierzcholka
    }
};