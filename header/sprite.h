#pragma once 
#include <windows.h>
#include <stdint.h>
#include <stdio.h>

//TODO: Refactor To decouple currentRow and Frame
typedef interface tagSprites
{
    HDC memDC;
    HBITMAP sheet;
    BITMAP  sheetInfo;
    uint32_t height;
    uint32_t width;
    uint8_t  totalCols;
    uint8_t  totalRows;
}SPRITE;

typedef struct Frame
{
    uint8_t row, col;
}Frame;

void ShowLastErrorMessage(const char* where);
void SetupSprite(SPRITE *sprite, LPCSTR filepath, uint8_t totalRows, uint8_t totalCols);
void setFrameRow(const SPRITE* sprite, Frame* frame, uint8_t rowIndex);
void setFrameCol(const SPRITE* sprite, Frame* frame, uint8_t frameIndex);
void SpriteCleanup(SPRITE* sprite);

