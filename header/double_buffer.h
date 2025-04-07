#ifndef DOUBLE_BUFFER_H
#define DOUBLE_BUFFER_H
#pragma once
#include <windows.h>

typedef struct tagDoubleBuffer
{
    HBITMAP screen;
    HDC BackBuffer;
}DOUBLE_BUFFER, *PDOUBLE_BUFFER; 

void setup_double_buffer(HWND hwnd, PDOUBLE_BUFFER lpDoubleBuffer, int cx, int cy);
void double_buffer_cleanup(PDOUBLE_BUFFER lpDoubleBuffer);

#endif