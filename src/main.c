#include "window.h"

GameEntryPoint()
{
    CreateGameWindow(hInstance, GameWindowProc, SW_SHOW);
    HandleGameMessages();

    return (int32_t)msg.wParam;
}