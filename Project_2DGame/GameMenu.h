#pragma once
#include <SFML/Graphics.hpp>

class GameMenu {
    sf::RenderWindow menuWindow;
    sf::Text gameName;
    sf::Text playButton;
    sf::Text resumeButton;
    sf::Font gameFont;

public:
    GameMenu();
    auto loadFont() -> void;
    auto drawMenuObjects(sf::RenderWindow& window) -> void;
    auto clickedPlayButton(sf::Vector2i mousePos) -> bool;
    auto clickedResumeButton(sf::Vector2i mousePos) -> bool;
    auto textExpand(sf::RenderWindow& window) -> void;
    auto setParameters() -> void;
};



