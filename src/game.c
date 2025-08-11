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

boolean isCollisionSnakeBody(Snake *snake)
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

boolean isCollisionSnakePellet(Game* game)
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
    renderToBackBuffer(game->window->hwnd, &game->state, game->buffer->backBuffer, game->pellet, game->snake);
    copyToFrontBuffer(game->buffer->backBuffer, game->window->hdc, cx, cy);
}

void GameLoop(Game* game)
{
    const DWORD frameDelay = 100;
    DWORD lastFrameTime = GetTickCount();

    while (game->isRunning)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                game->isRunning = false;
                _exitCode = (int32_t)msg.wParam;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        DWORD currentTime = GetTickCount();
        if (currentTime - lastFrameTime >= frameDelay)
        {
            lastFrameTime = currentTime;

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            {
                PostMessage(game->window->hwnd, WM_CLOSE, 0, 0);
            }

            if (GetAsyncKeyState(VK_F3) & 0x8000)
            {
                debugMode = !debugMode;
            }

            game->render(game, screen_width, screen_height);
            manageSound(game);
            startGame(game);
            resetGame(game);

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
    if(game)
    {
        //!Reset game
        if(game->snake && game->pellet)
        {
            if(game->snake)
            {
                logAndFreeSnakeMemory(game->snake->head,"free_logs/free_logs.txt");
                SAFE_FREE(game->snake);
            }
            SAFE_FREE(game->pellet);
        }
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
}

void startGame(Game *game)
{
    // Transition from MENU to WAIT_MOVE_INPUT if start was clicked
    if(game && ((game->state == MENU && startClicked) || (game->state == GAMEOVER && restartClicked)))
    {
        prepareGame(game);
        if(startClicked) startClicked = false;
        if(restartClicked) restartClicked = false;
        game->state = WAIT_MOVE_INPUT;
        SetupSprite(&game->pellet->sprite, "resources/assets/sprites/apple_1.bmp", 1, 1);
        SetupSprite(&game->snake->headSprite, "resources/assets/sprites/snakehead.bmp", 4, 3);
        SetupSprite(&game->snake->sprite, "resources/assets/sprites/snakebody.bmp", 5, 4);
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

void resetGame(Game *game)
{
    if(game->state == GAMEOVER)
    {
        POINT mouse;
        GetCursorPos(&mouse);
        ScreenToClient(game->window->hwnd, &mouse);
        if(hasClicked)
        {
            if(isPointInRect(&restartRect, mouse.x, mouse.y))
            {
                restartClicked = true;
                if(score > high_score) high_score = score;
                score = 0;
                hasClicked = false;
            }
        }
    }
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
            HWND window = game->window->hwnd;
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
    ScreenToClient(game->window->hwnd, &mouse);
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