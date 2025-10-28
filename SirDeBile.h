#pragma once
#include "Bila.h"
#include <list>

class SirDeBile {
private:
    std::list<Bila> bile;
    int* idSir;

public:
    SirDeBile();

    ~SirDeBile();
    SirDeBile(const SirDeBile& other);
    SirDeBile& operator=(const SirDeBile& other);

    void adaugaBilaInitiala(const Bila& b);

    void avanseazaBilele(float distanta);
    int insereazaBila(Bila bilaTrasa, float distantaLovire);

    bool eGol() const;
    bool aAjunsLaFinal(float distantaMaxima) const;

    friend std::ostream& operator<<(std::ostream& os, const SirDeBile& s);
};