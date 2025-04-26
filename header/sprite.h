#ifndef SPRITE_H
#define SPRITE_H
#pragma once 
#include <windows.h>
#include <stdint.h>
#include <stdio.h>

typedef interface tagSprites
{
    HDC memDC;
    HBITMAP sheet;
    BITMAP  sheetInfo;
    uint32_t height;
    uint32_t width;
    uint8_t  currentRow;
    uint8_t  currentFrame;
    uint8_t  totalCols;
    uint8_t  totalRows;
}SPRITE;

void ShowLastErrorMessage(const char* where);
void SetupSprite(SPRITE *sprite, LPCSTR filepath, uint8_t totalRows, uint8_t totalCols);
void setSpriteRow(SPRITE* sprite, uint8_t rowIndex);
void setSpriteFrame(SPRITE* sprite, uint8_t frameIndex);
void SpriteCleanup(SPRITE* sprite);


#endif