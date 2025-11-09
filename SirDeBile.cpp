#include "SirDeBile.h"

SirDeBile::SirDeBile() : viteza(0.f), progresCapSnake(0.f) {
    std::cout << "SirDeBile: Constructor default\n";
}

SirDeBile::SirDeBile(std::vector<sf::Vector2f> traseu, float viteza)
    : traseu(std::move(traseu)), viteza(viteza), progresCapSnake(0.f) {
    std::cout << "SirDeBile: Constructor cu parametri\n";
    adaugaBilaLaCoada(Culoare::ROSU);
    adaugaBilaLaCoada(Culoare::ROSU);
    adaugaBilaLaCoada(Culoare::VERDE);
    adaugaBilaLaCoada(Culoare::ALBASTRU);
    adaugaBilaLaCoada(Culoare::VERDE);
}

SirDeBile::SirDeBile(const SirDeBile& other)
    : bile(other.bile), traseu(other.traseu), viteza(other.viteza), progresCapSnake(other.progresCapSnake) {
    std::cout << "SirDeBile: Constructor de copiere\n";
}

SirDeBile& SirDeBile::operator=(const SirDeBile& other) {
    std::cout << "SirDeBile: Operator= de copiere\n";
    if (this != &other) {
        bile = other.bile;
        traseu = other.traseu;
        viteza = other.viteza;
        progresCapSnake = other.progresCapSnake;
    }
    return *this;
}

SirDeBile::~SirDeBile() {
    std::cout << "SirDeBile: Destructor\n";
}

void SirDeBile::actualizeaza(float deltaTime) {
    progresCapSnake += viteza * deltaTime;

    float progresCurent = progresCapSnake;
    for (auto& bila : bile) {
        // Logica de mișcare e simplificată; o implementare reală
        // ar trebui să interpoleze de-a lungul vectorului 'traseu'
        bila.setPozitie(getPozitiePeTraseu(progresCurent));
        progresCurent -= (2 * RAZA_BILA); // Spațiere între bile
    }
}

int SirDeBile::insereazaSiVerifica(std::list<Bila>::iterator it, const Bila& bilaNoua) {
    auto pozInserata = bile.insert(it, bilaNoua);

    Culoare c = pozInserata->getCuloare();
    std::vector<std::list<Bila>::iterator> potriviri;
    potriviri.push_back(pozInserata);

    auto st = pozInserata;
    while (st != bile.begin()) {
        --st;
        if (st->getCuloare() == c) potriviri.push_back(st);
        else break;
    }

    auto dr = pozInserata;
    ++dr;
    while (dr != bile.end()) {
        if (dr->getCuloare() == c) potriviri.push_back(dr);
        else break;
        ++dr;
    }

    if (potriviri.size() >= 3) {
        for (auto& p : potriviri) {
            bile.erase(p);
        }
        return (int)potriviri.size();
    }
    return 0;
}

void SirDeBile::adaugaBilaLaCoada(Culoare c) {
    float progres = progresCapSnake - (bile.size() * 2 * RAZA_BILA);
    if (progres < 0.f) progres = 0.f;
    bile.emplace_back(c, getPozitiePeTraseu(progres));
}

void SirDeBile::deseneaza(sf::RenderWindow& window) const {
    for (const auto& bila : bile) {
        bila.deseneaza(window);
    }
}

std::list<Bila>& SirDeBile::getBile() {
    return bile;
}

sf::Vector2f SirDeBile::getPozitiePeTraseu(float progres) const {
    if (traseu.empty()) return {0, 0};
    // Implementare foarte simplistă a traseului
    int index = static_cast<int>(progres / 10.f);
    if (index >= traseu.size()) {
        return traseu.back(); // A ajuns la capăt
    }
    if (index < 0) {
        return traseu.front();
    }
    return traseu[index];
}

std::ostream& operator<<(std::ostream& os, const SirDeBile& s) {
    os << "SirDeBile cu " << s.bile.size() << " bile:\n";
    for (const auto& b : s.bile) {
        os << "\t" << b << "\n";
    }
    return os;
}