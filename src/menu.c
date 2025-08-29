#include "menu.h"
#define LIMITED_GAME
#define LIMITED_WINDOW
#include "game.h"

static int32_t FetchTextCenteredWidth(HDC hdc, int w, LPCWSTR lpString)
{
    SIZE buffer;
    if(GetTextExtentPoint32W(hdc, lpString,lstrlenW(lpString), &buffer))
    {
        return (w - buffer.cx) / 2;
    }
    return - 1;
}

static void addGlowing(HDC hdc, const MenuStyle* style, COLORREF font_color, LPCWSTR text, int32_t y, int32_t x_offset, int32_t y_offset, int win_width)
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
        x = FetchTextCenteredWidth(hdc, win_width, text);
        TextOutW(hdc, x + offset[i][0], y + offset[i][1], text, lstrlenW(text));
    }
    SelectObject(hdc, oldFont);
    DeleteFont(&font);
}

static void DrawCenteredTextForMenu(HDC hdc, LPCWSTR lpString, int starting_y, int win_width)
{
    int32_t ctrwString = FetchTextCenteredWidth(hdc, win_width, lpString);
    TextOutW(hdc,
        ctrwString,
        starting_y, 
        lpString, 
        lstrlenW(lpString)
    );
}

static void handleClick(Game* game, const RECT *rect, uint8_t id, const POINT* mouse)
{
    static const int START_ID = 0, OPTION_ID = 1, EXIT_ID = 2;
    if(isPointInRect(rect, mouse->x, mouse->y))
    {
        switch(id)
        {
            case START_ID:
                game->starting = true;
                break;

            case OPTION_ID:
                MessageBoxW(NULL, L"Options Menu", L"Options", MB_OK | MB_ICONINFORMATION);
                break;

            case EXIT_ID:
                PostMessage(game->window->handle, WM_CLOSE, 0, 0);
                break;
        }
    }
}


void drawMenu(Game* game, Vector2 size, uint32_t topPart, MenuOptions *options, uint8_t n_options, const MenuStyle *style)
{
    HDC memDC = game->buffer->backBuffer;
    int maxTextWidth = 0;
    int totalHeight = n_options * style->spacing;
    int startY = (size.y - (totalHeight - topPart)) / 2;
    COLORREF rectBrushColor = golden_brown;

    HFONT font;
    HBRUSH brush = CreateSolidBrush(rectBrushColor); 
    HGDIOBJ oldBrush = (HBRUSH)SelectObject(memDC, brush);

    for (int i = 0; i < n_options; i++) {
        int w = FetchTextXW(memDC, options[i].text);
        if (w > maxTextWidth) maxTextWidth = w;
    }
    int startX = (size.x - maxTextWidth) / 2;

    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(game->window->handle, &mouse);

    RECT rect[n_options];

    for (int i = 0; i < n_options; i++) {
        int y = startY + (i * style->spacing);
        int hText = FetchTextYW(memDC, options[i].text);

        rect[i] = (RECT){
            startX - style->padding_x,
            y - style->padding_y,
            startX + maxTextWidth + style->padding_x,
            y + hText + style->padding_y
        };
        // Hover effect
        BOOL isHovered = isPointInRect(&rect[i], mouse.x, mouse.y);
        COLORREF borderColor = isHovered ? RGB(0, 255, 0) : RGB(17 ,61 ,62); 

        HPEN pen = CreatePen(PS_SOLID, 4, borderColor);
        HGDIOBJ oldPen = (HPEN)SelectObject(memDC, pen);

        COLORREF offsetColor = isHovered ? style->glow_color : RGB(0, 0, 0);

        // Draw box and text
        uint8_t offset = 3;
        uint8_t corner = 60;
        RoundRect(memDC, rect[i].left, rect[i].top, rect[i].right, rect[i].bottom, corner, corner);
        addGlowing(memDC, style, offsetColor, options[i].text, y, offset, offset, size.x);
        HFONT oldFont = CreateAndSelectFont(memDC, &font, -style->font_size, style->font_name, style->font_color);
        DrawCenteredTextForMenu(memDC, options[i].text, y, size.x);
        SelectObject(memDC, oldPen);
        DeleteObject(pen);
        SelectObject(memDC, oldFont);
        DeleteFont(&font);
    }
    
    if (game->input.mouse[MOUSE_LEFT].released) {
        for (int i = 0; i < n_options; i++) {
            handleClick(game, &rect[i], i, &mouse);
        }
    }
    SelectObject(memDC, oldBrush);
    DeleteObject(brush);
}