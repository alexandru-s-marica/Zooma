#include "EfectFactory.h"
#include <random>
#include <ctime>

std::unique_ptr<EfectBila> EfectFactory::genereazaEfectRandom() {
    static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));

    std::uniform_int_distribution<int> distTip(0, 3);

    int tip = distTip(gen);

    switch(tip) {
    case 0: return std::make_unique<EfectExplozie>();
    case 1: return std::make_unique<EfectRetro>();
    case 2: return std::make_unique<EfectAccuracy>();
    case 3: return std::make_unique<EfectBonusScor>();
    default: return nullptr;
    }
}