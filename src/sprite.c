#include "sprite.h"

void SetupSprite(SPRITE* sprite, LPCSTR filepath, uint8_t totalRows, uint8_t totalCols)
{
    sprite->totalRows = totalRows;
    sprite->totalCols = totalCols;
    sprite->sheet = LoadImageA(NULL, filepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if(!sprite->sheet)
    {
        ShowLastErrorMessage("sprite->sheet");
        return;//TODO: handle egde cases, destroy the game if sprite can't be loaded
    }
    if(GetObject(sprite->sheet, sizeof(sprite->sheetInfo), &sprite->sheetInfo) == 0)
    {
        ShowLastErrorMessage("sprite->sheetInfo");
        return;
    }
    sprite->width = sprite->sheetInfo.bmWidth / sprite->totalCols;
    sprite->height = sprite->sheetInfo.bmHeight / sprite->totalRows;
    if(sprite->width <= 0 ||sprite->height <= 0)
    {
        ShowLastErrorMessage("sprite->width/height");
        return;
    }
}

void setFrameRow(const SPRITE* sprite, Frame* frame, uint8_t rowIndex)
{
    if (rowIndex < sprite->totalRows)
    {
        frame->row = rowIndex;
    }
}

void setFrameCol(const SPRITE* sprite, Frame* frame, uint8_t frameIndex)
{
    if(frameIndex < sprite->totalCols)
    {
        frame->col = frameIndex;
    }
}

void SpriteCleanup(SPRITE *sprite)
{
    if(sprite)
    {
        if(sprite->memDC)
        {
            DeleteDC(sprite->memDC);
            sprite->memDC = NULL;
        }

        if(sprite->sheet)
        {
            DeleteObject(sprite->sheet);
            sprite->sheet = NULL;
        }
    }
}

void ShowLastErrorMessage(const char* where)
{
    char errorMessage[128];
    char caption[128];
    DWORD error = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        0,
        error,
        0,
        errorMessage,
        sizeof(errorMessage),
        NULL
        );
    sprintf(caption, "Error Message from %s", where);
    MessageBoxA(NULL, errorMessage, caption, MB_OK | MB_ICONERROR);
}
