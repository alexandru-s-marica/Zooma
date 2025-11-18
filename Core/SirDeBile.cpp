#include "SirDeBile.h"
#include <algorithm>
#include <random>
#include <cmath>

SirDeBile::SirDeBile() : viteza(0.f), progresCapSnake(0.f), distantaIntreBile(0.f), lungimeTotalaTraseu(0.f) {
    std::cout << "SirDeBile: Constructor default\n";
}

SirDeBile::SirDeBile(std::vector<Vec2f> traseu, float viteza, float distanta)
    : traseu(std::move(traseu)), viteza(viteza), distantaIntreBile(distanta), lungimeTotalaTraseu(0.f)
{
    std::cout << "SirDeBile: Constructor cu parametri, distanta intre bile: " << distantaIntreBile << "\n";

    for (size_t i = 0; i < this->traseu.size() - 1; ++i) {
        lungimeTotalaTraseu += (this->traseu[i+1] - this->traseu[i]).magnitude();
    }
    std::cout << "Lungimea totala a traseului: " << lungimeTotalaTraseu << "\n";

    const int NUMAR_BILE_INITIALE = 10;
    progresCapSnake = (NUMAR_BILE_INITIALE - 1) * distantaIntreBile;

    float progresCurent = progresCapSnake;
    for (int i = 0; i < NUMAR_BILE_INITIALE; ++i) {
        bile.emplace_back(
            static_cast<Culoare>(rand() % 5),
            getPozitiePeTraseu(progresCurent),
            20.f
        );
        progresCurent -= distantaIntreBile;
    }
}

SirDeBile::SirDeBile(const SirDeBile& other)
    : bile(other.bile), traseu(other.traseu), viteza(other.viteza),
      progresCapSnake(other.progresCapSnake), distantaIntreBile(other.distantaIntreBile),
      lungimeTotalaTraseu(other.lungimeTotalaTraseu)
{
    std::cout << "SirDeBile: Copy constructor\n";
}

SirDeBile& SirDeBile::operator=(const SirDeBile& other) {
    std::cout << "SirDeBile: Assignment operator\n";
    if (this != &other) {
        bile = other.bile;
        traseu = other.traseu;
        viteza = other.viteza;
        progresCapSnake = other.progresCapSnake;
        distantaIntreBile = other.distantaIntreBile;
        lungimeTotalaTraseu = other.lungimeTotalaTraseu;
    }
    return *this;
}

SirDeBile::~SirDeBile() {
    std::cout << "SirDeBile: Destructor\n";
}

Vec2f SirDeBile::getPozitiePeTraseu(float progres) const {
    if (traseu.empty() || lungimeTotalaTraseu == 0.f) return {0.f, 0.f};

    if (progres < 0) progres = 0;
    if (progres > lungimeTotalaTraseu) progres = lungimeTotalaTraseu;

    float distantaCurenta = 0;
    for (size_t i = 0; i < traseu.size() - 1; ++i) {
        float lungimeSegment = (traseu[i+1] - traseu[i]).magnitude();
        // Folosim o mică toleranță (epsilon) pentru a preveni erorile de floating point
        float epsilon = 0.001f;
        if (progres <= (distantaCurenta + lungimeSegment + epsilon)) {
            float progresInSegment = progres - distantaCurenta;
            float t = (lungimeSegment > 0) ? (progresInSegment / lungimeSegment) : 0.f;
            return traseu[i] + (traseu[i+1] - traseu[i]) * t;
        }
        distantaCurenta += lungimeSegment;
    }
    return traseu.back();
}

void SirDeBile::actualizeaza(float deltaTime) {
    progresCapSnake += viteza * deltaTime;

    float currentProgres = progresCapSnake;
    for (auto& bila : bile) {
        bila.setPozitie(getPozitiePeTraseu(currentProgres));
        currentProgres -= distantaIntreBile;
    }
}

std::list<Bila>& SirDeBile::getBile() {
    return bile;
}

const std::list<Bila>& SirDeBile::getBile() const {
    return bile;
}

int SirDeBile::insereazaSiVerifica(std::list<Bila>::iterator it_target, const Bila& bilaNoua) {
    if (it_target == bile.end()) return 0;

    it_target = bile.insert(it_target, bilaNoua);

    int scorAdaugat = 0;
    Culoare culoareInserata = bilaNoua.getCuloare();
    std::vector<std::list<Bila>::iterator> deEliminat;

    auto st = it_target;
    while (st != bile.begin()) {
        --st;
        if (st->getCuloare() == culoareInserata) deEliminat.push_back(st);
        else break;
    }
    std::reverse(deEliminat.begin(), deEliminat.end());

    deEliminat.push_back(it_target);

    auto dr = it_target;
    ++dr;
    while (dr != bile.end()) {
        if (dr->getCuloare() == culoareInserata) {
            deEliminat.push_back(dr);
            ++dr;
        } else break;
    }

    if (deEliminat.size() >= 3) {
        for (auto it = deEliminat.rbegin(); it != deEliminat.rend(); ++it) {
            bile.erase(*it);
            scorAdaugat += 10;
        }
    }

    return scorAdaugat;
}

bool SirDeBile::aAtingJucatorulSfarsitul() const {
    return (progresCapSnake >= lungimeTotalaTraseu) && (lungimeTotalaTraseu > 0);
}

std::ostream& operator<<(std::ostream& os, const SirDeBile& sirBile){
    os << "SirDeBile (Viteza: " << sirBile.viteza << ", Progres Cap: " << sirBile.progresCapSnake << ", Bile: " << sirBile.bile.size() << ")\n";
    for (const auto& b : sirBile.bile) {
        os << "  - " << b << "\n";
    }
    return os;
}