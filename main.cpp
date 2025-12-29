#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>

#include "App/GameRenderer.h"
#include "Core/Nivel.h"
#include "Core/Exceptions.h"

int main() {

    //am facut afisarea sa functioneze si testez aici
    //inainte de inceperea propriu-zisa a programului
    //(laboratorul 7)
    SirDeBile sirDeBile;
    std::cout<<sirDeBile<<std::endl;
    //functioneaza si afiseaza datele initiale (toate 0)

    //restul functiilor

    //Bila bila1;
    //std::cout<<bila1<<std::endl;
    //in restul programului apelez cu parametrii

    Proiector proiector1(Vec2f(0,0));
    std::cout<<proiector1<<std::endl;

    Nivel nivel1;
    std::cout<<nivel1<<std::endl;


    try {
        SirDeBile sirDeBile;
        std::cout << sirDeBile << std::endl;

        Proiector proiector1(Vec2f(0,0));
        std::cout << proiector1 << std::endl;

        Nivel nivel1;
        std::cout << nivel1 << std::endl;

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
        }
    }
    catch (const ResursaLipsaException& e) {
        std::cerr << "EROARE CRITICA " << e.what() << std::endl;
        return 1;
    }
    catch (const ZoomaException& e) {
        std::cerr << "EROARE JOC " << e.what() << std::endl;
        return 2;
    }
    catch (const std::exception& e) {
        std::cerr << "EROARE STANDARD " << e.what() << std::endl;
        return 3;
    }

    return 0;
}