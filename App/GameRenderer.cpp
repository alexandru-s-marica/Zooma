#include "GameRenderer.h"
#include <cmath>
#include <variant>

sf::Color getSfmlColor(Culoare c) {
    switch (c) {
        case Culoare::ROSU: return sf::Color::Red;
        case Culoare::VERDE: return sf::Color::Green;
        case Culoare::ALBASTRU: return sf::Color::Blue;
        case Culoare::GALBEN: return sf::Color::Yellow;
        case Culoare::VIOLET: return sf::Color::Magenta;
        case Culoare::PORTOCALIU: return sf::Color(255, 165, 0);
        case Culoare::UNKNOWN: return sf::Color::Black;
    }
    return sf::Color::Black;
}

GameRenderer::GameRenderer(sf::RenderWindow& win, Nivel& n)
    : window(win), nivel(n) {
    // AICI probabil vei vrea să inițializezi mesajManager, dacă nu o faci
    // deja în lista de inițializare a clasei (în .h)
}

void GameRenderer::handleInput() {
    // Nu permite input dacă jocul s-a terminat (decât pentru a închide)
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // Ignoră input-ul de joc dacă s-a terminat
        if (nivel.getStareJoc() == StareJoc::GAME_OVER) {
            continue;
        }

        if (auto mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            nivel.getProiector().rotesteSpre({static_cast<float>(mouseMove->position.x), static_cast<float>(mouseMove->position.y)});
        } else if (auto mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                Bila proiectil = nivel.getProiector().trage();

                Vec2f mousePos = {static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y)};
                Vec2f dir = mousePos - nivel.getProiector().getPozitie();
                dir = dir.normalize();

                nivel.adaugaProiectil(proiectil, dir);

            }
            if (mousePress->button == sf::Mouse::Button::Right) {
                nivel.getProiector().schimbaBila();
            }
        }
    }
}

// --- METODĂ REACTIVATĂ ---
void GameRenderer::actualizeazaStareUI() {
    if (nivel.getStareJoc() == StareJoc::GAME_OVER) {
        // Preluăm poziția din constantele definite în utils.h
        mesajManager.afiseaza("GAME OVER", {SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f});
    } else {
        mesajManager.ascunde();
    }
}

// --- FUNCȚIE REACTIVATĂ ---
void GameRenderer::draw() {
    window.clear(sf::Color(20, 20, 40));

    // ... (codul de desenare pentru bile, proiector, proiectile rămâne neschimbat) ...
    for (const auto& bilaCore : nivel.getSirDeBile()) {
        sf::CircleShape formaBila(bilaCore.getRaza());
        formaBila.setFillColor(getSfmlColor(bilaCore.getCuloare()));
        formaBila.setOrigin({bilaCore.getRaza(), bilaCore.getRaza()});
        formaBila.setPosition({bilaCore.getPozitie().x, bilaCore.getPozitie().y});
        window.draw(formaBila);
    }

    const Proiector& proiectorCore = nivel.getProiector();
    const Bila& bilaProiector = proiectorCore.getBilaCurenta();
    sf::CircleShape formaProiector(bilaProiector.getRaza());
    formaProiector.setFillColor(getSfmlColor(bilaProiector.getCuloare()));
    formaProiector.setOrigin({bilaProiector.getRaza(), bilaProiector.getRaza()});
    formaProiector.setPosition({proiectorCore.getPozitie().x, proiectorCore.getPozitie().y});
    window.draw(formaProiector);

    const Bila& bilaUrmatoare = proiectorCore.getBilaUrmatoare();
    sf::CircleShape formaBilaUrmatoare(bilaUrmatoare.getRaza() * 0.7f);
    formaBilaUrmatoare.setFillColor(getSfmlColor(bilaUrmatoare.getCuloare()));
    formaBilaUrmatoare.setOrigin({bilaUrmatoare.getRaza() * 0.7f, bilaUrmatoare.getRaza() * 0.7f});
    formaBilaUrmatoare.setPosition({proiectorCore.getPozitie().x + 50.f, proiectorCore.getPozitie().y + 50.f});
    window.draw(formaBilaUrmatoare);

    for (const auto& p : nivel.getProiectileInZbor()) {
        sf::CircleShape formaProiectil(p.first.getRaza());
        formaProiectil.setFillColor(getSfmlColor(p.first.getCuloare()));
        formaProiectil.setOrigin({p.first.getRaza(), p.first.getRaza()});
        formaProiectil.setPosition({p.first.getPozitie().x, p.first.getPozitie().y});
        window.draw(formaProiectil);
    }

    // --- DESENARE MESAJ REACTIVATĂ ---
    mesajManager.draw(window);

    window.display();
}