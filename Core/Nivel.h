#pragma once
#include "SirDeBile.h"
#include "Proiector.h"
#include "utils.h"
#include <list>
#include <vector>
#include <iostream>
#include <fstream>

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
    int highScore;
    StareJoc stare;

    int nivelCurent;

    bool modAccuracyActiv;
    float timpRamasAccuracy;

    void genereazaTraseu(int nivel);
    void gestioneazaColiziuni();
    void incarcaHighScore();
    void salveazaHighScore();

public:
    Nivel();

    void incarcaNivel(int numarNivel);
    void reset();

    void ruleazaFrame(float deltaTime);
    void adaugaProiectil(const Bila& p, const Vec2f& dir);
    Bila trageBilaJucator();

    const std::list<Bila>& getSirDeBile() const;
    Proiector& getProiector();
    const Proiector& getProiector() const;
    const std::list<std::pair<Bila, Vec2f>>& getProiectileInZbor() const;

    int getScor() const;
    int getHighScore() const;
    int getNivelCurent() const { return nivelCurent; }
    const std::vector<Vec2f>& getTraseu() const;

    StareJoc getStareJoc() const;
    bool esteTerminat() const;
    bool esteCastigat() const;

    bool esteAccuracyActiv() const { return modAccuracyActiv; }
    const std::vector<AnimatieExplozie>& getExploziiVizuale() const { return exploziiVizuale; }

    void activeazaExplozieLa(Vec2f pozitie, float raza);
    void activeazaRetro(float distanta);
    void activeazaModAccuracy(float durata);
    void adaugaScor(int valoare);

    friend std::ostream& operator<<(std::ostream& os, const Nivel& n);
};