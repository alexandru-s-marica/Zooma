#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Buton.h"

class Meniu {
private:
    sf::Font font;
    std::vector<Buton> butoane;
    sf::Text titlu;
    int indexSelectat;
    bool fontIncarcat;

public:
    Meniu(float width, float height, const std::string& titluMeniu, const std::vector<std::string>& optiuniText);

    void deseneaza(sf::RenderWindow& window);

    void miscaSus();
    void miscaJos();
    int getOptiuneSelectata() const;

    void updateMouse(sf::Vector2f mousePos);
    int getOptiuneLaClick(sf::Vector2f mousePos) const;
};