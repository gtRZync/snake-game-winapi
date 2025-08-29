#pragma once

#include "utilis.h"
#include "font.h"

struct Game;

typedef struct tagStyle
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

typedef struct tagMenu
{
    const wchar_t* text;
}MenuOptions;

void drawMenu(struct Game* game, Vector2 size, uint32_t topPart, MenuOptions *options, uint8_t n_options, const MenuStyle *style);


