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

        sf::Vector2f scale = sf::Vector2f(1.65,1.65);
        card->setScale(scale);

        // Get size of the card texture
        sf::Vector2u textureSize = faceCard->getSize();

        /// set origin to the middle of the card and set the position to the middle of the screen
        card->setOrigin(sf::Vector2f(textureSize.x / 2.f, textureSize.y / 2.f));
        card->setPosition(sf::Vector2f(672 / 2.f, 940 / 2.f));

        mTextures.push_back(faceCard);
        mCards.push_back(card);
    }

    mPackTexture = std::make_shared<sf::Texture>();
    if (mPackTexture->loadFromFile("images/normalPack.png")) {
        mPackSprite = std::make_shared<sf::Sprite>(*mPackTexture);
        sf::Vector2u packSize = mPackTexture->getSize();
        /// set origin to the middle of the card and set the position to the middle of the screen
        mPackSprite->setOrigin(sf::Vector2f(packSize.x / 2.f, packSize.y / 2.f));
        mPackSprite->setPosition(sf::Vector2f(672 / 2.f, 950 / 2.f));
    }
}

/**
 * Draw the pack opener to the window
 *
 * @param window the window we are drawing on
 */
void PackOpener::Draw(sf::RenderWindow* window) {
    GameMode::Draw(window);
    AdjustPack();
    /// cover the first card
    if (not mOpened) {
        window->draw(*mCards[0]);
        window->draw(*mPackSprite);
        return;
    }

    /// is opened, but not fully (showing first card)
    if (mOpened and not mFullOpened) {
        window->draw(*mCards[0]);
        return;
    }

    if (mOpened and not mFullOpened) {
        for (auto card : mCards) {
            window->draw(*card);
        }
    }

}

/**
 * Virtual ovveriden function that handles when a user has clicked their mouise
 * @param  mouseButton he event we are dealong with (mouse button released)
 */
void PackOpener::OnClick(const sf::Event::MouseButtonReleased * mouseButton) {
    if (not mOpened) {
        mOpening = true;
    }
}

void PackOpener::AdjustPack() {
    if (not mOpened && mOpening) {
        auto curPosition = mPackSprite->getPosition();
        if (curPosition.x < -247) {
            mOpened = true;
            mOpening = false;
            return;
        }
        mPackSprite->setPosition(sf::Vector2f(curPosition.x, curPosition.y  - .07));
    }
}