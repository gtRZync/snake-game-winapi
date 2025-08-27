#include "menu.h"

bool hasClicked = false, startClicked = false, restartClicked = false, hasPressed = false;

static void addGlowing(HDC hdc, const MenuStyle* style, COLORREF font_color, LPCWSTR text, int32_t y, int32_t x_offset, int32_t y_offset)
{
    HFONT font;
    int32_t x;
    //! In Windows API, a negative font height sets the character height in logical units (desired size), 
    //! while a positive value sets the total cell height including internal leading (space the font occupies).
    HFONT oldFont = CreateAndSelectFont(hdc, &font, -style->font_size, style->font_name, font_color);
    int32_t offset[][2] = {
        {-x_offset, -y_offset},{0,-y_offset},{x_offset,-y_offset},
        {-x_offset, 0},        {x_offset, 0},
        {-x_offset, y_offset}, {0, y_offset},{x_offset, y_offset}
    };
    size_t size = sizeof(offset) / sizeof(offset[0]);
    for(uint8_t i = 0 ; i < size; i++)
    {
        FetchTextCenteredMetrics(hdc, &x, NULL, text);
        TextOutW(hdc, x + offset[i][0], y + offset[i][1], text, lstrlenW(text));
    }
    SelectObject(hdc, oldFont);
    DeleteFont(&font);
}

static void DrawCenteredTextForMenu(HDC hdc, LPCWSTR lpString, int starting_y)
{
    int32_t ctrwString;
    FetchTextCenteredMetrics(hdc, &ctrwString, NULL, lpString);
    TextOutW(hdc,
        ctrwString,
        starting_y, 
        lpString, 
        lstrlenW(lpString)
    );
}

static void handleClick(HWND hwnd, const RECT *rect, uint8_t id, const POINT* mouse)
{
    if(isPointInRect(rect, mouse->x, mouse->y))
    {
        switch(id)
        {
            case 0:
                startClicked = true;
                break;

            case 1:
                MessageBoxW(NULL, L"Options Menu", L"Options", MB_OK | MB_ICONINFORMATION);
                break;

            case 2:
                PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
        }
    }
}



void drawMenu(HWND hwnd, HDC hdc, uint32_t topPart, MenuOptions* options, uint8_t n_options, COLORREF rectBrushColor, const MenuStyle* style)
{
    int maxTextWidth = 0;
    int totalHeight = n_options * style->spacing;
    int startY = (screen_height - (totalHeight - topPart)) / 2;

    HFONT font;
    HBRUSH brush = CreateSolidBrush(rectBrushColor); 
    HGDIOBJ oldBrush = (HBRUSH)SelectObject(hdc, brush);

    for (int i = 0; i < n_options; i++) {
        int w = FetchTextXW(hdc, options[i].text);
        if (w > maxTextWidth) maxTextWidth = w;
    }
    int startX = (screen_width - maxTextWidth) / 2;

    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(hwnd, &mouse);

    RECT rect[n_options];

    for (int i = 0; i < n_options; i++) {
        int y = startY + (i * style->spacing);
        int hText = FetchTextYW(hdc, options[i].text);

        rect[i] = (RECT){
            startX - style->padding_x,
            y - style->padding_y,
            startX + maxTextWidth + style->padding_x,
            y + hText + style->padding_y
        };
        // Hover effect
        BOOL isHovered = PtInRect(&rect[i], mouse);
        COLORREF borderColor = isHovered ? RGB(0, 255, 0) : RGB(17 ,61 ,62); 

        HPEN pen = CreatePen(PS_SOLID, 4, borderColor);
        HGDIOBJ oldPen = (HPEN)SelectObject(hdc, pen);

        COLORREF offsetColor = isHovered ? style->glow_color : RGB(0, 0, 0);

        // Draw box and text
        RoundRect(hdc, rect[i].left, rect[i].top, rect[i].right, rect[i].bottom, 60, 60);
        addGlowing(hdc, style, offsetColor, options[i].text, y, 3, 3);
        HFONT oldFont = CreateAndSelectFont(hdc, &font, -style->font_size, style->font_name, style->font_color);
        DrawCenteredTextForMenu(hdc, options[i].text, y);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
        SelectObject(hdc, oldFont);
        DeleteFont(&font);
    }
    
    if (hasClicked) {
        for (int i = 0; i < n_options; i++) {
            handleClick(hwnd, &rect[i], i, &mouse);
        }
        hasClicked = false;
    }
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}