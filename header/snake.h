#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#pragma once

#include "config.h"
#include "sprite.h"

typedef interface SnakeNode
{
    int32_t x, y;
    struct SnakeNode* next;
} SnakeNode;

typedef interface tagSnake
{
    int32_t cx;
    int32_t cy;
    int32_t previousX;
    int32_t previousY;
    RECT headRect;
    RECT body;
    SnakeNode* head;
    boolean shouldGrow;
    boolean isMoving;
    boolean isDead;//! useless rn , maybe for animation in the future
    SPRITE sprite;
} Snake;

SnakeNode* createNode(int cx, int cy);
SnakeNode* createSnakeList(int* coords, size_t array_size);  
void freeSnakeMemory(SnakeNode* head_ptr);
void logAndFreeSnakeMemory(SnakeNode* head_ptr, const char* filename);
void addHead(SnakeNode** head_ptr_ptr, int cx, int cy);      
void removeTail(SnakeNode** head_ptr_ptr);                   
Snake* createSnake();

#endif