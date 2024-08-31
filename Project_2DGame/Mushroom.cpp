#include "Mushroom.h"
#include <iostream>

Mushroom::Mushroom() : hp(20), atk(1) {
    loadTextures();
    enemy.setTexture(leftTexture);
    cooldownDuration = sf::seconds(1.5);
}

auto Mushroom::loadTextures() -> void {
    if (!leftTexture.loadFromFile("assets/mushroomie.png"))
        std::cout << "error loading back texture\n";
}

auto Mushroom::getHP() -> int {
    return hp;
}

auto Mushroom::setHP(int n) -> void {
    hp = n;
}

auto Mushroom::attack(Character &player, std::vector<Poison*>& bullets, sf::Texture& bulletTexture) -> void {
    auto enemyPos = enemy.getPosition();
    auto characterPos = player.getPosition();
    auto direction = characterPos - enemyPos;

    auto distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    auto distanceToFollow = 400.0f;

    if (distance < distanceToFollow) {
        if (canAttack()) {
            if (std::abs(direction.x) > std::abs(direction.y)) {
                if (direction.x > 0) {
                    bullets.push_back(new Poison(bulletTexture, enemyPos.x, enemyPos.y, 8, 0));
                } else if (direction.x < 0) {
                    bullets.push_back(new Poison(bulletTexture, enemyPos.x, enemyPos.y, -8, 0));
                }
            } else {
                if (direction.y > 0) {
                    bullets.push_back(new Poison(bulletTexture, enemyPos.x, enemyPos.y, 0, 8));
                } else if (direction.y < 0) {
                    bullets.push_back(new Poison(bulletTexture, enemyPos.x, enemyPos.y, 0, -8));
                }
            }
            cooldownClock.restart();
        }
    }
}

auto Mushroom::canAttack() -> bool {
    return cooldownClock.getElapsedTime() >= cooldownDuration;
}

auto Mushroom::getATK() -> int {
    return atk;
}

Poison::Poison(const sf::Texture &texture, float x, float y, float dirX, float dirY)
: direction(dirX, dirY), velocity(0.8f) {
    bullet.setTexture(texture);
    bullet.setPosition( x + 30, y + 40);
}

auto Poison::shoot() -> void {
    bullet.move(direction * velocity);
}

auto Poison::draw(sf::RenderWindow &window) -> void {
    window.draw(bullet);
}

auto Poison::isOffScreen(sf::RenderWindow& window) -> bool {
    sf::FloatRect bounds = bullet.getGlobalBounds();
    return bounds.left + bounds.width < 60 || bounds.left > (window.getSize().x - 60) ||
           bounds.top + bounds.height < 160 || bounds.top > (window.getSize().y - 60);
}

auto Poison::checkCollisionForPoisonBullets(std::vector<sf::Sprite> &obstacles) -> bool {
    for (auto& obstacle : obstacles) {
        if (bullet.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

auto Poison::getGlobalBounds() -> sf::FloatRect {
    return bullet.getGlobalBounds();
}

auto Poison::checkCollisionWithPlayer(Character &character) -> bool {
    return bullet.getGlobalBounds().intersects(character.getSprite().getGlobalBounds());
}

