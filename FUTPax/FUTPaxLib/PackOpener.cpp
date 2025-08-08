///
/// Created by Jackson White on 8/7/25.
///

#include "pch.h"
#include "PackOpener.h"
#include <random>
#include "Game.h"

using namespace std;

/**
 * Custom constructor for PackOpener game mode
 */
PackOpener::PackOpener(Game * game) : GameMode("backgrounds/background.png", game){
    mPackTexture = std::make_shared<sf::Texture>();
    if (mPackTexture->loadFromFile("images/normalPack.png")) {
        mPackSprite = std::make_shared<sf::Sprite>(*mPackTexture);
        sf::Vector2u packSize = mPackTexture->getSize();
        /// set origin to the middle of the card and set the position to the middle of the screen
        mPackSprite->setOrigin(sf::Vector2f(packSize.x / 2.f, packSize.y / 2.f));
        mPackSprite->setPosition(sf::Vector2f(672 / 2.f, 950 / 2.f));
    }
    mHomeTexture = std::make_shared<sf::Texture>();
    if (mHomeTexture->loadFromFile("images/home-icon.png")) {
        mHomeSprite = std::make_shared<sf::Sprite>(*mHomeTexture);
        mHomeSprite->setPosition(sf::Vector2f(5, 5));
    }
    GeneratePack();
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
        window->draw(*mCards[0].first.first);
        window->draw(*mPackSprite);
        if (not mOpening) {
            window->draw(*mHomeSprite);
        }
    }

    /// is opened, but not fully (showing first card)
    else if (mOpened and not mFullOpened) {
        window->draw(*mCards[0].first.first);
    }

    /// pack is fully opened showing all cards
    else if (mOpened and mFullOpened) {
        for (auto card : mCards) {
            window->draw(*card.first.first);
        }
        window->draw(*mHomeSprite);
    }

}

/**
 * Virtual ovveriden function that handles when a user has clicked their mouise
 * @param  mouseButton he event we are dealong with (mouse button released)
 */
void PackOpener::OnClick(const sf::Event::MouseButtonReleased * mouseButton) {


    if (mHomeSprite->getGlobalBounds().contains(sf::Vector2<float>(mouseButton->position))) {
        /// if the pack is currently opening or opened but not fully opened, skip since the home icon isnt being shown
        /// everything other than that should result in the user being sent back home
        if (not ((not mOpened && mOpening) || (mOpened and not mFullOpened))) {
            /// some sort of call to make us go back to the home menu
        }
    }

    if (not mOpened) {
        if (mPackSprite->getGlobalBounds().contains(sf::Vector2<float>(mouseButton->position))) {
            // Mouse clicked inside the pack sprite
            mOpening = true;
        }

    }
    else if (mOpened and not mFullOpened) {
        if (mCards[0].first.first->getGlobalBounds().contains(sf::Vector2<float>(mouseButton->position))) {
            mFullOpened = true;
            FullyOpenedCardPositions();
        }
    }

    else if (mOpened and mFullOpened) {
        NewPack();
    }

}

/**
 * adjust the pack on the screen
 * If the pack is currently opening, it needs to raise up
 */
void PackOpener::AdjustPack() {
    /// pack is currently opening
    if (not mOpened && mOpening) {
        auto curPosition = mPackSprite->getPosition();
        if (curPosition.y < -247) {
            mOpened = true;
            mOpening = false;
            return;
        }
        mPackSprite->setPosition(sf::Vector2f(curPosition.x, curPosition.y  - .07));
    }
}

/**
 * Generate the contents of the pack. Sort the pack so the top card is always the highest rated card
 */
void PackOpener::GeneratePack() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, GetGame()->GetAvailableCards().size() - 1);

    /// generate 9 cards
    for (auto cardsGenerated = 0; cardsGenerated < 9; cardsGenerated++ ) {
        string chosenCardName = GetGame()->GetAvailableCards()[dist(gen)];


        shared_ptr<sf::Texture> chosenCardTexture = std::make_shared<sf::Texture>();
        if (chosenCardTexture->loadFromFile("cards/" + chosenCardName)) {
            shared_ptr<sf::Sprite> chosenCardSprite = std::make_shared<sf::Sprite>(*chosenCardTexture);
            sf::Vector2u textureSize = chosenCardTexture->getSize();

            /// set origin to the middle of the card and set the position to the middle of the screen
            chosenCardSprite->setOrigin(sf::Vector2f(textureSize.x / 2.f, textureSize.y / 2.f));

            /// the rating of the cards position is 2 spots before the last _ in the card path name
            int pos = chosenCardName.rfind('_') -2;
            int cardOverall = stoi(chosenCardName.substr(pos,2));

            std::pair thisPair = std::make_pair(chosenCardSprite, chosenCardTexture);
            std::pair main = std::make_pair(thisPair, cardOverall);
            mCards.push_back(main);
        }
    }
    /// we need to now sort thse cards
    std::sort(mCards.begin(), mCards.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    /// the first card is going to be the face card,
    /// so increase the size of the sprite and position where it should be for drawing
    auto chosenCardSprite = mCards[0].first.first;
    auto chosenCardTexture = mCards[0].first.second;
    sf::Vector2f scale = sf::Vector2f(1.65,1.65);
    chosenCardSprite->setScale(scale);

    // Get size of the card texture
    sf::Vector2u textureSize = chosenCardTexture->getSize();

    /// set origin to the middle of the card and set the position to the middle of the screen
    chosenCardSprite->setOrigin(sf::Vector2f(textureSize.x / 2.f, textureSize.y / 2.f));
    chosenCardSprite->setPosition(sf::Vector2f(672 / 2.f, 940 / 2.f));
}


void PackOpener::FullyOpenedCardPositions() {
    const int cols = 3;
    const int rows = 3;
    const float spacingX = 190.f;  // horizontal spacing between card centers
    const float spacingY = 270.f;  // vertical spacing between card centers

    float centerX = 672 / 2.f;
    float centerY = 950 / 2.f;

    int index = 0;
    for (int row = -1; row <= 1; ++row) {
        for (int col = -1; col <= 1; ++col) {
            mCards[index].first.first->setScale(sf::Vector2f(0.75, 0.75));
            if (index >= mCards.size()) return;

            float x = centerX + col * spacingX;
            float y = centerY + row * spacingY;

            mCards[index].first.first->setPosition(sf::Vector2f(x, y));
            index++;
        }
    }
}

/**
 * Pack has been fully opened and user has went to open another pack
 */
void PackOpener::NewPack() {
    mOpened = false;
    mFullOpened = false;
    mOpening = false;
    mCards.clear();
    GeneratePack();
    mPackSprite->setPosition(sf::Vector2f(672 / 2.f, 950 / 2.f));
}