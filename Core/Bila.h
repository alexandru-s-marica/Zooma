#pragma once
#include "utils.h"
#include <memory>

class EfectBila;
class Nivel;

class Bila {
private:
    Culoare culoare;
    Vec2f pozitie;
    float raza;
    float progresPeTraseu;

    bool inDistrugere;
    bool gataDeSters;

    std::unique_ptr<EfectBila> efect;
    static int bileDistruseTotal;

public:
    Bila();
    Bila(Culoare c, Vec2f pos, float r, float progres = 0.f);


    Bila(const Bila& other);

    Bila& operator=(const Bila& other);

    ~Bila();

    void actualizeaza(float deltaTime);
    void activeazaEfect(Nivel& nivel);
    void adaugaEfect(std::unique_ptr<EfectBila> e);

    bool areEfect() const;
    Culoare getCuloare() const;
    Vec2f getPozitie() const;
    float getRaza() const;
    float getProgres() const;

    bool esteInDistrugere() const;
    bool eGataDeSters() const;

    Bounds getBounds() const;

    void setPozitie(Vec2f p);
    void setProgres(float p);
    void marcheazaPentruDistrugere();
    void avanseaza(float distanta);

    static int getBileDistruseTotal();
    static void incrementeazaBileDistruse();

    float getScaleVisual() const;
};