#pragma once
// implementacja struktury - Tablica
template <typename T>
class Tablica {
private:
    T* data;  //wskaznik na dynamiczną tablicę       
    size_t capacity; //aktualna pojemność tablicy
    size_t size;    //aktualna liczba elementów w tablicy

    void resize() { //funkcja do zmiany rozmiaru tablicy
        T* newData = new T[capacity * 2];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = capacity * 2;
    }

public:
    Tablica(size_t initialCapacity = 1) { //konstruktor z domyślną pojemnością
        size = 0;
        capacity = initialCapacity;
        data = new T[capacity];
    }
    // Destruktor do zwalniania pamięci
    ~Tablica() {
        delete[] data;
    }

    // Konstruktor kopiujący (Głęboka kopia)
    Tablica(const Tablica& other) : capacity(other.capacity), size(other.size) {
		data = new T[capacity]; // Alokuje nową pamięć dla tablicy
        for (size_t i = 0; i < size; i++) {
			data[i] = other.data[i]; // Kopiuje elementy z innej tablicy
        }
    }

    // Operator przypisania 
    Tablica& operator=(const Tablica& other) { 
		if (this != &other) { // Sprawdza, czy nie przypisuje do samego siebie
			delete[] data; // Zwalnia obecną pamięć
			capacity = other.capacity; // Kopiuje pojemność i rozmiar z innej tablicy
            size = other.size;
			data = new T[capacity]; // Alokuje nową pamięć
            for (size_t i = 0; i < size; i++) {
				data[i] = other.data[i]; // Kopiuje elementy z innej tablicy
            }
        }
        return *this;
    }
    
    // Funkcja do dodawania elementów na koniec tablicy
    void push_back(const T& value) {
        if (size == capacity)
            resize();
        data[size] = value;
        size++;
    }

    // Funkcja do usuwania elementu z konca tablicy 
    void pop_back() {
        if (size > 0) {
            size--;
        }
    }

    // Funkcja do pobierania aktualnej liczby elementów w tablicy
    size_t getSize() const {
        return size;
    }

    T& operator[](size_t index) { //operator indeksowania do odczytu i zapisu
        return data[index];
    }

	// Operator indeksowania do odczytu (const)
    const T& operator[](size_t index) const
    {
	    return data[index];
    }
};