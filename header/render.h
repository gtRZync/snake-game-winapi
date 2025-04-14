#ifndef RENDER_H
#define RENDER_H
#pragma once
#include "double_buffer.h"
#include "snake.h"
#include "pellet.h"
#include "font.h"


void renderGrid(HDC hdc);
void showScore(HDC hdc);
void drawSnake(HDC hdc, Snake* snake);
void drawPellet(HDC hdc, Pellet* pellet);
void displaySnakeList(SnakeNode* head_ptr, HDC hdc);
void renderToBackBuffer(HDC back_buffer, Pellet* pellet, Snake* snake);
void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy);
void gameOver(HDC hdc, Snake* snake);
void renderSprite(HDC hdc, SPRITE sprite, int32_t cx, int32_t cy, float scale);
void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT rect);
void renderArrowOnStart(HDC hdc, SPRITE sprite, uint8_t cx, uint8_t cy);

#endif 