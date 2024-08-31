#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <vector>

class CheshireCat : public Enemy {
    float velocity;
    int hp;
    int atk;

public:
    CheshireCat();
    auto loadTextures() -> void;
    auto run(Character& character, std::vector<sf::Sprite>& obstacles) -> void override;
    auto attack(Character& player) -> void override;
    auto getHP() -> int override;
    auto setHP(int n) -> void override;
};


