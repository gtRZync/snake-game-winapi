#include "game.h"

void moveSnake(Snake* snake, int nextX, int nextY)
{
    addHead(&snake->head, nextX, nextY); 

    if (snake->isMoving && !snake->shouldGrow)
    {
        removeTail(&snake->head);  
    }
    else
    {
        if (snake->shouldGrow)
        {
            snake->shouldGrow = FALSE;
        }
    }
}


boolean isCollisionSnakePellet(Snake *snake, Pellet* pellet)
{
    RECT collision_rect;
    return (IntersectRect(&collision_rect, &snake->headRect, &pellet->rect));
}

DIRECTIONS getReversedDirection(DIRECTIONS direction)
{
    switch(direction)
    {
        case UP: return DOWN;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
    }
}

void setRandomSeed()
{
    srand((unsigned) time(NULL));
}

void eatPellet(Snake* snake, Pellet* pellet)
{
    if(isCollisionSnakePellet(snake, pellet))
    {
        setPelletCoord(pellet);
        score+=1;
        snake->shouldGrow = TRUE;
    }
}


void updateSnakePosition(DIRECTIONS current_direction, Snake* snake)
{
    if(snake->isMoving)
    {
        switch(current_direction)
        {
            case UP:    snake->cy--; break;
            case LEFT:  snake->cx--; break;
            case RIGHT: snake->cx++; break;
            case DOWN:  snake->cy++; break;
        }
        if (snake->cx > GRID_WIDTH - 2 || snake->cx < 1 || 
            snake->cy > GRID_HEIGHT - 2 || snake->cy < 4)
        {
            snake->isMoving = FALSE;
            snake->isCollidedWithWall = TRUE;
            stopAtWall(current_direction, snake);
        }
        if(snake->isMoving)
        {
            moveSnake(snake, snake->cx, snake->cy);
        }
    }
}

void changeDirection(DIRECTIONS *current_direction, Snake* snake)
{
    DIRECTIONS new_direction = *current_direction;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) new_direction = LEFT;
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) new_direction = RIGHT;
    else if (GetAsyncKeyState(VK_UP) & 0x8000) new_direction = UP;
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) new_direction = DOWN;

    if (getReversedDirection(*current_direction) != new_direction) {
        *current_direction = new_direction;
    }
}

void animatePellet(Pellet* pellet, boolean snakeIsMoving)
{
    static float tick = 0.0f;
    const float TICK_INTERVAL = 0.5f;  
    const float DELAY = 4.0f;
    
    if(snakeIsMoving)
    {
        tick += TICK_INTERVAL;
    
        if(tick >= DELAY) {
            tick = fmod(tick, DELAY);
        }
    
        if (tick < 1.0f) {
            pellet->scale = interpolateScale(-5.0f, -4.0f, tick); // Larger transition from -5 to -4
        } else if (tick < 2.0f) {
            pellet->scale = interpolateScale(-4.0f, -3.0f, tick - 1.0f); // From -4 to -3
        } else if (tick < 3.0f) {
            pellet->scale = interpolateScale(-3.0f, -2.0f, tick - 2.0f); // From -3 to -2
        } else {
            pellet->scale = interpolateScale(-2.0f, -5.0f, tick - 3.0f); // Reverse: from -2 to -5
        }
    }
}

//? Linear interpolation for smooth scaling
float interpolateScale(float start, float end, float t) {
    return start + (end - start) * t;
}

void UpdateGame(Game *game)
{
    updateSnakePosition(game->snake_direction, game->snake);
    eatPellet(game->snake, game->pellet);
    animatePellet(game->pellet, game->snake->isMoving);
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
            if (msg.message == WM_QUIT || msg.message == WM_CLOSE)
            {
                game->isRunning = FALSE;
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
            // Render
            game->render(game, screen_width, screen_height);
            startGame(game);
            if(game->state == PLAYING)
            {
                // Input
                changeDirection(&game->snake_direction, game->snake);

                // Update logic
                UpdateGame(game);
            }
            Sleep(1);
        }
    }
}

void stopAtWall(DIRECTIONS direction, Snake* snake)
{
    switch(direction)
    {
        case UP:    snake->cy = 1; break;
        case LEFT:  snake->cx = 1; break;
        case RIGHT: snake->cx = GRID_WIDTH - 2; break;
        case DOWN:  snake->cy = GRID_HEIGHT - 2; break;
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

    game->isRunning = TRUE;
    game->state = MENU;
    game->createWindow = CreateGameWindow;
    game->destroy = GameDestroy;
    game->update = GameLoop;
    game->setupDoubleBuffering = setupDoubleBuffering;
    game->resizeDoubleBuffer = resizeDoubleBuffer;
    game->doubleBufferingCleanup = doubleBufferingCleanup;
    game->render = renderGame;
    game->snake_direction = RIGHT;
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
    if(game && game->state == MENU && startClicked)
    {
        prepareGame(game);
        startClicked = FALSE;
        game->state = WAIT_MOVE_INPUT;
    }
    if(game && game->state == WAIT_MOVE_INPUT)
    {
        if(
            (GetAsyncKeyState(VK_UP) & 0x8000) ||
            (GetAsyncKeyState(VK_DOWN) & 0x8000) || 
            (GetAsyncKeyState(VK_LEFT) & 0x8000) ||
            (GetAsyncKeyState(VK_RIGHT) & 0x8000))
            {
                game->state = PLAYING;
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

