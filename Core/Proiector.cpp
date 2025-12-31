#include "Proiector.h"
#include <cmath>
#include <random>
#include <ctime>

Proiector::Proiector(Vec2f pos)
    : pozitie(pos), unghi(0.f),
      bilaCurenta(Culoare::ROSU, pos, 20.f),
      bilaUrmatoare(Culoare::ALBASTRU, pos, 20.f)
{
    genereazaBilaUrmatoare({});
    bilaCurenta = bilaUrmatoare;
    genereazaBilaUrmatoare({});
}

void Proiector::genereazaBilaUrmatoare(const std::vector<Culoare>& culoriDisponibile) {
    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));

    int tipCuloare;
    if (culoriDisponibile.empty()) {
        std::uniform_int_distribution<int> dist(0, 4);
        tipCuloare = dist(generator);
    } else {
        std::uniform_int_distribution<size_t> dist(0, culoriDisponibile.size() - 1);
        tipCuloare = static_cast<int>(culoriDisponibile[dist(generator)]);
    }

    bilaUrmatoare = Bila(static_cast<Culoare>(tipCuloare), pozitie, 20.f);

    std::uniform_int_distribution<int> distEfect(0, 100);
    if (distEfect(generator) < 5) {
    }
}

void Proiector::rotesteSpre(Vec2f tinta) {
    float dx = tinta.x - pozitie.x;
    float dy = tinta.y - pozitie.y;
    unghi = std::atan2(dy, dx);
}

Bila Proiector::trage(const std::vector<Culoare>& culoriDisponibile) {
    Bila proiectil = bilaCurenta;
    proiectil.setPozitie(pozitie);

    bilaCurenta = bilaUrmatoare;
    genereazaBilaUrmatoare(culoriDisponibile);

    return proiectil;
}

void Proiector::schimbaBila() {
    using std::swap;
    swap(bilaCurenta, bilaUrmatoare);
    bilaCurenta.setPozitie(pozitie);
}

void Proiector::valideazaCulori(const std::vector<Culoare>& culoriActive) {
    bool curentaOk = false;
    bool urmatoareOk = false;

    for (Culoare c : culoriActive) {
        if (bilaCurenta.getCuloare() == c) curentaOk = true;
        if (bilaUrmatoare.getCuloare() == c) urmatoareOk = true;
    }

    if (!curentaOk && !culoriActive.empty()) {
        bilaCurenta = Bila(culoriActive[0], pozitie, 20.f);
    }
    if (!urmatoareOk && !culoriActive.empty()) {
        genereazaBilaUrmatoare(culoriActive);
    }
}

void Proiector::setPozitie(Vec2f pos) {
    pozitie = pos;
    bilaCurenta.setPozitie(pos);
}

Vec2f Proiector::getPozitie() const { return pozitie; }
const Bila& Proiector::getBilaCurenta() const { return bilaCurenta; }
const Bila& Proiector::getBilaUrmatoare() const { return bilaUrmatoare; }