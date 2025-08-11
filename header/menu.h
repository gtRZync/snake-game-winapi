#pragma once

#include "utilis.h"
#include "font.h"

typedef interface tagStyle
{
    UINT font_size;
    HFONT hFont;
    LPCSTR font_name;
    COLORREF font_color;
    COLORREF glow_color;
    int32_t padding_x;
    int32_t padding_y;
    int32_t spacing;
    uint8_t length;
}MenuStyle;

typedef interface tagMenu
{
    const wchar_t* text;
}MenuOptions;

void drawMenu(HWND hwnd, HDC hdc, uint32_t topPart, MenuOptions* options, uint8_t n_options, COLORREF rectBrushColor, const MenuStyle* styles);
void DrawCenteredTextForMenu(HDC hdc, LPCWSTR lpString, int starting_y);
void handleClick(HWND hwnd, const RECT* rect, uint8_t id, const POINT* mouse);
void addGlowing(HDC hdc, const MenuStyle* style, COLORREF font_color, LPCWSTR text, int32_t y, int32_t x_offset, int32_t y_offset);


