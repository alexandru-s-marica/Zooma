#include "Bila.h"

Bila::Bila(Culoare c, Vec2f pos, float r)
    : culoare(c), pozitie(pos), raza(r), seDistruge(false), timpPanaLaMoarte(0.0f) {}

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

void Bila::marcheazaPentruDistrugere() {
    seDistruge = true;
    timpPanaLaMoarte = 0.3f;
}

bool Bila::esteInDistrugere() const {
    return seDistruge;
}

void Bila::actualizeazaTimer(float deltaTime) {
    if (seDistruge) {
        timpPanaLaMoarte -= deltaTime;
    }
}

bool Bila::eGataDeSters() const {
    return seDistruge && (timpPanaLaMoarte <= 0.f);
}