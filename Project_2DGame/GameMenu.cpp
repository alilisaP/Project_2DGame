#include "GameMenu.h"
#include <iostream>

GameMenu::GameMenu() {
    loadFont();
    setParameters();
}

auto GameMenu::loadFont() -> void {
    if(!gameFont.loadFromFile("assets/Daydream.ttf"))
        std::cout << "error loading font" << '\n';
}

auto GameMenu::drawMenuObjects(sf::RenderWindow &window) -> void {
    window.draw(gameName);
    window.draw(playButton);
    window.draw(resumeButton);
}

auto GameMenu::clickedPlayButton(sf::Vector2i mousePos) -> bool {
    return playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

auto GameMenu::clickedResumeButton(sf::Vector2i mousePos) -> bool {
    return resumeButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}


auto GameMenu::textExpand(sf::RenderWindow& window) -> void {
    auto mousePos = sf::Mouse::getPosition(window);
    if(playButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        playButton.setCharacterSize(35);
    } else {
        playButton.setCharacterSize(30);
    }

    if(resumeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        resumeButton.setCharacterSize(35);
    } else {
        resumeButton.setCharacterSize(30);
    }

}

auto GameMenu::setParameters() -> void {
    //game title parameters
    gameName.setFont(gameFont);
    gameName.setString("           Alice\n                in\nWonderland");
    gameName.setFillColor(sf::Color::Black);
    gameName.setCharacterSize(45);
    gameName.setPosition({220, 150});

    //play button parameters
    playButton.setFont(gameFont);
    playButton.setFillColor(sf::Color(126, 220, 243));
    playButton.setString("Play");
    playButton.setCharacterSize(30);
    playButton.setPosition({415, 400});

    //resume playing button parameters
    resumeButton.setFont(gameFont);
    resumeButton.setFillColor(sf::Color(126, 220, 243));
    resumeButton.setString("Resume");
    resumeButton.setCharacterSize(30);
    resumeButton.setPosition({375, 480});
}


