#include "Object.h"

Object::Object(const std::string& imagePath)
{

    if (!texture.loadFromFile(imagePath))
    {

        throw std::runtime_error("Failed to load image.");
    }

    // Set up the sprite with the texture
    sprite.setTexture(texture);
    size = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

}

void Object::setScale(float x, float y)
{

    sprite.setScale(x, y);
}

const sf::Sprite& Object::getSprite() const
{

    return sprite;
}

void Object::setPosition(float x, float y)
{

    position = { x, y };
    sprite.setPosition(position);
}

void Object::draw(sf::RenderWindow& window)
{

        sprite.setPosition(position);
        window.draw(sprite);
}

void Object::rotate(float angle)
{

    // Apply the rotation
    sprite.rotate(angle);
}

void Object::centerOrigin()
{

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

Object::~Object()
{

    // SFML's textures and sprites automatically clean up when the object goes out of scope.
}