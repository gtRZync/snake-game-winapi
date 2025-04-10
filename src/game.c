#include "game.h"

void move_snake(Snake* snake, int nextX, int nextY)
{
    add_head(&snake->head, nextX, nextY); 

    if (snake->is_moving && !snake->should_grow)
    {
        remove_tail(&snake->head);  
    }
    else
    {
        if (snake->should_grow)
        {
            snake->should_grow = FALSE;
        }
    }
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


void move_player(DIRECTIONS current_direction, Snake* snake)
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
            snake->cy > GRID_HEIGHT - 2 || snake->cy < 1)
        {
            snake->is_moving = FALSE;
            snake->is_collided_with_wall = TRUE;
            stop_at_wall(current_direction, snake);
        }
        if(snake->is_moving)
        {
            move_snake(snake, snake->cx, snake->cy);
        }
    }
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



void stop_at_wall(DIRECTIONS direction, Snake* snake)
{
    switch(direction)
    {
        case UP:    snake->cy = 1; break;
        case LEFT:  snake->cx = 1; break;
        case RIGHT: snake->cx = GRID_WIDTH - 2; break;
        case DOWN:  snake->cy = GRID_HEIGHT - 2; break;
    }
}
