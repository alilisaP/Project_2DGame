#include "Character.h"
#include "Cup.h"
#include <iostream>

//member initializer list
Character::Character() : velocity(6.0f), shootMaxCooldown(20.0f){
    loadTextures();

    player.setTexture(frontTexture);
    player.setPosition(375, 350);
    player.setScale(1.3f, 1.3f);

    shootCooldown = sf::milliseconds(150);

    upKeyHeld = false;
    downKeyHeld = false;
    leftKeyHeld = false;
    rightKeyHeld = false;

    HP = 6;
    ATK = 3;

    atk.setTexture(atkTexture);
    hp.setTexture(hpTexture);

    playerState = ALIVE;

}

//making the character run in different directions while changing the corresponding texture
auto Character::running(sf::RenderWindow& window, std::vector<sf::Sprite>& obstacles) -> void {
    if (playerState == DEAD) return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.getPosition().x <
        (window.getSize().x - 45) - player.getGlobalBounds().width) {
        player.setTexture(rightTexture);
        player.move(velocity, 0);
        for (auto& obstacle : obstacles) {
            if(checkCollisionForPlayer(obstacle)) {
                player.move(-velocity, 0);
            }
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.getPosition().x > 45) {
        player.setTexture(leftTexture);
        player.move(-velocity, 0);
        for (auto& obstacle : obstacles) {
            if(checkCollisionForPlayer(obstacle)) {
                player.move(velocity, 0);
            }
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)  && player.getPosition().y > 110) {
        player.setTexture(backTexture);
        player.move(0, -velocity);
        for (auto& obstacle : obstacles) {
            if(checkCollisionForPlayer(obstacle)) {
                player.move(0, velocity);
            }
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player.getPosition().y <
        (window.getSize().y - 58) - player.getGlobalBounds().height) {
        player.setTexture(frontTexture);
        player.move(0, velocity);
        for (auto& obstacle : obstacles) {
            if(checkCollisionForPlayer(obstacle)) {
                player.move(0, -velocity);
            }
        }
    }
}

//checking if the player can shoot
auto Character::canShoot() -> bool {
    /*if(shootCooldown >= shootMaxCooldown) {

        //reset the cooldown
        shootCooldown = 0.f;
        return true;
    }
    return false;*/
    return shootClock.getElapsedTime() >= shootCooldown;
}

auto Character::shooting(std::vector<Cup*>& cups, sf::Texture& cupTexture) -> void {
    if (playerState == DEAD) return;
    if (canShoot()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (!upKeyHeld) {
                cups.push_back(new Cup(cupTexture, player.getPosition().x, player.getPosition().y, 0, -8));
                upKeyHeld = true;
                //shootCooldown = -10.0f;
            } else {
                upKeyHeld = false;
            }
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (!downKeyHeld) {
                cups.push_back(new Cup(cupTexture, player.getPosition().x, player.getPosition().y, 0, 8));
                downKeyHeld = true;
                //shootCooldown = 0.f;
            } else {
                downKeyHeld = false;
            }
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (!leftKeyHeld) {
                cups.push_back(new Cup(cupTexture, player.getPosition().x, player.getPosition().y, -8, 0));
                leftKeyHeld = true;
                //shootCooldown = 0.f;
            } else {
                leftKeyHeld = false;
            }
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (!rightKeyHeld) {
                cups.push_back(new Cup(cupTexture, player.getPosition().x, player.getPosition().y, 8, 0));
                rightKeyHeld = true;
                //shootCooldown = 0.f;
            } else {
                rightKeyHeld = false;
            }
        }
        shootClock.restart();
    }
}

auto Character::draw(sf::RenderWindow& window) -> void{
    window.draw(player);
    for (int h = 0; h < HP; h++) {
        hp.setPosition(365 + h * 40.f, 15);
        window.draw(hp);
    }
    for (int a = 0; a < ATK; a++) {
        atk.setPosition(375 + a * 45.f, 55);
        window.draw(atk);
    }
}

auto Character::checkCollisionForPlayer(sf::Sprite &obstacle) -> bool {
    return player.getGlobalBounds().intersects(obstacle.getGlobalBounds());
}

//loading the textures
auto Character::loadTextures() -> void {
    if (!backTexture.loadFromFile("assets/alice4.png"))
        std::cout << "error loading back texture\n";
    if (!rightTexture.loadFromFile("assets/alice2.png"))
        std::cout << "error loading right texture\n";
    if (!leftTexture.loadFromFile("assets/alice3.png"))
        std::cout << "error loading left texture\n";
    if (!frontTexture.loadFromFile("assets/alice1.png"))
        std::cout << "error loading front texture\n";
    if(!hpTexture.loadFromFile("assets/hp.png"))
        std::cout << "error loading front texture\n";
    if(!atkTexture.loadFromFile("assets/atk.png"))
        std::cout << "error loading front texture\n";
}

auto Character::getPosition() -> sf::Vector2f {
    return player.getPosition();
}

auto Character::setPosition(float x, float y) -> void {
    player.setPosition(x, y);
}

auto Character::getSprite() -> sf::Sprite& {
    return player;
}

auto Character::getHP() -> int {
    return HP;
}

auto Character::setHP(int n) -> void {
    HP = n;
}

auto Character::dying() -> void {
    if (HP <= 0) playerState = DEAD;
}

auto Character::setState(Character::PlayerState state) -> void {
    playerState = state;
}

auto Character::getATK() -> int {
    return ATK;
}

auto Character::setATK(int n) -> void {
    ATK = n;
}

auto Character::getGlobalBounds() -> sf::FloatRect {
    return player.getGlobalBounds();
}




