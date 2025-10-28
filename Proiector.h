#pragma once
#include "Bila.h"

class Proiector {
private:
    Bila bilaCurenta;
    Bila bilaUrmatoare;
    Point pozitieFixa;

    Bila genereazaBilaNoua();

public:
    explicit Proiector(Point p = {0, 0});
    Bila trageBila();
    void incarcaBilaUrmatoare();

    friend std::ostream& operator<<(std::ostream& os, const Proiector& p);
};