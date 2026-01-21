#include <iostream>
#include "App/Game.h"
#include "Core/Exceptions.h"
#include "Core/ResourceManager.h"

int main() {
    try {
        ResourceManager::getInstance().incarcaFont("arial.ttf");

        Game game;
        game.run();
    }
    catch (const ZoomaException& e) {
        std::cerr << "[ZOOMA EXCEPTION] " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << "[STD EXCEPTION] " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "[UNKNOWN EXCEPTION] Eroare critica necunoscuta!" << std::endl;
        return -1;
    }

    return 0;
}