#include "double_buffer.h"

void setupDoubleBuffering(HWND hwnd, DOUBLE_BUFFER* lpDoubleBuffer, int cx, int cy)
{
    HDC hdcWindow = GetDC(hwnd);
    lpDoubleBuffer->backBuffer = CreateCompatibleDC(hdcWindow);
    if (!lpDoubleBuffer->backBuffer)
    {
        MessageBoxW(hwnd, L"Failed to create the back buffer device context.", L"Buffer Creation Error", MB_OK | MB_ICONERROR);
        return;
    }
        
    lpDoubleBuffer->screen = CreateCompatibleBitmap(hdcWindow, cx, cy);
    if (!lpDoubleBuffer->screen)
    {
        MessageBoxW(hwnd, L"Failed to create the off-screen bitmap.", L"Bitmap Creation Error", MB_OK | MB_ICONERROR);  
        DeleteDC(lpDoubleBuffer->backBuffer);
        return;
    }

    SelectObject(lpDoubleBuffer->backBuffer, lpDoubleBuffer->screen);

    ReleaseDC(hwnd, hdcWindow);
}


void resizeDoubleBuffer(HWND hwnd, DOUBLE_BUFFER *lpDoubleBuffer, int cx, int cy)
{
    doubleBufferingCleanup(lpDoubleBuffer);
    setupDoubleBuffering(hwnd, lpDoubleBuffer, cx, cy);
}

void doubleBufferingCleanup(DOUBLE_BUFFER* lpDoubleBuffer)
{
    if(lpDoubleBuffer->screen)
    {
        DeleteObject(lpDoubleBuffer->screen);
        lpDoubleBuffer->screen = NULL;
    }

    if(lpDoubleBuffer->backBuffer)
    {
        DeleteDC(lpDoubleBuffer->backBuffer);
        lpDoubleBuffer->backBuffer = NULL;
    }
}