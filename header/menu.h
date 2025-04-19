#ifndef MENU_H
#define MENU_H
#pragma once

#include "config.h"
#include "font.h"

typedef interface tagStyle
{
    UINT font_size;
    LPCSTR font_name;
    COLORREF font_color;
    COLORREF glow_color;
    const int32_t padding_x;
    const int32_t padding_y;
    const int32_t spacing;
}MenuStyle;

typedef interface tagMenu
{
    const wchar_t* text;
}MenuOptions;

void drawMenu(HWND hwnd, HDC hdc, const wchar_t* Title, MenuOptions* options, uint8_t n_options, COLORREF rectBrushColor, MenuStyle* styles);
void DrawCenteredTextForMenu(HDC hdc, LPCWSTR lpString, int starting_y);
void detectClick(HWND hwnd, RECT rect, LPCWSTR text, int clickedX, int clickedY);
void addGlowing(HDC hdc, UINT font_size, LPCSTR font_name, COLORREF font_color, LPCWSTR text, int y, int x_offset, int y_offset);




#endif 