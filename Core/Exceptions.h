#pragma once
#include <exception>
#include <string>

class ZoomaException : public std::exception {
protected:
    std::string mesaj;
public:
    explicit ZoomaException(const std::string& m) : mesaj(m) {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

class ResursaLipsaException : public ZoomaException {
public:
    explicit ResursaLipsaException(const std::string& nume)
        : ZoomaException("CRITIC: Resursa lipsa -> " + nume) {}
};

class FisierCoruptException : public ZoomaException {
public:
    explicit FisierCoruptException(const std::string& detalii)
        : ZoomaException("WARN: Fisier corupt sau inaccesibil -> " + detalii) {}
};

class LogicaJocException : public ZoomaException {
public:
    explicit LogicaJocException(const std::string& detalii)
        : ZoomaException("LOGIC: Stare invalida -> " + detalii) {}
};