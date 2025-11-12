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

// cppcheck-suppress unusedFunction
float Bila::getRaza() const {
    return raza;
}

Bounds Bila::getBounds() const {
    return {pozitie, raza};
}

std::ostream& Bila::operator<<(std::ostream& os) const {
    os << "Bila (Culoare: " << culoare << ", Pozitie: " << pozitie << ", Raza: " << raza << ")";
    return os;
}