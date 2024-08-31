#include "CheshireCat.h"
#include <iostream>

CheshireCat::CheshireCat() : velocity(2.5f), hp(30), atk(1) {
    loadTextures();
    enemy.setPosition(120, 180);
    enemy.setTexture(leftTexture);
    enemy.setColor(sf::Color(255, 255, 255, 255));
    cooldownDuration = sf::seconds(2);

}

auto CheshireCat::loadTextures() -> void {
    if (!leftTexture.loadFromFile("assets/cheshire2.png"))
        std::cout << "error loading back texture\n";
    if (!rightTexture.loadFromFile("assets/cheshire1.png"))
        std::cout << "error loading back texture\n";
}

auto CheshireCat::run(Character& character, std::vector<sf::Sprite>& obstacles) -> void {
    auto enemyPos = enemy.getPosition();
    auto characterPos = character.getPosition();
    auto direction = characterPos - enemyPos;

    auto distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    auto distanceToFollow = 600.0f;
    if (distance < distanceToFollow) {
        if (direction.x > 0) {
            enemy.move(velocity, 0);
            enemy.setTexture(rightTexture);
        } else if (direction.x < 0){
            enemy.move(-velocity, 0);
            enemy.setTexture(leftTexture);
        } else if (direction.x == 0) {
            enemy.setTexture(leftTexture);
        }
        if (direction.y > 0) {
            enemy.move(0, velocity);
        } else if (direction.y < 0){
            enemy.move(0, -velocity);
        } else if (direction.y == 0 && direction.x > 0) {
            enemy.setTexture(rightTexture);
        } else if (direction.y == 0 && direction.x < 0) {
            enemy.setTexture(leftTexture);
        }
    }

    bool collisionDetected = false;
    for (auto& obstacle : obstacles){
        if (checkCollisionForEnemy(obstacle)){
            collisionDetected = true;
            break;
        }
    }
    // Cheshire Cat becomes invisible
    if (collisionDetected) {
        enemy.setColor(sf::Color(255, 255, 255, 160));
    } else {
        enemy.setColor(sf::Color(255, 255, 255, 255));
    }
}


auto CheshireCat::attack(Character& player) -> void {
    auto enemyPos = enemy.getPosition();
    auto characterPos = player.getPosition();
    auto direction = characterPos - enemyPos;

    /*std::cout << "Enemy: " << enemyPos.x << ", " << enemyPos.y << '\n';
    std::cout << "Character: " << characterPos.x << ", " << characterPos.y << '\n';*/

    auto character = player.getSprite();
    auto characterHP = player.getHP();

    if (checkCollisionForEnemy(character)) {
        if (cooldownClock.getElapsedTime() >= cooldownDuration) {
            //std::cout << "Collision detected!" << '\n';
            player.setHP(characterHP - atk);

            cooldownClock.restart();
            if (direction.x > 0) {
                enemy.move(-velocity, 0);
            } else {
                enemy.move(velocity, 0);
            }

            if (direction.y > 0) {
                enemy.move(0, -velocity);
            } else {
                enemy.move(0, velocity);
            }
        }
    }
}

auto CheshireCat::getHP() -> int {
    return hp;
}

auto CheshireCat::setHP(int n) -> void {
    hp = n;
}
