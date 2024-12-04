#include "Main.h"
#include <stack>

Object SelectionBar("Resources/Selectionbar.png");
Object Slidingbar("Resources/Slidingbar.png");
Object Background("Resources/Background.jpg");
Object Topbar("Resources/Topbar.png");
Object ExitIcon("Resources/Exit.png");
Object RedIcon("Resources/Redicon.png");
Object BlueIcon("Resources/Blueicon.png");
Object MessageBox("Resources/MessageBox.png");

Object DisplayMessage1("Resources/MessageBox.png");
Object DisplayMessage2("Resources/MessageBox.png");
Object DisplayMessage3("Resources/MessageBox.png");
Object DisplayMessage4("Resources/MessageBox.png");
Object DisplayMessage5("Resources/MessageBox.png");


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

    MessageBox.setPosition(10 + + SlideValue, 100);
    MessageBox.setScale(1.3, 0.4);
    if (!isToggled && isToggled2 || contactSelected && !isToggled) MessageBox.draw(window);

    RedIcon.setPosition(10 + SlideValue, 100);
    RedIcon.setScale(0.15, 0.15);
    if (!isToggled && clientContact.getName() == "Alice") RedIcon.draw(window);

    BlueIcon.setPosition(10 + SlideValue, 100);
    BlueIcon.setScale(0.15, 0.15);
    if (!isToggled && clientContact.getName() == "Bob") BlueIcon.draw(window);


    SelectionBar.centerOrigin();
    SelectionBar.setPosition(50, 40);
    SelectionBar.setScale(0.8,0.8);
    SelectionBar.draw(window);

    std::ifstream inFile("Source/MessageCache.txt");

    int numberOfMessages = 0;
    std::stack<std::string> messageStack;

    if (inFile.is_open() && contactSelected)
    {

        std::string line;
        while (std::getline(inFile, line))
        {

            numberOfMessages++;
            
            messageStack.push(line);
        }

        sf::Font font3;
        font3.loadFromFile("Resources/Arial.ttf");

        DisplayMessage1.setPosition(380 + SlideValue, 750);
        DisplayMessage1.setScale(2.15 + (-SlideValue/400), 0.5);
        if (numberOfMessages >= 1) DisplayMessage1.draw(window);

        if (numberOfMessages >= 1) 
        {

            sf::Text DisplayMessage1Text(messageStack.top(), font3, 30);
            DisplayMessage1Text.setFillColor(sf::Color::Black);
            DisplayMessage1Text.setPosition(420 + SlideValue, 760);
            window.draw(DisplayMessage1Text);
            messageStack.pop();
        }

        DisplayMessage2.setPosition(380 + SlideValue, 600);
        DisplayMessage2.setScale(2.15 + (-SlideValue / 400), 0.5);
        if (numberOfMessages >= 2) DisplayMessage2.draw(window);

        if (numberOfMessages >= 2)
        {

            sf::Text DisplayMessage2Text(messageStack.top(), font3, 30);
            DisplayMessage2Text.setFillColor(sf::Color::Black);
            DisplayMessage2Text.setPosition(420 + SlideValue, 610);
            window.draw(DisplayMessage2Text);
            messageStack.pop();
        }

        DisplayMessage3.setPosition(380 + SlideValue, 450);
        DisplayMessage3.setScale(2.15 + (-SlideValue / 400), 0.5);
        if (numberOfMessages >= 3) DisplayMessage3.draw(window);

        if (numberOfMessages >= 3)
        {

            sf::Text DisplayMessage3Text(messageStack.top(), font3, 30);
            DisplayMessage3Text.setFillColor(sf::Color::Black);
            DisplayMessage3Text.setPosition(420 + SlideValue, 460);
            window.draw(DisplayMessage3Text);
            messageStack.pop();
        }

        DisplayMessage4.setPosition(380, 300);
        DisplayMessage4.setScale(2.15 + (-SlideValue / 400), 0.5);
        if (numberOfMessages >= 4) DisplayMessage4.draw(window);

        if (numberOfMessages >= 4)
        {

            sf::Text DisplayMessage4Text(messageStack.top(), font3, 30);
            DisplayMessage4Text.setFillColor(sf::Color::Black);
            DisplayMessage4Text.setPosition(420 + SlideValue, 310);
            window.draw(DisplayMessage4Text);
            messageStack.pop();
        }

        DisplayMessage5.setPosition(380 + SlideValue, 150);
        DisplayMessage5.setScale(2.15 + (-SlideValue / 400), 0.5);
        if (numberOfMessages >= 5) DisplayMessage5.draw(window);

        if (numberOfMessages >= 5)
        {

            sf::Text DisplayMessage5Text(messageStack.top(), font3, 30);
            DisplayMessage5Text.setFillColor(sf::Color::Black);
            DisplayMessage5Text.setPosition(420 + SlideValue, 160);
            window.draw(DisplayMessage5Text);
            messageStack.pop();
        }

        inFile.close(); // Close the file after reading
    }

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
    text4.setPosition(150 + SlideValue, 120);
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