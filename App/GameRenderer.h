#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp> // Pentru evenimente SFML 3
#include "../Core/Nivel.h" // Include clasa Nivel pură
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