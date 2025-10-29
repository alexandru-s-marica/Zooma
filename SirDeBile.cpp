#include "SirDeBile.h"
#include <iostream>   // Adaugat pentru std::cout, std::endl
#include <iterator>   // Adaugat pentru std::prev, std::next

SirDeBile::SirDeBile() {
    // Folosim noul generator de numere
    idSir = new int(generareNumarRandom(10000, 99999));
}

SirDeBile::~SirDeBile() {
    delete idSir;
    idSir = nullptr;
}

SirDeBile::SirDeBile(const SirDeBile& other)
    : bile(other.bile)
{
    idSir = new int(*other.idSir);
}

SirDeBile& SirDeBile::operator=(const SirDeBile& other)
{
    if (this == &other) {
        return *this;
    }

    bile = other.bile;
    delete idSir; // Eliberam resursa veche
    idSir = new int(*other.idSir); // Alocam si copiem resursa noua

    return *this;
}

void SirDeBile::adaugaBilaInitiala(const Bila& b) {
    bile.push_back(b);
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

        // --- NOU: Colectam efectele ---
        std::list<TipPowerUp> efecteDeActivat;
        for (auto it = itStanga; it != std::next(itDreapta); ++it) {
            if (it->getEfect() != TipPowerUp::NIMIC) {
                efecteDeActivat.push_back(it->getEfect());
            }
        }
        // -----------------------------

        bile.erase(itStanga, std::next(itDreapta));

        // --- NOU: Activam efectele ---
        int scorAdaugat = 10 * count;
        for (auto efect : efecteDeActivat) {
            std::cout << "!!! ACTIVARE EFECT: " << powerUpToString(efect) << " !!!" << std::endl;
            scorAdaugat += activeazaEfect(efect); // Adaugam scorul bonus din efect
        }

        return scorAdaugat;
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

// Functia de avansare simpla (folosita de power-up si de Nivel)
void SirDeBile::avanseazaBilele(float distanta) {
    if (bile.empty()) return;
    for (auto& bila : bile) {
        float dNoua = bila.getDistanta() + distanta;
        if (dNoua < 0) dNoua = 0; // Prevenim sa mearga sub 0
        bila.setDistanta(dNoua);
    }
}

// Functia care aplica un efect nou pe o bila aleatorie
void SirDeBile::aplicaEfectAleatoriu(float sansa)
{
    if (bile.empty()) return;

    // sansa este intre 0.0 si 1.0 (ex. 0.1 pt 10%)
    if (generareNumarRandom(0, 100) < (sansa * 100))
    {
        // Alegem o bila aleatorie
        int index = generareNumarRandom(0, bile.size() - 1);
        auto it = bile.begin();
        std::advance(it, index);

        // Aplicam efectul doar daca bila nu are deja unul
        if (it->getEfect() == TipPowerUp::NIMIC) {
            // Genereaza 1 (BOMBA) sau 2 (INVERSARE)
            TipPowerUp tipNou = static_cast<TipPowerUp>(generareNumarRandom(1, 2));
            it->aplicaEfect(tipNou);
            std::cout << ">>> O bila a primit un efect: " << powerUpToString(tipNou) << std::endl;
        }
    }
}

// Functia private care executa logica efectului
int SirDeBile::activeazaEfect(TipPowerUp tip) {
    switch (tip)
    {
        case TipPowerUp::BOMBA:
        {
            // Distruge primele 3 bile de la capatul sirului
            int bileDistruse = 0;
            for (int i = 0; i < 3 && !bile.empty(); ++i) {
                bile.pop_front(); // Sterge bila cea mai din fata
                bileDistruse++;
            }
            std::cout << "Bomba a distrus " << bileDistruse << " bile de la capatul sirului!\n";
            return bileDistruse * 5; // Scor bonus
        }
        case TipPowerUp::INVERSARE:
        {
            // Impinge tot sirul inapoi cu 15 unitati
            avanseazaBilele(-15.0f);
            std::cout << "Sirul a fost impins inapoi cu 15 unitati!\n";
            return 20; // Scor bonus
        }
        case TipPowerUp::NIMIC:
        default:
            return 0;
    }
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