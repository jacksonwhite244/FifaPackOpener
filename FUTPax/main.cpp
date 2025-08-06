#include <iostream>
#include <Game.h>

int main()
{
    Game game;;

    while (game.GameRunning())
    {
        game.Update();

        game.Render();
    }
}