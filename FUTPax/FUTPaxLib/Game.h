///
/// Created by Jackson White on 8/6/25.
/// Game class manages all different modes
///

#ifndef GAME_H
#define GAME_H

class PackOpener;
class GameMode;

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
/**
 * Game class manages all different modes
 * Can be in draft mode, pack opening mode
 */
class Game {
private:
    void InitializeVariables();

    void InitializeWindow();

    void CheckEvents();

    void InitializeAvailableCards();

    /// the window that portrays the game
    sf::RenderWindow* mWindow;

    /// the size of the screen
    sf::VideoMode mVideoMode;

    /// the game mode that is selected
    std::shared_ptr<GameMode> mGameMode = nullptr;

    /// boolean if the game mode is being shown, or just in normal menus (will be false normally once menu started)
    bool mInGame = true;

    /// all the card path names in the cards folder
    std::vector<std::string> mAvailableCards;

public:
    Game();

    ~Game();

    bool GameRunning() const;

    void Render();

    void Update();

    /**
     * Get a list of all available cards in the game
     * @return vector of all card path names
     */
    std::vector<std::string> GetAvailableCards() { return mAvailableCards; };
};



#endif //GAME_H
