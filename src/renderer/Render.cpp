#include "Render.hpp"
#include "../analysis/Round.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

sf::Texture loadMapTexture(const std::string& mapName) {
    sf::Texture texture;
    if (!texture.loadFromFile("./maps/" + mapName + ".png")) {
        std::cerr << "Failed to load map texture for " << mapName << ".png, using default.\n";
        if (!texture.loadFromFile("./maps/default.png")) {
            std::cerr << "Failed to load default map texture.\n";
        }
    }
    return texture;
}

void updatePositions() {

}

void renderMap(Round& round)
{
    sf::RenderWindow window(sf::VideoMode({1024, 1024}), "Map Render");

    size_t currentTick = round.getStartTick();
    // size_t currentTick = 0;
    size_t lastTick = round.getEndTick();
    double scale = round.getMapScale();
    std::pair mapOffset = round.getMapOffset();
    const int tickRate = round.getTickRate();
    const sf::Time timePerTick = sf::seconds(1.0f / tickRate);
    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;

    sf::Texture texture = loadMapTexture(round.getMapName());
    sf::Sprite sprite(texture);
    clock.restart();

    sf::Texture dead("./sprites/ded.png");

    for (auto& [id, player] : round.getPlayers()) {
        if (auto tick = player.getTick(currentTick)) {
            player.updatePlayerCircle(*tick, *tick, 0.f, scale, mapOffset);
        }
    }
    clock.restart();

    while (window.isOpen() && currentTick + 1 < lastTick) {
        while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                        window.close();
            }

        sf::Time deltaTime = clock.restart();
        accumulator += deltaTime;
        // std::cout << deltaTime.asMilliseconds() << std::endl;

        while (accumulator >= timePerTick && currentTick + 1 < lastTick) {
            accumulator -= timePerTick;
            currentTick++;
        }
        float alpha = accumulator.asSeconds() / timePerTick.asSeconds();

        window.clear();
        window.draw(sprite);

        for (auto& [id,player] : round.getPlayers()) {
            auto tickA = player.getTick(currentTick);
            auto tickB = player.getTick(currentTick + 1);

            if (tickA && tickB) {
                player.updatePlayerCircle(*tickA, *tickB, alpha, scale, mapOffset);
            }
            else {
                player.updatePlayerCircle(dead);
            }

            window.draw(player.getPlayerCircle());
        }

        window.display();

    }
}