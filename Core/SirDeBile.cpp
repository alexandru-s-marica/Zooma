#include "SirDeBile.h"
#include "Efecte.h"
#include "Nivel.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <iterator>
#include <set>
#include <ctime>

SirDeBile::SirDeBile()
    : viteza(0.f), distantaIntreBile(0.f), lungimeTotalaTraseu(0.f),
      distantaRetroRamasa(0.f), timerGenerareEfectRandom(0.f) {
}

SirDeBile::SirDeBile(std::vector<Vec2f> traseu, float viteza, float distanta)
    : traseu(std::move(traseu)), viteza(viteza), distantaIntreBile(distanta), lungimeTotalaTraseu(0.f),
      distantaRetroRamasa(0.f), timerGenerareEfectRandom(0.f)
{
    for (size_t i = 0; i < this->traseu.size() - 1; ++i) {
        lungimeTotalaTraseu += (this->traseu[i+1] - this->traseu[i]).magnitude();
    }

    const int NUMAR_BILE_INITIALE = 10;
    float progresCurent = (NUMAR_BILE_INITIALE - 1) * distantaIntreBile;

    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distCuloare(0, 4);

    for (int i = 0; i < NUMAR_BILE_INITIALE; ++i) {
        bile.emplace_back(
            static_cast<Culoare>(distCuloare(generator)),
            getPozitiePeTraseu(progresCurent),
            20.f,
            progresCurent
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
        std::cout << "[Combo] Explozie magnetica! " << count << " bile distruse.\n";
        for (auto it = it_start; it != it_end; ++it) it->marcheazaPentruDistrugere();
    }
}

// Functie helper pentru a numara cate bile de aceeasi culoare sunt adiacente
int numaraAdiacente(std::list<Bila>& lista, std::list<Bila>::iterator startIt, bool directieSpreSpate) {
    if (startIt == lista.end()) return 0;
    Culoare c = startIt->getCuloare();
    int count = 0;
    auto it = startIt;

    while (it != lista.end()) {
        if (it->getCuloare() != c || it->esteInDistrugere()) break;
        count++;

        if (directieSpreSpate) {
            it++;
        } else {
            if (it == lista.begin()) break;
            it--;
        }
    }
    return count;
}

void SirDeBile::actualizeaza(float deltaTime, Nivel& nivel) {
    if (bile.empty()) return;

    for (auto it = bile.begin(); it != bile.end(); ) {
        it->actualizeaza(deltaTime);
        if (it->esteInDistrugere()) {
            if (it->eGataDeSters()) {
                if (it->areEfect()) it->activeazaEfect(nivel);
                Bila::incrementeazaBileDistruse();
                it = bile.erase(it);
            } else { ++it; }
        } else { ++it; }
    }

    if (bile.empty()) return;

    float deltaMiscare = viteza * deltaTime;

    if (distantaRetroRamasa > 0.f) {
        float vitezaRewind = viteza * 4.0f;
        float pas = vitezaRewind * deltaTime;
        if (pas > distantaRetroRamasa) pas = distantaRetroRamasa;
        deltaMiscare = -pas;
        distantaRetroRamasa -= pas;

        for (auto& bila : bile) {
            if (!bila.esteInDistrugere()) {
                bila.avanseaza(deltaMiscare);
                if (bila.getProgres() < 0.f) bila.setProgres(0.f);
                bila.setPozitie(getPozitiePeTraseu(bila.getProgres()));
            }
        }
    }
    else {
        auto it = bile.rbegin();

        if (it != bile.rend() && !it->esteInDistrugere()) {
            it->avanseaza(deltaMiscare);
            it->setPozitie(getPozitiePeTraseu(it->getProgres()));
        }

        auto prev_it = it;
        ++it;

        while (it != bile.rend()) {
            if (it->esteInDistrugere()) { ++it; continue; }

            float dist = it->getProgres() - prev_it->getProgres();
            float distantaIdeala = distantaIntreBile;

            if (dist <= distantaIdeala + 0.5f) {
                float nouProgres = prev_it->getProgres() + distantaIdeala;
                it->setProgres(nouProgres);
                it->setPozitie(getPozitiePeTraseu(nouProgres));

                prev_it = it;
            }
            else {
                bool activareMagnet = false;

                if (it->getCuloare() == prev_it->getCuloare()) {
                    auto it_spate_forward = std::prev(prev_it.base());
                    auto it_fata_forward = std::prev(it.base());

                    int countSpate = numaraAdiacente(bile, it_spate_forward, true);

                    int countFata = numaraAdiacente(bile, it_fata_forward, false);

                    if ((countSpate + countFata) >= 3) {
                        activareMagnet = true;
                    }
                }

                if (activareMagnet) {
                    float vitezaAtragere = viteza * 3.0f;
                    float pasAtragere = vitezaAtragere * deltaTime;

                    float progresNou = it->getProgres() - pasAtragere;
                    float target = prev_it->getProgres() + distantaIdeala;

                    if (progresNou <= target) {
                        it->setProgres(target);
                        it->setPozitie(getPozitiePeTraseu(target));

                        auto it_fata_fw = std::prev(it.base());
                        auto it_spate_fw = std::prev(prev_it.base());
                        verificaExplozieLant(it_fata_fw, it_spate_fw);
                    } else {
                        it->setProgres(progresNou);
                        it->setPozitie(getPozitiePeTraseu(progresNou));
                    }

                    prev_it = it;
                }
                else {
                    prev_it = it;
                }
            }
            ++it;
        }
    }

    timerGenerareEfectRandom += deltaTime;
    if (timerGenerareEfectRandom > 5.0f) {
        timerGenerareEfectRandom = 0.f;
        static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_int_distribution<int> chance(0, 100);
        if (chance(gen) < 40 && !bile.empty()) {
            std::uniform_int_distribution<size_t> distIdx(0, bile.size() - 1);
            size_t idx = distIdx(gen);
            auto it = bile.begin();
            std::advance(it, idx);
            if (!it->areEfect() && !it->esteInDistrugere()) {
                std::uniform_int_distribution<int> distTip(0, 3);
                int tip = distTip(gen);
                std::unique_ptr<EfectBila> e = nullptr;
                switch(tip) {
                    case 0: e = std::make_unique<EfectExplozie>(); break;
                    case 1: e = std::make_unique<EfectRetro>(); break;
                    case 2: e = std::make_unique<EfectAccuracy>(); break;
                    case 3: e = std::make_unique<EfectInghet>(); break;
                }
                if (e) it->adaugaEfect(std::move(e));
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
    float progresInserare = 0.0f;

    if (it_target != bile.end()) {
        progresInserare = it_target->getProgres();
        for (auto it = it_target; it != bile.end(); ++it) {
            if (!it->esteInDistrugere()) {
                it->setProgres(it->getProgres() - distantaIntreBile);
                it->setPozitie(getPozitiePeTraseu(it->getProgres()));
            }
        }
    } else {
        if (!bile.empty()) {
             progresInserare = bile.back().getProgres() - distantaIntreBile;
        }
    }

    Bila copie = bilaNoua;
    copie.setProgres(progresInserare);
    copie.setPozitie(getPozitiePeTraseu(progresInserare));

    auto it_inserat = bile.insert(it_target, copie);

    int scorAdaugat = 0;
    Culoare culoareInserata = copie.getCuloare();
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
    if (bile.empty()) return false;
    return bile.front().getProgres() >= lungimeTotalaTraseu;
}

std::ostream& operator<<(std::ostream& os, const SirDeBile& sirBile){
    os << "SirDeBile (Viteza: " << sirBile.viteza << ", Bile: " << sirBile.bile.size() << ")\n";
    return os;
}