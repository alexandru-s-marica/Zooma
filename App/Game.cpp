#include "Game.h"
#include "../Core/ResourceManager.h"
#include "../Core/utils.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({static_cast<unsigned int>(SCREEN_WIDTH), static_cast<unsigned int>(SCREEN_HEIGHT)}), "Zooma v1.0.0"),
      nivel(),
      renderer(window, nivel),
      meniuPrincipal(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), "ZOOMA", {"START JOC", "IESIRE"}),
      meniuPauza(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), "PAUZA", {"CONTINUA", "RESTART", "MENIU PRINCIPAL", "IESIRE"}),
      stareAplicatie(AppState::MENIU_PRINCIPAL)
{
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        switch (stareAplicatie) {
            case AppState::MENIU_PRINCIPAL:
                handleMeniuPrincipalInput(*event);
                break;
            case AppState::JOC:
                handleJocInput(*event);
                break;
            case AppState::PAUZA:
                handlePauzaInput(*event);
                break;
        }
    }
}

void Game::handleMeniuPrincipalInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Up) meniuPrincipal.miscaSus();
        if (keyEvent->code == sf::Keyboard::Key::Down) meniuPrincipal.miscaJos();
        if (keyEvent->code == sf::Keyboard::Key::Enter) {
            int opt = meniuPrincipal.getOptiuneSelectata();
            if (opt == 0) { stareAplicatie = AppState::JOC; nivel.incarcaNivel(1); }
            if (opt == 1) window.close();
        }
    }
    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        meniuPrincipal.updateMouse(window.mapPixelToCoords(mouseMove->position));
    }
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            int opt = meniuPrincipal.getOptiuneLaClick(window.mapPixelToCoords(mousePress->position));
            if (opt == 0) { stareAplicatie = AppState::JOC; nivel.incarcaNivel(1); }
            if (opt == 1) window.close();
        }
    }
}

void Game::handleJocInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) stareAplicatie = AppState::PAUZA;

        if (nivel.esteCastigat() && keyEvent->code == sf::Keyboard::Key::N) {
            nivel.incarcaNivel(nivel.getNivelCurent() + 1);
        }

        if (nivel.esteVictorieFinala() && keyEvent->code == sf::Keyboard::Key::R) {
            nivel.incarcaNivel(1);
        }
    }

    if (nivel.getStareJoc() == StareJoc::RULEAZA) {
        if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mPos = window.mapPixelToCoords(mouseMove->position);
            if (mPos.x > 800) mPos.x = 800;
            nivel.getProiector().rotesteSpre({mPos.x, mPos.y});
        }
        else if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                Bila p = nivel.trageBilaJucator();
                sf::Vector2f mPosSf = window.mapPixelToCoords(mousePress->position);
                Vec2f mPos = { mPosSf.x, mPosSf.y };
                if (mPos.x > 800) mPos.x = 800;
                Vec2f dir = mPos - nivel.getProiector().getPozitie();
                nivel.adaugaProiectil(p, dir.normalize());
            }
            if (mousePress->button == sf::Mouse::Button::Right) {
                nivel.getProiector().schimbaBila();
            }
        }
    }
    else if (nivel.esteTerminat()) {
        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::R) nivel.reset();
        }
    }
}

void Game::handlePauzaInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
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
    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        meniuPauza.updateMouse(window.mapPixelToCoords(mouseMove->position));
    }
    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left) {
            int opt = meniuPauza.getOptiuneLaClick(window.mapPixelToCoords(mousePress->position));
            if (opt == 0) stareAplicatie = AppState::JOC;
            if (opt == 1) { nivel.reset(); stareAplicatie = AppState::JOC; }
            if (opt == 2) stareAplicatie = AppState::MENIU_PRINCIPAL;
            if (opt == 3) window.close();
        }
    }
}

void Game::update(float deltaTime) {
    if (stareAplicatie == AppState::JOC) {
        nivel.ruleazaFrame(deltaTime);
        renderer.actualizeazaStareUI();
    }
}

void Game::render() {
    window.clear(sf::Color(20, 20, 40));

    if (stareAplicatie == AppState::MENIU_PRINCIPAL) {
        meniuPrincipal.deseneaza(window);
    }
    else if (stareAplicatie == AppState::JOC) {
        renderer.draw();
    }
    else if (stareAplicatie == AppState::PAUZA) {
        renderer.draw();
        meniuPauza.deseneaza(window);
    }

    window.display();
}