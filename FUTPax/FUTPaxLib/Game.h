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
    /// the window that portrays the game
    sf::RenderWindow* mWindow;

    /// the size of the screen
    sf::VideoMode mVideoMode;

    void InitializeVariables();

    void InitializeWindow();

    /// the game mode that is selected
    std::shared_ptr<GameMode> mGameMode = nullptr;



public:
    Game();

    ~Game();

    void CheckEvents();

    bool GameRunning() const;

    void Render();

    void Update();
};



#endif //GAME_H
