#pragma once
#include "SirDeBile.h"
#include "Proiector.h"

class Nivel {
private:
    sf::RenderWindow& window;

    SirDeBile sirBile;
    Proiector proiector;

    std::vector<std::pair<Bila, sf::Vector2f>> proiectileInZbor;

    int scor;
    std::vector<sf::Vector2f> traseu;

    void genereazaTraseu();
    void proceseazaInput();
    void gestioneazaColiziuni();

public:
    Nivel(sf::RenderWindow& win);

    void ruleazaFrame(float deltaTime);
    void deseneaza() const;
    bool esteTerminat() const;

    const SirDeBile& getSirBile() const { return sirBile; }

    friend std::ostream& operator<<(std::ostream& os, const Nivel& n);
};