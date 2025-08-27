#pragma once
#include "double_buffer.h"
#include "snake.h"
#include "pellet.h"
#include "font.h"
#include "menu.h"

extern SPRITE title;
extern SPRITE keys;
extern SPRITE sound;
extern SPRITE trophy;
extern SPRITE restart_sprite;
extern SPRITE home_sprite;


// Only public API functions — used in game.c
void renderToBackBuffer(HWND hwnd, GAMESTATE* gameState, HDC back_buffer, Pellet* pellet, Snake* snake);
void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy);
void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT* rect);
