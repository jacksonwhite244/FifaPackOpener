//
// Created by Jackson White on 8/16/25.
//

#include "pch.h"
#include "PlayerSelections.h"

using namespace std;

/**
 * Custom constructor
 *
 * @param game pointer to the game object
 * @param position the position of the player selections
 */
PlayerSelections::PlayerSelections(Game * game, string position) : GameMode("backgrounds/grey-background.png", game, {50,50}) {

}

/**
 * Draw the available players
 * @param window pointer the window we  are drawing to
 */
void PlayerSelections::Draw(sf::RenderWindow *window) {

}

/**
 * React to user input bt selecting the correct player
 * @param event the mouse click of the user
 */
void PlayerSelections::OnClick(const sf::Event::MouseButtonReleased *event) {

}

