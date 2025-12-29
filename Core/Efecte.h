#pragma once
#include "utils.h"
#include <iostream>
#include <memory>
#include <string>

class Nivel;

class EfectBila {
protected:
    std::string numeEfect;
    explicit EfectBila(std::string nume) : numeEfect(std::move(nume)) {}

    virtual void doAplica(Nivel& nivel, Vec2f pozitie) = 0;

public:
    virtual ~EfectBila() = default;
    virtual std::unique_ptr<EfectBila> clone() const = 0;

    const std::string& getNume() const { return numeEfect; }

    void activeaza(Nivel& nivel, Vec2f pozitie) {
        std::cout << "[Efect] Activare: " << getNume() << " la " << pozitie << "\n";
        doAplica(nivel, pozitie);
    }
};

class EfectExplozie : public EfectBila {
public:
    EfectExplozie() : EfectBila("Explozie (Boom)") {}
    std::unique_ptr<EfectBila> clone() const override {
        return std::make_unique<EfectExplozie>(*this);
    }
protected:
    void doAplica(Nivel& nivel, Vec2f pozitie) override;
};

class EfectRetro : public EfectBila {
public:
    EfectRetro() : EfectBila("Retro (Time Back)") {}
    std::unique_ptr<EfectBila> clone() const override {
        return std::make_unique<EfectRetro>(*this);
    }
protected:
    void doAplica(Nivel& nivel, Vec2f pozitie) override;
};

class EfectAccuracy : public EfectBila {
    float durata;
public:
    EfectAccuracy() : EfectBila("Accuracy (Laser Sight)"), durata(5.0f) {}
    std::unique_ptr<EfectBila> clone() const override {
        return std::make_unique<EfectAccuracy>(*this);
    }
protected:
    void doAplica(Nivel& nivel, Vec2f pozitie) override;
};