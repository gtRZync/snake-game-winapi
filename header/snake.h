#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#pragma once

#include "config.h"


typedef struct SnakeNode
{
    int32_t x, y;
    struct SnakeNode* next;
} SnakeNode;

typedef struct tagSnake
{
    int32_t cx;
    int32_t cy;
    RECT rect;
    SnakeNode* head;
    boolean should_grow;
} Snake;

SnakeNode* create_node(int cx, int cy);
void free_list(SnakeNode* head_ptr);
void _free_list(SnakeNode* head_ptr, const char* filename);
void add_head(SnakeNode** head_ptr_ptr, int cx, int cy);
void remove_tail(SnakeNode** head_ptr_ptr);
Snake create_snake(int cx, int cy);

#endif