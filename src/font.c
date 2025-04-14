#include "font.h"
int font_size = 16;

WINBOOL CreateAndSelectFont(HDC hdc, HFONT *hFont, UINT font_size, LPCSTR font_name, COLORREF font_color)
{
    *hFont = CreateFontA(
        font_size,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FF_SWISS | FF_DONTCARE,
        font_name
    );

    if(*hFont)
    {
        SelectObject(hdc, *hFont);
        SetTextColor(hdc, font_color);
        SetBkMode(hdc, TRANSPARENT);
        return TRUE;
    }
    else
    {
        MessageBoxW(NULL, L"Font Creation Failed!!", L"Font Error",MB_OK | MB_ICONWARNING);
        return FALSE;
    }
}

WINBOOL DeleteFont(HFONT *hFont)
{
    if(*hFont && hFont)
    {
        DeleteObject(*hFont);
        *hFont = NULL;
        return TRUE;
    }
    return FALSE;
}
