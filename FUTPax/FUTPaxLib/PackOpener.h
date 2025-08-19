///
/// Created by Jackson White on 8/7/25.
/// Pack Opener Mode, where user opens packs to get cards
///

#ifndef OPENER_H
#define OPENER_H

#include "GameMode.h"
class Game;
class Card;

/**
 * Pack Opener mode where user can open free packs. Derived from GameMode class
 * later on maybe the opener can use different pack odds based on what pack is being opened
 */
class PackOpener : public GameMode {
public:
    /// default constructor deleted
    PackOpener() =  delete;
    PackOpener(Game * game);

    void Draw(sf::RenderWindow *window) override;

    ~PackOpener();

private:
    /// vector of all sprites of packed cards
    std::vector<std::shared_ptr<Card>> mCards;

    /// the texture of the actual pack
    std::shared_ptr<sf::Texture> mPackTexture = nullptr;

    /// the sprite of the actual pack
    std::shared_ptr<sf::Sprite> mPackSprite = nullptr;

    /// the texture of the home icon
    std::shared_ptr<sf::Texture> mHomeTexture = nullptr;

    /// the sprite of the home icon
    std::shared_ptr<sf::Sprite> mHomeSprite = nullptr;

    /// boolean if the user has opened the pack yet
    bool mOpened = false;

    /// boolean if the user has fully opened the pack yet (all 9 cards displayed)
    bool mFullOpened = false;

    /// if the pack is currently in the changing state (pack is going up and face card is revealing)
    bool mOpening = false;

    bool OnClick(const sf::Event::MouseButtonReleased *) override;

    void AdjustPack();

    void GeneratePack();

    void FullyOpenedCardPositions();

    void NewPack();
};



#endif //OPENER_H
