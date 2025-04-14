#ifndef SPRITE_H
#define SPRITE_H
#pragma once 
#include <windows.h>
#include <stdint.h>

typedef interface tagSprites
{
    HDC memDC;
    HBITMAP sheet;
    BITMAP  sheetInfo;
    uint32_t height;
    uint32_t width;
    uint8_t  currentRow;
    uint8_t  currentFrame;
    uint8_t  totalFrames_on_x;
    uint8_t  numrows;
}SPRITE;

void ShowLastErrorMessage();
void SetupSprite(SPRITE* sprite, LPCSTR filepath, uint8_t numrows);
void SpriteCleanup(SPRITE* sprite);


#endif