#pragma once
#include "utils.h"

class Bila {
private:
    Culoare culoare;
    Vec2f pozitie;
    float raza;

public:
    Bila(Culoare c, Vec2f pos, float r);

    void setPozitie(Vec2f pos);
    Vec2f getPozitie() const;
    Culoare getCuloare() const;
    float getRaza() const;

    Bounds getBounds() const;

    std::ostream& operator<<(std::ostream& os) const;
};

inline std::ostream& operator<<(std::ostream& os, const Bila& b) {
    return b.operator<<(os);
}