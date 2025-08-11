#pragma once
#include "double_buffer.h"
#include "snake.h"
#include "pellet.h"
#include "font.h"
#include "menu.h"

extern SPRITE title;
extern SPRITE keys;
extern SPRITE sound;
extern SPRITE trophee;
extern SPRITE restart_sprite;

void renderMenu(HWND hwnd, HDC hdc, const GAMESTATE* gameState);
void renderGrid(HDC hdc);
void renderHeader(HDC hdc, SPRITE* sprite);
void drawSnake(HDC hdc, Snake* snake);
void drawPellet(HDC hdc, Pellet* pellet);
void debugStat(HDC hdc, GAMESTATE* gameState);
void renderToBackBuffer(HWND hwnd, GAMESTATE* gameState, HDC back_buffer, Pellet* pellet, Snake* snake);
void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy);
void renderOnGameOver(HDC hdc, Snake* snake, Pellet* pellet, GAMESTATE* gameState);
void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale, const Frame frame, UINT transparentColorKey);
void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT* rect);
void renderControlKeysOverlay(HDC hdc, SPRITE* sprite, GAMESTATE* gameState);
int32_t renderTitle(HDC hdc, SPRITE *sprite, float scaleFactor);
void renderSoundIcon(HDC hdc, SPRITE *sprite, float scaleFactor);
Frame getCornerFrame(DIRECTIONS fromDir, DIRECTIONS toDir);
void test(HDC hdc, const RECT* lprect);
