#include "Bila.h"

Bila::Bila(Culoare c, Point p, float dist)
    : culoare(c), pozitie(p), distantaPeTraseu(dist) {}

Bila::Bila() : culoare(Culoare::NIMIC), pozitie({0,0}), distantaPeTraseu(0) {}

Culoare Bila::getCuloare() const { return culoare; }
float Bila::getDistanta() const { return distantaPeTraseu; }

void Bila::setPozitie(Point p) {
    pozitie = p;
}
void Bila::setDistanta(float d) {
    distantaPeTraseu = d;
}

std::ostream& operator<<(std::ostream& os, const Bila& b) {
    os << "[Bila: " << culoareToString(b.culoare)
       << " | Poz: (" << b.pozitie.x << ", " << b.pozitie.y << ")"
       << " | Dist: " << b.distantaPeTraseu << "]";
    return os;
}