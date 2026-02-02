#include "Nivel.h"
#include "Exceptions.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>

const float PI = 3.14159265f;

float calculeazaVitezaNivel(int nivel) {
    return 60.0f + (nivel - 1) * 20.0f;
}

float calculeazaDistantaBile(int nivel) {
    (void)nivel;
    return 40.0f;
}

void Nivel::genereazaTraseu(int nivel) {
    std::string numeFisier = "level_" + std::to_string(nivel) + ".txt";

    if (!incarcaTraseuDinFisier(numeFisier)) {
        std::cout << "[Nivel] Fisierul " << numeFisier << " nu exista. Se genereaza procedural...\n";
        genereazaTraseuProcedural(nivel);
        salveazaTraseuInFisier(numeFisier);
    } else {
        std::cout << "[Nivel] S-a incarcat traseul din " << numeFisier << "\n";
    }
}

bool Nivel::incarcaTraseuDinFisier(const std::string& numeFisier) {
    std::ifstream in(numeFisier);
    if (!in.is_open()) return false;

    traseu.clear();

    // Format fisier:
    // Linie 1: ProiectorX ProiectorY
    // Linie 2+: PunctX PunctY

    float px, py;
    if (in >> px >> py) {
        proiector.setPozitie({px, py});
    } else {
        return false;
    }

    float tx, ty;
    while (in >> tx >> ty) {
        traseu.push_back({tx, ty});
    }

    in.close();
    return !traseu.empty();
}

void Nivel::salveazaTraseuInFisier(const std::string& numeFisier) {
    std::ofstream out(numeFisier);
    if (!out.is_open()) {
        std::cerr << "[Eroare] Nu s-a putut salva nivelul in " << numeFisier << "\n";
        return;
    }

    Vec2f posP = proiector.getPozitie();
    out << posP.x << " " << posP.y << "\n";

    for (const auto& p : traseu) {
        out << p.x << " " << p.y << "\n";
    }

    out.close();
    std::cout << "[Nivel] Salvat configuratia in " << numeFisier << "\n";
}

void Nivel::genereazaTraseuProcedural(int nivel) {
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
    else if (nivel == 3) {
        pozProiector = {100.f, 550.f};
        std::vector<Vec2f> pts = {{50.f, 50.f}, {750.f, 50.f}, {50.f, 200.f}, {750.f, 200.f}, {50.f, 350.f}, {750.f, 350.f}, {50.f, 500.f}, {750.f, 500.f}};
        for (size_t k = 0; k < pts.size() - 1; ++k) {
            Vec2f p1 = pts[k]; Vec2f p2 = pts[k+1]; float dist = (p2 - p1).magnitude(); int pasi = static_cast<int>(dist / 2.0f);
            for (int i = 0; i < pasi; ++i) traseu.push_back(p1 + (p2 - p1) * ((float)i / pasi));
        }
    }
    else if (nivel == 4) {
        pozProiector = {400.f, 100.f};
        for (int i = 0; i < 700; ++i) {
            float t = (float)i / 700;
            traseu.push_back({50.f + t * 700.f, 300.f + 200.f * std::sin(t * 4.0f * PI)});
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
        std::vector<Vec2f> pts = {{50.f, 50.f}, {750.f, 50.f}, {400.f, 250.f}, {50.f, 550.f}, {750.f, 550.f}};
        for (size_t k = 0; k < pts.size() - 1; ++k) {
            Vec2f p1 = pts[k]; Vec2f p2 = pts[k+1]; float dist = (p2 - p1).magnitude(); int pasi = static_cast<int>(dist / 2.0f);
            for (int i = 0; i < pasi; ++i) traseu.push_back(p1 + (p2 - p1) * ((float)i / pasi));
        }
    }
    else if (nivel == 7) {
        pozProiector = {400.f, 100.f};
        for (int i = 0; i < 800; ++i) {
            float t = (float)i / 800 * 2.0f * PI;
            traseu.push_back({400.f + 350.f * std::cos(t), 300.f + 150.f * std::sin(2.0f * t)});
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
        for (int i = 0; i < 800; ++i) {
            float t = (float)i / 800 * 2.0f * PI; float r = 300.f * std::cos(4.0f * t);
            traseu.push_back({400.f + r * std::cos(t), 300.f + r * std::sin(t)});
        }
    }
    else {
        // Nivel 10 (si fallback pentru generare)
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
    : sirBile(), proiector({400.f, 500.f}),
      scor(0),
      timerAccuracy(0.0f),
      highScore(0),
      stare(StareJoc::RULEAZA), nivelCurent(1),
      modAccuracyActiv(false)
{
    try {
        incarcaHighScore();
    } catch (const FisierCoruptException& e) {
        std::cerr << e.what() << std::endl;
        highScore = 0;
    }

    incarcaNivel(1);
}

void Nivel::incarcaHighScore() {
    std::ifstream in("highscore.txt");
    if (!in.is_open()) { highScore = 0; return; }
    if (!(in >> highScore)) throw FisierCoruptException("Nu s-a putut citi scorul integer.");
    in.close();
}

void Nivel::salveazaHighScore() {
    std::ofstream out("highscore.txt");
    if (out.is_open()) { out << highScore; out.close(); }
}

void Nivel::incarcaNivel(int numarNivel) {
    if (numarNivel > 10) {
        stare = StareJoc::VICTORIE_FINALA;
        std::cout << "--- FELICITARI! AI TERMINAT TOATE NIVELELE! ---\n";
        return;
    }

    nivelCurent = numarNivel;
    reset();
    std::cout << "--- INCARCARE NIVEL " << nivelCurent << " ---\n";
}

void Nivel::reset() {
    stare = StareJoc::RULEAZA;
    modAccuracyActiv = false;
    timerAccuracy.reset(0.0f);

    proiectileInZbor.clear();
    exploziiVizuale.clear();

    genereazaTraseu(nivelCurent);

    sirBile = SirDeBile(traseu, calculeazaVitezaNivel(nivelCurent), calculeazaDistantaBile(nivelCurent));
    std::vector<Culoare> culori = sirBile.getCuloriActive();
    if (!culori.empty()) proiector.valideazaCulori(culori);
}

void Nivel::ruleazaFrame(float deltaTime) {
    if (stare == StareJoc::VICTORIE_FINALA) return;
    if (stare != StareJoc::RULEAZA) return;

    float vitezaProiectil = 1000.f;
    if (modAccuracyActiv) vitezaProiectil = 2500.f;

    for (auto& proiectil : proiectileInZbor) {
        proiectil.first.setPozitie(proiectil.first.getPozitie() + proiectil.second * vitezaProiectil * deltaTime);
    }

    if (modAccuracyActiv) {
        timerAccuracy.scade(deltaTime);

        float timpSecurizat = clampValoare(timerAccuracy.get(), 0.0f, 100.0f);

        if (timpSecurizat <= 0.001f) {
            modAccuracyActiv = false;
            timerAccuracy.reset(0.0f);
        }
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

        float clampedX = clampValoare(pos.x, 0.0f, (float)SCREEN_WIDTH);
        float clampedY = clampValoare(pos.y, 0.0f, (float)SCREEN_HEIGHT);

        return (pos.x != clampedX || pos.y != clampedY);
    });

    gestioneazaColiziuni();

    if (sirBile.aAtingJucatorulSfarsitul()) {
        stare = StareJoc::GAME_OVER;
        if (scor.get() > highScore) { highScore = scor.get(); salveazaHighScore(); }
    } else if (sirBile.getBile().empty() && stare == StareJoc::RULEAZA) {
        stare = StareJoc::CASTIGAT;
        if (scor.get() > highScore) { highScore = scor.get(); salveazaHighScore(); }
    }
}

void Nivel::gestioneazaColiziuni() {
    auto it_proiectil = proiectileInZbor.begin();
    while (it_proiectil != proiectileInZbor.end()) {
        bool coliziuneDetectata = false;
        for (auto it_bila_sir = sirBile.getBile().begin(); it_bila_sir != sirBile.getBile().end(); ++it_bila_sir) {
            if (it_proiectil->first.getBounds().findIntersection(it_bila_sir->getBounds())) {
                int puncte = sirBile.insereazaSiVerifica(it_bila_sir, it_proiectil->first);

                puncte = clampValoare(puncte, 0, 1000);

                scor.adauga(puncte);

                it_proiectil = proiectileInZbor.erase(it_proiectil);
                coliziuneDetectata = true; break;
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

int Nivel::getScor() const {
    return scor.get();
}

int Nivel::getHighScore() const { return highScore; }
const std::vector<Vec2f>& Nivel::getTraseu() const { return traseu; }
StareJoc Nivel::getStareJoc() const { return stare; }
bool Nivel::esteTerminat() const { return stare == StareJoc::GAME_OVER; }
bool Nivel::esteCastigat() const { return stare == StareJoc::CASTIGAT; }

bool Nivel::esteVictorieFinala() const { return stare == StareJoc::VICTORIE_FINALA; }

std::ostream& operator<<(std::ostream& os, const Nivel& n) {
    os << "Nivel " << n.nivelCurent << " (Scor: " << n.scor.get() << ")\n";
    return os;
}

Bila Nivel::trageBilaJucator() {
    std::vector<Culoare> c = sirBile.getCuloriActive();
    return proiector.trage(c);
}

void Nivel::activeazaExplozieLa(Vec2f pozitie, float raza) {
    sirBile.explodeazaZona(pozitie, raza);
    exploziiVizuale.push_back({pozitie, raza, 1.0f});
}

void Nivel::activeazaRetro(float distanta) {
    sirBile.aplicaRetro(distanta);
}

void Nivel::activeazaModAccuracy(float durata) {
    modAccuracyActiv = true;
    timerAccuracy.setValoare(durata);
}

void Nivel::adaugaScor(int valoare) {
    scor.adauga(valoare);
}