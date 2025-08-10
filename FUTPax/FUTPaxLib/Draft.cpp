//
// Created by Jackson White on 8/6/25.
//

#include "pch.h"
#include "Draft.h"

/**
 * Custom constructor
 * @param game pointer to the game object
 */
Draft::Draft(Game * game) : GameMode("backgrounds/background.png", game){
    mHomeTexture = std::make_shared<sf::Texture>();
    if (mHomeTexture->loadFromFile("images/home-icon.png")) {
        mHomeSprite = std::make_shared<sf::Sprite>(*mHomeTexture);
        mHomeSprite->setPosition(sf::Vector2f(5, 5));
    }
    mPitchTexture = std::make_shared<sf::Texture>();
    if (mPitchTexture->loadFromFile("backgrounds/pitch.png")) {
        mPitchSprite = std::make_shared<sf::Sprite>(*mPitchTexture);
        mPitchSprite->setOrigin({672/2,557/2});
        mPitchSprite->setPosition(sf::Vector2f(672/2, 950/2));
    }
}

/**
 * Override draw function to draw the draft to the screen
 *
 * @param window the window we are drawing on
 */
void Draft::Draw(sf::RenderWindow *window) {
    GameMode::Draw(window);
    window->draw(*mHomeSprite);
    window->draw(*mPitchSprite);
}

/**
 * override onclick function to take users input and do what the user wants (selecting players / formations)
 */
void Draft::OnClick(const sf::Event::MouseButtonReleased *) {

}
