///
/// Created by Jackson White on 8/7/25.
///
/// Game Mode base class to every game mode we create
///

#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <SFML/Graphics.hpp>

/**
 * Base class for every game mode we create
 */
class GameMode {
private:
    /// the sprite of the background
    sf::Sprite* mBackgroundSprite = nullptr;

    /// the texture of the background
    sf::Texture* mBackgroundTexture = nullptr;

public:
    virtual ~GameMode();

    GameMode(std::string backgroundName);

    virtual void Draw(sf::RenderWindow *window);

    /// pure virtual function where the game mode will interact when the user clicks on the screen
    virtual void OnClick(const sf::Event::MouseButtonReleased *) = 0;
};



#endif //GAMEMODE_H
