#include "Proiector.h"
#include "Efecte.h"
#include <ctime>
#include <iostream>
#include <algorithm>

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

void Proiector::genereazaBilaNoua(const std::vector<Culoare>& culoriPermise) {
    Culoare c;
    if (culoriPermise.empty()) {
        c = static_cast<Culoare>(distributieCuloare(generator));
    } else {
        std::uniform_int_distribution<int> dist(0, culoriPermise.size() - 1);
        int index = dist(generator);
        c = culoriPermise[index];
    }

    if (bilaCurenta.getCuloare() == Culoare::UNKNOWN) {
        bilaCurenta = Bila(c, pozitie, 20.f);
    } else {
        bilaUrmatoare = Bila(c, pozitie, 20.f);
    }
}

Bila Proiector::trage(const std::vector<Culoare>& culoriPermise) {
    Bila bilaTrasa = bilaCurenta;
    bilaCurenta = bilaUrmatoare;
    genereazaBilaNoua(culoriPermise);
    return bilaTrasa;
}

void Proiector::valideazaCulori(const std::vector<Culoare>& culoriPermise) {
    if (culoriPermise.empty()) return;

    bool curentaValida = false;
    for (Culoare c : culoriPermise) {
        if (bilaCurenta.getCuloare() == c) { curentaValida = true; break; }
    }
    if (!curentaValida) {
        genereazaBilaNoua(culoriPermise);
        bilaCurenta = bilaUrmatoare;
        genereazaBilaNoua(culoriPermise);
    }

    bool urmatoareaValida = false;
    for (Culoare c : culoriPermise) {
        if (bilaUrmatoare.getCuloare() == c) { urmatoareaValida = true; break; }
    }
    if (!urmatoareaValida) {
        Bila temp = bilaCurenta;
        genereazaBilaNoua(culoriPermise);
        bilaCurenta = temp;
    }
}

void Proiector::schimbaBila() {
    std::swap(bilaCurenta, bilaUrmatoare);
}

void Proiector::rotesteSpre(Vec2f tinta) { (void)tinta; }
Vec2f Proiector::getPozitie() const { return pozitie; }
const Bila& Proiector::getBilaCurenta() const { return bilaCurenta; }
const Bila& Proiector::getBilaUrmatoare() const { return bilaUrmatoare; }

std::ostream& operator<<(std::ostream& os, const Proiector& p){
    os << "Proiector (Pozitie: " << p.pozitie << ", Bila Curenta: " << p.bilaCurenta.getCuloare() << ", Bila Urmatoare: " << p.bilaUrmatoare.getCuloare() << ")";
    return os;
}