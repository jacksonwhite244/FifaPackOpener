//
// Created by Jackson White on 8/12/25.
//

#include "pch.h"
#include "Formation.h"

/**
 * Custom constructor for Formation class. Each formation has a sprite + texture, and a name
 *
 * @param sprite a shared pointer to the sprite
 * @param texture a shared pointer to the texture
 * @param name the name of the formation as a string (most likely will just be the filename)
 */
Formation::Formation(std::shared_ptr<sf::Sprite> sprite, std::shared_ptr<sf::Texture> texture, std::string name) : mSprite(sprite), mTexture(texture), mName(name) {

}

/**
 * Check if the formation was selected by the user
 *
 * @param mouseButton the mouse button event we are checking
 * @return true if the sprite was clicked by the user, otherwise False
 */
bool Formation::WasClicked(const sf::Event::MouseButtonReleased * mouseButton) {
    if (mSprite->getGlobalBounds().contains(sf::Vector2<float>(mouseButton->position))) {
        return true;
    }
    return false;
}
