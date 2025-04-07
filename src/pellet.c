#include "pellet.h"

void init_pellet(lpPellet pellet)
{
    set_pellet_coord(pellet);
    pellet->scale = 1;
}

void set_pellet_coord(lpPellet pellet)
{
    pellet->cx = get_rand_min_max(1, GRID_WIDTH - 2);
    pellet->cy = get_rand_min_max(1, GRID_HEIGHT - 2);
}

int get_rand_min_max(int min, int max)
{
    return min + rand() % (max - min + 1);
}
