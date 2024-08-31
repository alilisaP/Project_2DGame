#pragma once
#include "Enemy.h"
#include "SFML/Graphics.hpp"


class Cup{
public:
    Cup(const sf::Texture& texture, float x, float y, float dirX, float dirY);
    auto shoot() -> void;
    auto draw(sf::RenderWindow& window) -> void;
    auto isOffScreen(sf::RenderWindow& window) -> bool;
    auto checkCollisionForCups(std::vector<sf::Sprite>& obstacles) -> bool;
    auto getGlobalBounds() -> sf::FloatRect;
    auto getPosition() -> sf::Vector2f;

private:
    sf::Sprite shape;
    float velocity;
    sf::Vector2f direction;
};

