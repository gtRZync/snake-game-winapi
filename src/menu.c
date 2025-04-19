#include "menu.h"

extern int clickedX, clickedY;
extern BOOLEAN has_clicked;

void addGlowing(HDC hdc, UINT font_size, LPCSTR font_name, COLORREF font_color, LPCWSTR text, int y, int x_offset, int y_offset)
{
    HFONT font;
    int x;
    CreateAndSelectFont(hdc, &font, font_size, font_name, font_color);
    int offset[][2] = {
        {-x_offset, -y_offset},{0,-y_offset},{x_offset,-y_offset},
        {-x_offset, 0},        {x_offset, 0},
        {-x_offset, y_offset}, {0, y_offset},{x_offset, y_offset}
    };
    size_t size = sizeof(offset) / sizeof(offset[0]);
    for(int i = 0 ; i < size; i++)
    {
        FetchTextCenteredMetrics(hdc, &x, NULL, text);
        TextOutW(hdc, x + offset[i][0], y + offset[i][1], text, lstrlenW(text));
    }
    DeleteFont(&font);
}


void drawMenu(HWND hwnd, HDC hdc, const wchar_t* Title, MenuOptions* options, uint8_t n_options, COLORREF rectBrushColor, MenuStyle* style)
{
    int maxTextWidth = 0;

    // Title positioning
    int hTitle = FetchTextYW(hdc, Title);
    int titleY = (screen_height - hTitle) / 20;
    int topPart = titleY + hTitle;
    int totalHeight = n_options * style->spacing;
    int startY = (screen_height - (totalHeight - topPart)) / 2;

    // Font and brush setup
    HFONT font;
    HBRUSH brush = CreateSolidBrush(rectBrushColor);
    SelectObject(hdc, brush);

    // Draw title
    DrawCenteredTextForMenu(hdc, Title, titleY);

    // Determine max width for centering
    for (int i = 0; i < n_options; i++) {
        int w = FetchTextXW(hdc, options[i].text);
        if (w > maxTextWidth) maxTextWidth = w;
    }
    int startX = (screen_width - maxTextWidth) / 2;

    // Mouse position for hover
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(hwnd, &mouse);

    // Store rects
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
        SelectObject(hdc, pen);

        COLORREF offsetColor = isHovered ? style->glow_color : RGB(0, 0, 0);

        // Draw box and text
        RoundRect(hdc, rect[i].left, rect[i].top, rect[i].right, rect[i].bottom, 60, 60);
        addGlowing(hdc, -style->font_size, style->font_name, offsetColor, options[i].text, y, 3, 3);
        CreateAndSelectFont(hdc, &font, -style->font_size, style->font_name, style->font_color);
        DrawCenteredTextForMenu(hdc, options[i].text, y);
        DeleteObject(pen);
    }
    
    // Handle click once per frame
    if (has_clicked) {
        for (int i = 0; i < n_options; i++) {
            detectClick(hwnd, rect[i], options[i].text, clickedX, clickedY);
        }
        has_clicked = FALSE;
    }
    
    // Sound icon
    TextOutW(hdc, screen_width - 50, 0, L"🔈", 3);
    DeleteObject(brush);
    DeleteFont(&font);
}

void detectClick(HWND hwnd, RECT rect, LPCWSTR text, int clickedX, int clickedY)
{
    static BOOLEAN startShown = FALSE;
    static BOOLEAN optionsShown = FALSE;

    if(clickedX > rect.left && clickedX < rect.right && clickedY > rect.top && clickedY < rect.bottom)
    {
        if(wcscmp(L"START", text) == 0 && !startShown)
        {
            MessageBoxW(NULL, L"Starting Game....", L"Start", MB_OK | MB_ICONINFORMATION);
            startShown = TRUE;
        }
        else if(wcscmp(L"OPTIONS", text) == 0 && !optionsShown)
        {
            MessageBoxW(NULL, L"Options Menu", L"Options", MB_OK | MB_ICONINFORMATION);
            optionsShown = TRUE;
        }
        else if(wcscmp(L"EXIT", text) == 0)
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
    }
}

void DrawCenteredTextForMenu(HDC hdc, LPCWSTR lpString, int starting_y)
{
    int hString, ctrwString;
    wchar_t buffer[32];
    wsprintfW(buffer, L"%s", lpString);
    FetchTextCenteredMetrics(hdc, &ctrwString, &hString, lpString);
    TextOutW(hdc,
        ctrwString,
        starting_y, 
        buffer, 
        lstrlenW(buffer)
    );
}