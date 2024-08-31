#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
//#include "Cup.h"

struct Cup;

class Character {
public:
    Character();
    enum PlayerState {
        ALIVE, DEAD
    };
    auto loadTextures() -> void;
    auto checkCollisionForPlayer(sf::Sprite& obstacle) -> bool;
    auto running(sf::RenderWindow& window, std::vector<sf::Sprite>& obstacles) -> void;
    auto shooting(std::vector<Cup*>& cups, sf::Texture& cupTexture) -> void;
    auto canShoot() -> bool;
    auto dying() -> void;
    auto draw(sf::RenderWindow& window) -> void;
    auto getPosition() -> sf::Vector2f;
    auto setPosition(float x, float y) -> void;
    auto getSprite() -> sf::Sprite&;
    auto getHP() -> int;
    auto setHP(int n) -> void;
    auto setState(PlayerState state) -> void;
    auto getATK() -> int;
    auto setATK(int n) -> void;
    auto getGlobalBounds() -> sf::FloatRect;

private:

    sf::Sprite player;

    //textures
    sf::Texture frontTexture;
    sf::Texture backTexture;
    sf::Texture leftTexture;
    sf::Texture rightTexture;

    //player's velocity
    float velocity;

    //the bullets(cups)
    std::vector<sf::Texture*> cupTextures;
    std::vector<Cup*> cups;

    //shooting cooldown
    sf::Clock shootClock;
    sf::Time shootCooldown;
    //float shootCooldown;
    float shootMaxCooldown;

    //stats
    int HP;
    int ATK;

    sf::Sprite hp;
    sf::Sprite atk;
    sf::Texture hpTexture;
    sf::Texture atkTexture;

    //checking if the button is being held
    bool upKeyHeld;
    bool downKeyHeld;
    bool leftKeyHeld;
    bool rightKeyHeld;

    PlayerState playerState;

};


