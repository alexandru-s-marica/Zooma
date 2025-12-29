#pragma once
#include <exception>
#include <string>

class ZoomaException : public std::exception {
protected:
    std::string mesaj;
public:
    explicit ZoomaException(const std::string& msg) : mesaj(msg) {}

    const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class ResursaLipsaException : public ZoomaException {
public:
    explicit ResursaLipsaException(const std::string& numeResursa)
        : ZoomaException("EROARE CRITICA RESURSA LIPSA: " + numeResursa) {}
};

class LogicaJocException : public ZoomaException {
public:
    explicit LogicaJocException(const std::string& detaliu)
        : ZoomaException("EROARE LOGICA: " + detaliu) {}
};

class InitializareEsuataException : public ZoomaException {
public:
    explicit InitializareEsuataException(const std::string& componenta)
        : ZoomaException("INIT FAIL - NU S-A PUTUT INITIALIZA: " + componenta) {}
};