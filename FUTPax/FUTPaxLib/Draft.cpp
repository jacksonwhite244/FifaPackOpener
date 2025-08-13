//
// Created by Jackson White on 8/6/25.
//

#include "pch.h"
#include "Draft.h"
#include "Game.h"
#include "Formation.h"
#include "Card.h"

using namespace std;

/**
 * Custom constructor
 * @param game pointer to the game object
 */
Draft::Draft(Game * game) : GameMode("backgrounds/grey-background.png", game){
    LoadSprites();
    LoadFormations();
    mMode = ChoosingFormation;
}

/**
 * Override draw function to draw the draft to the screen
 *
 * @param window the window we are drawing on
 */
void Draft::Draw(sf::RenderWindow *window) {
    GameMode::Draw(window);
    if (mMode == ChoosingFormation) {
        for (auto formation : mFormations) {
            window->draw(*formation->GetSprite());
        }
    }
    else if (mMode == DisplayingTeam) {
        window->draw(*mPitchSprite);
        for (auto card : mCards) {
            window->draw(*card->GetSprite());
        }
    }

    if (mMode != PickingPlayer) {
        window->draw(*mHomeSprite);
    }
}

/**
 * override onclick function to take users input and do what the user wants (selecting players / formations)
 */
void Draft::OnClick(const sf::Event::MouseButtonReleased * mouseButton) {
    /// user has clicked on the home sprite
    if (mHomeSprite->getGlobalBounds().contains(sf::Vector2<float>(mouseButton->position))) {
        /// as long as the user is not picking a player, we can exit the game
        if (mMode != PickingPlayer) {
            ExitGame();
        }
    }
    else if (mMode == ChoosingFormation) {
        for (auto formation : mFormations) {
            if (formation->WasClicked(mouseButton)) {
                mName = formation->GetName();
                SelectFormation();
                SetLocations();
                break;
            }
        }
    }
    else if (mMode == DisplayingTeam) {
        for (auto card : mCards) {
            if (card->WasClicked(mouseButton)) {
                /// start the logic for selecting the player
            }
        }
    }
}

/**
 * Load all the sprites that we are using in this game Mode
 */
void Draft::LoadSprites() {
    mHomeTexture = std::make_shared<sf::Texture>();
    //if (mHomeTexture->loadFromFile("images/home-icon.png")) {
    if (mHomeTexture->loadFromFile("images/home-icon.png")) {
        mHomeSprite = std::make_shared<sf::Sprite>(*mHomeTexture);
        mHomeSprite->setPosition(sf::Vector2f(5, 5));
    }
    mPitchTexture = std::make_shared<sf::Texture>();
    if (mPitchTexture->loadFromFile("backgrounds/pitch.png")) {
        mPitchSprite = std::make_shared<sf::Sprite>(*mPitchTexture);
        mPitchSprite->setOrigin({672/2,557/2});
        mPitchSprite->setScale({1, 1.75});
        mPitchSprite->setPosition(sf::Vector2f(672/2, 950/2));

    }
}

/**
 * Load 6 formations for the player to choose from
 */
void Draft::LoadFormations() {
    auto availableFormations  = GetGame()->GetFormations();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, availableFormations.size() - 1);

    /// generate 6 formations
    for (auto formationsGenerated = 0; formationsGenerated < 6; formationsGenerated++ ) {
        string chosenFormationName = availableFormations[dist(gen)];

        shared_ptr<sf::Texture> chosenFormationTexture = std::make_shared<sf::Texture>();
        if (chosenFormationTexture->loadFromFile("formation/" + chosenFormationName)) {
            shared_ptr<sf::Sprite> chosenFormationSprite = std::make_shared<sf::Sprite>(*chosenFormationTexture);

            auto formation = std::make_shared<Formation>(chosenFormationSprite, chosenFormationTexture, chosenFormationName);
            mFormations.push_back(formation);
        }
        availableFormations.erase(remove(availableFormations.begin(), availableFormations.end(), chosenFormationName), availableFormations.end());
        dist = uniform_int_distribution<>(0, availableFormations.size()-1);
    }
    /// place the sprites in a 2 x 3 grid
    int index = 0;
    for (int col = 0; col < 2; col++) {
        for (int row = 0; row < 3; row++) {
            /// pointer to the sprite of the formation
            auto formation = mFormations[index]->GetSprite();

            formation->setPosition({col * 336 + 5.f, 270.f* row + 60});
            index++;
        }
    }
}

/**
 * Apply the users selection by switching the mode and clearing all the formations. We will have to set up the locations
 * of the player blocks next
 */
void Draft::SelectFormation() {
    mMode = DisplayingTeam;
    mFormations.clear();
}


/**
 * Custom destructor
 * Clear the vector of formations
 */
Draft::~Draft() {
    mFormations.clear();
}

void Draft::SetLocations() {
    for (int i = 0; i < 11; i++) {
        std::shared_ptr<sf::Texture> cardTexture = std::make_shared<sf::Texture>();
        if (cardTexture->loadFromFile("images/emptySlot.png")) {
            std::shared_ptr<sf::Sprite> cardSprite = std::make_shared<sf::Sprite>(*cardTexture);
            cardSprite->setScale({0.25f, 0.25f});
            cardSprite->setOrigin({cardTexture->getSize().x / 2.f, cardTexture->getSize().y / 2.f});
            auto card = make_shared<Card>(cardSprite, cardTexture, i);
            mCards.push_back(card);
        }
    }
    mCards[0]->GetSprite()->setPosition(sf::Vector2f(672 /2, 860));

    /// DEFENDERS
    float defenderHeight = 700;
    if (mName[0] == '3') {
        mCards[1]->GetSprite()->setPosition(sf::Vector2f(672.f /5, defenderHeight));
        mCards[2]->GetSprite()->setPosition(sf::Vector2f(672.f / 2 , defenderHeight));
        mCards[3]->GetSprite()->setPosition(sf::Vector2f(672.f * 4 / 5, defenderHeight));
    }
    else if (mName[0] == '4') {
        mCards[1]->GetSprite()->setPosition(sf::Vector2f(672.f /10, defenderHeight));
        mCards[2]->GetSprite()->setPosition(sf::Vector2f(672.f / 3 , defenderHeight));
        mCards[3]->GetSprite()->setPosition(sf::Vector2f(672 - (672.f / 3), defenderHeight));
        mCards[4]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /10), defenderHeight));
    }
    else if (mName[0] == '5') {
        mCards[1]->GetSprite()->setPosition(sf::Vector2f(672.f /10, defenderHeight));
        mCards[2]->GetSprite()->setPosition(sf::Vector2f(672.f / 3.35 , defenderHeight));
        mCards[3]->GetSprite()->setPosition(sf::Vector2f(672.f /2, defenderHeight));
        mCards[4]->GetSprite()->setPosition(sf::Vector2f(672 - (672.f / 3.35), defenderHeight));
        mCards[5]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /10), defenderHeight));
    }
}
