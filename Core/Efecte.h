#pragma once
#include <memory>
#include "utils.h"

class Nivel;

class EfectBila {
public:
    virtual ~EfectBila() = default;
    void activeazaEfect(Nivel& nivel);
    virtual std::unique_ptr<EfectBila> clone() const = 0;
protected:
    virtual void doAplica(Nivel& nivel) = 0;
};

class EfectExplozie : public EfectBila {
protected:
    void doAplica(Nivel& nivel) override;
public:
    std::unique_ptr<EfectBila> clone() const override;
};

class EfectRetro : public EfectBila {
protected:
    void doAplica(Nivel& nivel) override;
public:
    std::unique_ptr<EfectBila> clone() const override;
};

class EfectAccuracy : public EfectBila {
protected:
    void doAplica(Nivel& nivel) override;
public:
    std::unique_ptr<EfectBila> clone() const override;
};

class EfectBonusScor : public EfectBila {
protected:
    void doAplica(Nivel& nivel) override;
public:
    std::unique_ptr<EfectBila> clone() const override;
};