///
/// Created by Jackson White on 8/7/25.
/// Pack Opener Mode, where user opens packs to get cards
///

#ifndef OPENER_H
#define OPENER_H

#include "GameMode.h"

/**
 * Pack Opener mode where user can open free packs. Derived from GameMode class
 * later on maybe the opener can use different pack odds based on what pack is being opened
 */
class PackOpener : public GameMode {
public:
    PackOpener();

    void Draw(sf::RenderWindow *window) override;

    std::vector<std::shared_ptr<sf::Sprite>> mCards;

    std::vector<std::shared_ptr<sf::Texture>> mTextures;

private:

};



#endif //OPENER_H
