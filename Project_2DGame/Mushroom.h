#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Poison{
public:
    Poison(const sf::Texture& texture, float x, float y, float dirX, float dirY);
    auto shoot() -> void;
    auto draw(sf::RenderWindow& window) -> void;
    auto isOffScreen(sf::RenderWindow& window) -> bool;
    auto checkCollisionForPoisonBullets(std::vector<sf::Sprite>& obstacles) -> bool;
    auto checkCollisionWithPlayer(Character& character) -> bool;
    auto getGlobalBounds() -> sf::FloatRect ;

private:
    sf::Sprite bullet;
    float velocity;
    sf::Vector2f direction;
};

class Mushroom : public Enemy{
private:
    int hp;
    int atk;
    sf::Clock cooldownClock;
    sf::Time cooldownDuration;

public:
    Mushroom();
    auto loadTextures() -> void;
    auto getHP() -> int override;
    auto attack(Character& player, std::vector<Poison*>& cups, sf::Texture& bulletTexture) -> void;
    auto canAttack() -> bool;
    auto setHP(int n) -> void override;
    auto getATK() -> int;
};





