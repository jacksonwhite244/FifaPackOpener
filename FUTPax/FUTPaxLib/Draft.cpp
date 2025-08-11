//
// Created by Jackson White on 8/6/25.
//

#include "pch.h"
#include "Draft.h"
#include "Game.h"

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
            window->draw(*formation.first.first);
        }
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
        if (not mMode == PickingPlayer) {
            ExitGame();
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
            shared_ptr<sf::Sprite> chosenCardSprite = std::make_shared<sf::Sprite>(*chosenFormationTexture);

            std::pair thisPair = std::make_pair(chosenCardSprite, chosenFormationTexture);
            std::pair main = std::make_pair(thisPair, chosenFormationName);
            mFormations.push_back(main);
        }
        availableFormations.erase(remove(availableFormations.begin(), availableFormations.end(), chosenFormationName), availableFormations.end());
        dist = uniform_int_distribution<>(0, availableFormations.size()-1);
    }
    /// place the sprites in a 2 x 3 grid
    int index = 0;
    for (int col = 0; col < 2; col++) {
        for (int row = 0; row < 3; row++) {
            /// pointer to the sprite of the formation
            auto formation = mFormations[index].first.first;

            formation->setPosition({col * 336 + 5.f, 270.f* row + 60});
            index++;
        }
    }
}

/**
 * Custom destructor
 * Clear the vector of formations
 */
Draft::~Draft() {
    mFormations.clear();
}
