#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../Core/Nivel.h"
#include "GameRenderer.h"
#include "Meniu.h"

enum class AppState { MENIU_PRINCIPAL, JOC, PAUZA };

class Game {
private:
    sf::RenderWindow window;
    Nivel nivel;
    GameRenderer renderer;

    Meniu meniuPrincipal;
    Meniu meniuPauza;

    AppState stareAplicatie;
    sf::Clock clock;

    void processEvents();
    void update(float deltaTime);
    void render();

    void handleMeniuPrincipalInput(const sf::Event& event);
    void handleJocInput(const sf::Event& event);
    void handlePauzaInput(const sf::Event& event);

public:
    Game();
    void run();
};