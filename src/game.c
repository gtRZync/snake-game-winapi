#include "game.h"

void move_snake(Snake* snake, int nextX, int nextY)
{
    add_head(&snake->head, nextX, nextY);
    if(!snake->should_grow)
        remove_tail(&snake->head);
    else
        snake->should_grow = FALSE;
}

boolean is_collision_snake_walls(Snake *snake)
{
    for(int y = 0 ; y < GRID_HEIGHT ; y++)
    {
        for(int x = 0 ; x < GRID_WIDTH ; x++)
        {
            if(x == 0 || x == GRID_WIDTH - 1 || y == 0 || y == GRID_HEIGHT - 1 )
            {
                if((y) == snake->cy && (x + 1) == snake->cx && current_direction == UP || current_direction == LEFT
                || (y - 1) == snake->cy && (x) == snake->cx && current_direction == DOWN || current_direction == RIGHT)
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

boolean is_collisions_snake_pellet(Snake *snake, lpPellet pellet)
{
    RECT collision_rect;
    return (IntersectRect(&collision_rect, &snake->rect, &pellet->rect));
}

DIRECTIONS get_reverse_direction(DIRECTIONS direction)
{
    switch(direction)
    {
        case UP: return DOWN;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
    }
}

void set_rand_seed()
{
    srand((unsigned) time(NULL));
}

void eat_pellet(Snake* snake, lpPellet pellet)
{
    if(is_collisions_snake_pellet(snake, pellet))
    {
        set_pellet_coord(pellet);
        score+=1;
        snake->should_grow = TRUE;
    }
}

void game_over(HDC hdc, Snake *snake)
{
    int32_t center_x, center_y;
    center_x = GRID_WIDTH / 2;
    center_y = GRID_HEIGHT / 2;
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    HBRUSH brush = CreateSolidBrush(RGB(34, 67, 245));
    RECT rect = {(center_x - 6) * TILE_SIZE, (center_y - 3) * TILE_SIZE, (center_x + 6) * TILE_SIZE, (center_y + 3) * TILE_SIZE};
    if(is_collision_snake_walls(snake))
    {
        FillRect(hdc, &rect, brush);
        TextOutA(hdc, (center_x) * TILE_SIZE, center_y * TILE_SIZE , "GameOver Buddy", 15);
    }
    DeleteObject(brush);
}

void move_player(DIRECTIONS current_direction, Snake* snake)
{
    switch(current_direction)
    {
        case UP:    snake->cy--; break;
        case LEFT:  snake->cx--; break;
        case RIGHT: snake->cx++; break;
        case DOWN:  snake->cy++; break;
    }
    //! can potentitally use this to detect collision with walls
    if (snake->cx > GRID_WIDTH - 2) snake->cx = GRID_WIDTH - 2;
    if ( snake->cx < 1) snake->cx = 1;
    if (snake->cy > GRID_HEIGHT - 2) snake->cy = GRID_HEIGHT - 2;
    if (snake->cy < 1) snake->cy = 1;
    move_snake(snake, snake->cx, snake->cy);
}

void change_direction(DIRECTIONS *current_direction, Snake* snake, WPARAM wParam)
{
    DIRECTIONS new_direction = *current_direction;
    if (wParam == VK_LEFT && snake->cx > 0) new_direction = LEFT;
    else if (wParam == VK_RIGHT && snake->cx < GRID_WIDTH - 1) new_direction = RIGHT;
    else if (wParam == VK_UP && snake->cy > 0) new_direction = UP;
    else if (wParam == VK_DOWN && snake->cy < GRID_HEIGHT - 1) new_direction = DOWN;

    if (get_reverse_direction(*current_direction) != new_direction) {
        *current_direction = new_direction;
    }
}

void animate_pellet(lpPellet pellet)
{
    static float tick = .0f;
    const float TICK_INTERVAL = .3f;
    const float DELAY = 2.0f;
    tick += TICK_INTERVAL;
    if(tick >= 1.0f)
    {
        pellet->scale = 3;
        if(tick >= DELAY)
        {
            tick -= DELAY;
            pellet->scale = 0;
        }
    }
}
