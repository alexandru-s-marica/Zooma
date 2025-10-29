#pragma once
#include "Bila.h"
#include <list>

class SirDeBile {
private:
    std::list<Bila> bile;
    int* idSir;
    int activeazaEfect(TipPowerUp tip);

public:
    // DOAR declaratiile functiilor
    SirDeBile();
    ~SirDeBile();
    SirDeBile(const SirDeBile& other);
    SirDeBile& operator=(const SirDeBile& other);

    void adaugaBilaInitiala(const Bila& b);
    int insereazaBila(Bila bilaTrasa, float distantaLovire);
    void avanseazaBilele(float distanta);
    void aplicaEfectAleatoriu(float sansa);

    bool eGol() const;
    bool aAjunsLaFinal(float distantaMaxima) const;

    friend std::ostream& operator<<(std::ostream& os, const SirDeBile& s);
};