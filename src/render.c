#include "render.h"

void draw_snake(HDC hdc, Snake* snake_ptr)
{
    SnakeNode* current = snake_ptr->head;
    HBRUSH headBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(23, 255, 0));
    for(SnakeNode* i = snake_ptr->head; i != NULL ; i = i->next)
    {
        if(i == snake_ptr->head)
        {
            SelectObject(hdc, headBrush);
            snake_ptr->rect =(RECT) {i->x * TILE_SIZE, i->y * TILE_SIZE, (i->x + 1) * TILE_SIZE, (i->y + 1) * TILE_SIZE};
        }
        else
        {
            SelectObject(hdc , bodyBrush);
        }
        Rectangle(hdc, i->x * TILE_SIZE, i->y * TILE_SIZE, (i->x + 1) * TILE_SIZE, (i->y + 1) * TILE_SIZE);
    }
    DeleteObject(headBrush);
    DeleteObject(bodyBrush);
}

void draw_pellet(HDC hdc, lpPellet pellet)
{
    pellet->rect = (RECT){
        pellet->cx * TILE_SIZE,
        pellet->cy * TILE_SIZE,
        (pellet->cx + 1) * TILE_SIZE,
        (pellet->cy + 1) * TILE_SIZE,
    };
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    Ellipse(hdc, pellet->rect.left - pellet->scale, pellet->rect.top - pellet->scale, pellet->rect.right + pellet->scale, pellet->rect.bottom + pellet->scale);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void debug_list(SnakeNode* head_ptr, HDC hdc)
{
    char buffer[64];
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    SnakeNode* current = head_ptr;
    printf("Debugging snake list:\n");
    while (current != NULL)
    {
        sprintf(buffer, "Node at (%d, %d), Next: %#x\n", current->x, current->y,(void*)current->next);
        current = current->next;
        TextOutA(hdc, 0, 30, buffer, lstrlenA(buffer));
    }
}

void render_to_back_buffer(HDC back_buffer, lpPellet pellet, Snake* snake)
{
    RECT screen = {0, 0, screen_width, screen_height};
    FillRect(back_buffer, &screen, (HBRUSH)GetStockObject(BLACK_BRUSH));
    render_grid(back_buffer);
    draw_pellet(back_buffer, pellet);
    draw_snake(back_buffer, snake);
    show_score(back_buffer);
}

void copy_to_front_buffer(HDC back_buffer, HDC front_buffer, int cx, int cy)
{
    BitBlt(front_buffer, 0, 0, cx, cy, back_buffer, 0, 0, SRCCOPY);
}

void render_grid(HDC hdc )
{
    HBRUSH darkOrange = CreateSolidBrush(RGB(252, 178, 79));
    HBRUSH lightOrange = CreateSolidBrush(RGB(251, 193, 78));
    HBRUSH wallBrush = CreateSolidBrush(RGB(183,80,48));
    for(int y = 0 ; y < GRID_HEIGHT ; y++)
    {
        
        for(int x = 0 ; x < GRID_WIDTH ; x++)
        {
            RECT rect = {x * TILE_SIZE, y * TILE_SIZE, (x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE};
            if(x == 0 || x == GRID_WIDTH - 1 || y == 0 || y == GRID_HEIGHT - 1 )
            {
                FillRect(hdc, &rect, wallBrush);
            }
            else
            {
                if(y % 2 == 0){
                    if(x % 2 == 0)
                    {
                        FillRect(hdc, &rect, darkOrange);
                    }
                    else
                    {
                        FillRect(hdc, &rect, lightOrange);
                    }
                }
                else
                {
                    if(x % 2 != 0)
                    {
                        FillRect(hdc, &rect, darkOrange);
                    }
                    else
                    {
                        FillRect(hdc, &rect, lightOrange);
                    }
                }
            }
        }
    }
    DeleteObject(darkOrange);
    DeleteObject(lightOrange);
    DeleteObject(wallBrush);
}

void show_score(HDC hdc)
{
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    char buffer[16];
    sprintf(buffer, "Score : %d", score);
    TextOutA(hdc, (GRID_WIDTH - 4) * TILE_SIZE, TILE_SIZE, buffer, lstrlen(buffer));
}
