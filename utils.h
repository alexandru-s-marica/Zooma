#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <random> // Adaugat pentru C++ modern random

struct Point {
    float x=0.0f;
    float y = 0.0f;
};

enum class Culoare {
    ROSU,
    VERDE,
    ALBASTRU,
    NIMIC
};

// --- ADAUGAT PENTRU EFECTE ---
enum class TipPowerUp {
    NIMIC,
    BOMBA,
    INVERSARE
};
// ----------------------------

// Functie helper pentru numere aleatoare (inlocuieste rand())
inline int generareNumarRandom(int min, int max) {
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distributie(min, max);
    return distributie(generator);
}

inline std::string culoareToString(Culoare c)
{
    switch (c) {
        case Culoare::ROSU: return "Rosu";
        case Culoare::VERDE: return "Verde";
        case Culoare::ALBASTRU: return "Albastru";
        default: return "N/A";
    }
}

// --- ADAUGAT PENTRU EFECTE ---
inline std::string powerUpToString(TipPowerUp tip)
{
    switch (tip) {
        case TipPowerUp::BOMBA: return "BOMBA";
        case TipPowerUp::INVERSARE: return "INVERSARE";
        default: return "";
    }
}
// ----------------------------