///
/// Created by Jackson White on 8/6/25.
///

#include "pch.h"
#include "Game.h"
#include "Draft.h"
#include "PackOpener.h"

using namespace std;

/**
 * Base constructor for the game, Initializes variables and creates the window
 */
Game::Game() {
    InitializeAvailableCards();
    InitializeWindow();
    InitializeMenuOptions();
    mGameMode = std::make_shared<Draft>(this);
    mInGame = true;
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

    /// if in a game, draw the game mode
    if (mInGame) {
        mGameMode->Draw(mWindow);
    }

    /// if not, draw the main menu
    else {
        for (auto sprite : mMenuSprites) {
            mWindow->draw(*sprite);
        }
    }


    mWindow->display();
}

/**
 * Initialize all the variables of the game
 */
void Game::InitializeVariables() {

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
            if (buttonPressed->button == sf::Mouse::Button::Left) {
                if (mInGame) {
                    mGameMode->OnClick(buttonPressed);
                }
                else {
                    /// user has clicked pack opener button
                    if (mMenuSprites[2]->getGlobalBounds().contains(sf::Vector2<float>(buttonPressed->position))) {
                        mGameMode = std::make_shared<PackOpener>(this);
                        mInGame = true;
                    }
                }
            }
        }
    }
    if (mGameMode && mInGame && mGameMode->WantsToExit()) {
        mGameMode = nullptr;
        mInGame = false;
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

/**
 * Create the sprites and textures of all the menu options
 */
void Game::InitializeMenuOptions() {
    /// background sprite + texture
    shared_ptr<sf::Texture> backgroundTexture = make_shared<sf::Texture>();
    if (backgroundTexture->loadFromFile("backgrounds/background.png")) {
        shared_ptr<sf::Sprite> draft = make_shared<sf::Sprite>(*backgroundTexture);
        mMenuSprites.push_back(draft);
        mMenuTextures.push_back(backgroundTexture);
    }

    /// draft mode sprite + texture
    shared_ptr<sf::Texture> draftTexture = make_shared<sf::Texture>();
    if (draftTexture->loadFromFile("images/draft.png")) {
        shared_ptr<sf::Sprite> draft = make_shared<sf::Sprite>(*draftTexture);
        draft->setPosition({16, 44.76});
        mMenuSprites.push_back(draft);
        mMenuTextures.push_back(draftTexture);
    }

    /// pack opener sprite + texture
    shared_ptr<sf::Texture> packModeTexture = make_shared<sf::Texture>();
    if (packModeTexture->loadFromFile("images/pack.png")) {
        shared_ptr<sf::Sprite> packMode = make_shared<sf::Sprite>(*packModeTexture);
        packMode->setPosition({16, 260.94});
        mMenuSprites.push_back(packMode);
        mMenuTextures.push_back(packModeTexture);
    }
}
