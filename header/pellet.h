#ifndef PELLET_H
#define PELLET_H
#pragma once

#include "config.h"
#include "sprite.h"

typedef interface tagPellet
{
    int32_t cx;
    int32_t cy;
    int8_t scale;
    RECT rect;
    SPRITE sprite;
}Pellet;

Pellet* initPellet();
void setPelletCoord(Pellet* pellet);
int get_rand_min_max(int min, int max);

#endif