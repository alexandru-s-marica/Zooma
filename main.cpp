#include "utils.h"
#include "Nivel.h"

void ruleazaTestareCerinte(const Nivel& nivel) {
    std::cout << "\n========= INCEPUT TESTARE CERINTE =========\n";

    std::cout << nivel;

    std::cout << "\n--- Testare Constructor de Copiere (SirDeBile) ---\n";
    SirDeBile sirCopiat = nivel.getSirBile();
    std::cout << "Sirul copiat:\n" << sirCopiat;

    std::cout << "\n--- Testare Operator= (SirDeBile) ---\n";
    SirDeBile sirAtribuit = sirCopiat;
    sirAtribuit.adaugaBilaLaCoada(Culoare::MOV);
    sirAtribuit = nivel.getSirBile();
    std::cout << "Sirul atribuit:\n" << sirAtribuit;

    std::cout << "========= SFARSIT TESTARE CERINTE =========\n\n";
}

int main() {
    // CORECTIE: sf::VideoMode ia acum un sf::Vector2u
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Zooma Proiect POO");
    window.setFramerateLimit(60);

    Nivel nivel(window);

    ruleazaTestareCerinte(nivel);

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        nivel.ruleazaFrame(deltaTime);
        nivel.deseneaza();

        if (nivel.esteTerminat()) {
            window.close();
        }
    }

    return 0;
}