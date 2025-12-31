#include "Nivel.h"
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
        pozProiector = {400.f, 500.f};
        for (int i = 0; i < 200; ++i) traseu.push_back({100.f + i * 3, 100.f});
        for (int i = 0; i < 150; ++i) traseu.push_back({700.f, 100.f + i * 2});
        for (int i = 0; i < 200; ++i) traseu.push_back({700.f - i * 3, 400.f});
        for (int i = 0; i < 100; ++i) traseu.push_back({100.f, 400.f - i * 2});
    }
    else if (nivel == 2) {
        pozProiector = {400.f, 300.f};
        float razaStart = 350.f;
        float unghiStart = 0.f;
        float unghiTotal = 4.0f * PI;
        int pasi = 600;
        for (int i = 0; i < pasi; ++i) {
            float t = (float)i / pasi;
            float unghi = unghiStart + t * unghiTotal;
            float raza = razaStart * (1.0f - t * 0.8f);
            float x = 400.f + raza * std::cos(unghi);
            float y = 300.f + raza * std::sin(unghi);
            traseu.push_back({x, y});
        }
    }
    else if (nivel == 3) {
        pozProiector = {100.f, 550.f};
        std::vector<Vec2f> points = {
            {50.f, 50.f}, {750.f, 50.f},
            {50.f, 200.f}, {750.f, 200.f},
            {50.f, 350.f}, {750.f, 350.f},
            {50.f, 500.f}, {750.f, 500.f}
        };
        for (size_t k = 0; k < points.size() - 1; ++k) {
            Vec2f p1 = points[k];
            Vec2f p2 = points[k+1];
            float dist = (p2 - p1).magnitude();
            int pasi = static_cast<int>(dist / 2.0f);
            for (int i = 0; i < pasi; ++i) {
                float t = (float)i / pasi;
                traseu.push_back(p1 + (p2 - p1) * t);
            }
        }
    }
    else if (nivel == 4) {
        pozProiector = {400.f, 100.f};
        int pasi = 700;
        for (int i = 0; i < pasi; ++i) {
            float t = (float)i / pasi;
            float x = 50.f + t * 700.f;
            float y = 300.f + 200.f * std::sin(t * 4.0f * PI);
            traseu.push_back({x, y});
        }
    }
    else if (nivel == 5) {
        pozProiector = {400.f, 300.f};
        for(int i=0; i<700; i+=2) traseu.push_back({50.f + i, 50.f});
        for(int i=0; i<500; i+=2) traseu.push_back({750.f, 50.f + i});
        for(int i=0; i<700; i+=2) traseu.push_back({750.f - i, 550.f});
        for(int i=0; i<400; i+=2) traseu.push_back({50.f, 550.f - i});
        for(int i=0; i<300; i+=2) traseu.push_back({50.f + i, 150.f});
    }
    else if (nivel == 6) {
        pozProiector = {400.f, 300.f};

        std::vector<Vec2f> puncte = {
            {50.f, 50.f}, {750.f, 50.f},
            {400.f, 250.f},
            {50.f, 550.f}, {750.f, 550.f}
        };

        for (size_t k = 0; k < puncte.size() - 1; ++k) {
            Vec2f p1 = puncte[k];
            Vec2f p2 = puncte[k+1];
            float dist = (p2 - p1).magnitude();
            int pasi = static_cast<int>(dist / 2.0f);
            for (int i = 0; i < pasi; ++i) {
                float t = (float)i / pasi;
                traseu.push_back(p1 + (p2 - p1) * t);
            }
        }
    }
    else if (nivel == 7) {
        pozProiector = {400.f, 100.f};
        int pasi = 800;
        for (int i = 0; i < pasi; ++i) {
            float t = (float)i / pasi * 2.0f * PI;
            float x = 400.f + 350.f * std::cos(t);
            float y = 300.f + 150.f * std::sin(2.0f * t);
            traseu.push_back({x, y});
        }
    }
    else if (nivel == 8) {
        pozProiector = {400.f, 550.f};

        for(int row=0; row<5; ++row) {
            float y = 50.f + row * 100.f;
            if (row % 2 == 0) {
                for(int x=50; x<=750; x+=2) traseu.push_back({(float)x, y});
                for(int dy=0; dy<100; dy+=2) traseu.push_back({750.f, y + dy});
            } else {
                for(int x=750; x>=50; x-=2) traseu.push_back({(float)x, y});
                for(int dy=0; dy<100; dy+=2) traseu.push_back({50.f, y + dy});
            }
        }
    }
    else if (nivel == 9) {
        pozProiector = {400.f, 300.f};
        int pasi = 800;
        for (int i = 0; i < pasi; ++i) {
            float t = (float)i / pasi * 2.0f * PI;
            float k = 4.0f;
            float r = 300.f * std::cos(k * t);
            float x = 400.f + r * std::cos(t);
            float y = 300.f + r * std::sin(t);
            traseu.push_back({x, y});
        }
    }
    else {
        pozProiector = {400.f, 250.f};
        int pasi = 1000;
        for (int i = 0; i < pasi; ++i) {
            float t = (float)i / pasi * 2.0f * PI;
            float x = 400.f + 12.f * (16.f * std::pow(std::sin(t), 3));
            float y = 300.f - 12.f * (13.f * std::cos(t) - 5.f * std::cos(2*t) - 2.f * std::cos(3*t) - std::cos(4*t));
            traseu.push_back({x, y});
        }
    }

    proiector.setPozitie(pozProiector);
}

Nivel::Nivel()
    : sirBile(),
      proiector({400.f, 500.f}),
      scor(0),
      stare(StareJoc::RULEAZA),
      nivelCurent(1),
      modAccuracyActiv(false),
      timpRamasAccuracy(0.0f),
      esteInghetat(false),
      timpRamasInghet(0.0f)
{
    genereazaTraseu(1);
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

    genereazaTraseu(nivelCurent);

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
const std::vector<Vec2f>& Nivel::getTraseu() const { return traseu; }
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