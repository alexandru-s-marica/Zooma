#include "Efecte.h"
#include "Nivel.h"
#include "Bila.h" // Necesar pentru sursa.getPozitie()
#include <iostream>

void EfectBila::activeazaEfect(Nivel& nivel, Bila& sursa) {
    doAplica(nivel, sursa);
}

void EfectExplozie::doAplica(Nivel& nivel, Bila& sursa) {
    float razaExplozie = 250.0f;
    std::cout << "[EfectExplozie] BOOM la " << sursa.getPozitie() << " cu raza " << razaExplozie << "\n";
    nivel.activeazaExplozieLa(sursa.getPozitie(), razaExplozie);
}

std::unique_ptr<EfectBila> EfectExplozie::clone() const {
    return std::make_unique<EfectExplozie>(*this);
}

void EfectRetro::doAplica(Nivel& nivel, Bila& sursa) {
    (void)sursa;
    float distanta = 300.0f;
    std::cout << "[EfectRetro] Timpul se intoarce cu " << distanta << " unitati.\n";
    nivel.activeazaRetro(distanta);
}

std::unique_ptr<EfectBila> EfectRetro::clone() const {
    return std::make_unique<EfectRetro>(*this);
}

void EfectAccuracy::doAplica(Nivel& nivel, Bila& sursa) {
    (void)sursa;
    float durata = 10.0f;
    std::cout << "[EfectAccuracy] Precizie activata timp de " << durata << " secunde.\n";
    nivel.activeazaModAccuracy(durata);
}

std::unique_ptr<EfectBila> EfectAccuracy::clone() const {
    return std::make_unique<EfectAccuracy>(*this);
}

void EfectBonusScor::doAplica(Nivel& nivel, Bila& sursa) {
    (void)sursa;
    int puncte = 500;
    std::cout << "[EfectBonusScor] Bonus! +" << puncte << " puncte.\n";
    nivel.adaugaScor(puncte);
}

std::unique_ptr<EfectBila> EfectBonusScor::clone() const {
    return std::make_unique<EfectBonusScor>(*this);
}