#ifndef SPRITE_H
#define SPRITE_H
#pragma once
#include <windows.h>

extern int font_size;

WINBOOL CreateAndSelectFont(HDC hdc, HFONT* hFont, UINT font_size, LPCSTR font_name, COLORREF font_color);
WINBOOL DeleteFont(HFONT* hFont);
#endif