#ifndef RENDER_H
#define RENDER_H
#pragma once
#include "double_buffer.h"
#include "snake.h"
#include "pellet.h"
#include "font.h"
#include "menu.h"

extern SPRITE title;
extern SPRITE keys;
extern SPRITE sound;

void renderMenu(HWND hwnd, HDC hdc, const GAMESTATE* gameState);
void renderGrid(HDC hdc);
void renderHeader(HDC hdc, SPRITE* sprite);
void drawSnake(HDC hdc, Snake* snake);
void drawPellet(HDC hdc, Pellet* pellet);
void displaySnakeList(SnakeNode* head_ptr, HDC hdc);
void renderToBackBuffer(HWND hwnd, GAMESTATE* gameState, HDC back_buffer, Pellet* pellet, Snake* snake);
void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy);
void gameOver(HDC hdc, Snake* snake, GAMESTATE* gameState);
void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale, UINT transparentColorKey);
void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT rect);
void renderControlKeysOverlay(HDC hdc, SPRITE* sprite, GAMESTATE* gameState);
int32_t renderTitle(HDC hdc, SPRITE *sprite, float scaleFactor);
void renderSoundIcon(HDC hdc, SPRITE *sprite, float scaleFactor, RECT* audio);
void test(HDC);

#endif 