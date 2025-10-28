#include "SirDeBile.h"

SirDeBile::SirDeBile() {
    idSir = new int(rand());
}

SirDeBile::~SirDeBile() {
    delete idSir;
    idSir = nullptr;
}

SirDeBile::SirDeBile(const SirDeBile& other) {
    idSir = new int(*other.idSir);
    bile = other.bile;
}

SirDeBile& SirDeBile::operator=(const SirDeBile& other)
{
    if (this == &other) {
        return *this;
    }
    delete idSir;
    idSir = new int(*other.idSir);
    bile = other.bile;
    return *this;
}

void SirDeBile::adaugaBilaInitiala(const Bila& b) {
    bile.push_back(b);
}

void SirDeBile::avanseazaBilele(float distanta) {
    if (bile.empty()) return;

    for (auto& bila : bile) {
        float dNoua = bila.getDistanta() + distanta;
        bila.setDistanta(dNoua);
    }
}

int SirDeBile::insereazaBila(Bila bilaTrasa, float distantaLovire) {

    auto it = bile.begin();
    while (it != bile.end() && it->getDistanta() < distantaLovire) {
        ++it;
    }

    bilaTrasa.setDistanta(distantaLovire);
    auto itInserat = bile.insert(it, bilaTrasa);

    Culoare c = itInserat->getCuloare();
    int count = 0;

    auto itStanga = itInserat;
    while (itStanga != bile.begin() && std::prev(itStanga)->getCuloare() == c) {
        --itStanga;
        count++;
    }

    auto itDreapta = itInserat;
    while (std::next(itDreapta) != bile.end() && std::next(itDreapta)->getCuloare() == c) {
        ++itDreapta;
        count++;
    }

    count++;

    if (count >= 3) {
        std::cout << ">>> POTRIVIRE DE " << count << " " << culoareToString(c) << "!" << std::endl;
        bile.erase(itStanga, std::next(itDreapta));
        return 10 * count;
    }
    return 0;
}

bool SirDeBile::eGol() const {
    return bile.empty();
}

bool SirDeBile::aAjunsLaFinal(float distantaMaxima) const {
    if (bile.empty()) return false;
    return bile.back().getDistanta() >= distantaMaxima;
}

std::ostream& operator<<(std::ostream& os, const SirDeBile& s) {
    os << "[SirDeBile (ID: " << *s.idSir << "): \n";
    if (s.bile.empty()) {
        os << "\t(gol)\n";
    }
    for (const auto& b : s.bile) {
        os << "\t" << b << "\n";
    }
    os << "]";
    return os;
}