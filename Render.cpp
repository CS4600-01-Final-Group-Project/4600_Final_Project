#include "Main.h"

SFMLApp::SFMLApp()
    : window(sf::VideoMode(960, 1080), "Secure Messager") {}

void SFMLApp::render()
{
    window.clear(sf::Color::Black); // Clear the screen
    // Rendering code goes here (e.g., drawing objects)
    window.display(); // Display the contents on the screen
}