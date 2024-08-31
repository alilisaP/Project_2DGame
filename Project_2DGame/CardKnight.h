#pragma once
#include "Enemy.h"

class CardKnight : public Enemy{
    float velocity;
    int hp;
    int atk;
    sf::Clock cooldownClock;
    sf::Time cooldownDuration;
public:
    CardKnight();
    auto loadTextures() -> void;
    auto run(Character& character, std::vector<sf::Sprite>& obstacles) -> void override;
    auto attack(Character& player) -> void override;
    auto setPosition(float x, float y) -> void override;
    auto getHP() -> int override;
    auto setHP(int n) -> void override;
};

