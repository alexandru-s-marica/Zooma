#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <memory>
#include <SFML/Graphics.hpp>

const float RAZA_BILA = 15.f;
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

enum class Culoare {
    ROSU,
    VERDE,
    ALBASTRU,
    GALBEN,
    MOV,
    NONE
};

static sf::Color getSfmlColor(Culoare c) {
    switch (c) {
        case Culoare::ROSU: return sf::Color::Red;
        case Culoare::VERDE: return sf::Color::Green;
        case Culoare::ALBASTRU: return sf::Color::Blue;
        case Culoare::GALBEN: return sf::Color::Yellow;
        case Culoare::MOV: return sf::Color::Magenta;
        default: return sf::Color::Black;
    }
}