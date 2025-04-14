#ifndef CONFIG_H
#define CONFIG_H
#pragma once
#define TILE_SIZE 32
#define GRID_WIDTH  26
#define GRID_HEIGHT 19 
#define TIMER_ID 1

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#define SCALE_RECT(cx, cy, scale_x, scale_y) (RECT){(cx - scale_x) * TILE_SIZE, (cy - scale_y) * TILE_SIZE, (cx + scale_x) * TILE_SIZE, (cy + scale_y) * TILE_SIZE}
#define SETUP_RECT(cx, cy, scaled_size) (RECT){(cx) * TILE_SIZE, (cy) * TILE_SIZE, (cx + scaled_size) * TILE_SIZE, (cy + scaled_size) * TILE_SIZE}

typedef enum tagDirections
{
    UP,
    LEFT,
    RIGHT,
    DOWN,
} DIRECTIONS;

extern int32_t timer_intervalUID; 
extern int32_t screen_height, screen_width;
extern int32_t score;

//----------------------------Color section -------------------------------------
extern const COLORREF red;
extern const COLORREF green;
extern const COLORREF blue;
extern const COLORREF white;
extern const COLORREF black;
extern const COLORREF gray;
extern const COLORREF yellow;
extern const COLORREF cyan;
extern const COLORREF magenta;
extern const COLORREF orange;
extern const COLORREF purple;
extern const COLORREF teal;
extern const COLORREF navy;
extern const COLORREF pink;
extern const COLORREF gold;
extern const COLORREF darkOrange;
extern const COLORREF lightOrange;
extern const COLORREF copperRed;
extern const COLORREF SNAKE_HEAD_COLOR;
extern const COLORREF SNAKE_BODY_COLOR;

//------------------- Defining scales ---------------------------------
//! maybe in tha future
#define DPI_SCALE_100 96
#define DPI_SCALE_125 120
#define DPI_SCALE_150 144
#define DPI_SCALE_200 192


#endif