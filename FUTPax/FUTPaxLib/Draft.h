///
/// Created by Jackson White on 8/6/25.
/// Draft Mode, where user drafts a team to play with
///

#ifndef DRAFT_H
#define DRAFT_H

#include "GameMode.h"
class Game;
class Formation;
class Card;
class PlayerSelections;

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

    /// all of the empty slots we can select a player for
    std::vector<std::shared_ptr<Card>> mAvailableSpots;

    /// all of the players that were chosen for our team
    std::vector<std::shared_ptr<Card>> mChosenPlayers;

    std::shared_ptr<PlayerSelections> mPlayerSelector;

    /// all the possible modes that are available during the gameplay
    enum Mode { ChoosingFormation, PickingPlayer, DisplayingTeam};

    /// the current mode
    Mode mMode;

    /// vector of all  optional formations
    std::vector<std::shared_ptr<Formation>> mFormations;

    /// name
    std::string mName;

    bool OnClick(const sf::Event::MouseButtonReleased *) override;

    void LoadSprites();

    void LoadFormations();

    void SelectFormation();

    void Draw(sf::RenderWindow *window) override;

    int mChoosingSpot = -1;

public:
    /// default constructor deleted
    Draft() = delete;

    Draft(Game *game);


    ~Draft();

    void SetLocations();
};



#endif //DRAFT_H
