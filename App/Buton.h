#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Buton {
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered;

public:
    Buton(float x, float y, float width, float height, const std::string& label, const sf::Font& font)
        : text(font), isHovered(false)
    {
        shape.setPosition({x, y});
        shape.setSize({width, height});
        shape.setFillColor(sf::Color(70, 70, 70, 200));
        shape.setOutlineThickness(2.f);
        shape.setOutlineColor(sf::Color::White);

        text.setString(label);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({textBounds.size.x / 2.f + textBounds.position.x, textBounds.size.y / 2.f + textBounds.position.y});
        text.setPosition({x + width / 2.f, y + height / 2.f});
    }

    void update(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            isHovered = true;
            shape.setFillColor(sf::Color(100, 100, 100, 255));
            shape.setOutlineColor(sf::Color::Cyan);
            text.setFillColor(sf::Color::Cyan);
        } else {
            isHovered = false;
            shape.setFillColor(sf::Color(70, 70, 70, 200));
            shape.setOutlineColor(sf::Color::White);
            text.setFillColor(sf::Color::White);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    bool isClicked(sf::Vector2f mousePos) const {
        return shape.getGlobalBounds().contains(mousePos);
    }

    void setSelectat(bool selectat) {
        if (selectat) {
            shape.setOutlineColor(sf::Color::Cyan);
            text.setFillColor(sf::Color::Cyan);
        } else {
            shape.setOutlineColor(sf::Color::White);
            text.setFillColor(sf::Color::White);
        }
    }
};