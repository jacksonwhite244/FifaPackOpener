//
// Created by Jackson White on 8/16/25.
//

#include "pch.h"
#include "PlayerSelections.h"
#include "Game.h"
#include "Card.h"

using namespace std;

/**
 * Custom constructor
 *
 * @param game pointer to the game object
 * @param position the position of the player selections
 */
PlayerSelections::PlayerSelections(Game * game, string position) : mPosition(position), GameMode("backgrounds/white-background.png", game, {35,300}) {
    GenerateOptions();
}

/**
 * Draw the available players
 * @param window pointer the window we  are drawing to
 */
void PlayerSelections::Draw(sf::RenderWindow *window) {
    GameMode::Draw(window);
    for (auto card: mCards) {
        window->draw(*card->GetSprite());
    }
}

/**
 * React to user input bt selecting the correct player
 * @param event the mouse click of the user
 */
void PlayerSelections::OnClick(const sf::Event::MouseButtonReleased *event) {

}

void PlayerSelections::GenerateOptions() {
    random_device rd;
    mt19937 gen(rd());
    auto availableCards = GetGame()->GetCardsByPosition(mPosition);
    uniform_int_distribution<> dist(0, availableCards.size() - 1);

    /// generate 5 cards
    for (auto cardsGenerated = 0; cardsGenerated < 5; cardsGenerated++ ) {
        string chosenCardName = availableCards[dist(gen)];


        shared_ptr<sf::Texture> chosenCardTexture = std::make_shared<sf::Texture>();
        if (chosenCardTexture->loadFromFile("cards/" + chosenCardName)) {
            shared_ptr<sf::Sprite> chosenCardSprite = std::make_shared<sf::Sprite>(*chosenCardTexture);
            sf::Vector2u textureSize = chosenCardTexture->getSize();

            /// set origin to the middle of the card and set the position to the middle of the screen
            chosenCardSprite->setScale({0.5, 0.5});
            chosenCardSprite->setOrigin(sf::Vector2f(textureSize.x / 2.f, textureSize.y / 2.f));

            /// the rating of the cards position is 2 spots before the last _ in the card path name
            int pos = chosenCardName.rfind('_') -2;
            int cardOverall = stoi(chosenCardName.substr(pos,2));

            auto card = std::make_shared<Card>(chosenCardSprite, chosenCardTexture, cardOverall);
            mCards.push_back(card);
        }
        availableCards.erase(remove(availableCards.begin(), availableCards.end(), chosenCardName), availableCards.end());
        dist = uniform_int_distribution<>(0, availableCards.size()-1);
    }

    for (int col = 0; col < 5; col++) {
        auto card = mCards[col]->GetSprite();
        card->setPosition({col * 110 + 130.f, 350.f});
    }
}


