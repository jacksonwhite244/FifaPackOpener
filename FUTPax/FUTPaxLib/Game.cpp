///
/// Created by Jackson White on 8/6/25.
///

#include "pch.h"
#include "Game.h"

#include "PackOpener.h"

using namespace std;

/**
 * Base constructor for the game, Initializes variables and creates the window
 */
Game::Game() {
    InitializeAvailableCards();
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

    mGameMode->Draw(mWindow);

    mWindow->display();
}

/**
 * Initialize all the variables of the game
 */
void Game::InitializeVariables() {
    mGameMode = std::make_shared<PackOpener>(this);
}

/**
 * Initialize the window with a certain size
 */
void Game::InitializeWindow() {
    mVideoMode.size = {672, 950};
    mWindow = new sf::RenderWindow(mVideoMode, "FUTPax", sf::Style::Titlebar | sf::Style::Close);
}

bool Game::GameRunning() const {
    return mWindow->isOpen();
}

void Game::CheckEvents() {
    while (std::optional<sf::Event> event = mWindow->pollEvent())
    {
        if (event->is<sf::Event::Closed>()) {
            mWindow->close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                mWindow->close();
        }

        /// the user has clicked the mouse
        else if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (buttonPressed->button == sf::Mouse::Button::Left && mInGame) {
                mGameMode->OnClick(buttonPressed);
            }
        }
    }
}

/**
 * load in all card image names from the cards folder
 * Store the image names inside a vector
 */
void Game::InitializeAvailableCards() {
    string cardPath = "/Users/jacksonwhite/CodingProjects/FifaPackOpener/FUTPax/cards";
    for (const auto& entry : std::filesystem::directory_iterator(cardPath)) {
        if (entry.is_regular_file()) {
            auto path = entry.path();
            // Filter for image extensions if needed
            if (path.extension() == ".png" || path.extension() == ".jpg") {
                mAvailableCards.push_back(path.filename().string());
            }
        }
    }
}
