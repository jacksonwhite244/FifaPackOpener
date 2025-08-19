//
// Created by Jackson White on 8/16/25.
//

#ifndef PLAYERSELECTIONS_H
#define PLAYERSELECTIONS_H

#include "GameMode.h"
class Game;
class Card;

class PlayerSelections : public GameMode {
public:
    /// default constructor disabled
    PlayerSelections()=delete;

    PlayerSelections(Game * game, std::string position);

    void Draw(sf::RenderWindow *window) override;

private:

    void OnClick(const sf::Event::MouseButtonReleased *) override;

    void GenerateOptions();

    /// the vector of cards
    std::vector<std::shared_ptr<Card>> mCards;

    /// the position we are loading cards for
    std::string mPosition;

private:

};



#endif //PLAYERSELECTIONS_H
