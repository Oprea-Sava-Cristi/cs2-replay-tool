#include "Render.hpp"
#include "../analysis/Round.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


void renderMap(Round& m)
{
    sf::RenderWindow window(sf::VideoMode({1024, 1024}), "Map Render");
    window.setFramerateLimit(60);
    size_t currentTick = 1280;
    sf::Clock clock;
    const sf::Time frameDuration = sf::milliseconds(static_cast<int32_t>(1000.f/ static_cast<float>(m.getTickRate())));
    double scale = m.getMapScale();
    sf::Texture texture;

    try {
        std::string mapName = "./Maps/" + m.getMapName() + ".png";
        texture = sf::Texture(mapName);
    } catch (const sf::Exception& ex1) {
        std::cerr << "Failed to load map: " << ex1.what() << std::endl;
        try {
            texture = sf::Texture("./Maps/default.png");
        } catch (const sf::Exception& ex2) {
            std::cerr << "Failed to load default.png: " << ex2.what() << std::endl;
            return;
        }
    }

    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                        window.close();
            }

        if (clock.getElapsedTime() >= frameDuration || true) {
            for (auto& [id,player] : m.getPlayers()) {
                TickData tick = player.getTick(currentTick);
                player.updatePlayerCircle(tick, scale);
            }
            currentTick++;
        }

        window.clear();
        window.draw(sprite);

        for (auto& [id,player] : m.getPlayers()) {
            window.draw(player.getPlayerCircle());
        }

        window.display();

    }

    // std::vector<sf::CircleShape> circles;
    // for (int i = 0; i < 10; ++i)
    // {
    //     sf::CircleShape circle(10.f);
    //     circle.setFillColor(sf::Color(150, 50, 250));
    //     circle.setOutlineThickness(1.f);
    //     circle.setOutlineColor(sf::Color(250, 150, 100));
    //
    //     float xPosition = 100.f + i * 80.f;
    //     float yPosition = 100.f + (i % 3) * 120.f;
    //     circle.setPosition({xPosition, yPosition});
    //
    //     circles.push_back(circle);
    // }
    //
    // while (window.isOpen())
    // {
    //     while (const std::optional event = window.pollEvent())
    //     {
    //         if (event->is<sf::Event::Closed>())
    //             window.close();
    //     }
    //
    //     window.clear();
    //
    //     window.draw(sprite);
    //
    //     for (const auto& circle : circles)
    //     {
    //         window.draw(circle);
    //     }
    //
    //     window.display();
    // }
}