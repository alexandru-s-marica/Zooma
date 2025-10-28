#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

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

inline std::string culoareToString(Culoare c)
{
    switch (c) {
        case Culoare::ROSU: return "Rosu";
        case Culoare::VERDE: return "Verde";
        case Culoare::ALBASTRU: return "Albastru";
        default: return "N/A";
    }
}