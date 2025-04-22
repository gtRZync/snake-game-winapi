#include "pellet.h"

Pellet* initPellet()
{
    Pellet* pellet = (Pellet*)malloc(sizeof(Pellet));
    if(pellet == NULL)
    {
        MessageBoxW(NULL, L"Memory Allocation for pellet failed.", L"malloc failed", MB_OK | MB_ICONERROR);
        return NULL;
    }
    setPelletCoord(pellet);
    pellet->scale = 2;
    pellet->rect = SETUP_RECT(pellet->cx, pellet->cy, 1);
    pellet->sprite = (SPRITE){ };
    
    return pellet;
}

void setPelletCoord(Pellet* pellet)
{
    pellet->cx = get_rand_min_max(1, (GRID_WIDTH - 2));
    pellet->cy = get_rand_min_max(4, (GRID_HEIGHT - 5));
}

int get_rand_min_max(int min, int max)
{
    return min + rand() % (max - min + 1);
}
