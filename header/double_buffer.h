#ifndef DOUBLE_BUFFER_H
#define DOUBLE_BUFFER_H
#pragma once
#include <windows.h>

typedef interface tagDoubleBuffer
{
    HBITMAP screen;
    HDC BackBuffer;
}DOUBLE_BUFFER, *PDOUBLE_BUFFER; 

void setupDoubleBuffering(HWND hwnd, PDOUBLE_BUFFER lpDoubleBuffer, int cx, int cy);
void doubleBufferingCleanup(PDOUBLE_BUFFER lpDoubleBuffer);

#endif