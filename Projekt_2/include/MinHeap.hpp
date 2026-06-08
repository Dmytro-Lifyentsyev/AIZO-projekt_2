#pragma once
#include "Graph.hpp"
#include "Tablica.hpp"

class MinHeap {
private:
    Tablica<Edge> heap;

	int parent(int i) { return (i - 1) / 2; } // rodzic
	int leftChild(int i) { return 2 * i + 1; } // lewe dziecko
	int rightChild(int i) { return 2 * i + 2; } // prawe dziecko

	// Funkcja pomocnicza do zamiany dwóch elementów w kopcu
    void swap(Edge& a, Edge& b) {
        Edge temp = a;
        a = b;
        b = temp;
    }

	// Funkcja do utrzymania własności kopca po dodaniu nowego elementu
    void heapifyUp(int i) {
        while (i != 0 && heap[parent(i)].weight > heap[i].weight) {
			swap(heap[i], heap[parent(i)]); // Zamiana z rodzicem
			i = parent(i); // Przechodzenie w górę kopca
        }
    }

	// Funkcja do utrzymania własności kopca po usunięciu elementu
    void heapifyDown(int i) {
		int smallest = i; // Inicjalizuje najmniejszy element jako bieżący
		int left = leftChild(i); // Indeks lewego dziecka
		int right = rightChild(i); // Indeks prawego dziecka
		int n = heap.getSize(); // Rozmiar kopca

        if (left < n && heap[left].weight < heap[smallest].weight)
			smallest = left; // Jeśli lewe dziecko jest mniejsze niż bieżący najmniejszy, aktualizuj indeks najmniejszego

        if (right < n && heap[right].weight < heap[smallest].weight)
			smallest = right; // Jeśli prawe dziecko jest mniejsze niż bieżący najmniejszy, aktualizuj indeks najmniejszego

		if (smallest != i) { // Jeśli najmniejszy element nie jest bieżącym, zamień i rekurencyjnie utrzymaj własność kopca
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
	// Dodaje krawędź do kopca i utrzymuje jego własność
    void push(const Edge& edge) {
        heap.push_back(edge);
        int index = heap.getSize() - 1;
        heapifyUp(index);
    }

    Edge pop() {
        if (heap.getSize() == 0) return Edge();

		if (heap.getSize() == 1) { // Jeśli jest tylko jeden element, usuń go i zwróć
            Edge root = heap[0];
            heap.pop_back();
			return root; 
        }

		Edge root = heap[0]; // Zapisuje korzeń (najmniejszy element) do zwrócenia
		heap[0] = heap[heap.getSize() - 1]; // Przenosi ostatni element na miejsce korzenia
		heap.pop_back(); // Usuwa ostatni element             
		heapifyDown(0); // Utrzymuje własność kopca od korzenia w dół
		return root; // Zwraca najmniejszy element
    }

	// Sprawdza, czy kopiec jest pusty
    bool isEmpty() const {
        return heap.getSize() == 0;
    }
};