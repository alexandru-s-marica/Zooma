#include "Meniu.h"
#include "../Core/ResourceManager.h"
#include <iostream>

Meniu::Meniu(float width, float height, const std::string& titluMeniu, const std::vector<std::string>& optiuniText)
    : titlu(ResourceManager::getInstance().getFont()),
      indexSelectat(-1)
{
    const sf::Font& fontRef = ResourceManager::getInstance().getFont();

    titlu.setString(titluMeniu);
    titlu.setCharacterSize(50);
    titlu.setFillColor(sf::Color::Yellow);
    titlu.setStyle(sf::Text::Bold);

    sf::FloatRect titleBounds = titlu.getLocalBounds();
    titlu.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    titlu.setPosition({width / 2.f, height * 0.2f});

    float startY = height * 0.4f;
    float gap = 70.f;
    float btnWidth = 300.f;
    float btnHeight = 50.f;

    for (size_t i = 0; i < optiuniText.size(); ++i) {
        float x = (width - btnWidth) / 2.f;
        float y = startY + i * gap;
        butoane.emplace_back(x, y, btnWidth, btnHeight, optiuniText[i], fontRef);
    }
}

void Meniu::deseneaza(sf::RenderWindow& window) {
    sf::RectangleShape overlay(sf::Vector2f((float)window.getSize().x, (float)window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    window.draw(titlu);
    for (auto& btn : butoane) {
        btn.draw(window);
    }
}

void Meniu::miscaSus() {
    if (indexSelectat - 1 >= 0) {
        butoane[indexSelectat].setSelectat(false);
        indexSelectat--;
        butoane[indexSelectat].setSelectat(true);
    } else {
        if (indexSelectat != -1) butoane[indexSelectat].setSelectat(false);
        indexSelectat = butoane.size() - 1;
        butoane[indexSelectat].setSelectat(true);
    }
}

void Meniu::miscaJos() {
    if (indexSelectat + 1 < static_cast<int>(butoane.size())) {
        if (indexSelectat != -1) butoane[indexSelectat].setSelectat(false);
        indexSelectat++;
        butoane[indexSelectat].setSelectat(true);
    } else {
        if (indexSelectat != -1) butoane[indexSelectat].setSelectat(false);
        indexSelectat = 0;
        butoane[indexSelectat].setSelectat(true);
    }
}

int Meniu::getOptiuneSelectata() const {
    return indexSelectat;
}

void Meniu::updateMouse(sf::Vector2f mousePos) {
    if (indexSelectat != -1) {
        butoane[indexSelectat].setSelectat(false);
        indexSelectat = -1;
    }

    for (auto& btn : butoane) {
        btn.update(mousePos);
    }
}

int Meniu::getOptiuneLaClick(sf::Vector2f mousePos) const {
    for (size_t i = 0; i < butoane.size(); ++i) {
        if (butoane[i].isClicked(mousePos)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}