#ifndef RENDER_H
#define RENDER_H
#pragma once
#include "double_buffer.h"
#include "snake.h"
#include "pellet.h"
#include "font.h"


void render_grid(HDC hdc);
void show_score(HDC hdc);
void draw_snake(HDC hdc, Snake* snake_ptr);
void draw_pellet(HDC hdc, lpPellet pellet);
void debug_list(SnakeNode* head_ptr, HDC hdc);
void render_to_back_buffer(HDC back_buffer, lpPellet pellet, Snake* snake);
void copy_to_front_buffer(HDC back_buffer, HDC front_buffer, int cx, int cy);
void game_over(HDC hdc, Snake* snake);


#endif 