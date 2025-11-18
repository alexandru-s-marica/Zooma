#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include "../Core/Nivel.h"
#include "MesajUI.h"

// helpers pentru conversia culorilor in culori SFML
sf::Color getSfmlColor(Culoare c);

class GameRenderer {
private:
    sf::RenderWindow& window;
    Nivel& nivel;
    MesajUI mesajManager;

public:
    GameRenderer(sf::RenderWindow& win, Nivel& n);
    void handleInput();
    void actualizeazaStareUI();
    void draw();
};