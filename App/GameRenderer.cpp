#include "GameRenderer.h"
#include <cmath>
#include <variant>
#include <cstdint>

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
    else if (nivel.esteCastigat()) {
        mesajManager.afiseaza("VICTORIE!\nApasa 'R' pentru Restart", {600.f, 400.f});
    }
    else {
        mesajManager.ascunde();
    }
}

void GameRenderer::draw() {
    window.clear(sf::Color(20, 20, 40));

    for (const auto& bilaCore : nivel.getSirDeBile()) {
        sf::CircleShape formaBila(bilaCore.getRaza());
        sf::Color c = getSfmlColor(bilaCore.getCuloare());

        if (bilaCore.esteInDistrugere()) {
            c.a = 100;
            formaBila.setScale({0.8f, 0.8f});
        }
        formaBila.setFillColor(c);

        if (bilaCore.areEfect()) {
            formaBila.setOutlineThickness(3.0f);
            formaBila.setOutlineColor(sf::Color::White);
        } else {
            formaBila.setOutlineThickness(0.0f);
        }

        formaBila.setOrigin({bilaCore.getRaza(), bilaCore.getRaza()});
        formaBila.setPosition({bilaCore.getPozitie().x, bilaCore.getPozitie().y});
        window.draw(formaBila);
    }

    for (const auto& explozie : nivel.getExploziiVizuale()) {
        float progres = 1.0f - explozie.timer;
        float razaCurenta = explozie.razaMaxima * progres;

        sf::CircleShape cerc(razaCurenta);
        cerc.setOrigin({razaCurenta, razaCurenta});
        cerc.setPosition({explozie.pozitie.x, explozie.pozitie.y});

        sf::Color c = sf::Color::Red;
        c.a = static_cast<unsigned char>(200 * explozie.timer);
        cerc.setFillColor(c);

        sf::CircleShape inel(razaCurenta);
        inel.setOrigin({razaCurenta, razaCurenta});
        inel.setPosition({explozie.pozitie.x, explozie.pozitie.y});
        inel.setFillColor(sf::Color::Transparent);
        inel.setOutlineThickness(2.0f);
        sf::Color outlineC = sf::Color::Yellow;
        outlineC.a = c.a;
        inel.setOutlineColor(outlineC);

        window.draw(cerc);
        window.draw(inel);
    }

    if (nivel.esteAccuracyActiv()) {
        Vec2f posProiector = nivel.getProiector().getPozitie();

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

        sf::Vertex linie[] = {
            sf::Vertex(sf::Vector2f(posProiector.x, posProiector.y), sf::Color::Red),
            sf::Vertex(mouseWorld, sf::Color::Red)
        };

        window.draw(linie, 2, sf::PrimitiveType::Lines);
    }

    const Proiector& proiectorCore = nivel.getProiector();
    const Bila& bilaProiector = proiectorCore.getBilaCurenta();
    sf::CircleShape formaProiector(bilaProiector.getRaza());
    formaProiector.setFillColor(getSfmlColor(bilaProiector.getCuloare()));

    if (bilaProiector.areEfect()) {
        formaProiector.setOutlineThickness(3.0f);
        formaProiector.setOutlineColor(sf::Color::White);
    }
    formaProiector.setOrigin({bilaProiector.getRaza(), bilaProiector.getRaza()});
    formaProiector.setPosition({proiectorCore.getPozitie().x, proiectorCore.getPozitie().y});
    window.draw(formaProiector);

    const Bila& bilaUrmatoare = proiectorCore.getBilaUrmatoare();
    float razaMica = bilaUrmatoare.getRaza() * 0.7f;
    sf::CircleShape formaBilaUrmatoare(razaMica);
    formaBilaUrmatoare.setFillColor(getSfmlColor(bilaUrmatoare.getCuloare()));
    if (bilaUrmatoare.areEfect()) {
        formaBilaUrmatoare.setOutlineThickness(2.0f);
        formaBilaUrmatoare.setOutlineColor(sf::Color::White);
    }
    formaBilaUrmatoare.setOrigin({razaMica, razaMica});
    formaBilaUrmatoare.setPosition({proiectorCore.getPozitie().x + 50.f, proiectorCore.getPozitie().y + 50.f});
    window.draw(formaBilaUrmatoare);

    for (const auto& p : nivel.getProiectileInZbor()) {
        const Bila& b = p.first;
        sf::CircleShape formaProiectil(b.getRaza());
        formaProiectil.setFillColor(getSfmlColor(b.getCuloare()));
        if (b.areEfect()) {
            formaProiectil.setOutlineThickness(3.0f);
            formaProiectil.setOutlineColor(sf::Color::White);
        }
        formaProiectil.setOrigin({b.getRaza(), b.getRaza()});
        formaProiectil.setPosition({b.getPozitie().x, b.getPozitie().y});
        window.draw(formaProiectil);
    }

    mesajManager.draw(window);
    window.display();
}