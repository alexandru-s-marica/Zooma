#pragma once
#include "utils.h"
#include <iostream>

class Bila {
private:
    Culoare culoare;
    Vec2f pozitie;
    float raza;

public:
    //Bila()=default;
    //am folosit doar pentru testul de afisare

    Bila(Culoare c, Vec2f pos, float r);

    void setPozitie(Vec2f pos);
    Vec2f getPozitie() const;
    Culoare getCuloare() const;
    float getRaza() const;

    Bounds getBounds() const;

    friend std::ostream& operator<<(std::ostream& os, const Bila& b);
};