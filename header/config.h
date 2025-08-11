#pragma once
#define TILE_SIZE 32
#define GRID_WIDTH  26
#define GRID_HEIGHT 19 
#define DEBUG

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "sprite.h"

#define NUM_DIRECTIONS 4
#define HEAD 0
typedef enum tagDirections
{
    UP,
    RIGHT,
    LEFT,
    DOWN,
} DIRECTIONS;

typedef enum tagSegmentTypes
{
    SEGMENT_STRAIGHT,    
    SEGMENT_CURVE_UP,   
    SEGMENT_CURVE_DOWN,   
    SEGMENT_TAIL,        
    SEGMENT_CURVE_TAIL         
} SEGMENT_TYPE;

typedef enum tagGameStates
{
    MENU,
    WAIT_MOVE_INPUT,
    PLAYING,
    PAUSED,
    GAMEOVER
}GAMESTATE;

typedef enum INDEXES {RESTART, SOUND, NUM_INDEXES}INDEXES;

extern Frame frames[NUM_INDEXES];
extern int32_t screen_height, screen_width;
extern int32_t score, high_score;
extern int32_t _exitCode;
extern boolean hasClicked, startClicked, restartClicked;
extern RECT audioRect;
extern RECT restartRect;
extern bool debugMode;

//------------------- Defining scales ---------------------------------
//! maybe in da future
#define DPI_SCALE_100 96
#define DPI_SCALE_125 120
#define DPI_SCALE_150 144
#define DPI_SCALE_200 192
