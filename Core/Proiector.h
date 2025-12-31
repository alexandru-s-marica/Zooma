#pragma once
#include "Bila.h"
#include <vector>

class Proiector {
private:
    Vec2f pozitie;
    float unghi;
    Bila bilaCurenta;
    Bila bilaUrmatoare;

    void genereazaBilaUrmatoare(const std::vector<Culoare>& culoriDisponibile);

public:
    explicit Proiector(Vec2f pos);

    void rotesteSpre(Vec2f tinta);
    Bila trage(const std::vector<Culoare>& culoriDisponibile);
    void schimbaBila();
    void valideazaCulori(const std::vector<Culoare>& culoriActive);

    void setPozitie(Vec2f pos);

    Vec2f getPozitie() const;
    const Bila& getBilaCurenta() const;
    const Bila& getBilaUrmatoare() const;
};