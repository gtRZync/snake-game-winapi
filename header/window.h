#pragma once

#ifndef LIMITED_WINDOW
#define WINDOW_WIDTH ((uint32_t)0x00000346)
#define WINDOW_HEIGHT 635
//! may add scaling in the future(dpi wise)
// #define WINDOW_WIDTH ((uint32_t)0x00000346)
// #define WINDOW_HEIGHT ((uint32_t)0x00003FD)

#define CW_CENTERED_X ((GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2)
#define CW_CENTERED_Y ((GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2)

#include "double_buffer.h"
#include "utilis.h"
#endif
typedef struct Window
{
    HWND handle;
    HDC hdc;
#ifndef LIMITED_WINDOW
    WNDCLASS class;
    MSG msg;
    WNDPROC gameProc;
    HCURSOR cursor;
    const char* CLASS_NAME;
#endif
    int32_t width;
    int32_t height;
}Window;

#ifndef LIMITED_WINDOW
struct Game;
#define GameEntryPoint()\
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateGameWindow(struct Game* game, HINSTANCE hInstance, int nShowCmd);
void setInputStateAfter();
void setInputContext(Input *ctx);
#endif