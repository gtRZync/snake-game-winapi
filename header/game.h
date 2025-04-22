#ifndef GAME_H
#define GAME_H
#pragma once

#include "window.h"
#include "menu.h"
#include "render.h"


typedef interface Game
{
    GAMESTATE state;
    Window* window;
    Snake* snake;
    Pellet* pellet;
    HINSTANCE hInstance;
    DOUBLE_BUFFER* buffer;
    DIRECTIONS snake_direction;

    void (*createWindow)(interface Game* game, HINSTANCE hInstance, int nShowCmd);
    void (*update)(interface Game* game);
    void (*destroy)(interface Game* game);
    void (*setupDoubleBuffering)(HWND hwnd, DOUBLE_BUFFER* lpDoubleBuffer, int cx, int cy);
    void (*resizeDoubleBuffer)(HWND hwnd, DOUBLE_BUFFER *lpDoubleBuffer, int cx, int cy);
    void (*doubleBufferingCleanup)(DOUBLE_BUFFER* lpDoubleBuffer);
    void (*render)(interface Game* game, int32_t cx, int32_t cy);

    boolean isRunning;
    float deltatime;
}Game;


void setRandomSeed();
Game* InitializeGame();
void FatalAllocError(LPCWSTR what);
DIRECTIONS getReversedDirection(DIRECTIONS direction);
float interpolateScale(float start, float end, float t);
void moveSnake(Snake* snake, int nextX, int nextY);
boolean is_collision_snake_body(Snake* snake);
boolean isCollisionSnakePellet(Snake* snake, Pellet* pellet);
void eatPellet(Snake* snake, Pellet* pellet);
void updateSnakePosition(DIRECTIONS current_direction, Snake* snake);
void changeDirection(DIRECTIONS* current_direction, Snake* snake);
void stopAtWall(DIRECTIONS direction, Snake* snake);
void animatePellet(Pellet* pellet, boolean snakeIsMoving);
void UpdateGame(Game* game);
void renderGame(Game* game, int32_t cx, int32_t cy);
void GameLoop(Game* game);
void GameDestroy(Game* game);
void prepareGame(Game *game);
void startGame(Game* game);
#endif