// kahan se aya me hu don

#include <SFML/Graphics.hpp>
#include <optional>

int main() 
{
    // SFML 3 uses sf::VideoMode({width, height})
    auto window = sf::RenderWindow(sf::VideoMode({ 800, 600 }), "SFML 3 + vcpkg Success!");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin({ 100.f, 100.f }); // Center the circle
    shape.setPosition({ 400.f, 300.f });

    while (window.isOpen()) {
        // SFML 3 new event polling style
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}