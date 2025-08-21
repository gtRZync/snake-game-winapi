#include "game.h"
#include "utilis.h"

bool debugMode = false;
int32_t _exitCode;


void moveSnake(Snake* snake, int nextX, int nextY, DIRECTIONS dir)
{
    addHead(&snake->head, nextX, nextY, dir); 

    if (snake->isMoving && !snake->shouldGrow)
    {
        removeTail(&snake->head);  
    }
    else
    {
        if (snake->shouldGrow)
        {
            snake->shouldGrow = false;
        }
    }
}

void checkSelfCollision(Game* game)
{
    if(isCollisionSnakeBody(game->snake))
    {
        stopAtSelf(game);
        game->snake->isMoving = false;
        if(game->state == PLAYING)
            game->state = GAMEOVER;
    }
}

void checkWallCollision(Game* game)
{
    if (game->snake->cx > GRID_WIDTH - 2 || game->snake->cx < 1 || 
        game->snake->cy > GRID_HEIGHT - 2 || game->snake->cy < 4)
    {
        stopAtWall(game);
        game->snake->isMoving = false;
        if(game->state == PLAYING)
            game->state = GAMEOVER;
    }
}

void checkCollisions(Game* game)
{
    checkSelfCollision(game);
    checkWallCollision(game);
#ifdef DEBUG
    static bool show = true;
    if(game->state == GAMEOVER) {
        if(show) {  
            printSnakeDirections(game->snake->head);
            show = false;
        }
    }
    else
    {
        show = true;
    }
#endif
}

bool isCollisionSnakeBody(Snake *snake)
{
    for(SnakeNode* i = snake->head ; i != NULL ; i = i->next)
        {
            if(i->x == snake->cx && i->y == snake->cy)
            {
                return true;
            }
        }
    return false;
}

bool isCollisionSnakePellet(Game* game)
{
    RECT collisionRect;
    return (IntersectRect(&collisionRect, &game->snake->headRect, &game->pellet->rect));
}

DIRECTIONS getReversedDirection(DIRECTIONS direction)
{
    switch(direction)
    {
        case UP: return DOWN;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
        default: return direction;
    }
}

void setRandomSeed()
{
    srand((unsigned) time(NULL));
}

void eatPellet(Game* game)
{
    if(isCollisionSnakePellet(game))
    {
        setPelletCoord(game->pellet);
        score+=1;
        game->snake->shouldGrow = true;
        if(score > high_score) high_score = score;
    }
}


void updateSnakePosition(Game* game)
{
    int32_t oldX = game->snake->cx;
    int32_t oldY = game->snake->cy;
    if(game->snake->isMoving)
    {
        switch(game->snake->direction)
        {
            case UP:    game->snake->cy--; break;
            case LEFT:  game->snake->cx--; break;
            case RIGHT: game->snake->cx++; break;
            case DOWN:  game->snake->cy++; break;
        }
        checkCollisions(game);
        if(game->snake->isMoving)
        {
            game->snake->previousX = oldX;
            game->snake->previousY = oldY;
            moveSnake(game->snake, game->snake->cx, game->snake->cy, game->snake->direction);
        }
    }
}


void changeDirection(DIRECTIONS *currentDirection, SPRITE* sprite)
{
    DIRECTIONS newDirection = *currentDirection;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) newDirection = LEFT;
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) newDirection = RIGHT;
    else if (GetAsyncKeyState(VK_UP) & 0x8000) newDirection = UP;
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) newDirection = DOWN;


    if (getReversedDirection(*currentDirection) != newDirection) {
        *currentDirection = newDirection;
    }
}

void animatePellet(Game* game)
{
    static float tick = 0.0f;
    const float TICK_INTERVAL = 0.5f;  
    const float DELAY = 4.0f;
    
    if(game->snake->isMoving)
    {
        tick += TICK_INTERVAL;
    
        if(tick >= DELAY) {
            tick = fmod(tick, DELAY);
        }
    
        if (tick < 1.0f) {
            game->pellet->scale = interpolateScale(2.0f, 2.6f, tick); 
        } else if (tick < 2.0f) {
            game->pellet->scale = interpolateScale(2.6f, 3.0f, tick - 1.0f); 
        } else if (tick < 3.0f) {
            game->pellet->scale = interpolateScale(3.0f, 2.6f, tick - 2.0f); 
        } else {
            game->pellet->scale = interpolateScale(2.6f, 2.0f, tick - 3.0f); 
        }
    }
}

//? Linear interpolation for smooth scaling
float interpolateScale(float start, float end, float t) {
    return start + (end - start) * t;
}

void UpdateGame(Game *game)
{
    updateSnakePosition(game);
    eatPellet(game);
    animatePellet(game);
}

void renderGame(Game* game, int32_t cx, int32_t cy)
{
    renderToBackBuffer(game->window->handle, &game->state, game->buffer->backBuffer, game->pellet, game->snake);
    if(debugMode)debugStat(game->buffer->backBuffer, game);
    copyToFrontBuffer(game->buffer->backBuffer, game->window->hdc, cx, cy);
}

void GameLoop(Game* game)
{
    const DWORD frameDelay = 130;
    ULONGLONG lastFrameTime = GetTickCount64();

    while (game->isRunning)
    {
        while (PeekMessage(&game->window->msg, NULL, 0, 0, PM_REMOVE))
        {
            if (game->window->msg.message == WM_QUIT)
            {
                game->isRunning = false;
                _exitCode = (int32_t)game->window->msg.wParam; 
                break;
            }

            TranslateMessage(&game->window->msg);
            DispatchMessage(&game->window->msg);
        }

        ULONGLONG currentTime = GetTickCount64();
        if (currentTime - lastFrameTime >= frameDelay)
        {
            lastFrameTime = currentTime;

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            {
                PostMessage(game->window->handle, WM_CLOSE, 0, 0);
            }

            if (GetAsyncKeyState(VK_F3) & 0x8000)
            {
                debugMode = !debugMode;
            }

            game->render(game, screen_width, screen_height);
            startGame(game);
            handleButtonClick(game);
            processGameOverButton(game->window->handle);

            if (game->state == PLAYING)
            {
                changeDirection(&game->snake->direction, &game->snake->headSprite);
                UpdateGame(game);
            }

            Sleep(1);
        }
    }
}

void stopAtWall(Game* game)
{
    switch(game->snake->direction)
    {
        case UP:    game->snake->cy = 1; break;
        case LEFT:  game->snake->cx = 1; break;
        case RIGHT: game->snake->cx = GRID_WIDTH - 2; break;
        case DOWN:  game->snake->cy = GRID_HEIGHT - 2; break;
    }
}


void stopAtSelf(Game* game)
{
    switch(game->snake->direction)
    {
        case UP:    game->snake->cy = game->snake->previousY; break;
        case LEFT:  game->snake->cx = game->snake->previousX; break;
        case RIGHT: game->snake->cx = game->snake->previousX; break;
        case DOWN:  game->snake->cy = game->snake->previousY; break;
    }
}

Game* InitializeGame()
{
    Game* game = (Game*)malloc(sizeof(Game));
    if(!game)
    {
        FatalAllocError(L"Memory Allocation for game failed.");
        exit(EXIT_FAILURE);
    }

    game->window = (Window*)malloc(sizeof(Window));
    if(!game->window)
    {
        FatalAllocError(L"Memory Allocation for game->window failed.");
        GameDestroy(game);
        exit(EXIT_FAILURE);
    }
    game->window->gameProc = GameWindowProc;

    game->isRunning = true;
    game->state = MENU;
    game->isMuted = false;
    game->createWindow = CreateGameWindow;
    game->destroy = GameDestroy;
    game->update = GameLoop;
    game->setupDoubleBuffering = setupDoubleBuffering;
    game->resizeDoubleBuffer = resizeDoubleBuffer;
    game->doubleBufferingCleanup = doubleBufferingCleanup;
    game->render = renderGame;
    game->deltatime = .0f;
    game->buffer = (DOUBLE_BUFFER*)malloc(sizeof(DOUBLE_BUFFER));
    if(!game->buffer)
    {
        FatalAllocError(L"Memory Allocation for game->buffer failed.");
        GameDestroy(game);
        exit(EXIT_FAILURE);
    }
    game->snake = NULL;
    game->pellet = NULL;
    
    return game;
}

void prepareGame(Game *game)
{
    if (!game) return;
    if(game->snake == NULL && game->pellet == NULL)
    {
        game->pellet = initPellet();
        game->snake = createSnake();
        if(!game->snake)
        {
            FatalAllocError(L"Memory Allocation for game->snake failed.");
            GameDestroy(game);
            exit(EXIT_FAILURE);
        }
    }
}

void resetGame(Game *game)
{
    if (!game) return;
    if(game->snake && game->pellet)
    {
        if(game->snake)
        {
            logAndFreeSnakeMemory(game->snake->head,"free_logs/free_logs.txt");
            SAFE_FREE(game->snake);
        }
        SAFE_FREE(game->pellet);
        if(score > high_score) high_score = score;
        score = 0;
    }
}
void startGame(Game *game)
{
    if (!game) return;
    // Transition from MENU to WAIT_MOVE_INPUT if start was clicked
    if((game->state == MENU && startClicked) || (game->state == GAMEOVER && restartClicked))
    {
        resetGame(game);
        prepareGame(game);
        if(startClicked) startClicked = false;
        if(restartClicked) restartClicked = false;
        game->state = WAIT_MOVE_INPUT;
        SetupSprite(&game->pellet->sprite, "resources/assets/sprites/apple.bmp", (const Frame){.totalRows=1, .totalCols=1});
        SetupSprite(&game->snake->headSprite, "resources/assets/sprites/snakehead.bmp", (const Frame){.totalRows=4, .totalCols=2});
        SetupSprite(&game->snake->sprite, "resources/assets/sprites/snakebody.bmp", (const Frame){.totalRows=5, .totalCols=4});
    }

    // Transition to PLAYING if a direction key is pressed
    if(game && game->state == WAIT_MOVE_INPUT)
    {
        if(
            (GetAsyncKeyState(VK_UP) & 0x8000)   ||
            (GetAsyncKeyState(VK_DOWN) & 0x8000) || 
            (GetAsyncKeyState(VK_LEFT) & 0x8000) ||
            (GetAsyncKeyState(VK_RIGHT) & 0x8000))
            {
                game->state = PLAYING;
            }
    }
}

void processGameOverClick(Game *game, const RECT* rect, bool isRestarting)
{
    if (!game) return;

    if  (game->state != GAMEOVER) return;

    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(game->window->handle, &mouse);
    if(isPointInRect(rect, mouse.x, mouse.y))
    {
        if(hasClicked)
        {
            if(isRestarting) {
                restartClicked = true;
            }
            else
            {
                redirectMenu(game);
            }
            hasClicked = false;
        }
    }
}

void handleButtonClick(Game* game) {
    bool isRestarting = true;
    manageSound(game);
    processGameOverClick(game, &restartRect, isRestarting);
    processGameOverClick(game, &homeRect, !isRestarting);
}

void GameDestroy(Game* game)
{
    if(game)
    {
        if(game->snake)
        {
            logAndFreeSnakeMemory(game->snake->head,"free_logs/free_logs.txt");
            SAFE_FREE(game->snake);
        }
        SAFE_FREE(game->pellet);
        if(game->buffer)
        {
            game->doubleBufferingCleanup(game->buffer);
            SAFE_FREE(game->buffer);
        }
        if(game->window)
        {
            HWND window = game->window->handle;
            ReleaseDC(window, game->window->hdc);
            DestroyWindow(window);
            SAFE_FREE(game->window);
        }
        SAFE_FREE(game);
    }
}

void FatalAllocError(LPCWSTR what)
{
    MessageBoxW(NULL, what, L"malloc failed", MB_OK | MB_ICONERROR);
}

void manageSound(Game* game)
{
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(game->window->handle, &mouse);
    if(hasClicked)
    {
        if(isPointInRect(&audioRect, mouse.x, mouse.y))
        {
            game->isMuted = !game->isMuted;
            hasClicked = false;
        }
    }
    if(game->isMuted)
        muteGame(&sound);
    else
        playGameSound(&game->state, &sound, game->isMuted);
}

void animateButton(HWND hwnd, const RECT* rect, const SPRITE* sprite, Frame* frame) {
    const int DEFAULT_INDEX = 0;
    const int HOVERED_INDEX = 1;
    const int CLICKED_INDEX = 2;
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(hwnd, &mouse);
    if(!hasPressed && isPointInRect(rect, mouse.x, mouse.y)) {
        setFrameCol(sprite, frame, HOVERED_INDEX);
    }
    else if (hasPressed && isPointInRect(rect, mouse.x, mouse.y)) 
    {
        setFrameCol(sprite, frame, CLICKED_INDEX);
    }
    else 
    {
        setFrameCol(sprite, frame, DEFAULT_INDEX);
    }
}

void processGameOverButton(HWND hwnd) {
    animateButton(hwnd, &restartRect, &restart_sprite, &frames[RESTART]);
    animateButton(hwnd, &homeRect, &home_sprite, &frames[HOME]);
}

void redirectMenu(Game* game) {
    resetGame(game);
    game->state = MENU;
}

void debugStat(HDC hdc, Game* game)
{
    uint8_t scale_x = 3, scale_y = 8;
    int32_t x, center_y;
    x = 3;
    center_y = ((GRID_HEIGHT) / 2) + 2; //? adding two to center it according to the grid
    HFONT hFont = NULL;
    char buffer[64];
    const char* state = (game->state == MENU) ? "MENU" :
                            (game->state == WAIT_MOVE_INPUT) ? "WAIT_MOVE_INPUT" :
                            (game->state == PLAYING) ? "PLAYING" :
                            (game->state == PAUSED) ? "PAUSED" : "GAMEOVER";
                            

    RECT rect = SCALE_RECT(x, center_y, scale_x, scale_y);
    HFONT oldFont = CreateAndSelectFont(hdc, &hFont, -12, "JetBrains Mono", white);
    renderTransparentLayer(hdc, true, &rect);

    sprintf(buffer, "GameState = %s", state);
    TextOut(hdc, ((x - scale_x) + 2)* TILE_SIZE, (center_y - scale_y) * TILE_SIZE, "Debug : ", 9);
    TextOut(hdc, (x - scale_x) * TILE_SIZE, ((center_y - scale_y) + 1) * TILE_SIZE, buffer, lstrlen(buffer));
    sprintf(buffer, "hasClicked = %s", hasClicked ? "true":"false");
    TextOut(hdc, (x - scale_x) * TILE_SIZE, ((center_y - scale_y) + 2) * TILE_SIZE, buffer, lstrlen(buffer));
    sprintf(buffer, "restartClicked = %s", restartClicked ? "true":"false");
    TextOut(hdc, (x - scale_x) * TILE_SIZE, ((center_y - scale_y) + 3) * TILE_SIZE, buffer, lstrlen(buffer));
    sprintf(buffer, "isMuted = %s", game->isMuted ? "true":"false");
    TextOut(hdc, (x - scale_x) * TILE_SIZE, ((center_y - scale_y) + 4) * TILE_SIZE, buffer, lstrlen(buffer));

    SelectObject(hdc, oldFont);
    DeleteFont(&hFont);
}