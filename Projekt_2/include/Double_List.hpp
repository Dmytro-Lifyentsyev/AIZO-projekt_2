#pragma once

// implementacja struktury - Lista dwukierunkowa
template <typename T>
class Double_List {
private:
	struct Node {
		T data;
		Node* next;	// wskaznik na nastepny i poprzedni wezel
		Node* prev;

		Node(const T& value) {
			data = value;	// inicjalizacja danych wezla
			next = nullptr; //inicjalizacja wskaznikow next i prev na nullptr
			prev = nullptr;
		}
	};
	Node* head; //wskaznik na pierwszy wezel listy
	Node* tail; //wskaznik na ostatni wezel listy
	size_t size; //aktualna liczba elementow w liscie

public:
	Double_List() { // konstruktor
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	~Double_List() // Destruktor do zwalniania pamięci 
	{
		clear();
	}

	void clear() {
		Node* current = head;
		while (current != nullptr) {
			Node* nextNode = current->next;
			delete current;
			current = nextNode;
		}
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	// Konstruktor kopiujacy
	Double_List(const Double_List& other) : head(nullptr), tail(nullptr), size(0) {
		Node* current = other.head;
		while (current != nullptr) {
			push_back(current->data);
			current = current->next;
		}
	}

	// Operator przypisania
	Double_List& operator=(const Double_List& other) {
		if (this != &other) {
			clear();
			Node* current = other.head;
			while (current != nullptr) {
				push_back(current->data);
				current = current->next;
			}
		}
		return *this;
	}


	// Funkcja do dodawania elementow na koniec listy
	void push_back(const T& value) {
		Node* newNode = new Node(value);
		if (head == nullptr)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			tail->next = newNode; //ustawia wskaznik next ostatniego wezla na nowy wezel
			newNode->prev = tail; //ustawia wskaznik prev nowego wezla na aktualny tail
			tail = newNode; //aktualizuje wskaznik tail na nowy wezel
		}
		size++;
	}

	size_t getSize() const {
		return size;
	}
	// Operator [] dla Listy Dwukierunkowej - optymalizacja poprzez wybor kierunku przeszukiwania
	T& operator[](size_t index) {
		// Sprawdzamy, czy szybciej bedzie isc od poczatku, czy od konca
		if (index < size / 2) {
			Node* current = head; // zaczynamy od poczatku
			for (size_t i = 0; i < index; ++i) {
				current = current->next; //przechodzi do nastepnego wezla
			}
			return current->data; //zwraca dane z aktualnego wezla

		}
		else {
			Node* current = tail; //	zaczynamy od konca
			for (size_t i = size - 1; i > index; --i) {
				current = current->prev; //przechodzi do poprzedniego wezla
			}
			return current->data;
		}
	}

	// Definicja klasy Iterator dla Listy Dwukierunkowej
	class Iterator {
		Node* current; // wskaznik na aktualny wezel podczas iteracji
	public:
		Iterator(Node* node) : current(node) {} // konstruktor inicjujacy iterator na danym wezle
		bool operator!=(const Iterator& other) const // operator nierownosci - sprawdza, czy dwa iteratory wskazuja na rozne wezly
		{
			return current != other.current; 
		}
		Iterator& operator++() // operator inkrementacji - przesuwa iterator do nastepnego wezla
		{
			current = current->next; return *this; 
		}
		T& operator*() // zwraca dane z aktualnego wezla, na ktory wskazuje iterator
		{
			return current->data; 
		}
	};

	Iterator begin() const { return Iterator(head); } // zwraca iterator wskazujacy na pierwszy wezel listy
	Iterator end() const { return Iterator(nullptr); } // zwraca iterator wskazujacy na koniec listy (nullptr)
};