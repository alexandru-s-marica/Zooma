#include "Nivel.h"
#include <variant>

void Nivel::genereazaTraseu() {
    traseu.clear();
    for (int i = 0; i < 200; ++i) traseu.push_back({100.f + i * 3, 100.f});
    for (int i = 0; i < 150; ++i) traseu.push_back({700.f, 100.f + i * 2});
    for (int i = 0; i < 200; ++i) traseu.push_back({700.f - i * 3, 400.f});
    for (int i = 0; i < 100; ++i) traseu.push_back({100.f, 400.f - i * 2});
}

Nivel::Nivel(sf::RenderWindow& win)
    : window(win),
      // Inițializează membrii care nu au dependențe
      proiector({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 100.f}),
      scor(0)
      // sirBile și traseu sunt inițializate implicit (goale)
{
    std::cout << "Nivel: Constructor\n";

    // 1. Populează vectorul 'traseu' al Nivelului
    genereazaTraseu();

    // 2. Acum, construiește și atribuie sirBile folosind 'traseu'
    //    care este acum plin.
    sirBile = SirDeBile(traseu, 40.f);
}

void Nivel::ruleazaFrame(float deltaTime) {
    proceseazaInput();
    sirBile.actualizeaza(deltaTime);

    for (auto it = proiectileInZbor.begin(); it != proiectileInZbor.end(); ) {
        it->first.setPozitie(it->first.getPozitie() + it->second * 1000.f * deltaTime);

        sf::Vector2f pos = it->first.getPozitie();
        if (pos.x < 0 || pos.x > SCREEN_WIDTH || pos.y < 0 || pos.y > SCREEN_HEIGHT) {
            it = proiectileInZbor.erase(it);
        } else {
            ++it;
        }
    }

    gestioneazaColiziuni();
}

void Nivel::proceseazaInput() {
    // 'event' este un std::optional<sf::Event>
    while (auto event = window.pollEvent())
    {
        // Folosim operatorul -> pe optional pentru a accesa membrul eventului

        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (auto mouseMove = event->getIf<sf::Event::MouseMoved>())
        {
            // getIf returnează un pointer, deci folosim ->position
            proiector.rotesteSpre(static_cast<sf::Vector2f>(mouseMove->position));
        }
        else if (auto mousePress = event->getIf<sf::Event::MouseButtonPressed>())
        {
            // mousePress este un pointer
            if (mousePress->button == sf::Mouse::Button::Left)
            {
                Bila proiectil = proiector.trage();

                auto mousePos = static_cast<sf::Vector2f>(mousePress->position);
                sf::Vector2f dir = mousePos - proiector.getPozitie();
                float mag = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                dir = {dir.x / mag, dir.y / mag};

                proiectileInZbor.push_back({proiectil, dir});
            }
            if (mousePress->button == sf::Mouse::Button::Right)
            {
                proiector.schimbaBila();
            }
        }
    }
}

void Nivel::gestioneazaColiziuni() {
    auto& bileSir = sirBile.getBile();

    // Folosim o buclă 'while' pentru a gestiona corect ștergerea elementelor
    auto it_proiectil = proiectileInZbor.begin();
    while (it_proiectil != proiectileInZbor.end()) {
        bool coliziune = false;

        for (auto it_bila_sir = bileSir.begin(); it_bila_sir != bileSir.end(); ++it_bila_sir) {

            if (it_proiectil->first.getBounds().findIntersection(it_bila_sir->getBounds())) {
                int scorAdaugat = sirBile.insereazaSiVerifica(it_bila_sir, it_proiectil->first);
                scor += scorAdaugat;

                // erase() returnează iteratorul la următorul element
                it_proiectil = proiectileInZbor.erase(it_proiectil);
                coliziune = true;
                break; // Ieși din bucla interioară (bileSir)
            }
        }

        if (!coliziune) {
            // Dacă NU a fost nicio coliziune pentru acest proiectil,
            // avansăm manual iteratorul.
            ++it_proiectil;
        }
        // Dacă A FOST o coliziune, 'it_proiectil' a fost deja avansat
        // de 'erase', așa că bucla 'while' continuă cu noul iterator.
    }
}

void Nivel::deseneaza() const {
    window.clear(sf::Color(20, 20, 40));

    sirBile.deseneaza(window);
    proiector.deseneaza(window);

    for (const auto& p : proiectileInZbor) {
        p.first.deseneaza(window);
    }

    window.display();
}

bool Nivel::esteTerminat() const {
    return false;
}

std::ostream& operator<<(std::ostream& os, const Nivel& n) {
    os << "--- Stare Nivel ---\n";
    os << "Scor: " << n.scor << "\n";
    os << n.proiector << "\n";
    os << n.sirBile << "\n";
    os << "-------------------\n";
    return os;
}