#include "Proiector.h"
#include <ctime>

Proiector::Proiector(Vec2f pos)
    : pozitie(pos),
      bilaCurenta(Culoare::UNKNOWN, pos, 20.f),
      bilaUrmatoare(Culoare::UNKNOWN, pos, 20.f),
      generator(static_cast<unsigned int>(time(0))),
      distributieCuloare(0, 5)
{
    genereazaBilaNoua();
    genereazaBilaNoua();
}

void Proiector::genereazaBilaNoua() {
    Culoare c = static_cast<Culoare>(distributieCuloare(generator));
    if (bilaCurenta.getCuloare() == Culoare::UNKNOWN) {
        bilaCurenta = Bila(c, pozitie, 20.f);
    } else {
        bilaUrmatoare = Bila(c, pozitie, 20.f);
    }
}

// cppcheck-suppress unusedFunction
Bila Proiector::trage() {
    Bila bilaTrasa = bilaCurenta;
    bilaCurenta = bilaUrmatoare;
    genereazaBilaNoua();
    return bilaTrasa;
}

// cppcheck-suppress unusedFunction
void Proiector::schimbaBila() {
    std::swap(bilaCurenta, bilaUrmatoare);
}

// cppcheck-suppress unusedFunction
void Proiector::rotesteSpre(Vec2f tinta) {
    (void)tinta;
}

// cppcheck-suppress unusedFunction
Vec2f Proiector::getPozitie() const {
    return pozitie;
}

// cppcheck-suppress unusedFunction
const Bila& Proiector::getBilaCurenta() const {
    return bilaCurenta;
}

// cppcheck-suppress unusedFunction
const Bila& Proiector::getBilaUrmatoare() const {
    return bilaUrmatoare;
}

std::ostream& operator<<(std::ostream& os, const Proiector& p){
    os << "Proiector (Pozitie: " << p.pozitie << ", Bila Curenta: " << p.bilaCurenta.getCuloare() << ", Bila Urmatoare: " << p.bilaUrmatoare.getCuloare() << ")";
    return os;
}