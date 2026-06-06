#include "Reader.hpp"
int main() {
    std::string filename = "/mnt/d/Nauka/Studia_I/Algorytmy/Projekt_2/test_graph.txt";

    Graph* Graph = Reader::loadFromFile(filename, false, false);

    if (Graph != nullptr) {
        Graph->display();
        delete Graph;
    }
    return 0;
}