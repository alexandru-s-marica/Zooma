#pragma once
#include <memory>
#include "Efecte.h"

class EfectFactory {
public:
    static std::unique_ptr<EfectBila> genereazaEfectRandom();
};