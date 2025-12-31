#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ButonMeniu {
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered;
    bool isSelected;

public:
    ButonMeniu(float x, float y, float w, float h, const std::string& label, const sf::Font& font)
        : text(font), isHovered(false), isSelected(false)
    {
        shape.setPosition({x, y});
        shape.setSize({w, h});
        shape.setFillColor(sf::Color(100, 100, 200));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.0f);

        text.setString(label);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.size.x / 2.0f, textRect.size.y / 2.0f});
        text.setPosition({x + w / 2.0f, y + h / 2.0f});
    }

    void draw(sf::RenderWindow& window) {
        if (isSelected) shape.setFillColor(sf::Color(150, 150, 250));
        else if (isHovered) shape.setFillColor(sf::Color(120, 120, 220));
        else shape.setFillColor(sf::Color(100, 100, 200));
        window.draw(shape);
        window.draw(text);
    }

    void update(sf::Vector2f mousePos) {
        isHovered = shape.getGlobalBounds().contains(mousePos);
    }

    void setSelectat(bool val) { isSelected = val; }
    bool isClicked(sf::Vector2f mousePos) const { return shape.getGlobalBounds().contains(mousePos); }
};

class Meniu {
private:
    std::vector<ButonMeniu> butoane;
    sf::Text titlu;
    int indexSelectat;

public:
    Meniu(float width, float height, const std::string& titluMeniu, const std::vector<std::string>& optiuniText);
    void deseneaza(sf::RenderWindow& window);
    void miscaSus();
    void miscaJos();
    int getOptiuneSelectata() const;
    void updateMouse(sf::Vector2f mousePos);
    int getOptiuneLaClick(sf::Vector2f mousePos) const;
};