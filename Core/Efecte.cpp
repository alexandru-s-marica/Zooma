#include "Efecte.h"
#include "Nivel.h"
#include <iostream>

void EfectBila::activeazaEfect(Nivel& nivel) {
    doAplica(nivel);
}

void EfectExplozie::doAplica(Nivel& nivel) {
    (void)nivel;
}

std::unique_ptr<EfectBila> EfectExplozie::clone() const {
    return std::make_unique<EfectExplozie>(*this);
}

void EfectRetro::doAplica(Nivel& nivel) {
    nivel.activeazaRetro();
}

std::unique_ptr<EfectBila> EfectRetro::clone() const {
    return std::make_unique<EfectRetro>(*this);
}

void EfectAccuracy::doAplica(Nivel& nivel) {
    nivel.activeazaModAccuracy(10.0f);
}

std::unique_ptr<EfectBila> EfectAccuracy::clone() const {
    return std::make_unique<EfectAccuracy>(*this);
}