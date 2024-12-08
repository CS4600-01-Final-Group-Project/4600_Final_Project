#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <fstream>

std::string encode(const Contact& clientContact);
std::string decode(std::string decodedText, const Contact& clientContact);

class TextBox {
public:
    TextBox(float x, float y, unsigned int width, unsigned int height, unsigned int charSize = 30)
        : cursorVisible(true), cursorBlinkTime(0.5f), elapsedTime(0.f) {
        // Set the position and size of the text box
        box.setSize(sf::Vector2f(width, height));
        box.setPosition(x, y);
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::Black);

        // Set font and character size for the text
        if (!font.loadFromFile("Resources/arial.ttf")) {  // Ensure you have a font file, e.g., "arial.ttf"
            std::cerr << "Error loading font!" << std::endl;
        }
        text.setFont(font);
        text.setCharacterSize(charSize);
        text.setFillColor(sf::Color::Black);
        text.setPosition(x + 5, y + 5);  // Padding inside the text box

        // Set up the cursor as a vertical line
        cursor.setSize(sf::Vector2f(2, height - 10));  // Vertical line for the cursor
        cursor.setFillColor(sf::Color::Black);
        cursor.setPosition(x + 5 + text.getGlobalBounds().width, y + 5);  // Positioned next to the text
    }

    // Function to update the text box with user input
    void handleInput(const sf::Event& event, const Contact& clientContact) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) {  // Backspace key
                if (!inputText.isEmpty()) {
                    inputText.erase(inputText.getSize() - 1, 1); // Remove last character
                }
            }
            else if (event.text.unicode == 13) {  // Enter key
                // Process the input text (for example, store it in a variable or print it)
                // Once Enter is pressed, clear the inputText to show the ghost text again
                // (Only if you want to clear the text box upon pressing Enter)
                outputText = inputText;  // Store the entered text in the output variable
                inputText.clear();  // Clear the input text              


                //Sending file gets cleared after each message
                std::ofstream outFile("Source/sending.txt", std::ios::trunc);  // Open in truncation mode to clear the file
                if (outFile.is_open()) {

                    outFile << outputText << std::endl;  // Write to file
                    outFile.close();  // Close the file after writing
                }

                // Encrypt
                std::string encodedText = encode(clientContact);

                // Add Decrypted to Cache so we can see previous messages
                std::ofstream outFile2("Source/MessageCache.txt", std::ios::app);  // Open in append mode
                if (outFile2.is_open()) {

                    outFile2 << clientContact.getName() << ": " << decode(encodedText, clientContact) << std::endl;  // Write to file
                    outFile2.close();  // Close the file after writing
                }

            }
            else if (event.text.unicode < 128) {  // Only accept ASCII characters
                inputText += event.text.unicode;
            }

            text.setString(inputText);  // Update displayed text
        }
    }

    // Function to update the cursor visibility
    void updateCursor(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= cursorBlinkTime) {
            cursorVisible = !cursorVisible;  // Toggle cursor visibility
            elapsedTime = 0.f;
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(box);   // Draw the text box

        if (inputText.isEmpty()) {
            // Show ghost text if input is empty
            text.setString("Type message here");
            text.setFillColor(sf::Color(150, 150, 150));  // Light gray for ghost text
        }
        else {
            text.setFillColor(sf::Color::Black);  // Normal text color
        }

        window.draw(text);  // Draw the text

        // Only draw the cursor if there is input text
        if (!inputText.isEmpty() && cursorVisible) {
            cursor.setPosition(text.getPosition().x + text.getGlobalBounds().width, text.getPosition().y);
            window.draw(cursor);  // Draw the blinking cursor
        }
    }

    // Function to get the entered text
    std::string getText() const {
        return inputText;
    }

    // Set the position of the text box
    void setPosition(float x, float y) {
        box.setPosition(x, y);
        text.setPosition(x + 5, y + 5);  // Adjust text position to match the box
    }

    // Set the position of the text box
    void setSize(float x, float y) {
        box.setSize(sf::Vector2f(x, y));
    }

    // Output variable to store the text entered
    std::string getOutputText() const {
        return outputText;
    }

private:
    sf::RectangleShape box;
    sf::Font font;
    sf::Text text;
    sf::String inputText;
    sf::RectangleShape cursor;  // Blinking cursor
    bool cursorVisible;         // Visibility of the cursor
    float cursorBlinkTime;      // Time between cursor blinks
    float elapsedTime;          // Time elapsed for cursor blinking
    std::string outputText;     // Stores the text after Enter key is pressed
};