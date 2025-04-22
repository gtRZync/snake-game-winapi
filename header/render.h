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

void renderMenu(HWND hwnd, HDC hdc, GAMESTATE* state);
void renderGrid(HDC hdc);
void showScore(HDC hdc);
void drawSnake(HDC hdc, Snake* snake);
void drawPellet(HDC hdc, Pellet* pellet);
void displaySnakeList(SnakeNode* head_ptr, HDC hdc);
void renderToBackBuffer(HWND hwnd, GAMESTATE* state, HDC back_buffer, Pellet* pellet, Snake* snake);
void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy);
void gameOver(HDC hdc, Snake* snake);
void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale);
void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT rect);
void renderControlKeys(HDC hdc, SPRITE* sprite, GAMESTATE* state);
int32_t renderTitle(HDC hdc, SPRITE* sprite, float scaleFactor);

#endif 