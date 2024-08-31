#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Character.h"
#include <iostream>

class Collectibles {
protected:
    sf::Sprite collectible;
    sf::Texture texture;
    Character character;

public:
    virtual auto buff() -> void;
    virtual auto draw(sf::RenderWindow& window) -> void;
    virtual auto getGlobalBounds() -> sf::FloatRect;
    virtual auto setScale(float x, float y) -> void;
    virtual auto setPosition(float x, float y) -> void;
    virtual auto getPosition() -> sf::Vector2f;
};

class Hat : public Collectibles {
public:
    Hat();
    auto buff() -> void override;
    auto loadTexture() -> void;
};

class Potion : public Collectibles {
public:
    Potion();
    auto buff() -> void override;
    auto loadTexture() -> void;

};
