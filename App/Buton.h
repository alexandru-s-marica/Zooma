#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Buton {
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered;

public:
    Buton(float x, float y, float width, float height, const std::string& label, const sf::Font& font);

    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
    bool isClicked(sf::Vector2f mousePos) const;
    void setSelectat(bool selectat);
};