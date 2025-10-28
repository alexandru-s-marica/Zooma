#pragma once
#include "Proiector.h"
#include "SirDeBile.h"

class Nivel {
private:
    Proiector proiector;
    SirDeBile sirBile;
    int scor;
    float lungimeTraseu;

public:
    Nivel(Point pozProiector, float lungime);
    void proceseazaComanda(const std::string& comanda, float valoare);
    int getScor() const;
    bool isTerminat() const;
    SirDeBile& getSirDeBile();
    void adaugaBileStart();

    friend std::ostream& operator<<(std::ostream& os, const Nivel& n);
};