#pragma once
#include "utils.h"

class Bila
{
private:
    Culoare culoare;
    Point pozitie;
    float distantaPeTraseu;

public:
    Bila(Culoare c, Point p, float dist = 0.0f);
    Bila();

    Culoare getCuloare() const;
    float getDistanta() const;

    void setPozitie(Point p);
    void setDistanta(float d);

    friend std::ostream& operator<<(std::ostream& os, const Bila& b);
};