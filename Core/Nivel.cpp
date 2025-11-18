#include "Nivel.h"
#include <cmath>
#include <iostream>
#include <algorithm>

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

    for (auto& proiectil : proiectileInZbor) {
        proiectil.first.setPozitie(proiectil.first.getPozitie() + proiectil.second * 1000.f * deltaTime);
    }

    proiectileInZbor.remove_if([](const std::pair<Bila, Vec2f>& p) {
        const Vec2f& pos = p.first.getPozitie();
        return pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y < 0 || pos.y > SCREEN_HEIGHT;
    });

    gestioneazaColiziuni();

    if (sirBile.aAtingJucatorulSfarsitul()) {
        std::cout << "GAME OVER: Bilele au ajuns la capat!\n";
        stare = StareJoc::GAME_OVER;

        (void)getScor();
        (void)esteTerminat();
    }
    else if (sirBile.getBile().empty() && stare == StareJoc::RULEAZA) {
        std::cout << "VICTORIE: Toate bilele au fost distruse!\n";
        stare = StareJoc::CASTIGAT;
    }
}

void Nivel::gestioneazaColiziuni() {
    auto it_proiectil = proiectileInZbor.begin();
    while (it_proiectil != proiectileInZbor.end()) {
        bool coliziuneDetectata = false;

        for (auto it_bila_sir = sirBile.getBile().begin(); it_bila_sir != sirBile.getBile().end(); ++it_bila_sir) {

            if (it_proiectil->first.getBounds().findIntersection(it_bila_sir->getBounds())) {

                int scorAdaugat = sirBile.insereazaSiVerifica(it_bila_sir, it_proiectil->first);
                scor += scorAdaugat;

                it_proiectil = proiectileInZbor.erase(it_proiectil);
                coliziuneDetectata = true;

                break;
            }
        }

        if (!coliziuneDetectata) {
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

std::ostream& operator<<(std::ostream& os, const Nivel& n) {
    os << "Nivel (Scor: " << n.scor << ", Stare: " << (int)n.stare << ")\n";
    os << "  - Proiector: " << n.proiector << "\n";
    return os;
}

void Nivel::reset(float initialBallSpacing) {
    scor = 0;
    stare = StareJoc::RULEAZA;
    proiectileInZbor.clear();
    proiector = Proiector({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f});
    traseu.clear();
    genereazaTraseu();
    sirBile = SirDeBile(traseu, 40.f, initialBallSpacing);
}

bool Nivel::esteCastigat() const {
    return stare == StareJoc::CASTIGAT;
}
