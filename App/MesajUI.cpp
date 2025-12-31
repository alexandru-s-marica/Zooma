#include "MesajUI.h"
#include "../Core/ResourceManager.h"

MesajUI::MesajUI()
    : font(ResourceManager::getInstance().getFont()),
      textMesaj(ResourceManager::getInstance().getFont()),
      activ(false)
{
    textMesaj.setCharacterSize(40);
    textMesaj.setFillColor(sf::Color::White);
    textMesaj.setOutlineColor(sf::Color::Black);
    textMesaj.setOutlineThickness(2.0f);
}

void MesajUI::afiseaza(const std::string& mesaj, Vec2f pozitie) {
    textMesaj.setString(mesaj);
    sf::FloatRect bounds = textMesaj.getLocalBounds();
    textMesaj.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    textMesaj.setPosition({pozitie.x, pozitie.y});
    activ = true;
}

void MesajUI::ascunde() {
    activ = false;
}

void MesajUI::draw(sf::RenderWindow& window) {
    if (activ) {
        window.draw(textMesaj);
    }
}