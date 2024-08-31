#include "Cup.h"

Cup::Cup(const sf::Texture& texture, float x, float y, float dirX, float dirY)
    : direction(dirX, dirY), velocity(0.8f){
    shape.setTexture(texture);
    shape.setPosition(x + 20, y + 40);
    shape.setScale(1.2f, 1.2f);
}

auto Cup::shoot() -> void {
    auto bounds = shape.getLocalBounds();
    shape.setOrigin(bounds.width/2, bounds.height/2);
    shape.move(direction * velocity);
    shape.rotate(5.0f);
}

auto Cup::draw(sf::RenderWindow &window) -> void {
    window.draw(shape);
}

auto Cup::isOffScreen(sf::RenderWindow &window) -> bool {
    auto bounds = shape.getGlobalBounds();
    return bounds.left + bounds.width < 60 || bounds.left > (window.getSize().x - 60) ||
    bounds.top + bounds.height < 160 || bounds.top > (window.getSize().y - 60);
}

auto Cup::checkCollisionForCups(std::vector<sf::Sprite>& obstacles) -> bool {
    for (auto& obstacle : obstacles) {
        if (shape.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

/*auto Cup::checkCollisionForEnemy(Enemy& enemy) -> bool {
    return shape.getGlobalBounds().intersects(enemy.getGlobalBounds());
}*/

auto Cup::getGlobalBounds() -> sf::FloatRect {
    return shape.getGlobalBounds();
}

auto Cup::getPosition() -> sf::Vector2f {
    return shape.getPosition();
}

