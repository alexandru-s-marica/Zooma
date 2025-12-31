#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Exceptions.h"

class ResourceManager {
private:
    sf::Font font;
    bool fontIncarcat;

    // Constructor privat (Singleton)
    ResourceManager() : fontIncarcat(false) {}

public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    static ResourceManager& getInstance() {
        static ResourceManager instanta;
        return instanta;
    }

    void incarcaFont(const std::string& cale) {
        if (!font.openFromFile(cale)) {
            throw ResursaLipsaException(cale);
        }
        fontIncarcat = true;
        std::cout << "[ResourceManager] Font incarcat: " << cale << "\n";
    }

    const sf::Font& getFont() const {
        if (!fontIncarcat) {
            throw LogicaJocException("S-a cerut fontul inainte de initializare!");
        }
        return font;
    }
};