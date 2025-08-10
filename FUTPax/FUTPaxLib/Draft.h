///
/// Created by Jackson White on 8/6/25.
/// Draft Mode, where user drafts a team to play with
///

#ifndef DRAFT_H
#define DRAFT_H

#include "GameMode.h"
class Game;

/**
 * Draft class game mode where the user can draft a squad
 */
class Draft : public GameMode{
private:
    /// the texture of the home icon
    std::shared_ptr<sf::Texture> mHomeTexture = nullptr;

    /// the sprite of the home icon
    std::shared_ptr<sf::Sprite> mHomeSprite = nullptr;

    /// the texture of pitch
    std::shared_ptr<sf::Texture> mPitchTexture = nullptr;

    /// the sprite of the pitch
    std::shared_ptr<sf::Sprite> mPitchSprite = nullptr;

    void OnClick(const sf::Event::MouseButtonReleased *) override;

public:
    /// default constructor deleted
    Draft() = delete;

    Draft(Game *game);

    void Draw(sf::RenderWindow *window) override;
};



#endif //DRAFT_H
