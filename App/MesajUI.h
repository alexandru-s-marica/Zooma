#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Core/utils.h"

class MesajUI {
private:
    const sf::Font& font;
    sf::Text textMesaj;
    bool activ;

public:
    MesajUI();
    void afiseaza(const std::string& mesaj, Vec2f pozitie);
    void ascunde();
    void draw(sf::RenderWindow& window);
};