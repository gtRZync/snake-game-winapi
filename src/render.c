#include "render.h"

int32_t screen_height, screen_width;
int32_t score = 0;
int32_t high_score = 0;
SPRITE title;
SPRITE keys;
SPRITE sound;
SPRITE trophee;
SPRITE restart_sprite;
RECT audioRect;
RECT restartRect;

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

Frame frames[NUM_INDEXES] = {
    {.row=0, .col=0},//RESTART
    {.row=0, .col=1}//SOUND
};

void drawSnake(HDC hdc, Snake* snake)
{
    snake->headRect = SETUP_RECT(snake->head->x,snake->head->y, 1);
    if(snake->isMoving)
        snake->head->frame =(Frame) {.row=directionToIndex(snake->head->direction),.col=HEAD_ALIVE};
    else
        snake->head->frame =(Frame) {.row=directionToIndex(snake->head->direction),.col=HEAD_DEAD};

    renderSprite(
        hdc, &snake->headSprite, 
        snake->headRect.left, 
        snake->headRect.top, 
        snake->scale, snake->head->frame, turquoise
    );

    DIRECTIONS prevDir = snake->head->direction; 
    SnakeNode* current = snake->head->next;
    while(current)
    {
        snake->body = SETUP_RECT(current->x, current->y, 1);
        if(current->next != NULL)
        {
            if(prevDir != current->direction) {
                current->frame = getCornerFrame(prevDir, current->direction);
                renderSprite(
                    hdc, &snake->sprite, 
                    snake->body.left, 
                    snake->body.top, 
                    snake->scale, current->frame, turquoise
                );
            }
            else 
            {
                current->frame = (Frame){.row=SEGMENT_STRAIGHT, .col=directionToIndex(current->direction)};
                renderSprite(
                    hdc, &snake->sprite, 
                    snake->body.left, 
                    snake->body.top, 
                    snake->scale, current->frame, turquoise
                );
            }
        }
        else
        {
            if(prevDir != current->direction) {
                current->frame = (Frame){.row=SEGMENT_CURVE_TAIL, .col=directionToIndex(prevDir)};
                renderSprite(
                    hdc, &snake->sprite, 
                    snake->body.left, 
                    snake->body.top, 
                    snake->scale, current->frame, turquoise
                );
            }
            else 
            {
                current->frame = (Frame){.row=SEGMENT_TAIL, .col=directionToIndex(current->direction)};
                renderSprite(
                    hdc, &snake->sprite, 
                    snake->body.left, 
                    snake->body.top, 
                    snake->scale, current->frame, turquoise
                );
            }
        }
        prevDir = current->direction;
        current = current->next;
    }
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

    renderSprite(hdc, &pellet->sprite, drawX, drawY, pellet->scale,(Frame){.row=0, .col=0}, turquoise);
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
        renderHeader(back_buffer, &pellet->sprite);
        renderOnGameOver(back_buffer, snake, pellet,gameState);
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
        int32_t topPart = renderTitle(hdc, &title, 1.2f);
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

void renderHeader(HDC hdc, SPRITE* sprite)
{
    float scale = 4.1f;
    renderSoundIcon(hdc, &sound, scale);
    renderSprite(hdc, sprite, (GRID_WIDTH - 5) * TILE_SIZE, 0, scale ,(Frame){.row=0, .col=0}, turquoise);
    HFONT hFont = NULL;
    char buffer[16];
    HFONT oldFont = CreateAndSelectFont(hdc, &hFont, font_size + 20, "Wobble Board", white);
    sprintf(buffer, " %d", score);
    TextOutA(hdc, ((GRID_WIDTH - 3) * TILE_SIZE) , ((sprite->height * scale)/2) - 10, buffer, lstrlen(buffer));
    SelectObject(hdc, oldFont);
    DeleteFont(&hFont);
}

void renderOnGameOver(HDC hdc, Snake *snake, Pellet* pellet, GAMESTATE* gameState)
{
    float scale = 5.8f;
    char buffer[16];
    int32_t sprite_x = (trophee.width) * scale; 
    int32_t sprite2_x = (pellet->sprite.width) * scale; 
    int32_t textY, textX;

    if(!snake->isMoving)
    {
        uint8_t rectScaleX = 8;
        uint8_t rectScaleY = 4;
        int32_t center_x, center_y;
        center_x = GRID_WIDTH / 2;
        center_y = ((GRID_HEIGHT) / 2) + 2;
        HFONT hFont = NULL;

        HFONT oldFont = CreateAndSelectFont(hdc, &hFont, font_size + 15, "Wobble Board", white);
        RECT rect = SCALE_RECT(center_x, center_y, rectScaleX, rectScaleY);
        
        if(*gameState == GAMEOVER)
        {
            renderTransparentLayer(hdc, true, &rect);
            TextOutA(hdc, (center_x - 7) * TILE_SIZE, (center_y + 2) * TILE_SIZE , "GameOver Buddy", 15);
            renderSprite(hdc, &trophee, (center_x - 5) * TILE_SIZE, (center_y - 4) * TILE_SIZE, scale,(Frame){.row=0, .col=0}, red);
            renderSprite(hdc, &pellet->sprite, (center_x + 2) * TILE_SIZE, (center_y - 4) * TILE_SIZE, scale ,(Frame){.row=0, .col=0}, turquoise);
            renderSprite(hdc, &restart_sprite, (center_x - 5) * TILE_SIZE, (center_y + (rectScaleY - 1) )* TILE_SIZE, scale - 1 ,frames[RESTART], turquoise);
            sprintf(buffer, "%d", high_score);
            
            int textW = FetchTextX(hdc, buffer);
            textY = (center_y - 1) * TILE_SIZE;
            textX = (center_x - 5) * TILE_SIZE + ((sprite_x - textW) / 2); 

            //TODO : add json or file read high_score
            //!HIGH_SCORE
            TextOutA(hdc,textX ,textY, buffer, lstrlen(buffer));
            //!SCORE
            sprintf(buffer, "%d", score);
            textW = FetchTextX(hdc, buffer);
            textX = (center_x + 2) * TILE_SIZE + ((sprite2_x - textW) / 2); 
            TextOutA(hdc,textX ,textY, buffer, lstrlen(buffer));
            
            restartRect = (RECT)
            {
                (center_x - 5) * TILE_SIZE, 
                (center_y + rectScaleY) * TILE_SIZE, 
                (center_x - 5) * TILE_SIZE + restart_sprite.width * scale, 
                (center_y + rectScaleY) * TILE_SIZE + restart_sprite.height * scale
            };
        }
        SelectObject(hdc, oldFont);
        DeleteFont(&hFont);
    }
}

void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale, const Frame frame, UINT transparentColorKey)
{
    sprite->memDC = CreateCompatibleDC(hdc);
    SelectObject(sprite->memDC, sprite->sheet);

    uint32_t frame_x = frame.col * sprite->width;
    uint32_t frame_y = frame.row * sprite->height;

    if(scale <= 0.f)
        scale = 1.0f;

    int destWidth = (int)(sprite->width * scale);
    int destHeight = (int)(sprite->height * scale);

    RECT src = {frame_x , frame_y, frame_x + sprite->width, frame_y + sprite->height};
    RECT dst = {cx, cy, cx + destWidth, cy + destHeight};

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

void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT* rect)
{
    int32_t width = (rect->right - rect->left);
    int32_t height = (rect->bottom - rect->top);
    uint8_t cx = rect->left;
    uint8_t cy = rect->top;
    HRGN roundRct;

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBRUSH overlayBrush = CreateSolidBrush(black);
    HGDIOBJ oldBrush =(HBRUSH)SelectObject(memDC, overlayBrush);
    HGDIOBJ oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
    
    FillRect(memDC, rect, overlayBrush);
    SelectObject(memDC, oldBrush);
    DeleteObject(overlayBrush);
    if(is_rounded)
    {
        roundRct = CreateRoundRectRgn(rect->left, rect->top, rect->right - 2, rect->bottom - 2, 30, 30);
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

        renderTransparentLayer(hdc, true, &rect);
        renderSprite(hdc, sprite, cx, cy + 50, 1.f,(Frame){.row=0, .col=0}, black);
    }
}

int32_t renderTitle(HDC hdc, SPRITE *sprite, float scaleFactor)
{
    if(sprite)
    {
        uint32_t titleX = (screen_width - (sprite->width * scaleFactor)) / 2;

        uint32_t titleY = (screen_height - (sprite->height * scaleFactor)) / 20;

        renderSprite(hdc, sprite, titleX , titleY, scaleFactor,(Frame){.row=0, .col=0}, black);

        return titleY + (sprite->height * scaleFactor);
    }
    return -1;
}

void renderSoundIcon(HDC hdc, SPRITE *sprite, float scaleFactor)
{
    if(sprite)
    {
        uint32_t iconX = 10;
        uint32_t iconY = 0;
        audioRect = (RECT){iconX, iconY, iconX + sprite->width * scaleFactor, iconY + sprite->height * scaleFactor};
        renderSprite(hdc, sprite, iconX , iconY, scaleFactor,frames[SOUND], red);
    }
}

void test(HDC hdc, const RECT* lprect)
{
    HPEN pen = CreatePen(PS_SOLID, 4, red);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    HPEN oldPen = SelectObject(hdc, pen);
    Rectangle(hdc, lprect->left, lprect->top, lprect->right, lprect->bottom);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

Frame getCornerFrame(DIRECTIONS fromDir, DIRECTIONS toDir)
{
    // Handle different corner combinations
    if ((fromDir == UP && toDir == RIGHT) )
        return (Frame){.row=SEGMENT_CURVE_UP, .col=CURVE_UP_TO_RIGHT_COL};
    else if(fromDir == LEFT && toDir == DOWN)
        return (Frame){.row=SEGMENT_CURVE_DOWN, .col=CURVE_LEFT_TO_DOWN_COL};
    else if ((fromDir == UP && toDir == LEFT) )
        return (Frame){.row=SEGMENT_CURVE_UP, .col=CURVE_UP_TO_LEFT_COL};
    else if (fromDir == RIGHT && toDir == DOWN)
        return (Frame){.row=SEGMENT_CURVE_DOWN, .col=CURVE_RIGHT_TO_DOWN_COL};  
    else if ((fromDir == DOWN && toDir == RIGHT))
        return (Frame){.row=SEGMENT_CURVE_DOWN, .col=CURVE_DOWN_TO_RIGHT_COL};
    else if (fromDir == LEFT && toDir == UP)
        return (Frame){.row=SEGMENT_CURVE_UP, .col=CURVE_LEFT_TO_UP_COL};
    else if ((fromDir == DOWN && toDir == LEFT))
        return (Frame){.row=SEGMENT_CURVE_DOWN, .col=CURVE_DOWN_TO_LEFT_COL};
    else if (fromDir == RIGHT && toDir == UP)
        return (Frame){.row=SEGMENT_CURVE_UP, .col=CURVE_RIGHT_TO_UP_COL};
    return (Frame){}; // Default
}