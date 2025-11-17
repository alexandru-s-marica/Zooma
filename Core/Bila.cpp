#include "Bila.h"

Bila::Bila(Culoare c, Vec2f pos, float r)
    : culoare(c), pozitie(pos), raza(r) {}

void Bila::setPozitie(Vec2f pos) {
    pozitie = pos;
}

Vec2f Bila::getPozitie() const {
    return pozitie;
}

Culoare Bila::getCuloare() const {
    return culoare;
}

float Bila::getRaza() const {
    return raza;
}

Bounds Bila::getBounds() const {
    return {pozitie, raza};
}

std::ostream& operator<<(std::ostream& os, const Bila& b) {
    os << "Bila (Culoare: " << b.culoare
       << ", Pozitie: " << b.pozitie
       << ", Raza: " << b.raza << ")";
    return os;
}