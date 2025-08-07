//
// Created by Jackson White on 8/6/25.
//

#include "pch.h"
#include "Draft.h"

/**
 * Custom constructor
 */
Draft::Draft() {
    mBackgroundTexture = new sf::Texture();
    if (mBackgroundTexture->loadFromFile("background.png")) {
        mBackgroundSprite = new sf::Sprite(*mBackgroundTexture);
    }
}
