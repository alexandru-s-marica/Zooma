#include "Nivel.h"
#include <cmath>

void Nivel::genereazaTraseu() {
    traseu.clear();
    for (int i = 0; i < 200; ++i) traseu.push_back({100.f + i * 3, 100.f});
    for (int i = 0; i < 150; ++i) traseu.push_back({700.f, 100.f + i * 2});
    for (int i = 0; i < 200; ++i) traseu.push_back({700.f - i * 3, 400.f});
    for (int i = 0; i < 100; ++i) traseu.push_back({100.f, 400.f - i * 2});
}

Nivel::Nivel()
    : sirBile(),
      proiector({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f}),
      scor(0),
      stare(StareJoc::RULEAZA)
{
    std::cout << "Nivel: Constructor default (pentru CI/tests)\n";
    genereazaTraseu();
}

Nivel::Nivel(float initialBallSpacing)
    : proiector({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f}),
      scor(0),
      stare(StareJoc::RULEAZA)
{
    std::cout << "Nivel: Constructor cu specificarea distantei intre bile\n";
    genereazaTraseu();
    sirBile = SirDeBile(traseu, 40.f, initialBallSpacing);
}

void Nivel::ruleazaFrame(float deltaTime) {
    if (stare == StareJoc::GAME_OVER) {
        return;
    }

    sirBile.actualizeaza(deltaTime);

    for (auto it = proiectileInZbor.begin(); it != proiectileInZbor.end(); ) {
        it->first.setPozitie(it->first.getPozitie() + it->second * 1000.f * deltaTime);

        Vec2f pos = it->first.getPozitie();
        if (pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y < 0 || pos.y > SCREEN_HEIGHT) {
            it = proiectileInZbor.erase(it);
        } else {
            ++it;
        }
    }

    gestioneazaColiziuni();

    if (sirBile.aAtingJucatorulSfarsitul()) {
        std::cout << "GAME OVER: Bilele au ajuns la capat!\n";
        stare = StareJoc::GAME_OVER;
    }
}

void Nivel::gestioneazaColiziuni() {
    auto& bileSir = sirBile.getBile();

    auto it_proiectil = proiectileInZbor.begin();
    while (it_proiectil != proiectileInZbor.end()) {
        bool coliziune = false;

        for (auto it_bila_sir = bileSir.begin(); it_bila_sir != bileSir.end(); ++it_bila_sir) {

            if (it_proiectil->first.getBounds().findIntersection(it_bila_sir->getBounds())) {
                int scorAdaugat = sirBile.insereazaSiVerifica(it_bila_sir, it_proiectil->first);
                scor += scorAdaugat;

                it_proiectil = proiectileInZbor.erase(it_proiectil);
                coliziune = true;
                break;
            }
        }

        if (!coliziune) {
            ++it_proiectil;
        }
    }
}

void Nivel::adaugaProiectil(const Bila& p, const Vec2f& dir) {
    proiectileInZbor.push_back({p, dir});
}

const std::list<Bila>& Nivel::getSirDeBile() const {
    return sirBile.getBile();
}

Proiector& Nivel::getProiector() {
    return proiector;
}

const Proiector& Nivel::getProiector() const {
    return proiector;
}

const std::list<std::pair<Bila, Vec2f>>& Nivel::getProiectileInZbor() const {
    return proiectileInZbor;
}

int Nivel::getScor() const {
    return scor;
}

StareJoc Nivel::getStareJoc() const {
    return stare;
}

bool Nivel::esteTerminat() const {
    return stare == StareJoc::GAME_OVER;
}

std::ostream& Nivel::operator<<(std::ostream& os) const {
    os << "--- Stare Nivel ---\n";
    os << "Scor: " << scor << "\n";
    os << proiector << "\n";
    os << sirBile << "\n";
    os << "-------------------\n";
    return os;
}