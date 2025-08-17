//
// Created by Jackson White on 8/16/25.
//

#ifndef PLAYERSELECTIONS_H
#define PLAYERSELECTIONS_H

#include "GameMode.h"
class Game;

class PlayerSelections : public GameMode {
public:
    /// default constructor disabled
    PlayerSelections()=delete;

    PlayerSelections(Game * game, std::string position);

    void Draw(sf::RenderWindow *window) override;

    void OnClick(const sf::Event::MouseButtonReleased *) override;

private:

};



#endif //PLAYERSELECTIONS_H
