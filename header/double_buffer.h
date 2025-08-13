#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct tagDoubleBuffer
{
    HBITMAP screen;
    HDC backBuffer;
}DOUBLE_BUFFER; 

void setupDoubleBuffering(HWND hwnd, DOUBLE_BUFFER* lpDoubleBuffer, int cx, int cy);
void resizeDoubleBuffer(HWND hwnd, DOUBLE_BUFFER *lpDoubleBuffer, int cx, int cy);
void doubleBufferingCleanup(DOUBLE_BUFFER* lpDoubleBuffer);
