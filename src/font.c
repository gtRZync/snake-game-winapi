#include "font.h"
int font_size = 16; //! default font size i chosed

HFONT CreateAndSelectFont(HDC hdc, HFONT *hFont, UINT font_size, LPCSTR font_name, COLORREF font_color)
{
    *hFont = CreateFontA(
        font_size,
        0,
        0,
        0,
        FW_NORMAL,
        false,
        false,
        false,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FF_SWISS | FF_DONTCARE,
        font_name
    );

    if(*hFont)
    {
        HFONT oldFont =(HFONT)SelectObject(hdc, *hFont);
        SetTextColor(hdc, font_color);
        SetBkMode(hdc, TRANSPARENT);
        return oldFont;
    }
    else
    {
        MessageBoxW(NULL, L"Font Creation Failed!!", L"Font Error",MB_OK | MB_ICONWARNING);
        return NULL;
    }
}

WINBOOL DeleteFont(HFONT *hFont)
{
    if(*hFont && hFont)
    {
        DeleteObject(*hFont);
        *hFont = NULL;
        return true;
    }
    return false;
}

int FetchTextY(HDC hdc, LPCTSTR lpString)
{
    SIZE buffer;
    if(GetTextExtentPoint32(hdc, lpString,lstrlen(lpString), &buffer))
    {
        return buffer.cy;
    }
    //* should never reach here
    return -1;
}

int FetchTextYW(HDC hdc, LPCWSTR lpString)
{
    SIZE buffer;
    if(GetTextExtentPoint32W(hdc, lpString,lstrlenW(lpString), &buffer))
    {
        return buffer.cy;
    }
    //* should never reach here
    return -1;
}

int FetchTextX(HDC hdc, LPCTSTR lpString)
{
    SIZE buffer;
    if(GetTextExtentPoint32(hdc, lpString,lstrlen(lpString), &buffer))
    {
        return buffer.cx;
    }
    //* should never reach here
    return -1;
}
int FetchTextXW(HDC hdc, LPCWSTR lpString)
{
    SIZE buffer;
    if(GetTextExtentPoint32W(hdc, lpString,lstrlenW(lpString), &buffer))
    {
        return buffer.cx;
    }
    //* should never reach here
    return -1;
}
