#pragma once

#include "utilis.h"

typedef interface SnakeNode
{
    int32_t x, y;
    struct SnakeNode* next;
    DIRECTIONS direction;
    Frame frame;
} SnakeNode;

typedef interface tagSnake
{
    int32_t cx;
    int32_t cy;
    int32_t previousX;
    int32_t previousY;
    DIRECTIONS direction;
    RECT headRect;
    RECT body;
    SnakeNode* head;
    boolean shouldGrow;
    boolean isMoving;
    boolean isDead;//! useless rn , maybe for animation in the future
    SPRITE sprite;
    SPRITE headSprite;
    uint8_t scale;
} Snake;



SnakeNode* createNode(int cx, int cy, DIRECTIONS dir);
SnakeNode* createSnakeList(int* coords, size_t array_size, DIRECTIONS dir);  
void freeSnakeMemory(SnakeNode* head_ptr);
void logAndFreeSnakeMemory(SnakeNode* head_ptr, const char* filename);
void addHead(SnakeNode** head_ptr_ptr, int cx, int cy, DIRECTIONS dir);      
void removeTail(SnakeNode** head_ptr_ptr);                   
Snake* createSnake();
#ifdef DEBUG
void printSnakeDirections(const SnakeNode* head);
#endif
