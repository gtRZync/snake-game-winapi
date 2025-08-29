#pragma once
#include "double_buffer.h"
#include "snake.h"
#include "pellet.h"
#include "font.h"

struct Game;

extern SPRITE title;
extern SPRITE keys;
extern SPRITE sound;
extern SPRITE trophy;
extern SPRITE restart_sprite;
extern SPRITE home_sprite;


// Only public API functions — used in game.c
void renderToBackBuffer(struct Game* game, Vector2 size);
void copyToFrontBuffer(struct Game* game, Vector2 size);
void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT* rect);

