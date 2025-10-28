#include "Nivel.h"

Nivel::Nivel(Point pozProiector, float lungime)
    : proiector(pozProiector), sirBile(), scor(0), lungimeTraseu(lungime) {
}

void Nivel::proceseazaComanda(const std::string& comanda, float valoare) {
    if (comanda == "trage") {
        std::cout << "--- Actiune: Trage la distanta " << valoare << " ---\n";
        Bila bilaTrasa = proiector.trageBila();
        int scorAdaugat = sirBile.insereazaBila(bilaTrasa, valoare);
        this->scor += scorAdaugat;
    }
    else if (comanda == "avanseaza") {
        std::cout << "--- Actiune: Avanseaza " << valoare << " unitati ---\n";
        sirBile.avanseazaBilele(valoare);
    }
}

int Nivel::getScor() const {
    return scor;
}

bool Nivel::isTerminat() const {
    return sirBile.aAjunsLaFinal(lungimeTraseu) || sirBile.eGol();
}

SirDeBile& Nivel::getSirDeBile() {
    return sirBile;
}

void Nivel::adaugaBileStart()
{
    sirBile.adaugaBilaInitiala(Bila(Culoare::ROSU, {0,0}, 10.0f));
    sirBile.adaugaBilaInitiala(Bila(Culoare::ROSU, {0,0}, 11.0f));
    sirBile.adaugaBilaInitiala(Bila(Culoare::VERDE, {0,0}, 12.0f));
    sirBile.adaugaBilaInitiala(Bila(Culoare::VERDE, {0,0}, 13.0f));
    sirBile.adaugaBilaInitiala(Bila(Culoare::ALBASTRU, {0,0}, 14.0f));
    sirBile.adaugaBilaInitiala(Bila(Culoare::ROSU, {0,0}, 15.0f));
    sirBile.adaugaBilaInitiala(Bila(Culoare::ROSU, {0,0}, 16.0f));
}

std::ostream& operator<<(std::ostream& os, const Nivel& n) {
    os << "\n=================== STARE JOC ===================\n";
    os << "SCOR: " << n.scor << "\n";
    os << "Lungime Traseu: " << n.lungimeTraseu << "\n";
    os << "Joc terminat: " << (n.isTerminat() ? "DA" : "NU") << "\n\n";

    os << n.proiector << "\n\n";
    os << n.sirBile << "\n";
    os << "=================================================\n";
    return os;
}