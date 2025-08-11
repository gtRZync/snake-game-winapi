#pragma once

#include "window.h"
#include "menu.h"
#include "render.h"
#include "audio.h"


typedef interface Game
{
    GAMESTATE state;
    Window* window;
    Snake* snake;
    Pellet* pellet;
    HINSTANCE hInstance;
    DOUBLE_BUFFER* buffer;

    void (*createWindow)(interface Game* game, HINSTANCE hInstance, int nShowCmd);
    void (*update)(interface Game* game);
    void (*destroy)(interface Game* game);
    void (*setupDoubleBuffering)(HWND hwnd, DOUBLE_BUFFER* lpDoubleBuffer, int cx, int cy);
    void (*resizeDoubleBuffer)(HWND hwnd, DOUBLE_BUFFER *lpDoubleBuffer, int cx, int cy);
    void (*doubleBufferingCleanup)(DOUBLE_BUFFER* lpDoubleBuffer);
    void (*render)(interface Game* game, int32_t cx, int32_t cy);
    void (*restart)(interface Game* game);

    boolean isRunning;
    boolean isMuted;
    float deltatime;
}Game;


void setRandomSeed();
Game* InitializeGame();
void FatalAllocError(LPCWSTR what);
DIRECTIONS getReversedDirection(DIRECTIONS direction);
float interpolateScale(float start, float end, float t);
void moveSnake(Snake* snake, int nextX, int nextY, DIRECTIONS dir);
boolean isCollisionSnakeBody(Snake* snake);
boolean isCollisionSnakePellet(Game* game);
void checkSelfCollision(Game* game);
void checkWallCollision(Game* game);
void checkCollisions(Game* game);
void eatPellet(Game* game);
void updateSnakePosition(Game* game);
void changeDirection(DIRECTIONS *currentDirection, SPRITE* sprite);
void stopAtWall(Game* game);
void stopAtSelf(Game* game);
void animatePellet(Game* game);
void UpdateGame(Game* game);
void renderGame(Game* game, int32_t cx, int32_t cy);
void GameLoop(Game* game);
void GameDestroy(Game* game);
void prepareGame(Game *game);
void startGame(Game* game);
void handleReset(Game* game);
void manageSound(Game* game);
