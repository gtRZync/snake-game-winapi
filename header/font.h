#ifndef SPRITE_H
#define SPRITE_H
#pragma once
#include <windows.h>

extern HFONT hFont;
extern int font_size;

WINBOOL CreateAndUseFont(HDC hdc, HFONT* hFont, UINT font_size, LPCSTR font_name, COLORREF font_color);
WINBOOL DeleteFont(HFONT* hFont);
#endif