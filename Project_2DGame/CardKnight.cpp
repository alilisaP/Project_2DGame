#include "CardKnight.h"
#include <iostream>

CardKnight::CardKnight(): velocity(2.0f), hp(20), atk(1) {
    loadTextures();
    enemy.setTexture(leftTexture);
    enemy.setScale(1.2f, 1.2f);
    cooldownDuration = sf::seconds(2);
}


auto CardKnight::loadTextures() -> void {
    if (!leftTexture.loadFromFile("/Users/alice/Downloads/knight1.png"))
        std::cout << "error loading back texture\n";
    if (!rightTexture.loadFromFile("/Users/alice/Downloads/knight2.png"))
        std::cout << "error loading back texture\n";
}

auto CardKnight::run(Character &character, std::vector<sf::Sprite> &obstacles) -> void {
    auto enemyPos = enemy.getPosition();
    auto characterPos = character.getPosition();
    auto direction = characterPos - enemyPos;

    auto distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    auto distanceToFollow = 500.0f;

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

    for (auto& o : obstacles){
        if (checkCollisionForEnemy(o)){

        }
    }
}

auto CardKnight::attack(Character &player) -> void {
    auto enemyPos = enemy.getPosition();
    auto characterPos = player.getPosition();
    auto direction = characterPos - enemyPos;

    /*std::cout << "Enemy: " << enemyPos.x << ", " << enemyPos.y << '\n';
    std::cout << "Character: " << characterPos.x << ", " << characterPos.y << '\n';*/

    auto character = player.getSprite();
    auto characterHP = player.getHP();

    if (checkCollisionForEnemy(character)) {
        if (cooldownClock.getElapsedTime() >= cooldownDuration) {
            std::cout << "Collision detected!" << '\n';
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

auto CardKnight::setPosition(float x, float y) -> void {
    enemy.setPosition(x, y);
}

auto CardKnight::getHP() -> int {
    return hp;
}

auto CardKnight::setHP(int n) -> void {
    hp = n;
}


