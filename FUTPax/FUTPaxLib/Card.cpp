//
// Created by Jackson White on 8/12/25.
//

#include "pch.h"
#include "Card.h"

#include <utility>

using namespace std;
/**
 * Custom constructor for the card
 *
 * @param sprite a shared pointer to the sprite
 * @param texture a shared pointer to the texture
 * @param rating the overall of the card
 */
Card::Card(shared_ptr<sf::Sprite> sprite, shared_ptr<sf::Texture> texture , int rating) : mSprite(std::move(sprite)), mTexture(std::move(texture)), mRating(rating) {

}

/**
 * Check if the card was selected by the user
 *
 * @param mouseButton the mouse button event we are checking
 * @return true if the sprite was clicked by the user, otherwise False
 */
bool Card::WasClicked(const sf::Event::MouseButtonReleased * mouseButton) {
    if (mSprite->getGlobalBounds().contains(sf::Vector2<float>(mouseButton->position))) {
        return true;
    }
    return false;
}
