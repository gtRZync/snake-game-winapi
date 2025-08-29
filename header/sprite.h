#pragma once 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

//TODO: Refactor To decouple currentRow and Frame
typedef struct tagSprites
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
    union
    {
       struct {uint8_t row, col; };
       struct {uint8_t totalRows, totalCols; };
    };
}Frame;

void ShowLastErrorMessage(const char* where);
void SetupSprite(SPRITE *sprite, LPCSTR filepath, const Frame dimensions);
void setFrameRow(const SPRITE* sprite, Frame* frame, uint8_t rowIndex);
void setFrameCol(const SPRITE* sprite, Frame* frame, uint8_t frameIndex);
void SpriteCleanup(SPRITE* sprite);

