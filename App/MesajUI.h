#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MesajUI {
private:
    sf::Font font;
    bool activ;
    sf::Text textMesaj;

public:
    MesajUI();

    void afiseaza(const std::string& mesaj, sf::Vector2f pozitie);
    void ascunde();

    void draw(sf::RenderWindow& window) const;
};