#pragma once

#include "utilis.h"

typedef interface tagPellet
{
    int32_t cx;
    int32_t cy;
    float scale;
    RECT rect;
    SPRITE sprite;
}Pellet;

Pellet* initPellet();
void setPelletCoord(Pellet* pellet);
int get_rand_min_max(int min, int max);
