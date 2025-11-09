#pragma once
#include "Bila.h"
#include <list>
#include <vector>

class SirDeBile {
private:
    std::list<Bila> bile;
    std::vector<sf::Vector2f> traseu;
    float viteza;
    float progresCapSnake;

    sf::Vector2f getPozitiePeTraseu(float progres) const;

public:
    SirDeBile();
    SirDeBile(std::vector<sf::Vector2f> traseu, float viteza);

    // Cerința #3
    SirDeBile(const SirDeBile& other);
    SirDeBile& operator=(const SirDeBile& other);
    ~SirDeBile();

    // Cerința #6
    void actualizeaza(float deltaTime);
    int insereazaSiVerifica(std::list<Bila>::iterator it, const Bila& bilaNoua);
    void adaugaBilaLaCoada(Culoare c);

    void deseneaza(sf::RenderWindow& window) const;
    std::list<Bila>& getBile();

    friend std::ostream& operator<<(std::ostream& os, const SirDeBile& s);
};