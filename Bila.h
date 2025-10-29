#pragma once
#include "utils.h"

class Bila
{
private:
    Culoare culoare;
    Point pozitie;
    float distantaPeTraseu;
    TipPowerUp efectSpecial; // ATRIBUTUL NOU

public:
    Bila(Culoare c, Point p, float dist = 0.0f);
    Bila();

    Culoare getCuloare() const;
    float getDistanta() const;
    void setDistanta(float d);

    // Functii noi pentru efecte
    TipPowerUp getEfect() const;
    void aplicaEfect(TipPowerUp tip);

    friend std::ostream& operator<<(std::ostream& os, const Bila& b);
};