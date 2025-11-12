#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>

#include "App/GameRenderer.h"
#include "Core/Nivel.h"

int main() {

    //am facut afisarea sa functioneze si testez aici
    //inainte de inceperea propriu-zisa a programului
    //(laboratorul 7)
    SirDeBile sirDeBile;
    std::cout<<sirDeBile;
    //functioneaza si afiseaza datele initiale (toate 0)

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

    return 0;
}