//
// Created by Jackson White on 8/12/25.
//

#include "pch.h"
#include "Card.h"

using namespace std;
/**
 * Custom constructor for the card
 *
 * @param sprite a shared pointer to the sprite
 * @param texture a shared pointer to the texture
 * @param rating the overall of the card
 */
Card::Card(shared_ptr<sf::Sprite> sprite, shared_ptr<sf::Texture> texture , int rating) : mSprite(sprite), mTexture(texture), mRating(rating) {

}
