#pragma once
#include "Tablica.hpp"

template <typename T>
class MinHeap {
private:
    Tablica<T> heap;

	int parent(int i) { return (i - 1) / 2; } // rodzic
	int leftChild(int i) { return 2 * i + 1; } // lewe dziecko
	int rightChild(int i) { return 2 * i + 2; } // prawe dziecko

	// Funkcja pomocnicza do zamiany dwóch elementów w kopcu
    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }

	// Funkcja do utrzymania własności kopca po dodaniu nowego elementu
    void heapifyUp(int i) {
        while (i != 0 && heap[parent(i)] > heap[i]) {
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

        if (left < n && heap[left] < heap[smallest])
			smallest = left; // Jeśli lewe dziecko jest mniejsze niż bieżący najmniejszy, aktualizuj indeks najmniejszego

        if (right < n && heap[right] < heap[smallest])
			smallest = right; // Jeśli prawe dziecko jest mniejsze niż bieżący najmniejszy, aktualizuj indeks najmniejszego

		if (smallest != i) { // Jeśli najmniejszy element nie jest bieżącym, zamień i rekurencyjnie utrzymaj własność kopca
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
	// Dodaje krawędź do kopca i utrzymuje jego własność
    void push(const T& element) {
        heap.push_back(element);
        int index = heap.getSize() - 1;
        heapifyUp(index);
    }

	T pop() {
        if (heap.getSize() == 0) return T();

		if (heap.getSize() == 1) { // Jeśli jest tylko jeden element, usuń go i zwróć
            T root = heap[0];
            heap.pop_back();
			return root; 
        }

		T root = heap[0]; // Zapisuje korzeń (najmniejszy element) do zwrócenia
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