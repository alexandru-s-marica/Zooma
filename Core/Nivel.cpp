#include "Nivel.h"
#include <cmath>
#include <iostream>
#include <algorithm>

// Viteza creste cu nivelul
float calculeazaVitezaNivel(int nivel) {
    return 60.0f + (nivel - 1) * 30.0f;
}

float calculeazaDistantaBile(int nivel) {
    (void)nivel;
    return 40.0f;
}

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
      nivelCurent(1),
      modAccuracyActiv(false),
      timpRamasAccuracy(0.0f),
      esteInghetat(false),
      timpRamasInghet(0.0f)
{
    genereazaTraseu();
    incarcaNivel(1);
}

void Nivel::incarcaNivel(int numarNivel) {
    nivelCurent = numarNivel;
    reset();
    std::cout << "--- INCARCARE NIVEL " << nivelCurent << " ---\n";
}

void Nivel::reset() {
    stare = StareJoc::RULEAZA;
    modAccuracyActiv = false;
    timpRamasAccuracy = 0.0f;
    esteInghetat = false;
    timpRamasInghet = 0.0f;

    proiectileInZbor.clear();
    exploziiVizuale.clear();

    proiector = Proiector({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f});

    // Configurare SirDeBile in functie de nivel
    float viteza = calculeazaVitezaNivel(nivelCurent);
    float distanta = calculeazaDistantaBile(nivelCurent);

    sirBile = SirDeBile(traseu, viteza, distanta);

    std::vector<Culoare> culori = sirBile.getCuloriActive();
    if (!culori.empty()) proiector.valideazaCulori(culori);
}

void Nivel::ruleazaFrame(float deltaTime) {
    if (stare != StareJoc::RULEAZA) return;

    float timeStepBile = deltaTime;
    if (esteInghetat) {
        timpRamasInghet -= deltaTime;
        if (timpRamasInghet <= 0.f) {
            esteInghetat = false;
            std::cout << "INGHET DEZACTIVAT\n";
        } else {
            timeStepBile = 0.0f;
        }
    }

    float vitezaProiectil = 1000.f;
    if (modAccuracyActiv) vitezaProiectil = 2500.f;

    for (auto& proiectil : proiectileInZbor) {
        proiectil.first.setPozitie(proiectil.first.getPozitie() + proiectil.second * vitezaProiectil * deltaTime);
    }

    if (modAccuracyActiv) {
        timpRamasAccuracy -= deltaTime;
        if (timpRamasAccuracy <= 0.f) modAccuracyActiv = false;
    }

    for (auto it = exploziiVizuale.begin(); it != exploziiVizuale.end(); ) {
        it->timer -= deltaTime * 1.5f;
        if (it->timer <= 0.f) it = exploziiVizuale.erase(it);
        else ++it;
    }

    sirBile.actualizeaza(timeStepBile, *this);

    std::vector<Culoare> culoriActive = sirBile.getCuloriActive();
    if (!culoriActive.empty()) proiector.valideazaCulori(culoriActive);

    proiectileInZbor.remove_if([](const std::pair<Bila, Vec2f>& p) {
        const Vec2f& pos = p.first.getPozitie();
        return pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y < 0 || pos.y > SCREEN_HEIGHT;
    });

    gestioneazaColiziuni();

    if (sirBile.aAtingJucatorulSfarsitul()) {
        stare = StareJoc::GAME_OVER;
    }
    else if (sirBile.getBile().empty() && stare == StareJoc::RULEAZA) {
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
    os << "Nivel " << n.nivelCurent << " (Scor: " << n.scor << ")\n";
    return os;
}

Bila Nivel::trageBilaJucator() {
    std::vector<Culoare> culoriDisponibile = sirBile.getCuloriActive();
    return proiector.trage(culoriDisponibile);
}

void Nivel::activeazaExplozieLa(Vec2f pozitie) {
    sirBile.explodeazaZona(pozitie, 250.0f);
    exploziiVizuale.push_back({pozitie, 250.0f, 1.0f});
}

void Nivel::activeazaRetro() { sirBile.aplicaRetro(300.0f); }
void Nivel::activeazaModAccuracy(float durata) { modAccuracyActiv = true; timpRamasAccuracy = durata; }
void Nivel::activeazaInghet(float durata) { esteInghetat = true; timpRamasInghet = durata; }