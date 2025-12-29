#include "SirDeBile.h"
#include "Efecte.h"
#include "Nivel.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <iterator>
#include <set>

SirDeBile::SirDeBile() : viteza(0.f), progresCapSnake(0.f), distantaIntreBile(0.f), lungimeTotalaTraseu(0.f), distantaRetroRamasa(0.f) {
    std::cout << "SirDeBile: Constructor default\n";
}

SirDeBile::SirDeBile(std::vector<Vec2f> traseu, float viteza, float distanta)
    : traseu(std::move(traseu)), viteza(viteza), distantaIntreBile(distanta), lungimeTotalaTraseu(0.f),
      distantaRetroRamasa(0.f) // Init cu 0
{
    std::cout << "SirDeBile: Constructor, Viteza = " << viteza << "\n";

    for (size_t i = 0; i < this->traseu.size() - 1; ++i) {
        lungimeTotalaTraseu += (this->traseu[i+1] - this->traseu[i]).magnitude();
    }

    const int NUMAR_BILE_INITIALE = 8;
    progresCapSnake = (NUMAR_BILE_INITIALE - 1) * distantaIntreBile;

    float progresCurent = progresCapSnake;

    auto genereazaEfectRandom = []() -> std::unique_ptr<EfectBila> {
        if ((rand() % 100) < 10) {
            int tip = rand() % 3;
            switch (tip) {
                case 0: return std::make_unique<EfectExplozie>();
                case 1: return std::make_unique<EfectRetro>();
                case 2: return std::make_unique<EfectAccuracy>();
                default: return nullptr;
            }
        }
        return nullptr;
    };

    for (int i = 0; i < NUMAR_BILE_INITIALE; ++i) {
        bile.emplace_back(
            static_cast<Culoare>(rand() % 5),
            getPozitiePeTraseu(progresCurent),
            20.f,
            genereazaEfectRandom()
        );
        progresCurent -= distantaIntreBile;
    }
}

Vec2f SirDeBile::getPozitiePeTraseu(float progres) const {
    if (traseu.empty() || lungimeTotalaTraseu == 0.f) return {0.f, 0.f};
    if (progres < 0) progres = 0;
    if (progres > lungimeTotalaTraseu) progres = lungimeTotalaTraseu;
    float distantaCurenta = 0;
    for (size_t i = 0; i < traseu.size() - 1; ++i) {
        float lungimeSegment = (traseu[i+1] - traseu[i]).magnitude();
        if (progres <= (distantaCurenta + lungimeSegment + 0.001f)) {
            float t = (lungimeSegment > 0) ? ((progres - distantaCurenta) / lungimeSegment) : 0.f;
            return traseu[i] + (traseu[i+1] - traseu[i]) * t;
        }
        distantaCurenta += lungimeSegment;
    }
    return traseu.back();
}

std::vector<Culoare> SirDeBile::getCuloriActive() const {
    std::set<Culoare> culoriUnice;
    for (const auto& bila : bile) {
        if (!bila.esteInDistrugere()) culoriUnice.insert(bila.getCuloare());
    }
    return std::vector<Culoare>(culoriUnice.begin(), culoriUnice.end());
}

void SirDeBile::verificaExplozieLant(std::list<Bila>::iterator stanga, std::list<Bila>::iterator dreapta) {
    if (bile.empty() || stanga == bile.end() || dreapta == bile.end()) return;
    if (stanga->esteInDistrugere() || dreapta->esteInDistrugere()) return;
    if (stanga->getCuloare() != dreapta->getCuloare()) return;

    Culoare culoareTinta = stanga->getCuloare();
    auto it_start = stanga;
    auto it_end = dreapta; it_end++;
    while (it_start != bile.begin()) {
        auto prev = std::prev(it_start);
        if (prev->getCuloare() == culoareTinta && !prev->esteInDistrugere()) it_start = prev; else break;
    }
    while (it_end != bile.end()) {
        if (it_end->getCuloare() == culoareTinta && !it_end->esteInDistrugere()) it_end++; else break;
    }
    int count = 0;
    for (auto it = it_start; it != it_end; ++it) count++;
    if (count >= 3) {
        for (auto it = it_start; it != it_end; ++it) it->marcheazaPentruDistrugere();
    }
}

void SirDeBile::actualizeaza(float deltaTime, Nivel& nivel) {
    if (bile.empty()) return;

    for (auto it = bile.begin(); it != bile.end(); ) {
        if (it->esteInDistrugere()) {
            it->actualizeazaTimer(deltaTime);
            if (it->eGataDeSters()) {
                if (it->areEfect()) {
                    it->activeazaEfect(nivel);
                }

                Bila::incrementeazaBileDistruse();

                it = bile.erase(it);

                if (it != bile.end() && it != bile.begin()) {
                    auto prev = std::prev(it);
                    if (!it->esteInDistrugere() && !prev->esteInDistrugere()) {
                        if (it->getCuloare() == prev->getCuloare()) verificaExplozieLant(prev, it);
                    }
                }
            } else ++it;
        } else ++it;
    }

    if (bile.empty()) return;

    if (distantaRetroRamasa > 0.f) {
        float vitezaRewind = viteza * 4.0f;
        float pas = vitezaRewind * deltaTime;

        if (pas > distantaRetroRamasa) pas = distantaRetroRamasa;

        progresCapSnake -= pas;
        distantaRetroRamasa -= pas;

        if (progresCapSnake < 0) {
            progresCapSnake = 0;
            distantaRetroRamasa = 0;
        }
    } else {
        progresCapSnake += viteza * deltaTime;
    }

    float currentProgres = progresCapSnake;
    for (auto& bila : bile) {
        bila.setPozitie(getPozitiePeTraseu(currentProgres));
        currentProgres -= distantaIntreBile;
    }

    for (auto it = bile.begin(); it != std::prev(bile.end()); ++it) {
        auto next_it = std::next(it);
        if (it->esteInDistrugere() || next_it->esteInDistrugere()) continue;

        Bila& b1 = *it;
        Bila& b2 = *next_it;
        float distSq = (b1.getPozitie() - b2.getPozitie()).magnitude();
        distSq *= distSq;
        float diametru = 2.0f * b1.getRaza();
        float diametruSq = diametru * diametru;

        if (distSq > diametruSq + 10.0f) {
            Vec2f directie = (b1.getPozitie() - b2.getPozitie()).normalize();
            float vitezaAtragere = 400.0f * deltaTime;
            b2.setPozitie(b2.getPozitie() + directie * vitezaAtragere);
            float nouaDistSq = (b1.getPozitie() - b2.getPozitie()).magnitude();
            nouaDistSq *= nouaDistSq;
            if (nouaDistSq <= diametruSq + 5.0f) {
                b2.setPozitie(b1.getPozitie() - directie * diametru);
                if (b1.getCuloare() == b2.getCuloare()) verificaExplozieLant(it, next_it);
            }
        }
    }
}

void SirDeBile::explodeazaZona(Vec2f centru, float raza) {
    float razaSq = raza * raza;
    for (auto& b : bile) {
        if (b.esteInDistrugere()) continue;
        Vec2f pos = b.getPozitie();
        float dx = pos.x - centru.x;
        float dy = pos.y - centru.y;
        if ((dx*dx + dy*dy) <= razaSq) {
            b.marcheazaPentruDistrugere();
        }
    }
}

void SirDeBile::aplicaRetro(float distantaInapoi) {
    distantaRetroRamasa += distantaInapoi;
}

std::list<Bila>& SirDeBile::getBile() { return bile; }
const std::list<Bila>& SirDeBile::getBile() const { return bile; }

int SirDeBile::insereazaSiVerifica(std::list<Bila>::iterator it_target, const Bila& bilaNoua) {
    if (it_target == bile.end()) { bile.push_back(bilaNoua); return 0; }
    auto it_inserat = bile.insert(it_target, bilaNoua);
    int scorAdaugat = 0;
    Culoare culoareInserata = bilaNoua.getCuloare();
    auto it_start = it_inserat; auto it_end = it_inserat; it_end++;
    while (it_start != bile.begin()) {
        auto prev = std::prev(it_start);
        if (prev->getCuloare() == culoareInserata && !prev->esteInDistrugere()) it_start = prev; else break;
    }
    while (it_end != bile.end()) {
        if (it_end->getCuloare() == culoareInserata && !it_end->esteInDistrugere()) it_end++; else break;
    }
    int count = 0;
    for (auto it = it_start; it != it_end; ++it) count++;
    if (count >= 3) {
        for (auto it = it_start; it != it_end; ++it) it->marcheazaPentruDistrugere();
        scorAdaugat += count * 10;
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