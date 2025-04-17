#ifndef WINDOW_H
#define WINDOW_H
#pragma once


#define WINDOW_WIDTH ((uint32_t)0x00000346)
#define WINDOW_HEIGHT 635
//! may add scaling in the future(dpi wise)
// #define WINDOW_WIDTH ((uint32_t)0x00000346)
// #define WINDOW_HEIGHT ((uint32_t)0x00003FD)

#define CW_CENTERED_X ((GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2)
#define CW_CENTERED_Y ((GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2)

#include "double_buffer.h"
#include "config.h"

interface Game;

typedef interface Window
{
    HWND hwnd;
    WNDCLASS wc;
    MSG msg;
    WNDPROC gameProc;
    const char* CLASS_NAME;
}Window;


extern MSG msg;

#define GameEntryPoint()\
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateGameWindow(interface Game* game, HINSTANCE hInstance, int nShowCmd);
int HandleGameMessages(interface Game* game);

#endif