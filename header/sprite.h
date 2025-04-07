#ifndef SPRITE_H
#define SPRITE_H
#pragma once 
#include <windows.h>
#include <stdint.h>

typedef struct tagSprites
{
    HBITMAP Spritesheet;
    BITMAP SpriteMetrics;
    int32_t SpriteHeight;
    int32_t SpriteWidth;
    int8_t CurrentRow;
    int8_t CurrentFrame;
}SPRITE;

void LoadSprite(SPRITE sprite, LPCSTR filename);


#endif