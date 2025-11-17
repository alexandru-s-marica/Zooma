#pragma once
#include "SirDeBile.h"
#include "Proiector.h"
#include <vector>

enum class StareJoc {
    RULEAZA,
    GAME_OVER
};

class Nivel {
private:
    std::vector<Vec2f> traseu;
    SirDeBile sirBile;
    Proiector proiector;

    std::list<std::pair<Bila, Vec2f>> proiectileInZbor;

    int scor;
    StareJoc stare;

    void genereazaTraseu();

public:
    Nivel();
    explicit Nivel(float initialBallSpacing);

    void ruleazaFrame(float deltaTime);
    void adaugaProiectil(const Bila& p, const Vec2f& dir);
    void gestioneazaColiziuni();

    const std::list<Bila>& getSirDeBile() const;
    Proiector& getProiector();
    const Proiector& getProiector() const;
    const std::list<std::pair<Bila, Vec2f>>& getProiectileInZbor() const;
    int getScor() const;

    StareJoc getStareJoc() const;
    bool esteTerminat() const;

    friend std::ostream& operator<<(std::ostream& os, const Nivel& n);
};