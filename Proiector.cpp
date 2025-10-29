#include "Proiector.h"

Proiector::Proiector(Point p) : pozitieFixa(p) {
    this->bilaCurenta = genereazaBilaNoua();
    this->bilaUrmatoare = genereazaBilaNoua();
}

Bila Proiector::genereazaBilaNoua() {
    // Folosim noul generator de numere din utils.h
    Culoare c = static_cast<Culoare>(generareNumarRandom(0, 2));
    return Bila(c, this->pozitieFixa);
}

void Proiector::incarcaBilaUrmatoare()
{
    this->bilaCurenta = this->bilaUrmatoare;
    this->bilaUrmatoare = genereazaBilaNoua();
}

Bila Proiector::trageBila() {
    Bila bilaTrasa = this->bilaCurenta;
    incarcaBilaUrmatoare();
    return bilaTrasa;
}

std::ostream& operator<<(std::ostream& os, const Proiector& p) {
    os << "[Proiector: Gata cu " << p.bilaCurenta
       << " | Urmeaza " << p.bilaUrmatoare << "]";
    return os;
}