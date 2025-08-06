//
// Created by Jackson White on 8/6/25.
//

#include "Game.h"

using namespace std;

/**
 * Base constructor for the game, Initializes variables and creates the window
 */
Game::Game() {
    InitializeVariables();
    InitializeWindow();
}

/**
 * Game destructor needs to delete all of the dynamically allocated memory
 */
Game::~Game() {
    delete mWindow;
}

/**
 * Update the game
 */
void Game::Update() {
    CheckEvents();
}

/**
 * visualize the game
 */
void Game::Render() {
    mWindow->clear(sf::Color::Transparent);
    mWindow->display();
}

/**
 * Initialize all the variables of the game
 */
void Game::InitializeVariables() {
    mWindow = nullptr;
}

/**
 * Initialize the window with a certain size
 */
void Game::InitializeWindow() {
    mVideoMode.size = {1536, 896};
    mWindow = new sf::RenderWindow(mVideoMode, "FUTPax", sf::Style::Titlebar | sf::Style::Close);
}

bool Game::GameRunning() const {
    return mWindow->isOpen();
}

void Game::CheckEvents() {
    while (std::optional<sf::Event> event = mWindow->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            mWindow->close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                mWindow->close();
        }
    }
}
