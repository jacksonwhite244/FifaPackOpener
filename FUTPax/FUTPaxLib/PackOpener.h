///
/// Created by Jackson White on 8/7/25.
/// Pack Opener Mode, where user opens packs to get cards
///

#ifndef OPENER_H
#define OPENER_H

#include "GameMode.h"
class Game;

/**
 * Pack Opener mode where user can open free packs. Derived from GameMode class
 * later on maybe the opener can use different pack odds based on what pack is being opened
 */
class PackOpener : public GameMode {
public:
    PackOpener() =  delete;
    PackOpener(Game * game);

    void Draw(sf::RenderWindow *window) override;

private:
    /// vector of all sprites of packed cards
    std::vector<std::pair<std::pair<std::shared_ptr<sf::Sprite>, std::shared_ptr<sf::Texture>>, int>> mCards;

    /// the texture of the actual pack
    std::shared_ptr<sf::Texture> mPackTexture = nullptr;

    /// the sprite of the actual pack
    std::shared_ptr<sf::Sprite> mPackSprite = nullptr;

    /// boolean if the user has opened the pack yet
    bool mOpened = false;

    /// boolean if the user has fully opened the pack yet (all 9 cards displayed)
    bool mFullOpened = false;

    /// if the pack is currently in the changing state (pack is going up and face card is revealing)
    bool mOpening = false;

    void OnClick(const sf::Event::MouseButtonReleased *) override;

    void AdjustPack();

    void GeneratePack();

    void FullyOpenedCardPositions();

    void NewPack();
};



#endif //OPENER_H
