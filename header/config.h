#pragma once
#define TILE_SIZE 32
#define SHADOW_OFFSET 6
#define BODY_OFFSET 8
#define GRID_WIDTH  26
#define GRID_HEIGHT 19 
#define DEBUG

// For SEGMENT_CURVE_UP row
#define CURVE_UP_TO_RIGHT_COL 2
#define CURVE_UP_TO_LEFT_COL 3
#define CURVE_LEFT_TO_UP_COL 0
#define CURVE_RIGHT_TO_UP_COL 1

// For SEGMENT_CURVE_DOWN row
#define CURVE_DOWN_TO_LEFT_COL 1
#define CURVE_DOWN_TO_RIGHT_COL 0
#define CURVE_LEFT_TO_DOWN_COL 2
#define CURVE_RIGHT_TO_DOWN_COL 3


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "sprite.h"

#define NUM_DIRECTIONS 4
#define HEAD_ALIVE 0
#define HEAD_DEAD 1
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

typedef enum MouseButton {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_BUTTON_COUNT
} MouseButton;

typedef enum KeyCode {
    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    ESCAPE,
    KEY_F3,
    KEY_Z,
    KEY_Q,
    KEY_S,
    KEY_D,
    KEY_COUNT
} KeyCode;
typedef struct Button
{
    bool held;
    bool pressed;
    bool released;
}Button;

typedef struct Input {
    Button keyBoard[KEY_COUNT];
    Button mouse[MOUSE_BUTTON_COUNT];
    bool focused;
}Input;

typedef struct Vector2 {
    int x, y;
}Vector2;

typedef enum INDEXES {SOUND, RESTART, HOME, NUM_INDEXES}INDEXES;

extern Frame frames[NUM_INDEXES];
extern int32_t score, high_score;
extern int32_t _exitCode;
extern RECT audioRect;
extern RECT restartRect;
extern RECT homeRect;
extern bool debugMode;

//------------------- Defining scales ---------------------------------
//! maybe in da future
#define DPI_SCALE_100 96
#define DPI_SCALE_125 120
#define DPI_SCALE_150 144
#define DPI_SCALE_200 192
