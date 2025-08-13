//
// Created by Jackson White on 8/12/25.
//

#ifndef CARD_H
#define CARD_H


/**
 * A single card holds a sprite, texture, and overall rating
 */
class Card {
private:
    /// the sprite of the card we are we have
    std::shared_ptr<sf::Sprite> mSprite = nullptr;

    /// the texture of the card
    std::shared_ptr<sf::Texture> mTexture = nullptr;

    /// the rating of the card
    int mRating;
public:
    /// default constructor disabled
    Card() = delete;

    Card(std::shared_ptr<sf::Sprite>, std::shared_ptr<sf::Texture>, int);

    bool WasClicked(const sf::Event::MouseButtonReleased *mouseButton);

    /**
     * Get the sprite of the card
     * @return a pointer to the sprite
     */
    std::shared_ptr<sf::Sprite> GetSprite() { return mSprite; }

    /**
     * Get the texture of the card
     * @return shared pointer to the texture
     */
    std::shared_ptr<sf::Texture> GetTexture() { return mTexture; }

    /**
     * Get the rating of the card
     * @return the rating of the card
     */
    int GetRating() { return mRating; }
};



#endif //CARD_H
