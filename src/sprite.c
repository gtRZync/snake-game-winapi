#include "sprite.h"

void SetupSprite(SPRITE *sprite, LPCSTR filepath, uint8_t numrows)
{
    sprite->numrows = numrows;
    sprite->sheet = LoadImageA(NULL, filepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if(!sprite->sheet)
    {
        ShowLastErrorMessage();
        exit(EXIT_FAILURE);
    }
    if(GetObject(sprite->sheet, sizeof(sprite->sheetInfo), &sprite->sheetInfo) == 0)
    {
        ShowLastErrorMessage();
        exit(EXIT_FAILURE);
    }
    double frames = (double)(sprite->sheetInfo.bmWidth) / (double)(sprite->sheetInfo.bmHeight / numrows);
    sprite->totalFrames_on_x = (uint8_t)frames;
    sprite->width = sprite->sheetInfo.bmWidth / sprite->totalFrames_on_x;
    sprite->height = sprite->sheetInfo.bmHeight / sprite->numrows;
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

void ShowLastErrorMessage()
{
    char errorMessage[128];
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

    MessageBoxA(NULL, errorMessage, "Last Error Message", MB_OK | MB_ICONERROR);
}
