#include "game.h"

void moveSnake(Snake* snake, int nextX, int nextY)
{
    addHead(&snake->head, nextX, nextY); 

    if (snake->is_moving && !snake->should_grow)
    {
        removeTail(&snake->head);  
    }
    else
    {
        if (snake->should_grow)
        {
            snake->should_grow = FALSE;
        }
    }
}


boolean isCollisionSnakePellet(Snake *snake, Pellet* pellet)
{
    RECT collision_rect;
    return (IntersectRect(&collision_rect, &snake->head_rect, &pellet->rect));
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
        set_pellet_coord(pellet);
        score+=1;
        snake->should_grow = TRUE;
    }
}


void updateSnakePosition(DIRECTIONS current_direction, Snake* snake)
{
    if(snake->is_moving)
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
            snake->is_moving = FALSE;
            snake->is_collided_with_wall = TRUE;
            stopAtWall(current_direction, snake);
        }
        if(snake->is_moving)
        {
            moveSnake(snake, snake->cx, snake->cy);
        }
    }
}

void changeDirection(DIRECTIONS *current_direction, Snake* snake, WPARAM wParam)
{
    DIRECTIONS new_direction = *current_direction;
    if (wParam == VK_LEFT && snake->cx > 0) new_direction = LEFT;
    else if (wParam == VK_RIGHT && snake->cx < GRID_WIDTH - 1) new_direction = RIGHT;
    else if (wParam == VK_UP && snake->cy > 0) new_direction = UP;
    else if (wParam == VK_DOWN && snake->cy < GRID_HEIGHT - 1) new_direction = DOWN;

    if (getReversedDirection(*current_direction) != new_direction) {
        *current_direction = new_direction;
    }
}

void animatePellet(Pellet* pellet)
{
    static float tick = 0.0f;
    const float TICK_INTERVAL = 0.5f;  
    const float DELAY = 4.0f;
    

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

//? Linear interpolation for smooth scaling
float interpolateScale(float start, float end, float t) {
    return start + (end - start) * t;
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
    if(game == NULL)
    {
        MessageBoxW(NULL, L"Memory Allocation for game failed.", L"malloc failed", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    game->window = (Window*)malloc(sizeof(Window));
    if(game->window == NULL)
    {
        MessageBoxW(NULL, L"Memory Allocation for game->window failed.", L"malloc failed", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
    game->window->gameProc = GameWindowProc;

    game->isRunning = TRUE;
    game->state = START;
    game->createWindow = CreateGameWindow;
    game->destroy = GameDestroy;
    game->update = HandleGameMessages;

    game->pellet = init_pellet();
    game->snake = createSnake();
    game->snake_direction = RIGHT;
    
    return game;
}
void GameDestroy(Game* game)
{
    if(game)
    {
        if(game->snake)
        {
            logAndFreeSnakeMemory(game->snake->head,"free_logs/free_logs.txt");
        }
        if(game->pellet)
        {
            free(game->pellet);
        }
        if(game->window)
        {
            HWND window = game->window->hwnd;
            KillTimer(window, TIMER_ID);
            DestroyWindow(window);
            free(game->window);
        }
        free(game);
    }
}

