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
GameMode::GameMode(std::string backgroundName, Game * game) : mGame(game) {
    mBackgroundTexture = new sf::Texture();
    if (mBackgroundTexture->loadFromFile(backgroundName)) {
        mBackgroundSprite = new sf::Sprite(*mBackgroundTexture);
        mBackgroundSprite->setOrigin({mBackgroundTexture->getSize().x / 2.f, mBackgroundTexture->getSize().y / 2.f});
        mBackgroundSprite->setPosition({336,475});
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
