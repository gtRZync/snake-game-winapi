#ifndef CONFIG_H
#define CONFIG_H
#pragma once
#define TILE_SIZE 32
#define GRID_WIDTH  26
#define GRID_HEIGHT 31 
#define TIMER_ID 1

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef enum tagDirections
{
    UP,
    LEFT,
    RIGHT,
    DOWN,
} DIRECTIONS;

typedef enum tagGameStates
{
    INTRO,
    PLAYING,
    PAUSED,
    GAMEOVER
}GAMESTATE;

extern DIRECTIONS current_direction;
extern int32_t timer_intervalUID; 
extern int32_t screen_height, screen_width;
extern int32_t score;
#endif