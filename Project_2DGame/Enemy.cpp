#include "Enemy.h"

auto Enemy::getPosition() -> sf::Vector2f {
    return enemy.getPosition();
}

auto Enemy::setPosition(float x, float y) -> void {
    enemy.setPosition(x, y);
}

auto Enemy::run(Character& character, std::vector<sf::Sprite>& obstacles) -> void { }

auto Enemy::attack(Character& player) -> void { }

auto Enemy::draw(sf::RenderWindow& window) -> void {
    window.draw(enemy);
}

auto Enemy::checkCollisionForEnemy(sf::Sprite& obstacle) -> bool {
    return enemy.getGlobalBounds().intersects(obstacle.getGlobalBounds());
}

auto Enemy::getGlobalBounds() -> sf::FloatRect {
    return enemy.getGlobalBounds();
}

