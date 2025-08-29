#include "game.h"

GameEntryPoint()
{
    Game* game = InitializeGame();

    setInputContext(&game->input);

    game->createWindow(game, hInstance, SW_SHOW);
    
    game->update(game); 

    game->destroy(game);

    return _exitCode;
}