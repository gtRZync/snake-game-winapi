#pragma once
#include "window.h"
#include "render.h"
#ifndef LIMITED_GAME
#include "menu.h"
#include "audio.h"
#endif
typedef struct Game
{
    GAMESTATE state;
    Window* window;
    InputManager inputManager;
    Snake* snake;
    Pellet* pellet;
    DOUBLE_BUFFER* buffer;
    HINSTANCE hInstance;

    void (*createWindow)(struct Game* game, HINSTANCE hInstance, int nShowCmd);
    void (*update)(struct Game* game);
    void (*destroy)(struct Game* game);
    void (*setupDoubleBuffering)(HWND hwnd, DOUBLE_BUFFER* lpDoubleBuffer, int cx, int cy);
    void (*resizeDoubleBuffer)(HWND hwnd, DOUBLE_BUFFER *lpDoubleBuffer, int cx, int cy);
    void (*doubleBufferingCleanup)(DOUBLE_BUFFER* lpDoubleBuffer);
    void (*render)(struct Game* game);

    bool isRunning;
    bool isMuted;
    float deltatime;
    bool starting;
    bool isRestarting;
}Game;

#ifndef LIMITED_GAME
Game* InitializeGame();
#endif