#pragma once
#include "Bila.h"
#include <list>
#include <vector>
#include <set>

class SirDeBile {
private:
    std::list<Bila> bile;
    std::vector<Vec2f> traseu;
    float viteza;
    float progresCapSnake;
    float distantaIntreBile;
    float lungimeTotalaTraseu;

    Vec2f getPozitiePeTraseu(float progres) const;

    void verificaExplozieLant(std::list<Bila>::iterator stanga, std::list<Bila>::iterator dreapta);

public:

    SirDeBile();
    SirDeBile(std::vector<Vec2f> traseu, float viteza, float distanta);

    SirDeBile(const SirDeBile& other);
    SirDeBile& operator=(const SirDeBile& other);
    ~SirDeBile();

    void actualizeaza(float deltaTime);
    std::list<Bila>& getBile();
    const std::list<Bila>& getBile() const;

    int insereazaSiVerifica(std::list<Bila>::iterator it_target, const Bila& bilaNoua);

    bool aAtingJucatorulSfarsitul() const;

    friend std::ostream& operator<<(std::ostream&, const SirDeBile&);

    std::vector<Culoare> getCuloriActive() const;
};

//std::ostream& operator<<(std::ostream&, const SirDeBile&);
