#include "Main.h"

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(960, 1080), "Secure Messager");

    // Main game loop
    while (window.isOpen())
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window
        window.clear(sf::Color::Black);

        // Display what was drawn
        window.display();
    }

    return 0;
}