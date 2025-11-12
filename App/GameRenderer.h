#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "../Core/Nivel.h"
#include "MesajUI.h"

// Helpers pentru conversia culorilor noastre în culori SFML
sf::Color getSfmlColor(Culoare c);

class GameRenderer {
private:
    sf::RenderWindow& window;
    Nivel& nivel;
    MesajUI mesajManager; // <-- MEMBRU NOU

public:
    GameRenderer(sf::RenderWindow& win, Nivel& n);
    void handleInput();
    void actualizeazaStareUI(); // <-- METODĂ NOUĂ
    void draw();
};