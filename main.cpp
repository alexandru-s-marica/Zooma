#include <SFML/Graphics.hpp>

int main() {
    // Creează o fereastră
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test SFML");

    // O formă simplă pentru test
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Bucla principală a jocului
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}