#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <SFML/Graphics.hpp>

class SFMLApp
{
private:
    sf::RenderWindow window;

public:
    SFMLApp();
    void run();

private:
    void processEvents();
    void update();
    void render();
};

#endif