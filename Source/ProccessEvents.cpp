#include "Main.h"

bool isToggled = false;
bool isToggled2 = false;
sf::Clock clock2;
sf::Clock fileCheckClock;

void SFMLApp::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (fileCheckClock.getElapsedTime().asSeconds() > 1) {
            std::ifstream file("Source/sending.txt");
            if (file.is_open()) {
                std::string line;
                std::getline(file, line);

                std::cout << "Message being decoded: " << line << std::endl;
                    
                int firstColon = line.find(":");
                if (line.substr(0, firstColon) == clientContact.getName()) {
                    std::ofstream outFile2("Source/MessageCache.txt", std::ios::app);  // Open in append mode
                    if (outFile2.is_open()) {

                        outFile2 << clientContact.getOnlineContact(contacts, clientContact.getName()) << ": " << decode(line.substr(firstColon + 1), clientContact) << std::endl;  // Write to file
                        outFile2.close();  // Close the file after writing
                    }

                    std::ofstream clearFile("Source/sending.txt", std::ios::trunc);
                    if (clearFile.is_open()) {
                        clearFile.close();  // Opening in trunc mode clears the file
                    }
                }
                file.close();
            }
            fileCheckClock.restart();
        }


        if (event.type == sf::Event::Closed)
            window.close();

        textBox.handleInput(event, clientContact, contacts);

        float deltaTime = clock2.restart().asSeconds();
        textBox.updateCursor(deltaTime);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (mousePos.x >= 11 && mousePos.x <= 342 && mousePos.y >= 99 && mousePos.y <= 203)
        {

            isToggled2 = true;

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                contactSelected = true;
            }
        }
        else { isToggled2 = false; }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {

                
            
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