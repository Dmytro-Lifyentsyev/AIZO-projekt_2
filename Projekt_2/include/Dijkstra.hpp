#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"
#include "MinHeap.hpp"
#include "Parameters.h"
#include "Utils.hpp"
#include <iostream>
#include <climits>

class Dijkstra {
private:
    struct DijkstraNode {
        int vertex; // Numer wierzchołka, w którym aktualnie jesteśmy
        int distance; // Całkowity koszt dotarcia z punktu startowego do tego wierzchołka

		DijkstraNode(int v = -1, int d = 0) : vertex(v), distance(d) {} // Konstruktor 

        bool operator<(const DijkstraNode& other) const { return distance < other.distance; }
        bool operator>(const DijkstraNode& other) const { return distance > other.distance; }
    };

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
                std::cout << "Informacja: Nie podano wierzcholka koncowego. Zostaną wyznaczone sciezki do wszystkich wierzcholkow.\n";
            }

        }
        else {
            Start = 0; // W trybie badan (benchmark) startowy to zawsze 0
        }

        Tablica<int> dist(V); // dist[i]: najkrótszy znany dotąd czas przejazdu do wierzchołka i
        Tablica<int> prev(V); // prev[i]: z jakiego wierzchołka najszybciej przyjsć do wierzchołka i
        Tablica<bool> visited(V); // visited[i]: czy wierzchołek i został już w pełni obsłużony

        for (int i = 0; i < V; ++i) {
            dist.push_back(INT_MAX); // Na początku dystans wszędzie to nieskończoność
            prev.push_back(-1); // Brak poprzednika
            visited.push_back(false); // Żaden wierzchołek nie jest odwiedzony
        }

        dist[Start] = 0; // Punkt startowy nie kosztuje nic, dystans do siebie samego to 0
		MinHeap<DijkstraNode> pq; // Kolejka priorytetowa do wyboru następnego wierzchołka o najniższym dystansie
		pq.push(DijkstraNode(Start, 0)); // Zaczynamy od punktu startowego

        while (!pq.isEmpty()) {
			DijkstraNode current = pq.pop(); // Pobieramy wierzchołek o najniższym dystansie z kolejki
			int u = current.vertex; // Numer wierzchołka, który aktualnie przetwarzamy

			if (visited[u]) continue; // Jeśli już ten wierzchołek jest 'obsłużony' czyli true, pomijamy go
            visited[u] = true; // Oznaczamy wierzchołek jako 'obsłużony'. Oznacza to, że nie da się znaleźć tańszej trasy do 'u' niż ta.

            if (Parameters::runMode == Parameters::RunModes::singleFile && endNode != -1 && u == endNode) {
                break;
            }

			Tablica<Edge> neighbors = graph->getNeighbors(u); // Pobieramy wszystkich sąsiadów wierzchołka u
            for (size_t i = 0; i < neighbors.getSize(); ++i) {
                int v = neighbors[i].dest;
                int weight = neighbors[i].weight;
				// relaksacja: jeśli znaleziono krótszą trasę do v przez u, aktualizujemy dystans i poprzednika
                if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight; // Aktualizuje dystans na lepszy
                    prev[v] = u; // Zaznaczamy, że nowa szybka droga prowadzi przez u
                    pq.push(DijkstraNode(v, dist[v])); // Wrzucamy do kopca ten wierzchołek z nowym rekordem
                }
            }
        }

        // wyswietlanie: Działa tylko w trybie SingleFile (w badaniach nic nie piszemy na ekran)
        if (Parameters::runMode == Parameters::RunModes::singleFile) {
            if (endNode >= 0 && endNode < V) {
                // Jeśli podano -e, wypisuje tylko tę jedną trasę
                Utils::printSinglePath(Start, endNode, dist, prev);
            }
            else {
                // Jeśli nie podano -e, wypisuje wszystkie trasy ze startu
                std::cout << "Sciezki od V" << Start << " do wszystkich wierzcholkow:\n";
                for (int i = 0; i < V; ++i) {
                    if (i != Start) Utils::printSinglePath(Start, i, dist, prev);
                }
            }
        }
    }
};