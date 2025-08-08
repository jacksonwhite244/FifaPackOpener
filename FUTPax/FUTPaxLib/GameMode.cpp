//
// Created by Jackson White on 8/7/25.
//

#include "pch.h"
#include "GameMode.h"
#include <SFML/Graphics.hpp>


using namespace std;

/**
 * Custom constructor for game mode.
 *
 * @param backgroundName the background filepath to the png
 */
GameMode::GameMode(std::string backgroundName) {
    mBackgroundTexture = new sf::Texture();
    if (mBackgroundTexture->loadFromFile(backgroundName)) {
        mBackgroundSprite = new sf::Sprite(*mBackgroundTexture);
        sf::Vector2f position(0, 0);
        mBackgroundSprite->setPosition(position);
    }
}

/**
 * Virtual function to draw the game mode
 * @param window the window we are drawing on
 */
void GameMode::Draw(sf::RenderWindow* window) {
    if (mBackgroundTexture) {
        window->draw(*mBackgroundSprite);
    }
}

/**
 * virtual destructor that deleted pointers to texture and sprites
 */
GameMode::~GameMode() {
    delete mBackgroundTexture;
    delete mBackgroundSprite;
};
