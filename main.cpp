#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>

#include "App/GameRenderer.h"
#include "Core/Nivel.h"

int main() {
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