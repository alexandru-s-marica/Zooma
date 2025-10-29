#include "Bila.h"
#include <ostream> // Adaugat pentru `ostream`

Bila::Bila(Culoare c, Point p, float dist)
    : culoare(c), pozitie(p), distantaPeTraseu(dist), efectSpecial(TipPowerUp::NIMIC) {}

Bila::Bila()
    : culoare(Culoare::NIMIC), pozitie({0,0}), distantaPeTraseu(0), efectSpecial(TipPowerUp::NIMIC) {}

Culoare Bila::getCuloare() const { return culoare; }
float Bila::getDistanta() const { return distantaPeTraseu; }

void Bila::setDistanta(float d) {
    distantaPeTraseu = d;
}

// --- FUNCTII NOI ---
TipPowerUp Bila::getEfect() const {
    return efectSpecial;
}

void Bila::aplicaEfect(TipPowerUp tip) {
    efectSpecial = tip;
}
// -------------------

std::ostream& operator<<(std::ostream& os, const Bila& b) {
    os << "[Bila: " << culoareToString(b.culoare)
       << " | Dist: " << b.distantaPeTraseu;

    // Afisam efectul daca exista
    if (b.efectSpecial != TipPowerUp::NIMIC) {
        os << " | Efect: " << powerUpToString(b.efectSpecial);
    }

    os << "]";
    return os;
}