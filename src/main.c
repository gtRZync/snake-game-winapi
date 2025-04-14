#include "game.h"

GameEntryPoint()
{
    Game* game = InitializeGame();

    game->createWindow(game, hInstance, SW_SHOW);
    game->update(); 

    game->destroy(game);

    return (int32_t)msg.wParam;
}