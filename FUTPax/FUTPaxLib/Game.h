//
// Created by Jackson White on 8/6/25.
//

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Game {
private:
    /// the window that portrays the game
    sf::RenderWindow* mWindow;

    /// the size of the screen
    sf::VideoMode mVideoMode;

    void InitializeVariables();

    void InitializeWindow();

public:
    Game();

    ~Game();

    void CheckEvents();

    bool GameRunning() const;

    void Render();

    void Update();
};



#endif //GAME_H
