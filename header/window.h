#ifndef WINDOW_H
#define WINDOW_H
#pragma once

#define WINDOW_WIDTH ((uint32_t)0x00000346)
#define WINDOW_HEIGHT ((uint32_t)0x00003FD)
#define CW_CENTERED_X ((GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2)
#define CW_CENTERED_Y ((GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2) - 20

#include "snake.h"
#include "pellet.h"
#include "render.h"
#include "game.h"
#include "double_buffer.h"
#include "config.h"

extern MSG msg;

#define GameEntryPoint() INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateGameWindow(HINSTANCE hInstance, WNDPROC wndProc, int nShowCmd);
int HandleGameMessages();


#endif