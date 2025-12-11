#include "SirDeBile.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <set>

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

    const int NUMAR_BILE_INITIALE = 8;
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

SirDeBile::SirDeBile(const SirDeBile& other) = default;
SirDeBile& SirDeBile::operator=(const SirDeBile& other) = default;
SirDeBile::~SirDeBile() = default;

Vec2f SirDeBile::getPozitiePeTraseu(float progres) const {
    if (traseu.empty() || lungimeTotalaTraseu == 0.f) return {0.f, 0.f};

    if (progres < 0) progres = 0;
    if (progres > lungimeTotalaTraseu) progres = lungimeTotalaTraseu;

    float distantaCurenta = 0;
    for (size_t i = 0; i < traseu.size() - 1; ++i) {
        float lungimeSegment = (traseu[i+1] - traseu[i]).magnitude();
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

void SirDeBile::verificaExplozieLant(std::list<Bila>::iterator stanga, std::list<Bila>::iterator dreapta) {
    if (bile.empty()) return;
    if (stanga == bile.end() || dreapta == bile.end()) return;

    if (stanga->esteInDistrugere() || dreapta->esteInDistrugere()) return;
    if (stanga->getCuloare() != dreapta->getCuloare()) return;

    Culoare culoareTinta = stanga->getCuloare();
    auto it_start = stanga;
    auto it_end = dreapta;
    it_end++;

    while (it_start != bile.begin()) {
        auto prev = std::prev(it_start);
        if (prev->getCuloare() == culoareTinta && !prev->esteInDistrugere()) {
            it_start = prev;
        } else {
            break;
        }
    }

    while (it_end != bile.end()) {
        if (it_end->getCuloare() == culoareTinta && !it_end->esteInDistrugere()) {
            it_end++;
        } else {
            break;
        }
    }

    int count = 0;
    for (auto it = it_start; it != it_end; ++it) count++;

    if (count >= 3) {
        std::cout << "COMBO! " << count << " bile activate.\n";
        for (auto it = it_start; it != it_end; ++it) {
            it->marcheazaPentruDistrugere();
        }
    }
}

void SirDeBile::actualizeaza(float deltaTime) {
    if (bile.empty()) return;

    bool sAuStersBile = false;

    for (auto it = bile.begin(); it != bile.end(); ) {
        if (it->esteInDistrugere()) {
            it->actualizeazaTimer(deltaTime);
            if (it->eGataDeSters()) {
                it = bile.erase(it);
                sAuStersBile = true;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    if (bile.empty()) return;

    progresCapSnake += viteza * deltaTime;
    float currentProgres = progresCapSnake;

    for (auto& bila : bile) {
        bila.setPozitie(getPozitiePeTraseu(currentProgres));
        currentProgres -= distantaIntreBile;
    }

    if (sAuStersBile) {
        if (bile.size() < 2) return;

        auto it = bile.begin();
        auto next = std::next(it);

        while (next != bile.end()) {
            if (!it->esteInDistrugere() && !next->esteInDistrugere()) {
                if (it->getCuloare() == next->getCuloare()) {
                    verificaExplozieLant(it, next);
                }
            }
            it++;
            next++;
        }
    }

    for (auto it = bile.begin(); it != std::prev(bile.end()); ++it) {
        auto next_it = std::next(it);
        if (it->esteInDistrugere() || next_it->esteInDistrugere()) continue;

        float distSq = (it->getPozitie() - next_it->getPozitie()).magnitude();
        distSq *= distSq;
        float diametru = 2.0f * it->getRaza();

        if (distSq > (diametru * diametru + 5.0f)) {
        }
    }
}

std::list<Bila>& SirDeBile::getBile() { return bile; }
const std::list<Bila>& SirDeBile::getBile() const { return bile; }

int SirDeBile::insereazaSiVerifica(std::list<Bila>::iterator it_target, const Bila& bilaNoua) {
    if (it_target == bile.end()) {
        bile.push_back(bilaNoua);
        return 0;
    }

    auto it_inserat = bile.insert(it_target, bilaNoua);
    int scorAdaugat = 0;
    Culoare culoareInserata = bilaNoua.getCuloare();

    auto it_start = it_inserat;
    auto it_end = it_inserat;
    it_end++;

    while (it_start != bile.begin()) {
        auto prev = std::prev(it_start);
        if (prev->getCuloare() == culoareInserata && !prev->esteInDistrugere()) it_start = prev;
        else break;
    }

    while (it_end != bile.end()) {
        if (it_end->getCuloare() == culoareInserata && !it_end->esteInDistrugere()) it_end++;
        else break;
    }

    int count = 0;
    for (auto it = it_start; it != it_end; ++it) count++;

    if (count >= 3) {
        for (auto it = it_start; it != it_end; ++it) {
            it->marcheazaPentruDistrugere();
        }
        scorAdaugat += count * 10;
    }

    return scorAdaugat;
}

bool SirDeBile::aAtingJucatorulSfarsitul() const {
    return (progresCapSnake >= lungimeTotalaTraseu) && (lungimeTotalaTraseu > 0);
}

std::vector<Culoare> SirDeBile::getCuloriActive() const {
    std::set<Culoare> culoriUnice;

    for (const auto& bila : bile) {
        if (!bila.esteInDistrugere()) {
            culoriUnice.insert(bila.getCuloare());
        }
    }

    std::vector<Culoare> rezultat(culoriUnice.begin(), culoriUnice.end());
    return rezultat;
}

std::ostream& operator<<(std::ostream& os, const SirDeBile& sirBile){
    os << "SirDeBile (Viteza: " << sirBile.viteza << ", Progres Cap: " << sirBile.progresCapSnake << ", Bile: " << sirBile.bile.size() << ")\n";
    for (const auto& b : sirBile.bile) {
        os << "  - " << b << "\n";
    }
    return os;
}