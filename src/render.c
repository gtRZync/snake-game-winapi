#include "render.h"

int32_t timer_intervalUID = 130;
int32_t screen_height, screen_width;
MSG msg = { };
int32_t score = 0;
SPRITE title;
SPRITE keys;
SPRITE sound;
RECT audioRect;

static MenuOptions mainMenu[] = {
    {L"Start"},
    {L"Options"},
    {L"Exit"}
};

static MenuStyle style = 
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
    HBRUSH headBrush = CreateSolidBrush(SNAKE_HEAD_COLOR);
    HBRUSH bodyBrush = CreateSolidBrush(SNAKE_BODY_COLOR);
    HBRUSH oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    COLORREF borderColor =  RGB(17 ,61 ,62); 
    HPEN pen = CreatePen(PS_SOLID, 2, borderColor);
    HGDIOBJ oldPen = (HPEN) SelectObject(hdc, pen);

    HGDIOBJ oldHeadBrush = SelectObject(hdc, headBrush);
    snake->headRect = SETUP_RECT(snake->head->x,snake->head->y, 1);
    Ellipse(hdc, snake->headRect.left, snake->headRect.top, snake->headRect.right, snake->headRect.bottom);
    
    SelectObject(hdc , bodyBrush);
    for(SnakeNode* i = snake->head->next ; i != NULL; i = i->next)
    {
        snake->body = SETUP_RECT(i->x, i->y, 1);
        Ellipse(hdc, snake->body.left, snake->body.top, snake->body.right, snake->body.bottom);
    }

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);

    DeleteObject(headBrush);
    DeleteObject(bodyBrush);
    DeleteObject(pen);
}

void drawPellet(HDC hdc, Pellet* pellet)
{
    // Compute the center of the tile
    int32_t centerX = pellet->cx * TILE_SIZE + TILE_SIZE / 2;
    int32_t centerY = pellet->cy * TILE_SIZE + TILE_SIZE / 2;

    pellet->rect = SETUP_RECT(pellet->cx, pellet->cy, 1);

    // Compute scaled sprite size
    int scaledW = pellet->sprite.width * pellet->scale;
    int scaledH = pellet->sprite.height * pellet->scale;

    // Position so the sprite's center stays at the tile center
    int drawX = centerX - scaledW / 2;
    int drawY = centerY - scaledH / 2;

    renderSprite(hdc, &pellet->sprite, drawX, drawY, pellet->scale, turquoise);
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

void renderToBackBuffer(HWND hwnd, GAMESTATE* gameState, HDC back_buffer, Pellet* pellet, Snake* snake)
{
    RECT screen = {0, 0, screen_width, screen_height};
    HBRUSH hBrush =  CreateSolidBrush(MENU_BG);
    FillRect(back_buffer, &screen, hBrush);
    DeleteObject(hBrush);
    renderMenu(hwnd, back_buffer, gameState);
    if(*gameState != MENU)
    {
        renderGrid(back_buffer);
        drawPellet(back_buffer, pellet);
        drawSnake(back_buffer, snake);
        showScore(back_buffer, &pellet->sprite);
        gameOver(back_buffer, snake, gameState);
        renderControlKeysOverlay(back_buffer, &keys, gameState);
    }
}

void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy)
{
    BitBlt(front_buffer, 0, 0, cx, cy, back_buffer, 0, 0, SRCCOPY);
}

void renderMenu(HWND hwnd, HDC hdc, const GAMESTATE* gameState)
{
    if(*gameState == MENU)
    {
        uint32_t titleY;
        int32_t topPart = renderTitle(hdc, &title, &titleY, 1.2f);
        // renderSoundIcon(hdc, &sound, titleY, 8.f, &audioRect);
        COLORREF brushColor = golden_brown;
        HFONT oldFont = CreateAndSelectFont(hdc, &style.hFont, -style.font_size, style.font_name, style.font_color);
        drawMenu(hwnd, hdc, topPart, mainMenu, style.length, brushColor, &style);
        SelectObject(hdc, oldFont);
        DeleteFont(&style.hFont);
    }
}

void renderGrid(HDC hdc)
{
    HBRUSH even_squareBrush = CreateSolidBrush(darkOrange);
    HBRUSH odd_squareBrush = CreateSolidBrush(lightOrange);
    HBRUSH wallBrush = CreateSolidBrush(copperRed);
    HBRUSH panelBrush = CreateSolidBrush(RGB(100,77,31)); //rgb(100,77,31)
    for(int y = 0 ; y < GRID_HEIGHT ; y++)
    {
        
        for(int x = 0 ; x < GRID_WIDTH ; x++)
        {
            RECT rect = SETUP_RECT(x, y, 1);
            if( y < 3)
            {
                FillRect(hdc, &rect, panelBrush);
            }
            else
            {
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
    }
    DeleteObject(even_squareBrush);
    DeleteObject(odd_squareBrush);
    DeleteObject(wallBrush);
    DeleteObject(panelBrush);
}

void showScore(HDC hdc, SPRITE* sprite)
{
    HFONT hFont = NULL;
    char buffer[16];
    HFONT oldFont = CreateAndSelectFont(hdc, &hFont, font_size + 8, "Wobble Board", teal);
    sprintf(buffer, "Score : %d", score);
    TextOutA(hdc, (GRID_WIDTH - 7) * TILE_SIZE, TILE_SIZE, buffer, lstrlen(buffer));
    SelectObject(hdc, oldFont);
    DeleteFont(&hFont);
}

void gameOver(HDC hdc, Snake *snake, GAMESTATE* gameState)
{
    if(!snake->isMoving)
    {
        int32_t center_x, center_y;
        center_x = GRID_WIDTH / 2;
        center_y = ((GRID_HEIGHT) / 2) + 2;
        HFONT hFont = NULL;
        CreateAndSelectFont(hdc, &hFont, font_size + 6, "Wobble Board", gray);
        RECT rect = SCALE_RECT(center_x, center_y, 8, 4);
        
        if(*gameState == GAMEOVER)
        {
            renderTransparentLayer(hdc, TRUE, rect);
            TextOutA(hdc, (center_x - 4) * TILE_SIZE, (center_y) * TILE_SIZE , "GameOver Buddy", 15);
        }
        DeleteFont(&hFont);
    }
}

void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale, UINT transparentColorKey)
{
    sprite->memDC = CreateCompatibleDC(hdc);
    HGDIOBJ oldBitmap =  SelectObject(sprite->memDC, sprite->sheet);

    uint32_t frame_x = sprite->currentFrame * sprite->width;
    uint32_t frame_y = sprite->currentRow * sprite->height;

    RECT src = {frame_x , frame_y, frame_x + sprite->width, frame_y + sprite->height};
    RECT dst = {cx, cy, cx + sprite->width * scale, cy + sprite->height * scale};

    uint32_t wSrc = src.right - src.left;
    uint32_t hSrc =  src.bottom - src.top;

    uint32_t wDest = dst.right - dst.left;
    uint32_t hDest =  dst.bottom - dst.top;

    TransparentBlt(
        hdc, 
        dst.left, 
        dst.top,
        wDest, 
        hDest, 
        sprite->memDC, 
        src.left, 
        src.top,
        wSrc, 
        hSrc, 
        transparentColorKey
    );
    DeleteDC(sprite->memDC);
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

void renderControlKeysOverlay(HDC hdc, SPRITE* sprite, GAMESTATE* gameState)
{
    if(*gameState == WAIT_MOVE_INPUT && sprite)
    {    
        int32_t center_x, center_y;
        center_x = GRID_WIDTH / 2;
        center_y = ((GRID_HEIGHT) / 2) + 2;

        RECT rect = SCALE_RECT(center_x, center_y, 6, 4);

        int32_t cx = (screen_width - (sprite->width)) / 2;
        int32_t cy = (screen_height - sprite->height) / 2;

        renderTransparentLayer(hdc, TRUE, rect);
        renderSprite(hdc, sprite, cx, cy + 50, 1.f, black);
    }
}

int32_t renderTitle(HDC hdc, SPRITE *sprite, uint32_t* Ycoord, float scaleFactor)
{
    if(sprite)
    {
        uint32_t titleX = (screen_width - (sprite->width * scaleFactor)) / 2;

        uint32_t titleY = (screen_height - (sprite->height * scaleFactor)) / 20;
        *Ycoord = titleY;

        renderSprite(hdc, sprite, titleX , titleY, scaleFactor, black);

        return titleY + (sprite->height * scaleFactor);
    }
    return -1;
}

void renderSoundIcon(HDC hdc, SPRITE *sprite, uint32_t iconY, float scaleFactor, RECT* audio)
{
    if(sprite)
    {
        uint32_t iconX = (screen_width - (sprite->width * scaleFactor));
        *audio = (RECT){iconX, iconY, iconX + sprite->width * scaleFactor, iconY + sprite->height * scaleFactor};
        renderSprite(hdc, sprite, iconX , iconY, scaleFactor, red);
    }
}