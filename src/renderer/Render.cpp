#include "Render.hpp"
#include "../analysis/Round.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


void renderMap(Round m)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({1024, 1024}), "Map Render");

    // Load a sprite to display
    sf::Texture texture;

    try {
        // Attempt to load the desired map texture
        std::string mapName = "./Maps/" + m.getMapName() + ".png";
        texture = sf::Texture(mapName);
    } catch (const sf::Exception& e) {
        std::cerr << "Failed to load desired_map.png: " << e.what() << std::endl;
        try {
            // Attempt to load the default texture
            texture = sf::Texture("./Maps/default.png");
        } catch (const sf::Exception& e) {
            std::cerr << "Failed to load default.png: " << e.what() << std::endl;
            return; // Exit if both textures fail to load
        }
    }

    sf::Sprite sprite(texture);



    // Create a graphical text to display
    // const sf::Font font("arial.ttf");
    // sf::Text text(font, "Hello SFML", 50);

    // Load a music to play
    // sf::Music music("nice_music.ogg");

    // Play the music
    // music.play();

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear screen
        window.clear();
        //
        // // Draw the sprite
        window.draw(sprite);
        //
        //  Draw the string
        // window.draw(text);

        // Update the window
        window.display();
    }
}