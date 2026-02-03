#pragma once
#include <iostream>

template <typename T>
class Statistica {
private:
    T valoareCurenta;
    T valoareMaxima;

public:
    explicit Statistica(T valoareInitiala = 0)
        : valoareCurenta(valoareInitiala), valoareMaxima(valoareInitiala) {}

    void setValoare(T val) {
        valoareCurenta = val;
        if (valoareCurenta > valoareMaxima) {
            valoareMaxima = valoareCurenta;
        }
    }

    void adauga(T delta) {
        valoareCurenta += delta;
        if (valoareCurenta > valoareMaxima) {
            valoareMaxima = valoareCurenta;
        }
    }

    void scade(T delta) {
        valoareCurenta -= delta;
        if (valoareCurenta < 0) valoareCurenta = 0;
    }

    T get() const { return valoareCurenta; }
    T getMax() const { return valoareMaxima; }

    void reset(T val) {
        valoareCurenta = val;
    }
};