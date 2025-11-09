#include "Bila.h"

Bila::Bila(Culoare c, sf::Vector2f pos)
    : culoare(c), pozitie(pos) {
    forma.setRadius(RAZA_BILA);
    forma.setFillColor(getSfmlColor(c));
    // CORECTIE: setOrigin ia acum un sf::Vector2f
    forma.setOrigin({RAZA_BILA, RAZA_BILA});
    forma.setPosition(pozitie);
}

void Bila::deseneaza(sf::RenderWindow& window) const {
    window.draw(forma);
}

void Bila::setPozitie(sf::Vector2f pos) {
    this->pozitie = pos;
    forma.setPosition(pos);
}

sf::Vector2f Bila::getPozitie() const {
    return pozitie;
}

Culoare Bila::getCuloare() const {
    return culoare;
}

sf::FloatRect Bila::getBounds() const {
    return forma.getGlobalBounds();
}

std::ostream& operator<<(std::ostream& os, const Bila& b) {
    os << "Bila(Culoare: " << static_cast<int>(b.culoare)
       << ", Poz: " << b.pozitie.x << "," << b.pozitie.y << ")";
    return os;
}