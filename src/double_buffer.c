#include "double_buffer.h"

void setupDoubleBuffering(HWND hwnd, PDOUBLE_BUFFER lpDoubleBuffer, int cx, int cy)
{
    HDC hdcWindow = GetDC(hwnd);
    lpDoubleBuffer->BackBuffer = CreateCompatibleDC(hdcWindow);
    lpDoubleBuffer->screen = CreateCompatibleBitmap(hdcWindow, cx, cy);
    SelectObject(lpDoubleBuffer->BackBuffer, lpDoubleBuffer->screen);

    ReleaseDC(hwnd, hdcWindow);
}

void doubleBufferingCleanup(PDOUBLE_BUFFER lpDoubleBuffer)
{
    if(lpDoubleBuffer->screen)
    {
        DeleteObject(lpDoubleBuffer->screen);
        lpDoubleBuffer->screen = NULL;
    }

    if(lpDoubleBuffer->BackBuffer)
    {
        DeleteDC(lpDoubleBuffer->BackBuffer);
        lpDoubleBuffer->BackBuffer = NULL;
    }
}