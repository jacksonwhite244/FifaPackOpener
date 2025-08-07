///
/// Created by Jackson White on 8/7/25.
///

#include "pch.h"
#include "PackOpener.h"

/**
 * Custom constructor for PackOpener game mode
 */
PackOpener::PackOpener() : GameMode("backgrounds/background.png"){
    std::shared_ptr<sf::Texture> faceCard= std::make_shared<sf::Texture>();
    if (faceCard->loadFromFile("cards/mbappé_gold-rare_91_ST.png")) {
        std::shared_ptr<sf::Sprite> card = std::make_shared<sf::Sprite>(*faceCard);

        // Get size of the card texture
        sf::Vector2u textureSize = faceCard->getSize();
        float cardWidth = static_cast<float>(textureSize.x);
        float cardHeight = static_cast<float>(textureSize.y);

        /// the size of the window we created in game.cpp
        float windowWidth = 672;
        float windowHeight = 950;

        // Center position
        sf::Vector2f position(
            (windowWidth - cardWidth) / 2.f,
            (windowHeight - cardHeight) / 2.f
        );

        card->setPosition(position);
        mTextures.push_back(faceCard);
        mCards.push_back(card);
    }
}

/**
 * Draw the pack opener to the window
 *
 * @param window the window we are drawing on
 */
void PackOpener::Draw(sf::RenderWindow* window) {
    GameMode::Draw(window);
    for (auto card : mCards) {
        window->draw(*card);
    }
}