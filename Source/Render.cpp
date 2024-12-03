#include "Main.h"

Object SelectionBar("Resources/Selectionbar.png");
Object Slidingbar("Resources/Slidingbar.png");
Object Background("Resources/Background.jpg");
Object Topbar("Resources/Topbar.png");
Object ExitIcon("Resources/Exit.png");
Object RedIcon("Resources/Redicon.png");
Object BlueIcon("Resources/Blueicon.png");
Object MessageBox("Resources/MessageBox.png");

float SlideValue = 0.0f;  
float targetValue = 0.0f;

void SFMLApp::render()
{

    targetValue = isToggled ? -360.0f : 0.0f;

    // Smooth transition Value
    if (std::abs(SlideValue - targetValue) > 0.1f)
    {

        SlideValue += (targetValue - SlideValue) * 0.05;
    }
    else
    {

        SlideValue = targetValue; // Snap to target when close enough
    }

    window.clear(sf::Color::White);
    
    Background.setScale(1.5, 1);
    Background.draw(window);

    Topbar.draw(window);

    Slidingbar.setScale(1.8, 1);
    Slidingbar.setPosition(SlideValue, 0);
    Slidingbar.draw(window);

    ExitIcon.setPosition(10,870);
    if (!isToggled) ExitIcon.draw(window);

    MessageBox.setPosition(10, 100);
    MessageBox.setScale(1.3, 0.4);
    if (!isToggled && isToggled2 || contactSelected && !isToggled) MessageBox.draw(window);

    RedIcon.setPosition(10, 100);
    RedIcon.setScale(0.15, 0.15);
    if (!isToggled && clientContact.getName() == "Alice") RedIcon.draw(window);

    BlueIcon.setPosition(10, 100);
    BlueIcon.setScale(0.15, 0.15);
    if (!isToggled && clientContact.getName() == "Bob") BlueIcon.draw(window);


    SelectionBar.centerOrigin();
    SelectionBar.setPosition(50,40);
    SelectionBar.setScale(0.8,0.8);
    SelectionBar.draw(window);

    

    sf::Font font;
    font.loadFromFile("Resources/Pacifico.ttf");

    sf::Font font2;
    font2.loadFromFile("Resources/KGHAPPY.ttf");

    sf::Text text("Contacts", font, 50);
    text.setFillColor(sf::Color::Black);
    text.setPosition(110, 10);
    if (!isToggled) window.draw(text);

    sf::Text text2("Hello " + clientContact.getName(), font2, 40);
    text2.setFillColor(sf::Color::Black);
    text2.setPosition(400, 10);
    if (!isToggled && !contactSelected) window.draw(text2);

    sf::Text text3("Please select a contact to message!", font2, 20);
    text3.setFillColor(sf::Color::Black);
    text3.setPosition(400, 60);
    if (!isToggled && !contactSelected) window.draw(text3);

    sf::Text text4(otherContact->getName(), font2, 40);
    text4.setFillColor(sf::Color::Black);
    text4.setPosition(150, 120);
    if (!isToggled) window.draw(text4);

    sf::Text text5("Messaging " + otherContact->getName(), font2, 30);
    text5.setFillColor(sf::Color::Black);
    text5.setPosition(500, 20);
    if (!isToggled && contactSelected) window.draw(text5);

    sf::Text text6("Messaging " + otherContact->getName(), font2, 30);
    text6.setFillColor(sf::Color::Black);
    text6.setPosition(130, 20);
    if (isToggled && contactSelected) window.draw(text6);



    textBox.setPosition(363 + SlideValue, 910);
    textBox.setSize(595 - SlideValue, 50);

    textBox.render(window);

    window.display(); // Display the contents on the screen
}