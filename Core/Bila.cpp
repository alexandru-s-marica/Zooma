#include "Bila.h"
#include "Efecte.h"
#include "Nivel.h"

int Bila::bileDistruseTotal = 0;

Bila::Bila() : culoare(Culoare::UNKNOWN), pozitie(0,0), raza(15.f), progresPeTraseu(0.f),
               inDistrugere(false), gataDeSters(false), efect(nullptr) {}

Bila::Bila(Culoare c, Vec2f pos, float r, float progres)
    : culoare(c), pozitie(pos), raza(r), progresPeTraseu(progres),
      inDistrugere(false), gataDeSters(false), efect(nullptr) {}

Bila::Bila(const Bila& other)
    : culoare(other.culoare), pozitie(other.pozitie), raza(other.raza),
      progresPeTraseu(other.progresPeTraseu), inDistrugere(other.inDistrugere),
      gataDeSters(other.gataDeSters) {
    if (other.efect) {
        efect = other.efect->clone();
    } else {
        efect = nullptr;
    }
}

Bila& Bila::operator=(const Bila& other) {
    if (this != &other) {
        culoare = other.culoare;
        pozitie = other.pozitie;
        raza = other.raza;
        progresPeTraseu = other.progresPeTraseu;
        inDistrugere = other.inDistrugere;
        gataDeSters = other.gataDeSters;
        if (other.efect) {
            efect = other.efect->clone();
        } else {
            efect = nullptr;
        }
    }
    return *this;
}

Bila::~Bila() = default;

void Bila::actualizeaza(float deltaTime) {
    if (inDistrugere) {
        raza -= 30.f * deltaTime;
        if (raza < 0.f) {
            raza = 0.f;
            gataDeSters = true;
        }
    }
}

void Bila::activeazaEfect(Nivel& nivel) {
    if (!efect) return;

    if (dynamic_cast<EfectExplozie*>(efect.get())) {
        nivel.activeazaExplozieLa(pozitie);
    }
    else {
        efect->activeazaEfect(nivel);
    }

    efect = nullptr;
}

void Bila::adaugaEfect(std::unique_ptr<EfectBila> e) {
    efect = std::move(e);
}

bool Bila::areEfect() const { return efect != nullptr; }
Culoare Bila::getCuloare() const { return culoare; }
Vec2f Bila::getPozitie() const { return pozitie; }
float Bila::getRaza() const { return raza; }
float Bila::getProgres() const { return progresPeTraseu; }
bool Bila::esteInDistrugere() const { return inDistrugere; }
bool Bila::eGataDeSters() const { return gataDeSters; }
Bounds Bila::getBounds() const { return {pozitie, raza}; }

void Bila::setPozitie(Vec2f p) { pozitie = p; }
void Bila::setProgres(float p) { progresPeTraseu = p; }
void Bila::marcheazaPentruDistrugere() { inDistrugere = true; }
void Bila::avanseaza(float distanta) { progresPeTraseu += distanta; }

int Bila::getBileDistruseTotal() { return bileDistruseTotal; }
void Bila::incrementeazaBileDistruse() { bileDistruseTotal++; }

float Bila::getScaleVisual() const { return 1.0f; }