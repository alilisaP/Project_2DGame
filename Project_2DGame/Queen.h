#pragma  once
#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Queen : public Enemy{
    float velocity;
    int hp;
    int atk;
public:
    Queen();
    auto loadTextures() -> void;
    auto run(Character& character, std::vector<sf::Sprite>& obstacles) -> void override;
    auto getHP() -> int override;
    auto attack(Character& player) -> void override;
    auto setHP(int n) -> void override;
};

