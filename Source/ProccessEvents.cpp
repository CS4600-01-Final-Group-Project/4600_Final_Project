#include "Main.h"

bool isToggled = false;
sf::Clock clock2;

void SFMLApp::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        textBox.handleInput(event);

        float deltaTime = clock2.restart().asSeconds();
        textBox.updateCursor(deltaTime);

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);         
            
            //std::cout << "X: " << mousePos.x << " Y: " << mousePos.y << std::endl;
            
            if (mousePos.x >= 22 && mousePos.x <= 76 && mousePos.y >= 23 && mousePos.y <= 60)
            {
                if (isToggled == false)
                    isToggled = true;
                else if (isToggled == true)
                    isToggled = false;
            }

            if (mousePos.x >= 10 && mousePos.x <= 69 && mousePos.y >= 870 && mousePos.y <= 942)
            {
                
                window.close();
            }
        }
    }
}