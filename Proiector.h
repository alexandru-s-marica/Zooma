#pragma once
#include "Bila.h"

class Proiector {
private:
    sf::Vector2f pozitie;
    // sf::Sprite sprite;
    // sf::Texture textura;
    float unghi;

    Bila bilaCurenta;
    Bila bilaUrmatoare;

    Culoare genereazaCuloareRandom();

public:
    Proiector(sf::Vector2f pos);

    void rotesteSpre(sf::Vector2f mousePos);
    Bila trage();
    void schimbaBila();

    void deseneaza(sf::RenderWindow& window) const;
    sf::Vector2f getPozitie() const;

    friend std::ostream& operator<<(std::ostream& os, const Proiector& p);
};