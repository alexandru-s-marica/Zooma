#include "Nivel.h"
#include "Exceptions.h"
#include <cmath>
#include <iostream>
#include <algorithm>

const float PI = 3.14159265f;

float calculeazaVitezaNivel(int nivel) {
    return 60.0f + (nivel - 1) * 20.0f;
}

float calculeazaDistantaBile(int nivel) {
    (void)nivel;
    return 40.0f;
}

void Nivel::genereazaTraseu(int nivel) {
    traseu.clear();
    Vec2f pozProiector = {400.f, 500.f};

    if (nivel == 1) {
        for (int i = 0; i < 200; ++i) traseu.push_back({100.f + i * 3, 100.f});
        for (int i = 0; i < 150; ++i) traseu.push_back({700.f, 100.f + i * 2});
        for (int i = 0; i < 200; ++i) traseu.push_back({700.f - i * 3, 400.f});
        for (int i = 0; i < 100; ++i) traseu.push_back({100.f, 400.f - i * 2});
    }
    else if (nivel == 2) {
        pozProiector = {400.f, 300.f};
        float razaStart = 350.f; float unghiStart = 0.f; float unghiTotal = 4.0f * PI; int pasi = 600;
        for (int i = 0; i < pasi; ++i) {
            float t = (float)i / pasi; float unghi = unghiStart + t * unghiTotal; float raza = razaStart * (1.0f - t * 0.8f);
            traseu.push_back({400.f + raza * std::cos(unghi), 300.f + raza * std::sin(unghi)});
        }
    }
    else {
        pozProiector = {400.f, 250.f};
        for (int i = 0; i < 1000; ++i) {
            float t = (float)i / 1000 * 2.0f * PI;
            float x = 400.f + 12.f * (16.f * std::pow(std::sin(t), 3));
            float y = 300.f - 12.f * (13.f * std::cos(t) - 5.f * std::cos(2*t) - 2.f * std::cos(3*t) - std::cos(4*t));
            traseu.push_back({x, y});
        }
    }
    proiector.setPozitie(pozProiector);
}

Nivel::Nivel()
    : sirBile(), proiector({400.f, 500.f}), scor(0), highScore(0),
      stare(StareJoc::RULEAZA), nivelCurent(1),
      modAccuracyActiv(false), timpRamasAccuracy(0.0f)
{
    try { incarcaHighScore(); } catch (const FisierCoruptException& e) { std::cerr << e.what() << std::endl; highScore = 0; }
    genereazaTraseu(1);
    incarcaNivel(1);
}

void Nivel::incarcaHighScore() {
    std::ifstream in("highscore.txt");
    if (!in.is_open()) { highScore = 0; return; }
    if (!(in >> highScore)) throw FisierCoruptException("Nu s-a putut citi scorul integer.");
    in.close();
}

void Nivel::salveazaHighScore() { std::ofstream out("highscore.txt"); if (out.is_open()) { out << highScore; out.close(); } }

void Nivel::incarcaNivel(int numarNivel) {
    nivelCurent = numarNivel; reset();
    std::cout << "--- INCARCARE NIVEL " << nivelCurent << " ---\n";
}

void Nivel::reset() {
    stare = StareJoc::RULEAZA; modAccuracyActiv = false; timpRamasAccuracy = 0.0f;
    proiectileInZbor.clear(); exploziiVizuale.clear();
    genereazaTraseu(nivelCurent);
    sirBile = SirDeBile(traseu, calculeazaVitezaNivel(nivelCurent), calculeazaDistantaBile(nivelCurent));
    std::vector<Culoare> culori = sirBile.getCuloriActive();
    if (!culori.empty()) proiector.valideazaCulori(culori);
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
        if (timpRamasAccuracy <= 0.f) modAccuracyActiv = false;
    }

    for (auto it = exploziiVizuale.begin(); it != exploziiVizuale.end(); ) {
        it->timer -= deltaTime * 1.5f;
        if (it->timer <= 0.f) it = exploziiVizuale.erase(it); else ++it;
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
        stare = StareJoc::GAME_OVER;
        if (scor > highScore) { highScore = scor; salveazaHighScore(); }
    } else if (sirBile.getBile().empty() && stare == StareJoc::RULEAZA) {
        stare = StareJoc::CASTIGAT;
        if (scor > highScore) { highScore = scor; salveazaHighScore(); }
    }
}

void Nivel::gestioneazaColiziuni() {
    auto it_proiectil = proiectileInZbor.begin();
    while (it_proiectil != proiectileInZbor.end()) {
        bool coliziuneDetectata = false;
        for (auto it_bila_sir = sirBile.getBile().begin(); it_bila_sir != sirBile.getBile().end(); ++it_bila_sir) {
            if (it_proiectil->first.getBounds().findIntersection(it_bila_sir->getBounds())) {
                scor += sirBile.insereazaSiVerifica(it_bila_sir, it_proiectil->first);
                it_proiectil = proiectileInZbor.erase(it_proiectil);
                coliziuneDetectata = true; break;
            }
        }
        if (!coliziuneDetectata) ++it_proiectil;
    }
}

void Nivel::adaugaProiectil(const Bila& p, const Vec2f& dir) { proiectileInZbor.push_back({p, dir}); }
const std::list<Bila>& Nivel::getSirDeBile() const { return sirBile.getBile(); }
Proiector& Nivel::getProiector() { return proiector; }
const Proiector& Nivel::getProiector() const { return proiector; }
const std::list<std::pair<Bila, Vec2f>>& Nivel::getProiectileInZbor() const { return proiectileInZbor; }
int Nivel::getScor() const { return scor; }
int Nivel::getHighScore() const { return highScore; }
const std::vector<Vec2f>& Nivel::getTraseu() const { return traseu; }
StareJoc Nivel::getStareJoc() const { return stare; }
bool Nivel::esteTerminat() const { return stare == StareJoc::GAME_OVER; }
bool Nivel::esteCastigat() const { return stare == StareJoc::CASTIGAT; }
std::ostream& operator<<(std::ostream& os, const Nivel& n) { os << "Nivel " << n.nivelCurent << " (Scor: " << n.scor << ")\n"; return os; }
Bila Nivel::trageBilaJucator() { std::vector<Culoare> c = sirBile.getCuloriActive(); return proiector.trage(c); }

void Nivel::activeazaExplozieLa(Vec2f pozitie, float raza) {
    sirBile.explodeazaZona(pozitie, raza);
    exploziiVizuale.push_back({pozitie, raza, 1.0f});
}

void Nivel::activeazaRetro(float distanta) {
    sirBile.aplicaRetro(distanta);
}

void Nivel::activeazaModAccuracy(float durata) {
    modAccuracyActiv = true;
    timpRamasAccuracy = durata;
}

void Nivel::adaugaScor(int valoare) {
    scor += valoare;
}