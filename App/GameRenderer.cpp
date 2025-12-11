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
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (auto keyPress = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code == sf::Keyboard::Key::R) {
                nivel.reset(40.f);
                actualizeazaStareUI();
            }
        }

        if (nivel.getStareJoc() != StareJoc::RULEAZA) {
            continue;
        }

        if (auto mouseMove = event->getIf<sf::Event::MouseMoved>()) {
            nivel.getProiector().rotesteSpre({
                static_cast<float>(mouseMove->position.x),
                static_cast<float>(mouseMove->position.y)
            });
        }
        else if (auto mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                Bila proiectil = nivel.trageBilaJucator();

                Vec2f mousePos = {
                    static_cast<float>(mousePress->position.x),
                    static_cast<float>(mousePress->position.y)
                };

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
    StareJoc stare = nivel.getStareJoc();

    if (stare == StareJoc::GAME_OVER) {
        mesajManager.afiseaza("GAME OVER\nApasa 'R' pentru Restart", {600.f, 400.f});
    }
    else if (stare == StareJoc::CASTIGAT) {
        mesajManager.afiseaza("VICTORIE!\nApasa 'R' pentru Restart", {600.f, 400.f});
    }
    else {
        mesajManager.ascunde();
    }
}

void GameRenderer::draw() {
    window.clear(sf::Color(20, 20, 40)); // Fundal

    for (const auto& bilaCore : nivel.getSirDeBile()) {
        sf::CircleShape formaBila(bilaCore.getRaza());

        sf::Color c = getSfmlColor(bilaCore.getCuloare());

        if (bilaCore.esteInDistrugere()) {
            c.a = 100; // Transparență
            formaBila.setScale({0.8f, 0.8f}); // Micșorare (Vector2f pentru SFML 3)
        }

        formaBila.setFillColor(c);
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
    float razaMica = bilaUrmatoare.getRaza() * 0.7f;

    sf::CircleShape formaBilaUrmatoare(razaMica);
    formaBilaUrmatoare.setFillColor(getSfmlColor(bilaUrmatoare.getCuloare()));
    formaBilaUrmatoare.setOrigin({razaMica, razaMica});
    formaBilaUrmatoare.setPosition({proiectorCore.getPozitie().x + 50.f, proiectorCore.getPozitie().y + 50.f});
    window.draw(formaBilaUrmatoare);

    for (const auto& p : nivel.getProiectileInZbor()) {
        const Bila& b = p.first;
        sf::CircleShape formaProiectil(b.getRaza());
        formaProiectil.setFillColor(getSfmlColor(b.getCuloare()));
        formaProiectil.setOrigin({b.getRaza(), b.getRaza()});
        formaProiectil.setPosition({b.getPozitie().x, b.getPozitie().y});
        window.draw(formaProiectil);
    }

    mesajManager.draw(window);

    window.display();
}