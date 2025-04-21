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
#define SAFE_FREE(p) do {if((p) != NULL){ free(p); p = NULL;}} while(0)

typedef enum tagDirections
{
    UP,
    LEFT,
    RIGHT,
    DOWN,
} DIRECTIONS;

typedef enum tagGameStates
{
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER
}GAMESTATE;

extern int32_t timer_intervalUID; 
extern int32_t screen_height, screen_width;
extern int32_t score;
extern int clickedX, clickedY;
extern boolean has_clicked;

//----------------------------Color section -------------------------------------
#define red              RGB(255, 0, 0)
#define green            RGB(0, 255, 0)
#define blue             RGB(0, 0, 255)
#define white            RGB(255, 255, 255)
#define black            RGB(0, 0, 0)
#define gray             RGB(128, 128, 128)
#define yellow           RGB(255, 255, 0)
#define cyan             RGB(0, 255, 255)
#define magenta          RGB(255, 0, 255)
#define orange           RGB(255, 165, 0)
#define darkOrange       RGB(252, 178, 79)
#define lightOrange      RGB(251, 193, 78)
#define purple           RGB(128, 0, 128)
#define teal             RGB(0, 128, 128)
#define navy             RGB(0, 0, 128)
#define pink             RGB(255, 192, 203)
#define gold             RGB(255, 215, 0)
#define copperRed        RGB(183, 80, 48)
#define SNAKE_HEAD_COLOR RGB(85, 107, 47)    // Olive
#define SNAKE_BODY_COLOR RGB(139, 69, 19)    // SaddleBrown
#define champagneSilver  RGB(230, 232, 234)
#define MENU_BG          RGB(36,94,99)

//------------------- Defining scales ---------------------------------
//! maybe in da future
#define DPI_SCALE_100 96
#define DPI_SCALE_125 120
#define DPI_SCALE_150 144
#define DPI_SCALE_200 192


#endif