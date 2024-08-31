#include "Collectibles.h"

auto Collectibles::buff() -> void {

}

auto Collectibles::draw(sf::RenderWindow &window) -> void {
    window.draw(collectible);

}

auto Collectibles::getGlobalBounds() -> sf::FloatRect {
    return collectible.getGlobalBounds();
}

auto Collectibles::setScale(float x, float y) -> void {
    collectible.setScale(x, y);
}

auto Collectibles::setPosition(float x, float y) -> void {
    collectible.setPosition(x, y);
}

auto Collectibles::getPosition() -> sf::Vector2f {
    return collectible.getPosition();
}

Hat::Hat() {
    loadTexture();
    collectible.setTexture(texture);
    collectible.setPosition(800, 350);
}

auto Hat::loadTexture() -> void {
    if (!texture.loadFromFile("assets/hat.png"))
        std::cout << "error loading back texture\n";
}

auto Hat::buff() -> void {
    character.setATK(character.getATK() + 2);
}


Potion::Potion() {
    loadTexture();
    collectible.setTexture(texture);
    collectible.setPosition(100, 550);
    collectible.setScale(1.f, 1.f);
}

auto Potion::loadTexture() -> void {
    if (!texture.loadFromFile("assets/potion.png"))
        std::cout << "error loading back texture\n";
}

auto Potion::buff() -> void {
    character.setHP(character.getHP() + 2);
}



