#pragma once
#include <vector>
#include <list>
#include "Bila.h"
#include "utils.h"

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

    void curataBileDistruse(Nivel& nivel);
    void proceseazaMiscare(float deltaTime);
    void aplicaMiscareRetro(float deltaTime);
    void aplicaMiscareNormala(float deltaTime);
    void genereazaEfecteRandom(float deltaTime);

public:
    SirDeBile();
    SirDeBile(std::vector<Vec2f> traseu, float viteza, float distanta);

    void actualizeaza(float deltaTime, Nivel& nivel);
    void explodeazaZona(Vec2f centru, float raza);
    void aplicaRetro(float distantaInapoi);

    std::vector<Culoare> getCuloriActive() const;
    std::list<Bila>& getBile();
    const std::list<Bila>& getBile() const;

    int insereazaSiVerifica(std::list<Bila>::iterator it_target, const Bila& bilaNoua);
    bool aAtingJucatorulSfarsitul() const;

    friend std::ostream& operator<<(std::ostream& os, const SirDeBile& sirBile);
};