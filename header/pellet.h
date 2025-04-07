#ifndef PELLET_H
#define PELLET_H
#pragma once

#include "config.h"

typedef struct tagPellet
{
    int32_t cx;
    int32_t cy;
    int8_t scale;
    RECT rect;
}Pellet, *lpPellet;

void init_pellet(lpPellet pellet);
void set_pellet_coord(lpPellet pellet);
int get_rand_min_max(int min, int max);


#endif