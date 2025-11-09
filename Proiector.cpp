#include "Proiector.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Proiector::Proiector(sf::Vector2f pos)
    : pozitie(pos), unghi(0.f),
      bilaCurenta(genereazaCuloareRandom(), pos),
      bilaUrmatoare(genereazaCuloareRandom(), {pos.x, pos.y + 50})
{
    // Aici ai încărca textura pentru sprite
}

void Proiector::rotesteSpre(sf::Vector2f mousePos) {
    float dx = mousePos.x - pozitie.x;
    float dy = mousePos.y - pozitie.y;
    unghi = std::atan2(dy, dx);

    // sprite.setRotation(unghi * 180.f / M_PI);
    bilaCurenta.setPozitie(pozitie);
}

Bila Proiector::trage() {
    Bila proiectil = bilaCurenta;

    bilaCurenta = bilaUrmatoare;
    bilaCurenta.setPozitie(pozitie);

    bilaUrmatoare = Bila(genereazaCuloareRandom(), {pozitie.x, pozitie.y + 50});

    return proiectil;
}

void Proiector::schimbaBila() {
    std::swap(bilaCurenta, bilaUrmatoare);
    bilaCurenta.setPozitie(pozitie);
    bilaUrmatoare.setPozitie({pozitie.x, pozitie.y + 50});
}

void Proiector::deseneaza(sf::RenderWindow& window) const {
    // window.draw(sprite);
    bilaCurenta.deseneaza(window);
    bilaUrmatoare.deseneaza(window);
}

sf::Vector2f Proiector::getPozitie() const {
    return pozitie;
}

Culoare Proiector::genereazaCuloareRandom() {
    int r = rand() % 5;
    return static_cast<Culoare>(r);
}

std::ostream& operator<<(std::ostream& os, const Proiector& p) {
    os << "Proiector la " << p.pozitie.x << "," << p.pozitie.y << "\n";
    os << "\tBila curenta: " << p.bilaCurenta << "\n";
    os << "\tBila urmatoare: " << p.bilaUrmatoare;
    return os;
}