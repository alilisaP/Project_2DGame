#include "GameMenu.h"
#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game() : gameState(MAIN_MENU), lairState(MENU) {

    initializeWindow();
    loadTextures();
    loadFont();
    setObjectsParameters();
    setBackgroundTexture();



}

Game::~Game() {

    //delete cup textures
    for (auto &t: cupTextures) delete t;

    //Delete the bullets(cups)
    for (auto &c: cups) delete c;

    //delete the mushrooms' bullets(poison)
    for (auto &p: poisonBullets) delete p;

    //delete the mushrooms' bullets textures
    for (auto &t: poisonTextures) delete t;

    //delete the cheshire cat
    delete cheshire;

    //delete the queen and card enemies
    for (auto &k: queenLairEnemies) delete k;

    //delete the mushroom enemies
    for (auto &m: mushroomLairEnemies) delete m;


}

auto Game::initializeWindow() -> void {
    window.create(sf::VideoMode({950, 700}), "Alice in Wonderland", sf::Style::Titlebar | sf::Style::Close,
                  sf::ContextSettings(0, 0, 8));
    window.setFramerateLimit(60);
}

auto Game::run() -> void {

    while (window.isOpen()) {
        if (gameState == MAIN_MENU || lairState == MENU) {
            menuEvents();
        } else if (gameState == IN_GAME || lairState == START_LAIR || lairState == CHESHIRE_LAIR ||
                   lairState == QUEEN_LAIR) {
            gameEvents();
        }

        window.clear(sf::Color::White);
        if (gameState == MAIN_MENU || lairState == MENU) {
            menu.drawMenuObjects(window);
        } else if (gameState == IN_GAME || lairState == START_LAIR || lairState == CHESHIRE_LAIR ||
                   lairState == QUEEN_LAIR) {
            drawGameObjects();
        }
        window.display();
    }

}

auto Game::menuEvents() -> void {

    auto event = sf::Event();
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed) {
            if (menu.clickedPlayButton(sf::Mouse::getPosition(window))) {
                gameState = IN_GAME;
                lairState = START_LAIR;
                character.setPosition(375, 350);
                character.setHP(6);
                character.setState(Character::ALIVE);
                //cheshire->setPosition(120, 180);
                setBackgroundTexture();
                if (windowOver.isOpen()) {
                    windowOver.close();
                }
            }
            if (menu.clickedResumeButton(sf::Mouse::getPosition(window))) {
                std::string filename = "saveGame.txt";
                if (!loadGame(filename)) {
                    noSaveWindow();
                }
                gameState = IN_GAME;
                lairState = START_LAIR;
            }
        }
    }
    menu.textExpand(window);

}

auto Game::gameEvents() -> void {

    auto event = sf::Event();
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = sf::Mouse::getPosition(window);
            if (save.getGlobalBounds().contains({static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)})){
                std::string filename = "saveGame.txt";
                saveGame(filename);
                gameState = MAIN_MENU;
                lairState = MENU;
            }
        }
    }
    update();

}

auto Game::allMushroomsDefeated(std::vector<Mushroom *> &mushrooms) {

    for (auto m: mushrooms) {
        if (m != nullptr) {
            return false;
        }
    }
    return true;

}

auto Game::drawGameObjects() -> void {

    window.draw(background);

    if (lairState == START_LAIR) {
        for (auto &obstacle: introObstacles)
            window.draw(obstacle);
    }

    if (lairState == CHESHIRE_LAIR) {
        for (auto &obstacle: cheshireObstacles)
            window.draw(obstacle);
        if (cheshire != nullptr) {
            cheshire->draw(window);
            window.draw(cheshireHpBar);
            window.draw(cheshireRedHpBar);
        }
    }

    if (lairState == QUEEN_LAIR) {
        for (auto &k: queenLairEnemies) {
            if (k != nullptr) {
                k->draw(window);
            }
        }
        if (queen != nullptr && queen->getHP() > 0) {
            window.draw(queenHpBar);
            window.draw(queenRedHpBar);
        }
    }

    if (lairState == MUSHROOM_LAIR) {
        for (auto &m: mushroomObstacles) {
            window.draw(m);
        }
        for (auto &m: mushroomLairEnemies) {
            m->draw(window);
        }
        for (auto &bullet: poisonBullets) {
            bullet->draw(window);
        }

    }

    if (lairState == TEA_LAIR) {
        for (auto &t: tearoomObstacles) {
            window.draw(t);
        }
        window.draw(rabbit);
        if (hat != nullptr && hatVisible) hat->draw(window);
    }


    for (auto *c: cups) c->draw(window);
    window.draw(statsBar);
    window.draw(atk);
    window.draw(hp);
    window.draw(save);
    window.draw(inventory);
    if (potion != nullptr) {
        potion->draw(window);
    }
    character.draw(window);

}

auto Game::update() -> void {

    if (gameState == IN_GAME) {

        //handling the obstacles on different lairs
        if (lairState == START_LAIR) {
            obstacles = introObstacles;
        }

        if (lairState == CHESHIRE_LAIR) {
            obstacles = cheshireObstacles;
        }

        if (lairState == QUEEN_LAIR) {
            obstacles = queenObstacles;
        }

        if (lairState == MUSHROOM_LAIR) {
            obstacles = mushroomObstacles;
        }

        if (lairState == TEA_LAIR) {
            obstacles = tearoomObstacles;
        }

        if (character.canShoot())
            character.shooting(cups, *cupTextures[0]);

        character.running(window, obstacles);

        //shooting cups
        for (auto cupIt = cups.begin(); cupIt != cups.end();) {
            (*cupIt)->shoot();
            if ((*cupIt)->isOffScreen(window) || (*cupIt)->checkCollisionForCups(obstacles)) {
                delete *cupIt;
                cupIt = cups.erase(cupIt);
            } else {
                ++cupIt;
            }
        }

        if (lairState == CHESHIRE_LAIR && cheshire != nullptr) {

            //cheshire cat attacking and shooting the cat
            cheshire->run(character, cheshireObstacles);
            cheshire->attack(character);
            for (auto cupIt = cups.begin(); cupIt != cups.end();) {
                if ((*cupIt)->getGlobalBounds().intersects(cheshire->getGlobalBounds())) {
                    delete *cupIt;
                    cupIt = cups.erase(cupIt);
                    cheshire->setHP(cheshire->getHP() - character.getATK());
                    updateHpBar();
                } else {
                    ++cupIt;
                }
            }

            if (cheshire->getHP() <= 0) {
                delete cheshire;
                cheshire = nullptr;
            }
        }

        if (lairState == QUEEN_LAIR) {

            //enemies attacking and shooting the enemies
            for (auto it = queenLairEnemies.begin(); it != queenLairEnemies.end();) {
                if (*it != nullptr) {
                    (*it)->run(character, cheshireObstacles);
                    (*it)->attack(character);
                    for (auto cupIt = cups.begin(); cupIt != cups.end();) {
                        if ((*cupIt)->getGlobalBounds().intersects((*it)->getGlobalBounds())) {
                            delete *cupIt;
                            cupIt = cups.erase(cupIt);
                            (*it)->setHP((*it)->getHP() - character.getATK());
                            updateHpBar();
                        } else {
                            ++cupIt;
                        }
                    }

                    // Check enemy HP
                    if ((*it)->getHP() <= 0) {
                        if (*it == queen) {
                            delete queen;
                            queen = nullptr;
                        } else if (*it == knight1){
                            delete knight1;
                            knight1 = nullptr;
                        } else if (*it == knight2){
                            delete knight2;
                            knight2 = nullptr;
                        }
                        it = queenLairEnemies.erase(it);
                    } else {
                        ++it;
                    }
                } else {
                    ++it;
                }
            }
        }

        if (lairState == MUSHROOM_LAIR) {

            //mushroom attacking and shooting the mushrooms
            for (auto it = mushroomLairEnemies.begin(); it != mushroomLairEnemies.end();) {
                if (*it != nullptr) {
                    (*it)->attack(character, poisonBullets, *poisonTextures[0]);
                    for (auto cupIt = cups.begin(); cupIt != cups.end();) {
                        if ((*cupIt)->getGlobalBounds().intersects((*it)->getGlobalBounds())) {
                            delete *cupIt;
                            cupIt = cups.erase(cupIt);
                            (*it)->setHP((*it)->getHP() - character.getATK());
                            updateHpBar();
                        } else {
                            ++cupIt;
                        }
                    }
                    if ((*it)->getHP() <= 0) {
                        delete *it;
                        it = mushroomLairEnemies.erase(it);
                    } else ++it;
                } else ++it;
            }

            //mushroom shooting
            for (auto poisonIt = poisonBullets.begin(); poisonIt != poisonBullets.end();) {
                (*poisonIt)->shoot();
                if ((*poisonIt)->isOffScreen(window) || (*poisonIt)->checkCollisionForPoisonBullets(obstacles)) {
                    delete *poisonIt;
                    poisonIt = poisonBullets.erase(poisonIt);
                } else if ((*poisonIt)->checkCollisionWithPlayer(character)) {
                    character.setHP(character.getHP() - 1);
                    delete *poisonIt;
                    poisonIt = poisonBullets.erase(poisonIt);
                } else {
                    ++poisonIt;
                }
            }
        }
        //creating the potion
        if (allMushroomsDefeated(mushroomLairEnemies)) {
            if (potion == nullptr && hasNotBeenUsed) {
                potion = new Potion;
            }

            //adding the potion to the inventory if collision happens
            if (potion != nullptr && character.getGlobalBounds().intersects(potion->getGlobalBounds())) {
                potion->setPosition(825, 20);
                isInInventory = true;
            }
        }

        //using the potion
        if (isInInventory && potion != nullptr) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && hasNotBeenUsed) {
                if (character.getHP() <= 4) {
                    character.setHP(character.getHP() + 2);
                } else character.setHP(6);
                delete potion;
                potion = nullptr;
                hasNotBeenUsed = false;
            }
        }

        if (lairState == TEA_LAIR) {
            checkRabbitHitByCup();
            if (rabbit.getPosition().y + rabbit.getGlobalBounds().height < 350 ||
                rabbit.getPosition().y + rabbit.getGlobalBounds().height > (window.getSize().y - 200)) {
                rabbitMoved = false;
                if (!rabbitMoved && !hatVisible && hat == nullptr) {
                    hat = new Hat;
                    hatVisible = true;
                }
            }
        }

        if (hatVisible && hat != nullptr && character.getGlobalBounds().intersects(hat->getGlobalBounds())) {
            character.setATK(character.getATK() + 2);
            hatVisible = false;
            delete hat;
        }

        if (queen == nullptr && !windowVictory.isOpen()) {
            gameState = GAME_OVER;
            victoryWindow();
        } else if (gameState == GAME_OVER) {
            gameOverEvents();
        }

        character.dying();
        if (character.getHP() <= 0) {
            gameState = GAME_OVER;
            gameOverWindow();
        } else if (gameState == GAME_OVER) {
            gameOverEvents();
        }

        switchState();

    }
}

auto Game::loadTextures() -> void {

    //cups (bullets)
    auto cupTexture = new sf::Texture();
    if (!cupTexture->loadFromFile("assets/cup.png"))
        std::cout << "error loading cup texture" << "\n";
    cupTextures.push_back(cupTexture);

    //poison (bullets)
    auto poisonTexture = new sf::Texture();
    if (!poisonTexture->loadFromFile("assets/bleh.png"))
        std::cout << "error loading cup texture" << "\n";
    poisonTextures.push_back(poisonTexture);

    //background
    if (!backgroundTexture1.loadFromFile("assets/bg1.png"))
        std::cout << "error loading background texture" << "\n";
    if (!backgroundTexture2.loadFromFile("assets/bg2.png"))
        std::cout << "error loading background texture" << "\n";
    if (!backgroundTexture3.loadFromFile("assets/bg3.png"))
        std::cout << "error loading background texture" << "\n";
    if (!backgroundTexture4.loadFromFile("assets/bg4.png"))
        std::cout << "error loading background texture" << "\n";
    if (!backgroundTexture5.loadFromFile("assets/bg5.png"))
        std::cout << "error loading background texture" << "\n";

    //stats bar
    if (!statsBarTexture.loadFromFile("assets/statsBar2.png"))
        std::cout << "error loading background texture" << "\n";

    //sign post
    if (!stickTexture.loadFromFile("assets/stick.png"))
        std::cout << "error loading background texture" << "\n";
    if (!rightSignTexture.loadFromFile("assets/rightt.png"))
        std::cout << "error loading background texture" << "\n";
    if (!leftSignTexture.loadFromFile("assets/leftt.png"))
        std::cout << "error loading background texture" << "\n";

    //gray bush
    if (!grayBushTexture.loadFromFile("assets/perfectBush.png"))
        std::cout << "error loading background texture" << "\n";

    //bushes (mushroom lair)
    if (!bush1Texture.loadFromFile("assets/bushit1.png"))
        std::cout << "error loading background texture" << "\n";
    if (!bush2Texture.loadFromFile("assets/bushit2.png"))
        std::cout << "error loading background texture" << "\n";
    if (!bush3Texture.loadFromFile("assets/bushit3.png"))
        std::cout << "error loading background texture" << "\n";
    if (!bush4Texture.loadFromFile("assets/bushit4.png"))
        std::cout << "error loading background texture" << "\n";
    if (!bush5Texture.loadFromFile("assets/bushit5.png"))
        std::cout << "error loading background texture" << "\n";

    //table
    if (!tableTexture.loadFromFile("assets/table.png"))
        std::cout << "error loading background texture" << "\n";

    //rabbit
    if (!rabbitTexture.loadFromFile("assets/rabbit2.png"))
        std::cout << "error loading background texture" << "\n";

}

auto Game::loadFont() -> void {
    if (!statsFont.loadFromFile("assets/PixelOperator-Bold.ttf"))
        std::cout << "error loading font" << '\n';
}

auto Game::setObjectsParameters() -> void {

    //background
    background.setTexture(backgroundTexture1);
    background.setPosition(0, 100);

    //stats bar
    statsBar.setTexture(statsBarTexture);
    statsBar.setScale(1.f, 1.01f);

    //character's attack stat
    atk.setString("ATK:");
    atk.setFont(statsFont);
    atk.setFillColor(sf::Color::Black);
    atk.setPosition(320, 50);

    //character's hp stat
    hp.setString("HP:");
    hp.setFont(statsFont);
    hp.setFillColor(sf::Color::Black);
    hp.setPosition(320, 10);

    //save text
    save.setString("SAVE");
    save.setFont(statsFont);
    save.setFillColor(sf::Color::Black);
    save.setCharacterSize(34);
    save.setPosition(115, 20);

    //inventory
    inventory.setString("Inventory:");
    inventory.setFont(statsFont);
    inventory.setFillColor(sf::Color::Black);
    inventory.setCharacterSize(26);
    inventory.setPosition(675, 3);

    //collectibles
    potion = nullptr;
    hat = nullptr;

    //the sign post located in the middle
    stick.setTexture(stickTexture);
    stick.setPosition(465, 260);

    leftSign.setTexture(leftSignTexture);
    leftSign.setPosition(473, 340);

    rightSign.setTexture(rightSignTexture);
    rightSign.setPosition(373, 275);

    introObstacles.push_back(stick);
    introObstacles.push_back(leftSign);
    introObstacles.push_back(rightSign);

    //gray bush
    grayBush1.setTexture(grayBushTexture);
    grayBush1.setPosition(130, 355);

    grayBush2.setTexture(grayBushTexture);
    grayBush2.setPosition(430, 215);

    grayBush3.setTexture(grayBushTexture);
    grayBush3.setPosition(510, 460);

    cheshireObstacles.push_back(grayBush1);
    cheshireObstacles.push_back(grayBush2);
    cheshireObstacles.push_back(grayBush3);

    //bushes
    bush1.setTexture(bush1Texture);
    bush2.setTexture(bush2Texture);
    bush3.setTexture(bush3Texture);
    bush4.setTexture(bush4Texture);
    bush5.setTexture(bush5Texture);

    bush1.setPosition(185, 300);
    bush2.setPosition(336, 162);
    bush3.setPosition(585, 285);
    bush4.setPosition(538, 453);
    bush5.setPosition(539, 510);

    mushroomObstacles.push_back(bush1);
    mushroomObstacles.push_back(bush2);
    mushroomObstacles.push_back(bush3);
    mushroomObstacles.push_back(bush4);
    mushroomObstacles.push_back(bush5);

    //table
    table.setTexture(tableTexture);
    table.setPosition(255, 240);
    tearoomObstacles.push_back(table);

    //Cheshire enemy
    cheshire = new CheshireCat;

    //Queen enemy
    queen = new Queen;
    queenLairEnemies.push_back(queen);

    //Card enemies
    knight1 = new CardKnight;
    knight2 = new CardKnight;

    knight1->setPosition(200, 150);
    knight2->setPosition(700, 150);

    queenLairEnemies.push_back(knight1);
    queenLairEnemies.push_back(knight2);

    //Mushroom enemies
    mushroom1 = new Mushroom;
    mushroom2 = new Mushroom;
    mushroom3 = new Mushroom;

    mushroom1->setPosition(70, 515);
    mushroom2->setPosition(775, 180);
    mushroom3->setPosition(600, 525);

    mushroomLairEnemies.push_back(mushroom1);
    mushroomLairEnemies.push_back(mushroom2);
    mushroomLairEnemies.push_back(mushroom3);

    //hp bar for enemies
    //cheshire
    cheshireHpBar.setSize(sf::Vector2f(400, 30));
    cheshireHpBar.setFillColor(sf::Color(167, 168, 168));
    cheshireHpBar.setOutlineColor(sf::Color::Black);
    cheshireHpBar.setOutlineThickness(3);
    cheshireHpBar.setPosition(275, 105);
    cheshireRedHpBar.setSize(sf::Vector2f(cheshire->getHP() * 13.2, 25));
    cheshireRedHpBar.setFillColor(sf::Color(183, 94, 201));
    cheshireRedHpBar.setPosition(277, 107);

    //queen
    queenHpBar.setSize(sf::Vector2f(400, 30));
    queenHpBar.setFillColor(sf::Color(167, 168, 168));
    queenHpBar.setOutlineColor(sf::Color::Black);
    queenHpBar.setOutlineThickness(3);
    queenHpBar.setPosition(275, 105);
    queenRedHpBar.setSize(sf::Vector2f(queen->getHP() * 7.9, 25));
    queenRedHpBar.setFillColor(sf::Color(213, 39, 57));
    queenRedHpBar.setPosition(277, 107);

    //the rabbit
    rabbit.setTexture(rabbitTexture);
    rabbit.setPosition(800, 350);

    //Game Over window
    gameOverText.setString("You are dead!\n   Game over");
    gameOverText.setFont(statsFont);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::Black);
    gameOverText.setPosition(100, 60);

    restartText.setFont(statsFont);
    restartText.setString("Restart");
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::Black);
    restartText.setPosition(90, 150);

    menuText.setFont(statsFont);
    menuText.setString("Menu");
    menuText.setCharacterSize(24);
    menuText.setFillColor(sf::Color::Black);
    menuText.setPosition(230, 150);

    //victory window
    victoryText.setString("Victory!\nYou won");
    victoryText.setFont(statsFont);
    victoryText.setCharacterSize(30);
    victoryText.setFillColor(sf::Color::Black);
    victoryText.setPosition(140, 50);

    menuText2.setFont(statsFont);
    menuText2.setString("Menu");
    menuText2.setCharacterSize(24);
    menuText2.setFillColor(sf::Color::Black);
    menuText2.setPosition(165, 150);

    //no saved game text
    noSavedGameText.setString("No saved game available");
    noSavedGameText.setFont(statsFont);
    noSavedGameText.setCharacterSize(30);
    noSavedGameText.setFillColor(sf::Color::Black);
    noSavedGameText.setPosition(80, 100);

    //flag to check if the hat is present
    hatVisible = false;

    //check if the potion is in inventory
    isInInventory = false;

    //check if the potion has already been used
    hasNotBeenUsed = true;
}

auto Game::switchState() -> void {
    auto position = character.getPosition();
    //std::cout << position.x << ", " << position.y << '\n';
    if (lairState == START_LAIR || gameState == INTRO_GAME) {
        if (position.x < 46 && position.x >= 40 &&
            position.y < 360 && position.y > 315) {
            lairState = CHESHIRE_LAIR;
            setBackgroundTexture();
            character.setPosition((window.getSize().x - 122), 340);
        } else if (position.x > 380 && position.x < 500 &&
                   position.y < 111 && position.y >= 106) {
            lairState = QUEEN_LAIR;
            setBackgroundTexture();
            character.setPosition(400, (window.getSize().y - 161));
        } else if (position.x > 380 && position.x < 500 &&
                   position.y < 544 && position.y >= 538) {
            lairState = MUSHROOM_LAIR;
            setBackgroundTexture();
            character.setPosition(400, 130);
        } else if (position.x >= 827 && position.x < 833 &&
                   position.y > 335 && position.y < 370) {
            lairState = TEA_LAIR;
            setBackgroundTexture();
            character.setPosition(50, 340);
        }
    } else if (lairState == CHESHIRE_LAIR && cheshire == nullptr) {
        if (position.x >= (window.getSize().x - 122) && position.x < (window.getSize().x - 120) &&
            position.y < 360 && position.y > 320) {
            lairState = START_LAIR;
            setBackgroundTexture();
            character.setPosition(46, 340);
        }
    } else if (lairState == QUEEN_LAIR) {
        if (queenLairEnemies.empty()) {
            if (position.x > 380 && position.x < 470 &&
                position.y >= 537 && position.y <= 540) {
                lairState = START_LAIR;
                setBackgroundTexture();
                character.setPosition(400, 109);
            }
        }
    } else if (lairState == MUSHROOM_LAIR) {
        if (mushroomLairEnemies.empty()) {
            if (position.x > 390 && position.x < 490 &&
                position.y < 108 && position.y >= 106) {
                lairState = START_LAIR;
                setBackgroundTexture();
                character.setPosition(400, 540);
            }
        }
    } else if (lairState == TEA_LAIR) {
        if (position.x >= 43 && position.x < 46 &&
            position.y > 325 && position.y < 375) {
            lairState = START_LAIR;
            setBackgroundTexture();
            character.setPosition(826, 340);
        }
    }
}

auto Game::setBackgroundTexture() -> void {
    if (lairState == START_LAIR)
        background.setTexture(backgroundTexture1);
    if (lairState == QUEEN_LAIR)
        background.setTexture(backgroundTexture2);
    if (lairState == CHESHIRE_LAIR)
        background.setTexture(backgroundTexture3);
    if (lairState == MUSHROOM_LAIR)
        background.setTexture(backgroundTexture4);
    if (lairState == TEA_LAIR)
        background.setTexture(backgroundTexture5);
}

auto Game::gameOverWindow() -> void {
    windowOver.create(sf::VideoMode(400, 300), "Game Over", sf::Style::Titlebar | sf::Style::Close);
    while (windowOver.isOpen()) {
        auto event = sf::Event();
        while (windowOver.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowOver.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                auto mousePos = sf::Mouse::getPosition(windowOver);
                if (restartText.getGlobalBounds().contains(
                        {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})) {
                    gameState = IN_GAME;
                    lairState = START_LAIR;
                    character.setPosition(375, 350);
                    character.setHP(6);
                    character.setState(Character::ALIVE);
                    resetParameters();
                    setBackgroundTexture();
                    windowOver.close();
                } else if (menuText.getGlobalBounds().contains(
                        {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})) {
                    lairState = MENU;
                    gameState = MAIN_MENU;
                    windowOver.close();
                }
            }
        }

        windowOver.clear(sf::Color::White);
        windowOver.draw(gameOverText);
        windowOver.draw(restartText);
        windowOver.draw(menuText);
        windowOver.display();
    }
}

auto Game::gameOverEvents() -> void {
    auto event = sf::Event();
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

auto Game::updateHpBar() -> void {
    if (cheshire != nullptr && cheshire->getHP() > 0) {
        cheshireRedHpBar.setSize(sf::Vector2f(cheshire->getHP() * 13.2, 25));
    }
    if (queen != nullptr && queen->getHP() > 0) {
        queenRedHpBar.setSize(sf::Vector2f(queen->getHP() * 7.9, 25));
    }
}

auto Game::resetParameters() -> void {

    for (auto &k: queenLairEnemies) {
        if (k != nullptr) {
            delete k;
            k = nullptr;
        }
    }
    queenLairEnemies.clear();

    queen = new Queen;
    queen->setHP(50);

    knight1 = new CardKnight;
    knight2 = new CardKnight;
    knight1->setHP(20);
    knight2->setHP(20);
    knight1->setPosition(200, 150);
    knight2->setPosition(700, 150);
    queenLairEnemies.push_back(queen);
    queenLairEnemies.push_back(knight1);
    queenLairEnemies.push_back(knight2);

    delete cheshire;
    cheshire = new CheshireCat;
    cheshire->setPosition(120, 180);
    cheshire->setHP(30);

    for (auto &m: mushroomLairEnemies) {
        if (m != nullptr) {
            delete m;
            m = nullptr;
        }
    }
    mushroomLairEnemies.clear();

    mushroom1 = new Mushroom;
    mushroom2 = new Mushroom;
    mushroom3 = new Mushroom;
    mushroom1->setPosition(70, 530);
    mushroom2->setPosition(790, 180);
    mushroom3->setPosition(600, 525);
    mushroomLairEnemies.push_back(mushroom1);
    mushroomLairEnemies.push_back(mushroom2);
    mushroomLairEnemies.push_back(mushroom3);
    for (auto &m: mushroomLairEnemies) {
        m->setHP(20);
    }

    updateHpBar();

    delete potion;
    potion = nullptr;
    delete hat;
    hat = nullptr;
    rabbit.setPosition(800, 350);

}

auto Game::victoryWindow() -> void {
    windowVictory.create(sf::VideoMode(400, 300), "Victory", sf::Style::Titlebar | sf::Style::Close);
    while (windowVictory.isOpen()) {
        auto event = sf::Event();
        while (windowVictory.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowVictory.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                auto mousePos = sf::Mouse::getPosition(windowVictory);
                if (menuText2.getGlobalBounds().contains(
                        {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})) {
                    lairState = MENU;
                    gameState = MAIN_MENU;
                    resetParameters();
                    windowVictory.close();
                }
            }
        }
        windowVictory.clear(sf::Color::White);
        windowVictory.draw(victoryText);
        windowVictory.draw(menuText2);
        windowVictory.display();
    }
}


auto Game::checkRabbitHitByCup() -> void {
    for (auto it = cups.begin(); it != cups.end(); ++it) {
        if ((*it)->getGlobalBounds().intersects(rabbit.getGlobalBounds())) {
            // Cup is hitting the rabbit from below
            if (character.getPosition().y < rabbit.getPosition().y) {
                rabbit.setPosition(rabbit.getPosition().x, rabbit.getPosition().y + 15);
                // Cup is hitting the rabbit from above
            } else if (character.getPosition().y > rabbit.getPosition().y + rabbit.getGlobalBounds().height) {
                rabbit.setPosition(rabbit.getPosition().x, rabbit.getPosition().y - 15);
            }
            // Remove the cup from the vector
            cups.erase(it);
            // Break out of the loop since the cup is deleted
            break;
        }
    }
}

auto Game::saveGame(std::string &filename) -> void {
    auto file = std::ofstream(filename, std::ios::trunc);

    file << static_cast<int>(lairState) << '\n';
    file << character.getPosition().x << " " << character.getPosition().y << '\n';
    file << character.getHP() << '\n';
    file << character.getATK() << '\n';

    if (cheshire != nullptr)
        file << cheshire->getPosition().x << " " << cheshire->getPosition().y << '\n';

    for (auto &e: queenLairEnemies) {
        if (e != nullptr) {
            file << e->getPosition().x << " " << e->getPosition().y << '\n';
        }
    }

    for (auto &e: mushroomLairEnemies) {
        if (e != nullptr) {
            file << e->getPosition().x << " " << e->getPosition().y << '\n';
        }
    }

    file << rabbit.getPosition().x << " " << rabbit.getPosition().y << '\n';
    if (potion != nullptr)
        file << potion->getPosition().x << " " << potion->getPosition().y << '\n';

    file.close();
}

auto Game::loadGame(std::string &filename) -> bool {
    auto file = std::ifstream(filename);
    if (!file.is_open()) {
        return false;
    }

    int state;
    file >> state;
    lairState = static_cast<LairState>(state);
    float x, y;
    file >> x >> y;
    character.setPosition(x, y);
    int hp, atk;
    file >> hp;
    character.setHP(hp);
    file >> atk;
    character.setATK(atk);

    if (cheshire != nullptr) {
        file >> x >> y;
        cheshire->setPosition(x, y);
    }

    for (auto& e : queenLairEnemies) {
        if (e != nullptr) {
            file >> x >> y;
            e->setPosition(x, y);
        }
    }

    for (auto& e : mushroomLairEnemies) {
        if (e != nullptr) {
            file >> x >> y;
            e->setPosition(x, y);
        }
    }

    file >> x >> y;
    rabbit.setPosition(x, y);
    if (potion != nullptr) {
        file >> x >> y;
        potion->setPosition(x, y);
    }

    file.close();
    return true;
}

auto Game::noSaveWindow() -> void {
    windowNoSave.create(sf::VideoMode(400, 300), "No saved game", sf::Style::Titlebar | sf::Style::Close);
    while (windowNoSave.isOpen()) {
        auto event = sf::Event();
        while (windowNoSave.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                windowNoSave.close();
        }
        windowNoSave.clear(sf::Color::White);
        windowNoSave.draw(noSavedGameText);
        windowNoSave.display();
    }
}



