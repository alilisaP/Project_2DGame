#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include <string>

class Enemy {
protected:
    sf::Clock cooldownClock;
    sf::Time cooldownDuration;
public:
    sf::Sprite enemy;
    sf::Texture leftTexture;
    sf::Texture rightTexture;
    virtual auto run(Character& character, std::vector<sf::Sprite>& obstacles) -> void;
    virtual auto attack(Character& player) -> void;
    virtual auto draw(sf::RenderWindow& window) -> void;
    virtual auto getPosition() -> sf::Vector2f;
    virtual auto setPosition(float x, float y) -> void;
    virtual auto checkCollisionForEnemy(sf::Sprite& obstacle) -> bool;
    virtual auto getHP() -> int = 0;
    virtual auto setHP(int n) -> void = 0;
    virtual auto getGlobalBounds() -> sf::FloatRect;
};

