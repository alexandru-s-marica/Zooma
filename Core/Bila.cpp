#include "Bila.h"
#include "Efecte.h"
#include <iostream>

int Bila::bileDistruseTotal = 0;

Bila::Bila(Culoare c, Vec2f pos, float r, float progres)
    : culoare(c), pozitie(pos), raza(r),
      seDistruge(false), timpPanaLaMoarte(0.0f),
      progresPeTraseu(progres),
      timerViataEfect(15.0f), // 15 secunde default
      efect(nullptr)
{}

Bila::Bila(Culoare c, Vec2f pos, float r, float progres, std::unique_ptr<EfectBila> efectInitial)
    : culoare(c), pozitie(pos), raza(r),
      seDistruge(false), timpPanaLaMoarte(0.0f),
      progresPeTraseu(progres),
      timerViataEfect(15.0f),
      efect(std::move(efectInitial))
{}

Bila::Bila(const Bila& other)
    : culoare(other.culoare), pozitie(other.pozitie), raza(other.raza),
      seDistruge(other.seDistruge), timpPanaLaMoarte(other.timpPanaLaMoarte),
      progresPeTraseu(other.progresPeTraseu),
      timerViataEfect(other.timerViataEfect)
{
    if (other.efect) {
        efect = other.efect->clone();
    } else {
        efect = nullptr;
    }
}

Bila::~Bila() = default;

void swap(Bila& first, Bila& second) noexcept {
    using std::swap;
    swap(first.culoare, second.culoare);
    swap(first.pozitie, second.pozitie);
    swap(first.raza, second.raza);
    swap(first.seDistruge, second.seDistruge);
    swap(first.timpPanaLaMoarte, second.timpPanaLaMoarte);
    swap(first.efect, second.efect);
    swap(first.progresPeTraseu, second.progresPeTraseu);
    swap(first.timerViataEfect, second.timerViataEfect);
}

Bila& Bila::operator=(Bila other) {
    swap(*this, other);
    return *this;
}

void Bila::setPozitie(Vec2f pos) { pozitie = pos; }
Vec2f Bila::getPozitie() const { return pozitie; }
void Bila::setProgres(float p) { progresPeTraseu = p; }
float Bila::getProgres() const { return progresPeTraseu; }
void Bila::avanseaza(float deltaDistanta) { progresPeTraseu += deltaDistanta; }

Culoare Bila::getCuloare() const { return culoare; }
float Bila::getRaza() const { return raza; }
Bounds Bila::getBounds() const { return {pozitie, raza}; }

void Bila::marcheazaPentruDistrugere() {
    if (!seDistruge) {
        seDistruge = true;
        timpPanaLaMoarte = 0.3f;
    }
}

bool Bila::esteInDistrugere() const { return seDistruge; }

void Bila::actualizeaza(float deltaTime) {
    if (seDistruge) {
        timpPanaLaMoarte -= deltaTime;
    }

    if (efect && !seDistruge) {
        timerViataEfect -= deltaTime;
        if (timerViataEfect <= 0.0f) {
            stergeEfect();
        }
    }
}

bool Bila::eGataDeSters() const {
    return seDistruge && (timpPanaLaMoarte <= 0.f);
}

void Bila::activeazaEfect(Nivel& nivel) {
    if (efect) {
        efect->activeaza(nivel, pozitie);
    }
}

bool Bila::areEfect() const {
    return (efect != nullptr);
}

void Bila::stergeEfect() {
    efect.reset();
}

void Bila::adaugaEfect(std::unique_ptr<EfectBila> efectNou) {
    efect = std::move(efectNou);
    timerViataEfect = 15.0f;
}

void Bila::incrementeazaBileDistruse() {
    bileDistruseTotal++;
}

int Bila::getBileDistruseTotal() {
    return bileDistruseTotal;
}

std::ostream& operator<<(std::ostream& os, const Bila& b) {
    os << "Bila (Culoare: " << b.culoare
       << ", Pozitie: " << b.pozitie
       << ", Raza: " << b.raza << ")";
    return os;
}