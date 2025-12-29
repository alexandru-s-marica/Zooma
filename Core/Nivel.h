#pragma once
#include "SirDeBile.h"
#include "Proiector.h"
#include "utils.h"
#include <list>
#include <vector>
#include <iostream>

enum class StareJoc { RULEAZA, GAME_OVER, CASTIGAT };

struct AnimatieExplozie {
    Vec2f pozitie;
    float razaMaxima;
    float timer;
};

class Nivel {
private:
    SirDeBile sirBile;
    Proiector proiector;
    std::vector<Vec2f> traseu;
    std::list<std::pair<Bila, Vec2f>> proiectileInZbor;

    std::vector<AnimatieExplozie> exploziiVizuale;

    int scor;
    StareJoc stare;

    bool modAccuracyActiv;
    float timpRamasAccuracy;

    void genereazaTraseu();
    void gestioneazaColiziuni();

public:
    Nivel();

    explicit Nivel(float initialBallSpacing);

    void reset(float initialBallSpacing);
    void ruleazaFrame(float deltaTime);
    void adaugaProiectil(const Bila& p, const Vec2f& dir);
    Bila trageBilaJucator();

    const std::list<Bila>& getSirDeBile() const;
    Proiector& getProiector();
    const Proiector& getProiector() const;
    const std::list<std::pair<Bila, Vec2f>>& getProiectileInZbor() const;
    int getScor() const;
    StareJoc getStareJoc() const;
    bool esteTerminat() const;
    bool esteCastigat() const;
    bool esteAccuracyActiv() const { return modAccuracyActiv; }

    const std::vector<AnimatieExplozie>& getExploziiVizuale() const { return exploziiVizuale; }

    void activeazaExplozieLa(Vec2f pozitie);
    void activeazaRetro();
    void activeazaModAccuracy(float durata);

    friend std::ostream& operator<<(std::ostream& os, const Nivel& n);
};