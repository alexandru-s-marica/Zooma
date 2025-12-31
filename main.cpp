#include <SFML/Graphics.hpp>
#include <iostream>
#include "App/GameRenderer.h"
#include "App/Meniu.h"
#include "Core/Nivel.h"
#include "Core/Exceptions.h"

enum class AppState { MENIU_PRINCIPAL, JOC, PAUZA };

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Zooma v0.5.0");
        window.setFramerateLimit(60);

        float w = static_cast<float>(window.getSize().x);
        float h = static_cast<float>(window.getSize().y);

        Meniu meniuPrincipal(w, h, "ZOOMA", {"START JOC", "IESIRE"});
        Meniu meniuPauza(w, h, "PAUZA", {"CONTINUA", "RESTART", "MENIU PRINCIPAL", "IESIRE"});

        Nivel nivel;
        GameRenderer renderer(window, nivel);

        AppState stareAplicatie = AppState::MENIU_PRINCIPAL;
        sf::Clock clock;

        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();

            while (const auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();

                if (stareAplicatie == AppState::MENIU_PRINCIPAL) {
                    if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                        if (keyEvent->code == sf::Keyboard::Key::Up) meniuPrincipal.miscaSus();
                        if (keyEvent->code == sf::Keyboard::Key::Down) meniuPrincipal.miscaJos();
                        if (keyEvent->code == sf::Keyboard::Key::Enter) {
                            int opt = meniuPrincipal.getOptiuneSelectata();
                            if (opt == 0) { stareAplicatie = AppState::JOC; nivel.incarcaNivel(1); }
                            if (opt == 1) window.close();
                        }
                    }
                    if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                        meniuPrincipal.updateMouse(window.mapPixelToCoords(mouseMove->position));
                    }
                    if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (mousePress->button == sf::Mouse::Button::Left) {
                            int opt = meniuPrincipal.getOptiuneLaClick(window.mapPixelToCoords(mousePress->position));
                            if (opt == 0) { stareAplicatie = AppState::JOC; nivel.incarcaNivel(1); }
                            if (opt == 1) window.close();
                        }
                    }
                }
                else if (stareAplicatie == AppState::JOC) {
                    if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                        if (keyEvent->code == sf::Keyboard::Key::Escape) stareAplicatie = AppState::PAUZA;

                        if (nivel.esteCastigat() && keyEvent->code == sf::Keyboard::Key::N) {
                            nivel.incarcaNivel(nivel.getNivelCurent() + 1);
                        }
                    }

                    if (nivel.getStareJoc() == StareJoc::RULEAZA) {
                        if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                            sf::Vector2f mPos = window.mapPixelToCoords(mouseMove->position);
                            nivel.getProiector().rotesteSpre({mPos.x, mPos.y});
                        }
                        else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                            if (mousePress->button == sf::Mouse::Button::Left) {
                                Bila p = nivel.trageBilaJucator();
                                sf::Vector2f mPosSf = window.mapPixelToCoords(mousePress->position);
                                Vec2f mPos = { mPosSf.x, mPosSf.y };

                                Vec2f dir = mPos - nivel.getProiector().getPozitie();
                                nivel.adaugaProiectil(p, dir.normalize());
                            }
                            if (mousePress->button == sf::Mouse::Button::Right) {
                                nivel.getProiector().schimbaBila();
                            }
                        }
                    } else if (nivel.getStareJoc() == StareJoc::GAME_OVER) {
                        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                            if (key->code == sf::Keyboard::Key::R) nivel.reset();
                        }
                    }
                }
                else if (stareAplicatie == AppState::PAUZA) {
                    if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                        if (keyEvent->code == sf::Keyboard::Key::Escape) stareAplicatie = AppState::JOC;
                        if (keyEvent->code == sf::Keyboard::Key::Up) meniuPauza.miscaSus();
                        if (keyEvent->code == sf::Keyboard::Key::Down) meniuPauza.miscaJos();
                        if (keyEvent->code == sf::Keyboard::Key::Enter) {
                            int opt = meniuPauza.getOptiuneSelectata();
                            if (opt == 0) stareAplicatie = AppState::JOC;
                            if (opt == 1) { nivel.reset(); stareAplicatie = AppState::JOC; }
                            if (opt == 2) stareAplicatie = AppState::MENIU_PRINCIPAL;
                            if (opt == 3) window.close();
                        }
                    }
                    if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                        meniuPauza.updateMouse(window.mapPixelToCoords(mouseMove->position));
                    }
                    if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (mousePress->button == sf::Mouse::Button::Left) {
                            int opt = meniuPauza.getOptiuneLaClick(window.mapPixelToCoords(mousePress->position));
                            if (opt == 0) stareAplicatie = AppState::JOC;
                            if (opt == 1) { nivel.reset(); stareAplicatie = AppState::JOC; }
                            if (opt == 2) stareAplicatie = AppState::MENIU_PRINCIPAL;
                            if (opt == 3) window.close();
                        }
                    }
                }
            }

            window.clear(sf::Color(20, 20, 40));

            if (stareAplicatie == AppState::MENIU_PRINCIPAL) {
                meniuPrincipal.deseneaza(window);
            }
            else if (stareAplicatie == AppState::JOC) {
                nivel.ruleazaFrame(deltaTime);

                renderer.actualizeazaStareUI();
                renderer.draw();
            }
            else if (stareAplicatie == AppState::PAUZA) {
                renderer.draw();
                meniuPauza.deseneaza(window);
            }

            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "CRITICAL ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}