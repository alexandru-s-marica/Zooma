#pragma once
#include "Bila.h"
#include <list>
#include <vector>

class SirDeBile {
private:
    std::list<Bila> bile;
    std::vector<Vec2f> traseu;
    float viteza;
    float progresCapSnake;
    float distantaIntreBile;
    float lungimeTotalaTraseu;

    Vec2f getPozitiePeTraseu(float progres) const;

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
};

std::ostream& operator<<(std::ostream&, const SirDeBile&);
