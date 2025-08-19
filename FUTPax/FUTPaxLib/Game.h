///
/// Created by Jackson White on 8/6/25.
/// Game class manages all different modes
///

#ifndef GAME_H
#define GAME_H

class PackOpener;
class GameMode;

#include <SFML/Graphics.hpp>
#include "map"


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

    void InitializeAvailableFormations();

    void InitializeMenuOptions();

    /// the window that portrays the game
    sf::RenderWindow* mWindow;

    /// the size of the screen
    sf::VideoMode mVideoMode;

    /// the game mode that is selected
    std::shared_ptr<GameMode> mGameMode = nullptr;

    /// boolean if the game mode is being shown, or just in normal menus (will be false normally once menu started)
    bool mInGame = false;

    /// all the card path names in the cards folder
    std::vector<std::string> mAvailableCards;

    /// all the formations in the formation folder
    std::vector<std::string> mFormations;

    /// all the sprites for the menu options
    std::vector<std::shared_ptr<sf::Sprite>> mMenuSprites;

    /// all the textures for menu options
    std::vector<std::shared_ptr<sf::Texture>> mMenuTextures;

    std::map<std::string, std::vector<std::string>> mAvailableCardsPositions;

public:
    Game();

    ~Game();

    /// boolean to check if the game is still running
    bool GameRunning() const { return mWindow->isOpen(); }

    void Render();

    void Update();

    /**
     * Get a list of all available cards in the game (Called in PackOpener)
     * @return vector of all card path names
     */
    std::vector<std::string> GetAvailableCards() { return mAvailableCards; };

    /**
     * Get the available formations for the draft (will be called in Draft)
     * @return a vector of all available formations
     */
    std::vector<std::string> GetFormations() { return mFormations; };

    std::vector<std::string> GetCardsByPosition(std::string pos) { return mAvailableCardsPositions[pos]; }


};



#endif //GAME_H
