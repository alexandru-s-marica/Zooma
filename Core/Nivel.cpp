#include "Nivel.h"
#include <cmath>
#include <iostream>
#include <algorithm>

const float VITEZA_SARPE = 70.0f;

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
      stare(StareJoc::RULEAZA),
      modAccuracyActiv(false),
      timpRamasAccuracy(0.0f)
{
    std::cout << "Nivel: Constructor default\n";
    genereazaTraseu();
}

Nivel::Nivel(float initialBallSpacing)
    : proiector({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f}),
      scor(0),
      stare(StareJoc::RULEAZA),
      modAccuracyActiv(false),
      timpRamasAccuracy(0.0f)
{
    std::cout << "Nivel: Constructor parametri\n";
    genereazaTraseu();
    sirBile = SirDeBile(traseu, VITEZA_SARPE, initialBallSpacing);

    std::vector<Culoare> culori = sirBile.getCuloriActive();
    if (!culori.empty()) {
        proiector.valideazaCulori(culori);
    }
}

void Nivel::ruleazaFrame(float deltaTime) {
    if (stare != StareJoc::RULEAZA) return;

    float vitezaProiectil = 1000.f;
    if (modAccuracyActiv) vitezaProiectil = 2500.f;

    for (auto& proiectil : proiectileInZbor) {
        proiectil.first.setPozitie(proiectil.first.getPozitie() + proiectil.second * vitezaProiectil * deltaTime);
    }

    if (modAccuracyActiv) {
        timpRamasAccuracy -= deltaTime;
        if (timpRamasAccuracy <= 0.f) {
            modAccuracyActiv = false;
            std::cout << "Mod Accuracy dezactivat.\n";
        }
    }

    for (auto it = exploziiVizuale.begin(); it != exploziiVizuale.end(); ) {
        it->timer -= deltaTime * 1.5f; // Viteza disparitie explozie
        if (it->timer <= 0.f) it = exploziiVizuale.erase(it);
        else ++it;
    }

    sirBile.actualizeaza(deltaTime, *this);

    std::vector<Culoare> culoriActive = sirBile.getCuloriActive();
    if (!culoriActive.empty()) proiector.valideazaCulori(culoriActive);

    proiectileInZbor.remove_if([](const std::pair<Bila, Vec2f>& p) {
        const Vec2f& pos = p.first.getPozitie();
        return pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y < 0 || pos.y > SCREEN_HEIGHT;
    });

    gestioneazaColiziuni();

    if (sirBile.aAtingJucatorulSfarsitul()) {
        std::cout << "GAME OVER!\n";
        stare = StareJoc::GAME_OVER;
    }
    else if (sirBile.getBile().empty() && stare == StareJoc::RULEAZA) {
        std::cout << "VICTORIE!\n";
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
        if (!coliziuneDetectata) ++it_proiectil;
    }
}

void Nivel::adaugaProiectil(const Bila& p, const Vec2f& dir) {
    proiectileInZbor.push_back({p, dir});
}

const std::list<Bila>& Nivel::getSirDeBile() const { return sirBile.getBile(); }
Proiector& Nivel::getProiector() { return proiector; }
const Proiector& Nivel::getProiector() const { return proiector; }
const std::list<std::pair<Bila, Vec2f>>& Nivel::getProiectileInZbor() const { return proiectileInZbor; }
int Nivel::getScor() const { return scor; }
StareJoc Nivel::getStareJoc() const { return stare; }
bool Nivel::esteTerminat() const { return stare == StareJoc::GAME_OVER; }
bool Nivel::esteCastigat() const { return stare == StareJoc::CASTIGAT; }

std::ostream& operator<<(std::ostream& os, const Nivel& n) {
    os << "Nivel (Scor: " << n.scor << ")\n";
    return os;
}

void Nivel::reset(float initialBallSpacing) {
    scor = 0;
    stare = StareJoc::RULEAZA;
    modAccuracyActiv = false;
    timpRamasAccuracy = 0.0f;

    proiectileInZbor.clear();
    exploziiVizuale.clear(); // Resetare efecte

    proiector = Proiector({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f});
    traseu.clear();
    genereazaTraseu();
    sirBile = SirDeBile(traseu, VITEZA_SARPE, initialBallSpacing);

    std::vector<Culoare> culori = sirBile.getCuloriActive();
    if (!culori.empty()) proiector.valideazaCulori(culori);
}

Bila Nivel::trageBilaJucator() {
    std::vector<Culoare> culoriDisponibile = sirBile.getCuloriActive();
    return proiector.trage(culoriDisponibile);
}

void Nivel::activeazaExplozieLa(Vec2f pozitie) {
    std::cout << "BOOM! Explozie la " << pozitie << "\n";
    sirBile.explodeazaZona(pozitie, 250.0f);
    exploziiVizuale.push_back({pozitie, 250.0f, 1.0f});
}

void Nivel::activeazaRetro() {
    std::cout << "RETRO ACTIVAT!\n";
    sirBile.aplicaRetro(300.0f);
}

void Nivel::activeazaModAccuracy(float durata) {
    std::cout << "ACCURACY ACTIVAT!\n";
    modAccuracyActiv = true;
    timpRamasAccuracy = durata;
}