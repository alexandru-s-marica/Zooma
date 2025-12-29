#include "MesajUI.h"
#include "../Core/Exceptions.h"
#include <iostream>

MesajUI::MesajUI()
    : font(),
      activ(false),
      textMesaj(font)
{
    if (!font.openFromFile("arial.ttf")) {
        throw ResursaLipsaException("arial.ttf");
    }

    textMesaj.setCharacterSize(40);
    textMesaj.setFillColor(sf::Color::White);
    textMesaj.setOutlineColor(sf::Color::Black);
    textMesaj.setOutlineThickness(2.f);
}

void MesajUI::afiseaza(const std::string& text, sf::Vector2f pozitie) {
    textMesaj.setString(text);

    sf::FloatRect bounds = textMesaj.getLocalBounds();

    textMesaj.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    textMesaj.setPosition(pozitie);

    activ = true;
}

void MesajUI::ascunde() {
    activ = false;
}

void MesajUI::draw(sf::RenderWindow& window) const {
    if (activ) {
        window.draw(textMesaj);
    }
}