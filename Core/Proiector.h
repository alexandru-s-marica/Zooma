#pragma once
#include "Bila.h"
#include <random>

class Proiector {
private:
    Vec2f pozitie;
    Bila bilaCurenta;
    Bila bilaUrmatoare;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distributieCuloare;

    void genereazaBilaNoua(const std::vector<Culoare>& culoriPermise = {});

public:
    explicit Proiector(Vec2f pos);

    Bila trage(const std::vector<Culoare>& culoriPermise);
    void schimbaBila();
    void rotesteSpre(Vec2f tinta);
    Vec2f getPozitie() const;

    const Bila& getBilaCurenta() const;
    const Bila& getBilaUrmatoare() const;

    void valideazaCulori(const std::vector<Culoare>& culoriPermise);

    friend std::ostream& operator<<(std::ostream& os, const Proiector& p);
};