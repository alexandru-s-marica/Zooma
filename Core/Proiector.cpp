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

Bila Proiector::trage() {
    Bila bilaTrasa = bilaCurenta;
    bilaCurenta = bilaUrmatoare;
    genereazaBilaNoua();
    return bilaTrasa;
}

void Proiector::schimbaBila() {
    std::swap(bilaCurenta, bilaUrmatoare);
}

void Proiector::rotesteSpre(Vec2f tinta) {
    (void)tinta; // Suprimă avertismentul de parametru nefolosit
}

Vec2f Proiector::getPozitie() const {
    return pozitie;
}

const Bila& Proiector::getBilaCurenta() const {
    return bilaCurenta;
}

// --- ADAUGĂ FUNCȚIA ASTA ---
const Bila& Proiector::getBilaUrmatoare() const {
    return bilaUrmatoare;
}
// --- SFÂRȘIT ADAUGARE ---

std::ostream& Proiector::operator<<(std::ostream& os) const {
    os << "Proiector (Pozitie: " << pozitie << ", Bila Curenta: " << bilaCurenta.getCuloare() << ", Bila Urmatoare: " << bilaUrmatoare.getCuloare() << ")";
    return os;
}