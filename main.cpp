#include "Nivel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main()
{
    // Am scos initializarea rand()

    Nivel joc({50, 50}, 25.0f);
    joc.adaugaBileStart();

    std::cout << "Stare initiala joc:" << std::endl;
    std::cout << joc << std::endl;

    std::ifstream f("tastatura.txt");
    if (!f.is_open()) {
        std::cerr << "EROARE: Nu am putut deschide tastatura.txt!" << std::endl;
        return 1;
    }

    std::string linie, comanda;
    float valoare;

    std::cout << "\n--- Incepe citirea comenzilor din tastatura.txt ---\n";
    while (std::getline(f, linie))
    {
        if (linie.empty() || linie[0] == '#') continue;

        std::stringstream ss(linie);
        ss >> comanda >> valoare;

        std::cout << "\n>>> COMANDA CITITA: " << comanda << " " << valoare << std::endl;

        joc.proceseazaComanda(comanda, valoare);

        std::cout << joc << std::endl;

        if (joc.isTerminat()) {
            std::cout << "JOCUL S-A TERMINAT!" << std::endl;
            break;
        }
    }
    f.close();
    std::cout << "--- Sfarsit citire comenzi ---\n";

    std::cout << "\nSCOR FINAL: " << joc.getScor() << std::endl;

    std::cout << "\n--- DEMONSTRATIE REGULA CELOR TREI ---\n";
    {
        std::cout << "Test Constructor Copiere (s2 = s1)...\n";
        SirDeBile s1_copie = joc.getSirDeBile();

        std::cout << "Test Operator Atribuire (s3 = s1)...\n";
        SirDeBile s2_atribuire;
        s2_atribuire = joc.getSirDeBile();

        // MODIFICAREA pentru a opri eroarea de "unused variable" (-Werror)
        (void)s1_copie;
        (void)s2_atribuire;

        std::cout << "Obiectele copie vor fi distruse acum (ies din scope)...\n";
    }

    std::cout << "Obiectul original 'joc' va fi distrus acum (ies din main)...\n";
    return 0;
}