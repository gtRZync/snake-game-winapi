#ifndef GAME_H
#define GAME_H
#pragma once

#include "render.h"
#include "window.h"


struct Window;

typedef enum tagGameStates
{
    START,
    PLAYING,
    PAUSED,
    GAMEOVER
}GAMESTATE;

typedef interface Game
{
    GAMESTATE state;
    Window* window;
    Snake* snake;
    Pellet* pellet;
    HINSTANCE hInstance;
    void (*createWindow)(interface Game* game, HINSTANCE hInstance, int nShowCmd);
    int (*update)(interface Game* game);
    void (*destroy)(interface Game* game);
    BOOLEAN isRunning;
    float deltatime;
    DIRECTIONS snake_direction;
}Game;


void setRandomSeed();
DIRECTIONS getReversedDirection(DIRECTIONS direction);
void moveSnake(Snake* snake, int nextX, int nextY);
boolean is_collision_snake_body(Snake* snake);
boolean isCollisionSnakePellet(Snake* snake, Pellet* pellet);
void eatPellet(Snake* snake, Pellet* pellet);
void updateSnakePosition(DIRECTIONS current_direction, Snake* snake);
void changeDirection(DIRECTIONS* current_direction, Snake* snake, WPARAM wParam);
void animatePellet(Pellet* pellet);
void stopAtWall(DIRECTIONS direction, Snake* snake);
float interpolateScale(float start, float end, float t);
void UpdateGame(Game* game);
Game* InitializeGame();
void GameDestroy(Game* game);
void FatalAllocError(LPCWSTR what);
#endif