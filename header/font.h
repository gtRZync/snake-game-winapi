#ifndef FONT_H
#define FONT_H
#pragma once
#include "utilis.h"

extern int font_size;

HFONT CreateAndSelectFont(HDC hdc, HFONT* hFont, UINT font_size, LPCSTR font_name, COLORREF font_color);
WINBOOL DeleteFont(HFONT* hFont);
int FetchTextYW(HDC hdc, LPCWSTR lpString);
int FetchTextY(HDC hdc, LPCTSTR lpString);
int FetchTextX(HDC hdc, LPCTSTR lpString);
int FetchTextXW(HDC hdc, LPCWSTR lpString);
void FetchTextCenteredMetrics(HDC hdc, int* cx, int* cy, LPCWSTR lpString);
void FetchTextCenteredMetricsA(HDC hdc, int* cx, int* cy, LPCTSTR lpString);

#endif