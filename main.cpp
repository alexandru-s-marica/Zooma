#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>

#include "App/GameRenderer.h"
#include "Core/Nivel.h"
#include "Core/Exceptions.h"

int main() {

    {
        SirDeBile sirDeBileTest;
        std::cout << sirDeBileTest << std::endl;

        Proiector proiectorTest(Vec2f(0,0));
        std::cout << proiectorTest << std::endl;

        Nivel nivelTest;
        std::cout << nivelTest << std::endl;
    }

    try {
        sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Zooma");
        window.setFramerateLimit(60);

        Nivel nivel(40.f);
        GameRenderer renderer(window, nivel);

        sf::Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();

            renderer.handleInput();
            nivel.ruleazaFrame(deltaTime);
            renderer.actualizeazaStareUI();
            renderer.draw();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Unknown)) {
                (void)nivel.getScor();
                (void)nivel.esteTerminat();
                (void)Bila::getBileDistruseTotal();
            }
        }
    }
    catch (const ResursaLipsaException& e) {
        std::cerr << "EROARE CRITICA: " << e.what() << std::endl;
        return 1;
    }
    catch (const ZoomaException& e) {
        std::cerr << "EROARE JOC: " << e.what() << std::endl;
        return 2;
    }
    catch (const std::exception& e) {
        std::cerr << "EROARE STANDARD: " << e.what() << std::endl;
        return 3;
    }

    return 0;
}