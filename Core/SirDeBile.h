#pragma once
#include "Bila.h"
#include <list>
#include <vector>
#include <set>
#include <iostream>

class Nivel;

class SirDeBile {
private:
    std::list<Bila> bile;
    std::vector<Vec2f> traseu;
    float viteza;

    float distantaIntreBile;
    float lungimeTotalaTraseu;

    float distantaRetroRamasa;

    float timerGenerareEfectRandom;

    Vec2f getPozitiePeTraseu(float progres) const;
    void verificaExplozieLant(std::list<Bila>::iterator stanga, std::list<Bila>::iterator dreapta);

public:
    SirDeBile();
    SirDeBile(std::vector<Vec2f> traseu, float viteza, float distanta);

    SirDeBile(const SirDeBile& other) = default;
    SirDeBile& operator=(const SirDeBile& other) = default;
    ~SirDeBile() = default;

    void actualizeaza(float deltaTime, Nivel& nivel);

    std::vector<Culoare> getCuloriActive() const;
    std::list<Bila>& getBile();
    const std::list<Bila>& getBile() const;

    int insereazaSiVerifica(std::list<Bila>::iterator it_target, const Bila& bilaNoua);
    bool aAtingJucatorulSfarsitul() const;

    void explodeazaZona(Vec2f centru, float raza);
    void aplicaRetro(float distantaInapoi);

    friend std::ostream& operator<<(std::ostream& os, const SirDeBile& s);
};