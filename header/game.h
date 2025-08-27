#pragma once

#include "window.h"
#include "menu.h"
#include "render.h"
#include "audio.h"


typedef struct Game
{
    GAMESTATE state;
    Window* window;
    Snake* snake;
    Pellet* pellet;
    HINSTANCE hInstance;
    DOUBLE_BUFFER* buffer;

    void (*createWindow)(struct Game* game, HINSTANCE hInstance, int nShowCmd);
    void (*update)(struct Game* game);
    void (*destroy)(struct Game* game);
    void (*setupDoubleBuffering)(HWND hwnd, DOUBLE_BUFFER* lpDoubleBuffer, int cx, int cy);
    void (*resizeDoubleBuffer)(HWND hwnd, DOUBLE_BUFFER *lpDoubleBuffer, int cx, int cy);
    void (*doubleBufferingCleanup)(DOUBLE_BUFFER* lpDoubleBuffer);
    void (*render)(struct Game* game, int32_t cx, int32_t cy);
    void (*restart)(struct Game* game);

    bool isRunning;
    bool isMuted;
    float deltatime;
}Game;


Game* InitializeGame();
