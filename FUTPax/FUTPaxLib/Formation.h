//
// Created by Jackson White on 8/12/25.
//

#ifndef FORMATION_H
#define FORMATION_H


/**
 * A formation is held inside the draft game mode
 */
class Formation {
private:
    /// the sprite of the formation we are we have
    std::shared_ptr<sf::Sprite> mSprite = nullptr;

    /// the texture of the formation
    std::shared_ptr<sf::Texture> mTexture = nullptr;

    /// the formation name, ex 433
    std::string mName;

public:
    /// default constructor disabled
    Formation() = delete;

    Formation(std::shared_ptr<sf::Sprite>, std::shared_ptr<sf::Texture>, std::string );

    bool WasClicked(const sf::Event::MouseButtonReleased *mouseButton);

    /**
     * Get the sprite of the formation
     *
     * @return a shared pointer to the sprite
     */
    std::shared_ptr<sf::Sprite> GetSprite() { return mSprite; };

    /**
     * Get the name of the formation
     * @return return the name of the formation as a string
     */
    std::string GetName() { return mName; };
};



#endif //FORMATION_H
