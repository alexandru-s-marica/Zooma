#include "Proiector.h"

Proiector::Proiector(Point p) : pozitieFixa(p) {
    this->bilaCurenta = genereazaBilaNoua();
    this->bilaUrmatoare = genereazaBilaNoua();
}

Bila Proiector::genereazaBilaNoua() {
    Culoare c = static_cast<Culoare>(rand() % 3);
    return Bila(c, this->pozitieFixa);
}

void Proiector::incarcaBilaUrmatoare()
{
    this->bilaCurenta = this->bilaUrmatoare;
    this->bilaUrmatoare = genereazaBilaNoua();
    this->bilaCurenta.setPozitie(this->pozitieFixa);
}

Bila Proiector::trageBila() {
    Bila bilaTrasa = this->bilaCurenta;
    incarcaBilaUrmatoare();
    return bilaTrasa;
}

const Bila& Proiector::getBilaCurenta() const {
    return bilaCurenta;
}

std::ostream& operator<<(std::ostream& os, const Proiector& p) {
    os << "[Proiector: Gata cu " << p.bilaCurenta
       << " | Urmeaza " << p.bilaUrmatoare << "]";
    return os;
}