#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Core/Nivel.h"
#include "MesajUI.h"

sf::Color getSfmlColor(Culoare c);

class GameRenderer {
private:
    sf::RenderWindow& window;
    Nivel& nivel;
    MesajUI mesajManager;

    sf::Font font;
    sf::Text textNivel;

public:
    GameRenderer(sf::RenderWindow& win, Nivel& n);

    void actualizeazaStareUI();
    void draw();
};