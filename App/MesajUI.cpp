#include "MesajUI.h"
#include <iostream>

MesajUI::MesajUI() : text(font), esteVizibil(false) {
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "[Eroare UI] Nu s-a putut încărca 'arial.ttf'!\n";
    }

    text.setFont(font);
    text.setCharacterSize(64);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);
}

void MesajUI::afiseaza(const std::string& mesaj, sf::Vector2f pozitie) {
    text.setString(mesaj);

    sf::FloatRect bounds = text.getLocalBounds();

    // CORECȚIE AICI: Folosim { ... } pentru a crea un sf::Vector2f
    text.setOrigin({bounds.position.x + bounds.size.x / 2.0f,
                   bounds.position.y + bounds.size.y / 2.0f});

    text.setPosition(pozitie);

    esteVizibil = true;
}

void MesajUI::ascunde() {
    esteVizibil = false;
}

void MesajUI::draw(sf::RenderWindow& window) const {
    if (esteVizibil) {
        window.draw(text);
    }
}