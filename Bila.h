#pragma once
#include "utils.h"

class Bila {
private:
    sf::CircleShape forma;
    Culoare culoare;
    sf::Vector2f pozitie;

public:
    Bila(Culoare c, sf::Vector2f pos);

    void deseneaza(sf::RenderWindow& window) const;
    void setPozitie(sf::Vector2f pos);
    sf::Vector2f getPozitie() const;
    Culoare getCuloare() const;
    sf::FloatRect getBounds() const;

    friend std::ostream& operator<<(std::ostream& os, const Bila& b);
};