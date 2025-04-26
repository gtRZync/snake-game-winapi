#ifndef CONFIG_H
#define CONFIG_H
#pragma once
#define TILE_SIZE 32
#define GRID_WIDTH  26
#define GRID_HEIGHT 19 

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

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
    WAIT_MOVE_INPUT,
    PLAYING,
    PAUSED,
    GAMEOVER
}GAMESTATE;

extern int32_t timer_intervalUID; 
extern int32_t screen_height, screen_width;
extern int32_t score;
extern boolean has_clicked, startClicked;
extern RECT audioRect;

//------------------- Defining scales ---------------------------------
//! maybe in da future
#define DPI_SCALE_100 96
#define DPI_SCALE_125 120
#define DPI_SCALE_150 144
#define DPI_SCALE_200 192

#endif