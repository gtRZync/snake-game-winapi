#include "render.h"

// STATIC INTERNALS – private to render.c
static void renderMenu(HWND hwnd, HDC hdc, const GAMESTATE* gameState);
static void renderGrid(HDC hdc);
static void renderHeader(HDC hdc, SPRITE* sprite);
static void drawSnake(HDC hdc, Snake* snake);
static void drawPellet(HDC hdc, Pellet* pellet);
static void renderOnGameOver(HDC hdc, Snake* snake, Pellet* pellet, GAMESTATE* gameState);
static void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale, const Frame frame, UINT transparentColorKey);
static void setupRectAndRenderSprite(HDC hdc, SPRITE *sprite, uint32_t posX, uint32_t posY, float scale, const Frame frame, UINT transparentColorKey, RECT* outRect, int8_t inflateX, int8_t inflateY);
static void renderControlKeysOverlay(HDC hdc, SPRITE* sprite, GAMESTATE* gameState);
static int32_t renderTitle(HDC hdc, SPRITE *sprite, float scaleFactor);
static void renderSoundIcon(HDC hdc, SPRITE *sprite, float scaleFactor);
static Frame getCornerFrame(DIRECTIONS fromDir, DIRECTIONS toDir);
static void test(HDC hdc, const RECT* lprect);
static void renderGameOverText(HDC hdc, RECT* rect);
static void renderTrophies(HDC hdc, int center_x, int center_y, float scale, Pellet* pellet) ;
static void renderButton(HDC hdc, SPRITE* sprite, int posX, int posY, float scale, Frame frame, COLORREF color, RECT* outRect, int inflateX, int inflateY);
static void renderScoreValue(HDC hdc, int value, int posX, int posY, int spriteW);
static void renderButtonsText(HDC hdc, int center_x, int center_y, int homeW, int restartW, int font_size);
static void renderScores(HDC hdc, int center_x, int center_y, int trophyW, int pelletW);

int32_t screen_height, screen_width;
int32_t score = 0;
int32_t high_score = 0;
SPRITE title;
SPRITE keys;
SPRITE sound;
SPRITE trophy;
SPRITE restart_sprite;
SPRITE home_sprite;
RECT audioRect;
RECT restartRect;
RECT homeRect;

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
    {.row=0, .col=1},//SOUND
    {.row=0, .col=0},//RESTART
    {.row=0, .col=0}//HOME
};

// --- PUBLIC API IMPLEMENTATIONS ---
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

// --- PRIVATE STATIC IMPLEMENTATIONS ---

//static void drawSnake(HDC hdc, Snake* snake) {

//     if(!snake || !snake->head) return;

//     SnakeNode* current = snake->head;
//     HPEN bodyPen = CreatePen(PS_SOLID, TILE_SIZE - 4, RGB(30, 200, 120));
//     HPEN shadowPen = CreatePen(PS_SOLID, TILE_SIZE - 4, black);
//     snake->headRect = SETUP_RECT(snake->head->x,snake->head->y, 1);
    
//     HGDIOBJ oldPen = SelectObject(hdc, shadowPen);
//     while (current)
//     {
//         if(current != snake->head) 
//             snake->body = SETUP_RECT(current->x,current->y, 1);
//         MoveToEx(hdc, CENTER(current->x), CENTER(current->y) + SHADOW_OFFSET, NULL);
//         current = current->next;
//         if(current)
//             LineTo(hdc, CENTER(current->x), CENTER(current->y) + SHADOW_OFFSET);
//     }
    
//     current = snake->head;
//     SelectObject(hdc, bodyPen);
//     while (current)
//     {
        
//         MoveToEx(hdc, CENTER(current->x), CENTER(current->y), NULL);
//         current = current->next;
//         if(current)
//             LineTo(hdc, CENTER(current->x), CENTER(current->y));
//     }
//     SelectObject(hdc, oldPen);
//     DeleteObject(bodyPen);
//     DeleteObject(shadowPen);
// }

static void drawSnake(HDC hdc, Snake* snake)
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

static void drawPellet(HDC hdc, Pellet* pellet)
{
    // Compute the center of the tile
    int32_t centerX = CENTER(pellet->cx);
    int32_t centerY = CENTER(pellet->cy);

    pellet->rect = SETUP_RECT(pellet->cx, pellet->cy, 1);

    // Compute scaled sprite size
    int scaledW = pellet->sprite.width * pellet->scale;
    int scaledH = pellet->sprite.height * pellet->scale;

    // Position so the sprite's center stays at the tile center
    int drawX = centerX - scaledW / 2;
    int drawY = centerY - scaledH / 2;

    renderSprite(hdc, &pellet->sprite, drawX, drawY, pellet->scale,(Frame){.row=0, .col=0}, turquoise);
}


static void renderMenu(HWND hwnd, HDC hdc, const GAMESTATE* gameState)
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

static void renderGrid(HDC hdc)
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

static void renderHeader(HDC hdc, SPRITE* sprite)
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

static void renderOnGameOver(HDC hdc, Snake *snake, Pellet* pellet, GAMESTATE* gameState) {
    if (*gameState != GAMEOVER || snake->isMoving) return;

    const float scale = 5.6f;
    const float buttonScale = scale - 1;
    const int inflateX = -21, inflateY = -21;

    const int center_x = GRID_WIDTH / 2;
    const int center_y = GRID_HEIGHT / 2 + 2;

    RECT rect = SCALE_RECT(center_x, center_y, 8, 7);

    // Fonts
    HFONT hFont = NULL;
    HFONT oldFont = CreateAndSelectFont(hdc, &hFont, -(font_size + 15), "Wobble Board", white);

    renderTransparentLayer(hdc, true, &rect);
    renderGameOverText(hdc, &rect);
    renderTrophies(hdc, center_x, center_y, scale, pellet);

    // Button Rendering
    int posY = (center_y + 3) * TILE_SIZE;
    int posX_restart = (center_x - 6) * TILE_SIZE;
    int posX_home = (center_x + 1) * TILE_SIZE;

    renderButton(hdc, &restart_sprite, posX_restart, posY, buttonScale, frames[RESTART], turquoise, &restartRect, inflateX, inflateY);
    renderButton(hdc, &home_sprite, posX_home, posY, buttonScale, frames[HOME], turquoise, &homeRect, inflateX, inflateY);

    // Score Rendering
    int trophyW = trophy.width * scale;
    int pelletW = pellet->sprite.width * scale;
    renderScores(hdc, center_x, center_y, trophyW, pelletW);

    // Button Text
    renderButtonsText(hdc, center_x, center_y, home_sprite.width * scale - 1, restart_sprite.width * scale - 1, font_size);

    // Cleanup
    SelectObject(hdc, oldFont);
    DeleteFont(&hFont);
}

static void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale, const Frame frame, UINT transparentColorKey)
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

static void setupRectAndRenderSprite(HDC hdc, SPRITE *sprite, uint32_t posX, uint32_t posY, float scale, const Frame frame, UINT transparentColorKey, RECT* outRect, int8_t inflateX, int8_t inflateY)
{
    renderSprite(hdc, sprite, posX, posY, scale, frame, transparentColorKey);
    if(outRect) {
        *outRect = (RECT)
        {
            posX,
            posY,
            posX + sprite->width * scale,
            posY + sprite->height * scale 
        };
        if(inflateX != 0 || inflateY != 0) {
            InflateRect(outRect, inflateX, inflateY);
        }
    }
}

static void renderControlKeysOverlay(HDC hdc, SPRITE* sprite, GAMESTATE* gameState)
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

static int32_t renderTitle(HDC hdc, SPRITE *sprite, float scaleFactor)
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

static void renderSoundIcon(HDC hdc, SPRITE *sprite, float scaleFactor)
{
    if(sprite)
    {
        uint32_t iconX = 10;
        uint32_t iconY = 0;
        audioRect = (RECT){iconX, iconY, iconX + sprite->width * scaleFactor, iconY + sprite->height * scaleFactor};
        renderSprite(hdc, sprite, iconX , iconY, scaleFactor,frames[SOUND], red);
    }
}

static void test(HDC hdc, const RECT* lprect)
{
    HPEN pen = CreatePen(PS_SOLID, 4, red);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    HPEN oldPen = SelectObject(hdc, pen);
    Rectangle(hdc, lprect->left, lprect->top, lprect->right, lprect->bottom);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

static Frame getCornerFrame(DIRECTIONS fromDir, DIRECTIONS toDir)
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


static void renderGameOverText(HDC hdc, RECT* rect) {
    const char* gameover = "GAMEOVER";
    RECT gameOverRect = *rect;
    gameOverRect.top += 35;

    DrawTextA(hdc, gameover, -1, &gameOverRect, DT_CENTER | DT_SINGLELINE);
}

static void renderTrophies(HDC hdc, int center_x, int center_y, float scale, Pellet* pellet) {
    renderSprite(hdc, &trophy, (center_x - 5) * TILE_SIZE, (center_y - 4) * TILE_SIZE, scale, (Frame){.row = 0, .col = 0}, red);
    renderSprite(hdc, &pellet->sprite, (center_x + 2) * TILE_SIZE, (center_y - 4) * TILE_SIZE, scale, (Frame){.row = 0, .col = 0}, turquoise);
}

static void renderButton(HDC hdc, SPRITE* sprite, int posX, int posY, float scale, Frame frame, COLORREF color, RECT* outRect, int inflateX, int inflateY) {
    setupRectAndRenderSprite(hdc, sprite, posX, posY, scale, frame, color, outRect, inflateX, inflateY);
}

static void renderScoreValue(HDC hdc, int value, int posX, int posY, int spriteW) {
    char buffer[16];
    sprintf(buffer, "%d", value);
    int textW = FetchTextX(hdc, buffer);
    int textX = posX + ((spriteW - textW) / 2);

    TextOutA(hdc, textX, posY, buffer, lstrlen(buffer));
}

static void renderButtonsText(HDC hdc, int center_x, int center_y, int homeW, int restartW, int font_size) {
    const char* restart = "RESTART";
    const char* home = "HOME";

    HFONT hFont = NULL;
    HFONT oldFont = CreateAndSelectFont(hdc, &hFont, -font_size, "Wobble Board", white);
    int textY = (center_y + 3) * TILE_SIZE;

    // HOME
    int homeTextW = FetchTextX(hdc, home);
    int homeX = (center_x + 1) * TILE_SIZE + ((homeW - homeTextW) / 2);
    TextOutA(hdc, homeX - 10, textY, home, lstrlen(home));

    // RESTART
    int restartTextW = FetchTextX(hdc, restart);
    int restartX = (center_x - 6) * TILE_SIZE + ((restartW - restartTextW) / 2);
    TextOutA(hdc, restartX - 15, textY, restart, lstrlen(restart));

    SelectObject(hdc, oldFont);
    DeleteFont(&hFont);
}

static void renderScores(HDC hdc, int center_x, int center_y, int trophyW, int pelletW) {
    int textY = center_y * TILE_SIZE;

    renderScoreValue(hdc, high_score, (center_x - 5) * TILE_SIZE, textY, trophyW);
    renderScoreValue(hdc, score, (center_x + 2) * TILE_SIZE, textY, pelletW);
}
