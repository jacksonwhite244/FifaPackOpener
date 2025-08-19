//
// Created by Jackson White on 8/6/25.
//

#include "pch.h"
#include "Draft.h"
#include "Game.h"
#include "Formation.h"
#include "Card.h"
#include "PlayerSelections.h"
#include "PlayerSelections.h"

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
    else if (mMode == PickingPlayer) {
        window->draw(*mPitchSprite);
        for (auto card : mCards) {
            window->draw(*card->GetSprite());
        }

        mPlayerSelector->Draw(window);
    }

    if (mMode != PickingPlayer) {
        window->draw(*mHomeSprite);
    }
}

/**
 * override onclick function to take users input and do what the user wants (selecting players / formations)
 */
bool Draft::OnClick(const sf::Event::MouseButtonReleased * mouseButton) {
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
                mMode = PickingPlayer;
                auto pos = card->GetPosition();
                mPlayerSelector = make_shared<PlayerSelections>(GetGame(), card->GetPosition());
            }
        }
    }
    else if (mMode == PickingPlayer) {
        mPlayerSelector->OnClick(mouseButton);
    }
    return false;
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
        mCards[1]->SetPosition("CB");
        mCards[2]->SetPosition("CB");
        mCards[3]->SetPosition("CB");
    }
    else if (mName[0] == '4') {
        mCards[1]->GetSprite()->setPosition(sf::Vector2f(672.f /10, defenderHeight));
        mCards[2]->GetSprite()->setPosition(sf::Vector2f(672.f / 3 , defenderHeight));
        mCards[3]->GetSprite()->setPosition(sf::Vector2f(672 - (672.f / 3), defenderHeight));
        mCards[4]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /10), defenderHeight));
        mCards[1]->SetPosition("LB");
        mCards[2]->SetPosition("CB");
        mCards[3]->SetPosition("CB");
        mCards[4]->SetPosition("RB");
    }
    else if (mName[0] == '5') {
        mCards[1]->GetSprite()->setPosition(sf::Vector2f(672.f /10, defenderHeight));
        mCards[2]->GetSprite()->setPosition(sf::Vector2f(672.f / 3.35 , defenderHeight));
        mCards[3]->GetSprite()->setPosition(sf::Vector2f(672.f /2, defenderHeight));
        mCards[4]->GetSprite()->setPosition(sf::Vector2f(672 - (672.f / 3.35), defenderHeight));
        mCards[5]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /10), defenderHeight));
        mCards[1]->SetPosition("LB");
        mCards[2]->SetPosition("CB");
        mCards[3]->SetPosition("CB");
        mCards[4]->SetPosition("CB");
        mCards[5]->SetPosition("RB");
    }

    /// Forwards
    /// check if the formation is a varient or just normal
    char numAttackers;
    if (mName.find('(') != string::npos) {
        numAttackers = mName[mName.find('(') - 1];
    }
    else {
        numAttackers = mName.back();
    }

    /// striker height and winger height of the sprites on the screen
    float strikerdHeight = 180;
    float wingerHeight = 240;
    if (numAttackers == '1') {
        mCards[10]->GetSprite()->setPosition(sf::Vector2f(672.f /2, strikerdHeight));
        mCards[10]->SetPosition("ST");
    }
    if (numAttackers == '2') {
        mCards[10]->GetSprite()->setPosition(sf::Vector2f(672.f /3, strikerdHeight));
        mCards[9]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /3 ), strikerdHeight));
        mCards[10]->SetPosition("ST");
        mCards[9]->SetPosition("ST");
    }
    else if (numAttackers == '3') {
        mCards[10]->GetSprite()->setPosition(sf::Vector2f(672.f / 5, wingerHeight));
        mCards[9]->GetSprite()->setPosition(sf::Vector2f(672.f / 2, strikerdHeight));
        mCards[8]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 5), wingerHeight));
        mCards[10]->SetPosition("LW");
        mCards[9]->SetPosition("ST");
        mCards[8]->SetPosition("RW");
    }
    else if (numAttackers == '4') {
        mCards[10]->GetSprite()->setPosition(sf::Vector2f(672.f / 10, wingerHeight));
        mCards[9]->GetSprite()->setPosition(sf::Vector2f(672.f / 3, strikerdHeight));
        mCards[8]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 3), strikerdHeight));
        mCards[7]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 10), wingerHeight));
        mCards[10]->SetPosition("LW");
        mCards[9]->SetPosition("ST");
        mCards[8]->SetPosition("ST");
        mCards[7]->SetPosition("RW");
    }

    /// Midfield
    /// need to filer out the varients and normal positions
    string midfield;
    int numDefenders = (mName[0] - '0') + 1;
    bool isVarient = false;
    int varience;
    if (mName.find('(') != string::npos) {
        isVarient = true;
        varience = (mName[mName.find('(') + 1]) - '0';
        midfield = mName.substr(1, mName.find('(') - 2);
    }
    else {
        midfield = mName.substr(1, mName.length() - 2);
    }

    /// numbers we need for height calculatons
    float defensiveMinded = 515.f;
    float attackingMinded = 340.f;
    float midHeight = 460.f;

    /// normal flat midfield
    if (midfield.length() == 1) {
        if (midfield == "2") {
            mCards[numDefenders]->GetSprite()->setPosition({672.f /3, midHeight});
            mCards[numDefenders+1]->GetSprite()->setPosition({672.f - (672.f /3), midHeight});
            mCards[numDefenders]->SetPosition("CM");
            mCards[numDefenders+1]->SetPosition("CM");
        }
        else if (midfield == "3") {
            if (not isVarient) {
                mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f / 5, midHeight));
                mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 2, midHeight));
                mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 5), midHeight));
                mCards[numDefenders]->SetPosition("CM");
                mCards[numDefenders+1]->SetPosition("CM");
                mCards[numDefenders + 2]->SetPosition("CM");
            }
            /// 1 cdm
            else if (varience == 2) {
                mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f / 5, midHeight));
                mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 2, defensiveMinded));
                mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 5), midHeight));
                mCards[numDefenders]->SetPosition("CM");
                mCards[numDefenders+1]->SetPosition("CDM");
                mCards[numDefenders + 2]->SetPosition("CM");
            }
            /// 2 cdms
            else if (varience == 3) {
                mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f / 5, defensiveMinded));
                mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 2, midHeight));
                mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 5), defensiveMinded));
                mCards[numDefenders]->SetPosition("CDM");
                mCards[numDefenders+1]->SetPosition("CM");
                mCards[numDefenders + 2]->SetPosition("CDM");
            }
            /// 1 cam
            else if (varience == 4) {
                mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f / 5, midHeight));
                mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 2, attackingMinded));
                mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 5), midHeight));
                mCards[numDefenders]->SetPosition("CM");
                mCards[numDefenders+1]->SetPosition("CAM");
                mCards[numDefenders + 2]->SetPosition("CM");
            }
        }
        else if (midfield == "4") {
            if (not isVarient) {
                mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f / 10, midHeight));
                mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 3, midHeight));
                mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 3), midHeight));
                mCards[numDefenders + 3]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 10), midHeight));
                mCards[numDefenders]->SetPosition("LM");
                mCards[numDefenders+1]->SetPosition("CM");
                mCards[numDefenders + 2]->SetPosition("CM");
                mCards[numDefenders + 3]->SetPosition("RM");
            }
            else {
                mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f / 10, midHeight));
                mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 3, defensiveMinded));
                mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 3), defensiveMinded));
                mCards[numDefenders + 3]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 10), midHeight));
                mCards[numDefenders]->SetPosition("LW");
                mCards[numDefenders+1]->SetPosition("CDM");
                mCards[numDefenders + 2]->SetPosition("CDM");
                mCards[numDefenders + 3]->SetPosition("RM");
            }
        }
        else if (midfield == "5") {
            if (not isVarient) {
                if (numDefenders -1 == 3) {
                    mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f /10, midHeight));
                    mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 3.35 , defensiveMinded));
                    mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f /2, attackingMinded));
                    mCards[numDefenders + 3]->GetSprite()->setPosition(sf::Vector2f(672 - (672.f / 3.35), defensiveMinded));
                    mCards[numDefenders + 4]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /10), midHeight));
                    mCards[numDefenders]->SetPosition("LM");
                    mCards[numDefenders+1]->SetPosition("CDM");
                    mCards[numDefenders + 2]->SetPosition("CAM");
                    mCards[numDefenders + 3]->SetPosition("CDM");
                    mCards[numDefenders + 4]->SetPosition("RM");
                }
                else {
                    mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f /10, midHeight));
                    mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 3.35 , attackingMinded));
                    mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f /2, midHeight));
                    mCards[numDefenders + 3]->GetSprite()->setPosition(sf::Vector2f(672 - (672.f / 3.35), attackingMinded));
                    mCards[numDefenders + 4]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /10), midHeight));
                    mCards[numDefenders]->SetPosition("LM");
                    mCards[numDefenders+1]->SetPosition("CAM");
                    mCards[numDefenders + 2]->SetPosition("CM");
                    mCards[numDefenders + 3]->SetPosition("CAM");
                    mCards[numDefenders + 4]->SetPosition("RM");
                }
            }
            else {
                mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f /10, midHeight));
                mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 3.35 , midHeight));
                mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f /2, midHeight));
                mCards[numDefenders + 3]->GetSprite()->setPosition(sf::Vector2f(672 - (672.f / 3.35), midHeight));
                mCards[numDefenders + 4]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f /10), midHeight));
                mCards[numDefenders]->SetPosition("LM");
                mCards[numDefenders+1]->SetPosition("CM");
                mCards[numDefenders + 2]->SetPosition("CM");
                mCards[numDefenders + 3]->SetPosition("CM");
                mCards[numDefenders + 4]->SetPosition("RM");
            }
        }
    }
    else if (midfield.length() == 2) {
        int first = (midfield[0] - '0');
        int second = (midfield[1] - '0');
        /// defensive mids
        if (first == 1) {
            mCards[numDefenders]->GetSprite()->setPosition({672.f /2, defensiveMinded});
            mCards[numDefenders]->SetPosition("CDM");
        }
        else if (first == 2) {
            mCards[numDefenders]->GetSprite()->setPosition({672.f /3, defensiveMinded});
            mCards[numDefenders + 1]->GetSprite()->setPosition({672.f - (672.f /3), defensiveMinded});
            mCards[numDefenders]->SetPosition("CDM");
            mCards[numDefenders + 1]->SetPosition("CDM");
        }
        else if (first == 3) {
            mCards[numDefenders]->GetSprite()->setPosition({672.f / 5, defensiveMinded});
            mCards[numDefenders + 1]->GetSprite()->setPosition({672.f /2, defensiveMinded});
            mCards[numDefenders + 2]->GetSprite()->setPosition({672.f - (672.f / 5), defensiveMinded});
            mCards[numDefenders]->SetPosition("CM");
            mCards[numDefenders + 1]->SetPosition("CM");
            mCards[numDefenders + 2]->SetPosition("CM");
        }
        else if (first == 4) {
            mCards[numDefenders]->GetSprite()->setPosition({(672.f / 10), defensiveMinded});
            mCards[numDefenders + 1]->GetSprite()->setPosition({(672.f / 3.35), defensiveMinded});
            mCards[numDefenders + 2]->GetSprite()->setPosition({672.f - (672.f / 3.35), defensiveMinded});
            mCards[numDefenders + 3]->GetSprite()->setPosition({672.f - (672.f / 10), defensiveMinded});
            mCards[numDefenders]->SetPosition("LM");
            mCards[numDefenders + 1]->SetPosition("CM");
            mCards[numDefenders + 2]->SetPosition("CM");
            mCards[numDefenders + 3]->SetPosition("RM");
        }

        /// attacking mids
        int firstSpot = first + numDefenders;
        if (second == 1) {
            mCards[firstSpot]->GetSprite()->setPosition({672.f /2, attackingMinded});
            mCards[firstSpot]->SetPosition("CAM");
        }
        else if (second == 2) {
            mCards[firstSpot]->GetSprite()->setPosition({(672.f / 7), attackingMinded});
            mCards[firstSpot + 1]->GetSprite()->setPosition({672.f - (672.f / 7), attackingMinded});
            mCards[firstSpot]->SetPosition("CAM");
            mCards[firstSpot + 1]->SetPosition("CAM");
        }
        else if (second == 3) {
            mCards[firstSpot]->GetSprite()->setPosition({(672.f / 5), attackingMinded});
            mCards[firstSpot + 1]->GetSprite()->setPosition({672.f / 2, attackingMinded});
            mCards[firstSpot + 2]->GetSprite()->setPosition({672.f - (672.f / 5), attackingMinded});
            mCards[firstSpot]->SetPosition("LM");
            mCards[firstSpot + 1]->SetPosition("CAM");
            mCards[firstSpot + 2]->SetPosition("RM");
        }
        else if (second == 4) {
            mCards[firstSpot]->GetSprite()->setPosition({(672.f / 10), attackingMinded});
            mCards[firstSpot + 1]->GetSprite()->setPosition({(672.f / 3), attackingMinded});
            mCards[firstSpot + 2]->GetSprite()->setPosition({672.f - (672.f / 3), attackingMinded});
            mCards[firstSpot + 3]->GetSprite()->setPosition({672.f - (672.f / 10), attackingMinded});
            mCards[firstSpot]->SetPosition("LM");
            mCards[firstSpot + 1]->SetPosition("CM");
            mCards[firstSpot + 2]->SetPosition("CM");
            mCards[firstSpot + 3]->SetPosition("RM");
        }
    }
    else if (midfield.length() == 3) {
        /// we know its going to be 1-2-1, but with a varience or not
        if (isVarient) {
            mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f(672.f / 5, midHeight));
            mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 2, defensiveMinded));
            mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 5), midHeight));
            mCards[numDefenders + 3]->GetSprite()->setPosition({672.f / 2, attackingMinded});
            mCards[numDefenders]->SetPosition("CM");
            mCards[numDefenders + 1]->SetPosition("CDM");
            mCards[numDefenders + 2]->SetPosition("CM");
            mCards[numDefenders + 3]->SetPosition("CAM");
        }
        else {
            mCards[numDefenders]->GetSprite()->setPosition(sf::Vector2f((672.f / 10), midHeight));
            mCards[numDefenders + 1]->GetSprite()->setPosition(sf::Vector2f(672.f / 2, defensiveMinded));
            mCards[numDefenders + 2]->GetSprite()->setPosition(sf::Vector2f(672.f - (672.f / 10), midHeight));
            mCards[numDefenders + 3]->GetSprite()->setPosition({672.f / 2, attackingMinded});
            mCards[numDefenders]->SetPosition("LM");
            mCards[numDefenders + 1]->SetPosition("CDM");
            mCards[numDefenders + 2]->SetPosition("RM");
            mCards[numDefenders + 3]->SetPosition("CAM");
        }
    }

}
