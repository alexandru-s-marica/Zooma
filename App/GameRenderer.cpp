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

}

void GameRenderer::handleInput() {
    // nu mai permite input dupa ce jocul s a terminat
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        //pentru RESTART (Tasta R)
        if (auto keyPress = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code == sf::Keyboard::Key::R) {
                // Daca jocul e terminat sau castigat, dam reset
                if (nivel.esteTerminat() || nivel.esteCastigat()) {
                    nivel.reset(40.f);
                    mesajManager.ascunde();
                }
            }
        }

        if (nivel.getStareJoc() != StareJoc::RULEAZA)

        if (auto mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            nivel.getProiector().rotesteSpre({static_cast<float>(mouseMove->position.x), static_cast<float>(mouseMove->position.y)});
        } if (auto mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
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

void GameRenderer::actualizeazaStareUI() {
    if (nivel.getStareJoc() == StareJoc::GAME_OVER) {
        mesajManager.afiseaza("GAME OVER\nApasa R pentru Replay", {SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f});
    }
    else if (nivel.getStareJoc() == StareJoc::CASTIGAT) {
        std::string mesaj = "VICTORIE!\nScor: " + std::to_string(nivel.getScor()) + "\nApasa R";
        mesajManager.afiseaza(mesaj, {SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f});
    }
    else {
        mesajManager.ascunde();
    }
}

void GameRenderer::draw() {
    window.clear(sf::Color(20, 20, 40));

    //codul de desenare pentru bile, proiector, proiectile,...
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

    mesajManager.draw(window);

    window.display();
}