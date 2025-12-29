#include "Efecte.h"
#include "Nivel.h"

void EfectExplozie::doAplica(Nivel& nivel, Vec2f pozitie) {
    nivel.activeazaExplozieLa(pozitie);
}

void EfectRetro::doAplica(Nivel& nivel, Vec2f pozitie) {
    (void)pozitie;
    nivel.activeazaRetro();
}

void EfectAccuracy::doAplica(Nivel& nivel, Vec2f pozitie) {
    (void)pozitie;
    nivel.activeazaModAccuracy(5.0f);
}