#pragma once

#include "utilis.h"

typedef struct Pellet
{
    int32_t cx;
    int32_t cy;
    float scale;
    RECT rect;
    SPRITE sprite;
}Pellet;

Pellet* initPellet();
void setPelletCoord(Pellet* pellet);
