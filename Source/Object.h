#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>

class Object
{

public:
    Object(const std::string& imagePath);
    ~Object();
    void setScale(float x, float y);
    const sf::Sprite& getSprite() const;
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window);
    void rotate(float angle);
    void centerOrigin();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f size;
};

#endif // OBJECT_H