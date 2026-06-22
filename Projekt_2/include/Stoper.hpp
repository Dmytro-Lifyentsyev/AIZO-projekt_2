#pragma once
#include <chrono>
#include <iomanip>
#include <sstream>

// Klasa Stoper do pomiaru czasu
class Stoper {
private:
    // Zmienne przechowujace moment startu i konca
    std::chrono::time_point<std::chrono::high_resolution_clock> punkt_startu;
    std::chrono::time_point<std::chrono::high_resolution_clock> punkt_konca;

public:
    // Wlacza stoper
    void start() {
        punkt_startu = std::chrono::high_resolution_clock::now();
    }

    // Wylacza stoper
    void stop() {
        punkt_konca = std::chrono::high_resolution_clock::now();
    }

    // Zwraca czas w mikrosekundach
    double getCzasMikrosekundy() const{
        std::chrono::duration<double, std::micro> czas_trwania = punkt_konca - punkt_startu;
        return czas_trwania.count();
    }

    // Zwraca czas w milisekundach
    double getCzasMilisekundy() const {
        std::chrono::duration<double, std::milli> czas_trwania = punkt_konca - punkt_startu;
        return czas_trwania.count();
    }

    // Zwraca czas w sekundach
    double getCzasSekundy() const {
        std::chrono::duration<double> czas_trwania = punkt_konca - punkt_startu;
        return czas_trwania.count();
    }

    // Funkcja pobierająca aktualny czas w formacie "YYYY-MM-DD HH:MM:SS"
    inline string pobierzAktualnyCzas() {
        auto teraz = chrono::system_clock::now();
        time_t czas_t = chrono::system_clock::to_time_t(teraz);
        tm czas_tm;

#if defined(_WIN32)
        localtime_s(&czas_tm, &czas_t);
#else
        localtime_r(&czas_t, &czas_tm);
#endif

        stringstream ss;
        ss << put_time(&czas_tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};