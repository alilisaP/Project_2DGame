#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Character.h"
#include "Cup.h"
#include "GameMenu.h"
#include "CheshireCat.h"
#include "Queen.h"
#include "CardKnight.h"
#include "Mushroom.h"
#include "Collectibles.h"

class Game {

    //enum that checks if the user is in the menu or playing the game
    enum LairState {
        MENU, START_LAIR, QUEEN_LAIR, MUSHROOM_LAIR, CHESHIRE_LAIR, TEA_LAIR
    };

    enum GameState {
        INTRO_GAME, IN_GAME, GAME_OVER, MAIN_MENU
    };

    //the lairState of the lair
    LairState lairState;

    //the lairState of the game
    GameState gameState;

    //the window
    sf::RenderWindow window;
    GameMenu menu;

    //game over window
    sf::RenderWindow windowOver;
    sf::Text gameOverText;
    sf::Text restartText;
    sf::Text menuText;

    //victory window
    sf::RenderWindow windowVictory;
    sf::Text victoryText;
    sf::Text menuText2;

    //no saved game window
    sf::RenderWindow windowNoSave;
    sf::Text noSavedGameText;

    //stats bar
    sf::Sprite statsBar;
    sf::Texture statsBarTexture;
    sf::Text hp;
    sf::Text atk;
    sf::Text inventory;
    sf::Font statsFont;
    sf::Text save;

    //hp bar for enemies
    sf::RectangleShape cheshireHpBar;
    sf::RectangleShape cheshireRedHpBar;
    sf::RectangleShape queenHpBar;
    sf::RectangleShape queenRedHpBar;

    //backgrounds
    sf::Sprite background;
    sf::Texture backgroundTexture1;
    sf::Texture backgroundTexture2;
    sf::Texture backgroundTexture3;
    sf::Texture backgroundTexture4;
    sf::Texture backgroundTexture5;

    //the player
    Character character;

    //the bullets(cups)
    std::vector<sf::Texture*> cupTextures;
    std::vector<Cup*> cups;

    //obstacles
    std::vector<sf::Sprite> obstacles;
    std::vector<sf::Sprite> introObstacles;
    std::vector<sf::Sprite> cheshireObstacles;
    std::vector<sf::Sprite> queenObstacles;
    std::vector<sf::Sprite> mushroomObstacles;
    std::vector<sf::Sprite> tearoomObstacles;

    //the sign post
    sf::Sprite stick;
    sf::Sprite leftSign;
    sf::Sprite rightSign;
    sf::Texture stickTexture;
    sf::Texture leftSignTexture;
    sf::Texture rightSignTexture;

    //bushes (cheshire lair)
    sf::Sprite grayBush1;
    sf::Sprite grayBush2;
    sf::Sprite grayBush3;
    sf::Texture grayBushTexture;

    //another bushes (mushroom lair)
    sf::Sprite bush1;
    sf::Sprite bush2;
    sf::Sprite bush3;
    sf::Sprite bush4;
    sf::Sprite bush5;
    sf::Texture bush1Texture;
    sf::Texture bush2Texture;
    sf::Texture bush3Texture;
    sf::Texture bush4Texture;
    sf::Texture bush5Texture;

    //table
    sf::Sprite table;
    sf::Texture tableTexture;

    //enemies
    std::vector<Enemy*> cheshireLairEnemies;
    CheshireCat* cheshire;
    Queen* queen;
    CardKnight* knight1;
    CardKnight* knight2;
    std::vector<Enemy*> queenLairEnemies;
    Mushroom* mushroom1;
    Mushroom* mushroom2;
    Mushroom* mushroom3;
    std::vector<Mushroom*> mushroomLairEnemies;

    //the rabbit
    sf::Sprite rabbit;
    sf::Texture rabbitTexture;
    float velocity;

    //mushroom bullets
    std::vector<Poison*> poisonBullets;
    std::vector<sf::Texture*> poisonTextures;

    //collectibles
    Collectibles* hat;
    Potion* potion;

    //flag for moving the rabbit
    bool rabbitMoved;

    //flag to check if the hat appears
    bool hatVisible;

    //flag to check if the potion is in inventory
    bool isInInventory;

    //flag to check if the potion has already been used
    bool hasNotBeenUsed;

public:
    Game();
    ~Game();
    auto run() -> void;
    auto initializeWindow() -> void;
    auto update() -> void;
    auto drawGameObjects() -> void;
    auto loadTextures() -> void;
    auto loadFont() -> void;
    auto setObjectsParameters() -> void;
    auto switchState() -> void;
    auto setBackgroundTexture() -> void;
    auto menuEvents() -> void;
    auto gameEvents() -> void;
    auto gameOverEvents() -> void;
    auto gameOverWindow() -> void;
    auto victoryWindow() -> void;
    auto updateHpBar() -> void;
    auto resetParameters() -> void;
    auto allMushroomsDefeated(std::vector<Mushroom*>& mushrooms);
    auto checkRabbitHitByCup() -> void;

    auto saveGame(std::string& filename) -> void;
    auto loadGame(std::string& filename) -> bool;
    auto noSaveWindow() -> void;
};
