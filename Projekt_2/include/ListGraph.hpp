#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"
#include "Double_List.hpp"
#include <iostream>

class ListGraph : public Graph {
private:
    struct Edge { // Pomocnicza struktura reprezentujaca krawedz wychodzaca z danego wierzcholka
		int destination; // Wierzcholek docelowy (sasiad)
		int weight; // Waga krawedzi 

		Edge(int dest = 0, int w = 0) : destination(dest), weight(w) {} // Konstruktor
    };

    Tablica<Double_List<Edge>> adjList; // Indeks tablicy to wierzcholek startowy, a na liscie sa jego sasiedzi.

public:
	ListGraph(int v, int e, bool directed) // Konstruktor inicjujacy podstawowe dane
        : Graph(v, e, directed) {

		for (int i = 0; i < verticesCount; ++i) { // Inicjalizacja tablicy listami sasiedztwa dla kazdego wierzcholka
			Double_List<Edge> emptyList; // Tworzymy pusta liste sasiedztwa dla kazdego wierzcholka
			adjList.push_back(emptyList); // Dodajemy pusta liste do tablicy sasiedztwa
        }
    }

    void addEdge(int startNode, int endNode, int weight) override {
		if (startNode >= verticesCount || endNode >= verticesCount) return; // Zabezpieczenie przed dodaniem krawedzi z nieistniejacymi wierzcholkami

        adjList[startNode].push_back(Edge(endNode, weight)); // Dodaje krawedz wychodzaca ze startNode do endNode

		if (!isDirected) { // W grafie nieskierowanym dodajemy krawedz w obie strony
            adjList[endNode].push_back(Edge(startNode, weight));
        }
    }

    void display() const override {
        for (int i = 0; i < verticesCount; ++i) {
            std::cout << "V" << i << " -> ";

            for (auto it = adjList[i].begin(); it != adjList[i].end(); ++it) {
                Edge e = *it;
                std::cout << "[V" << e.destination << " | " << e.weight << "]  ";
            }
            std::cout << "\n";
        }
    }
};