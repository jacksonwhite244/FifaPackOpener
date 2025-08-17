///
/// Created by Jackson White on 8/7/25.
///
/// Game Mode base class to every game mode we create
///

#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <SFML/Graphics.hpp>
class Game;

/**
 * Base class for every game mode we create
 */
class GameMode {
private:
    /// the sprite of the background
    sf::Sprite* mBackgroundSprite = nullptr;

    /// the texture of the background
    sf::Texture* mBackgroundTexture = nullptr;

    /// pointer to the game object
    Game * mGame = nullptr;

    /// boolean of the status of the game mode. If we exit, go back to main menu
    bool mExitGame = false;

public:
    /// default constructor disabled
    GameMode() = delete;

    virtual ~GameMode();

    GameMode(std::string backgroundName, Game *game, sf::Vector2f position);

    virtual void Draw(sf::RenderWindow *window);

    /// pure virtual function where the game mode will interact when the user clicks on the screen
    virtual void OnClick(const sf::Event::MouseButtonReleased *) = 0;

    /// return a pointer to the game inside the game mode
    Game * GetGame() {return mGame;};

    /// exit the game mode
    void ExitGame() {mExitGame = true;};

    /**
     * Check if we want to exit the game mode
     * @return boolean true if we want to exit, false if not
     */
    bool WantsToExit() { return mExitGame; };

};



#endif //GAMEMODE_H
