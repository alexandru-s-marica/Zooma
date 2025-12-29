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
    bool seDistruge;
    float timpPanaLaMoarte;

    float progresPeTraseu;
    float timerViataEfect;

    std::unique_ptr<EfectBila> efect;
    static int bileDistruseTotal;

public:
    Bila(Culoare c, Vec2f pos, float r, float progres = 0.0f);
    Bila(Culoare c, Vec2f pos, float r, float progres, std::unique_ptr<EfectBila> efectInitial);

    Bila(const Bila& other);
    Bila& operator=(Bila other);
    friend void swap(Bila& first, Bila& second) noexcept;
    ~Bila();

    void setPozitie(Vec2f pos);
    Vec2f getPozitie() const;

    void setProgres(float p);
    float getProgres() const;
    void avanseaza(float deltaDistanta);

    Culoare getCuloare() const;
    float getRaza() const;
    Bounds getBounds() const;

    void marcheazaPentruDistrugere();
    bool esteInDistrugere() const;

    void actualizeaza(float deltaTime);
    bool eGataDeSters() const;

    void activeazaEfect(Nivel& nivel);
    bool areEfect() const;
    void stergeEfect();
    void adaugaEfect(std::unique_ptr<EfectBila> efectNou);

    static void incrementeazaBileDistruse();
    static int getBileDistruseTotal();

    friend std::ostream& operator<<(std::ostream& os, const Bila& b);
};