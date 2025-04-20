#include "render.h"

MenuOptions mainMenu[] = {
    {L"Start"},
    {L"Options"},
    {L"Exit"}
};

MenuStyle style = 
{
    .font_color = champagneSilver,
    .font_name = "emulogic",
    .font_size = 54,
    .glow_color = blue,
    .padding_x = 10,
    .padding_y = 20,
    .spacing = 120,
    .length = sizeof(mainMenu) / sizeof(mainMenu[0]),
    .hFont = NULL
};

void drawSnake(HDC hdc, Snake* snake)
{
    SnakeNode* current = snake->head;
    HBRUSH headBrush = CreateSolidBrush(SNAKE_HEAD_COLOR);
    HBRUSH bodyBrush = CreateSolidBrush(SNAKE_BODY_COLOR);
    for(SnakeNode* i = snake->head; i != NULL; i = i->next)
    {
        if(i == snake->head)
        {
            SelectObject(hdc, headBrush);
            snake->headRect = SETUP_RECT(i->x,i->y, 1);
        }
        else
        {
            SelectObject(hdc , bodyBrush);
        }
        snake->body = SETUP_RECT(i->x, i->y, 1);
        Ellipse(hdc, snake->body.left, snake->body.top, snake->body.right, snake->body.bottom);
    }
    DeleteObject(headBrush);
    DeleteObject(bodyBrush);
}

void drawPellet(HDC hdc, Pellet* pellet)
{
    pellet->rect = SETUP_RECT(pellet->cx, pellet->cy, 1);
    HBRUSH hBrush = CreateSolidBrush(red);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Ellipse(hdc,
        pellet->rect.left - pellet->scale,
        pellet->rect.top - pellet->scale,
        pellet->rect.right + pellet->scale,
        pellet->rect.bottom + pellet->scale
        );

    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void displaySnakeList(SnakeNode* head_ptr, HDC hdc)
{
    char buffer[64];
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    SnakeNode* current = head_ptr;
    printf("Debugging snake list:\n");
    while (current != NULL)
    {
        sprintf(buffer, "Node at (%d, %d)\n Next: %#x\n", current->x, current->y,(void*)current->next);
        current = current->next;
        TextOutA(hdc, 32, 32, buffer, lstrlenA(buffer));
    }
}

void renderToBackBuffer(HWND hwnd, GAMESTATE* state, HDC back_buffer, Pellet* pellet, Snake* snake)
{
    RECT screen = {0, 0, screen_width, screen_height};
    FillRect(back_buffer, &screen, (*state == MENU) ? (HBRUSH)CreateSolidBrush(MENU_BG) : (HBRUSH)GetStockObject(BLACK_BRUSH));
    renderMenu(hwnd, back_buffer, state);
    if(*state != MENU)
    {
        renderGrid(back_buffer);
        drawPellet(back_buffer, pellet);
        drawSnake(back_buffer, snake);
        showScore(back_buffer);
        gameOver(back_buffer, snake);
    }
}

void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy)
{
    BitBlt(front_buffer, 0, 0, cx, cy, back_buffer, 0, 0, SRCCOPY);
}

void renderMenu(HWND hwnd, HDC hdc, GAMESTATE* state)
{
    if(*state == MENU)
    {
        COLORREF brushColor = RGB(194,144,48);
        CreateAndSelectFont(hdc, &style.hFont, -style.font_size, style.font_name, style.font_color);
        drawMenu(hwnd, hdc, L"SNAKE", mainMenu, style.length, brushColor, style, state);
        DeleteFont(&style.hFont);
    }
}

void renderGrid(HDC hdc)
{
    HBRUSH even_squareBrush = CreateSolidBrush(darkOrange);
    HBRUSH odd_squareBrush = CreateSolidBrush(lightOrange);
    HBRUSH wallBrush = CreateSolidBrush(copperRed);
    for(int y = 3 ; y < GRID_HEIGHT ; y++)
    {
        
        for(int x = 0 ; x < GRID_WIDTH ; x++)
        {
            RECT rect = SETUP_RECT(x, y, 1);
            if(x == 0 || x == GRID_WIDTH - 1 || y == 3 || y == GRID_HEIGHT - 1 )
            {
                FillRect(hdc, &rect, wallBrush);
            }
            else
            {
                if(y % 2 == 0){
                    if(x % 2 == 0)
                    {
                        FillRect(hdc, &rect, even_squareBrush);
                    }
                    else
                    {
                        FillRect(hdc, &rect, odd_squareBrush);
                    }
                }
                else
                {
                    if(x % 2 != 0)
                    {
                        FillRect(hdc, &rect, even_squareBrush);
                    }
                    else
                    {
                        FillRect(hdc, &rect, odd_squareBrush);
                    }
                }
            }
        }
    }
    DeleteObject(even_squareBrush);
    DeleteObject(odd_squareBrush);
    DeleteObject(wallBrush);
}

void showScore(HDC hdc)
{
    HBRUSH panelBrush = CreateSolidBrush(RGB(100,77,31));
    for(int y = 0 ; y < 3 ; y++)
    {
        
        for(int x = 0 ; x < GRID_WIDTH ; x++)
        {
            RECT rect = SETUP_RECT(x, y, 1);
            FillRect(hdc, &rect, panelBrush);
        }
    }
    HFONT hFont = NULL;
    char buffer[16];
    CreateAndSelectFont(hdc, &hFont, font_size + 4, "Wobble Board", teal);
    sprintf(buffer, "Score : %d", score);
    TextOutA(hdc, (GRID_WIDTH - 6) * TILE_SIZE, TILE_SIZE, buffer, lstrlen(buffer));
    DeleteFont(&hFont);
}

void gameOver(HDC hdc, Snake *snake)
{
    if(!snake->isMoving)
    {
        int32_t center_x, center_y;
        center_x = GRID_WIDTH / 2;
        center_y = ((GRID_HEIGHT) / 2) + 2;
        HFONT hFont = NULL;
        CreateAndSelectFont(hdc, &hFont, font_size + 6, "Wobble Board", gray);
        RECT rect = SCALE_RECT(center_x, center_y, 8, 4);
        
        if(snake->isCollidedWithWall)
        {
            renderTransparentLayer(hdc, TRUE, rect);
            TextOutA(hdc, (center_x - 4) * TILE_SIZE, (center_y) * TILE_SIZE , "GameOver Buddy", 15);
        }
        DeleteFont(&hFont);
    }
}

void renderSprite(HDC hdc, SPRITE sprite, int32_t cx, int32_t cy, float scale)
{
    sprite.memDC = CreateCompatibleDC(hdc);
    SelectObject(sprite.memDC, sprite.sheet);

    int32_t frame_x = sprite.currentFrame * sprite.width;
    int32_t frame_y = sprite.currentRow * sprite.height;

    RECT src = {frame_x , frame_y, frame_x + sprite.width, frame_y + sprite.height};
    RECT dst = {cx, cy, cx + scale, cy + scale};

    int32_t wSrc = src.right - src.left;
    int32_t hSrc =  src.bottom - src.top;

    int32_t wDest = dst.right - dst.left;
    int32_t hDest =  dst.bottom - dst.top;

    TransparentBlt(hdc, dst.left, dst.top,wDest, hDest, sprite.memDC, src.left, src.top, wSrc, hSrc, black);
    DeleteDC(sprite.memDC);
}

void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT rect)
{
    int32_t width = (rect.right - rect.left);
    int32_t height = (rect.bottom - rect.top);
    uint8_t cx = rect.left;
    uint8_t cy = rect.top;
    HRGN roundRct;

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBRUSH overlayBrush = CreateSolidBrush(black);
    HGDIOBJ oldBrush =(HBRUSH)SelectObject(memDC, overlayBrush);
    HGDIOBJ oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
    
    FillRect(memDC, &rect, overlayBrush);
    SelectObject(memDC, oldBrush);
    DeleteObject(overlayBrush);
    if(is_rounded)
    {
        roundRct = CreateRoundRectRgn(rect.left, rect.top, rect.right - 2, rect.bottom - 2, 30, 30);
        SelectClipRgn(hdc, roundRct);
        DeleteObject(roundRct);
    }

    
    BLENDFUNCTION blendFunc = {AC_SRC_OVER, 0, (BYTE)210, 0};
    AlphaBlend(hdc, cx, cy, width, height, memDC, 0 , 0, width, height, blendFunc);
    
    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
    DeleteObject(hBitmap);
    if(is_rounded)
        SelectClipRgn(hdc, NULL);
}

void renderArrowOnStart(HDC hdc, SPRITE sprite, uint8_t cx, uint8_t cy)
{
    RECT rect = SCALE_RECT(cx, cy, 4, 3);
    renderTransparentLayer(hdc, TRUE, rect);
    renderSprite(hdc, sprite, cx, cy, 2.0f);
}
