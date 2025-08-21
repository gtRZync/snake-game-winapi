#pragma once

#include "window.h"
#include "menu.h"
#include "render.h"
#include "audio.h"


typedef struct Game
{
    GAMESTATE state;
    Window* window;
    Snake* snake;
    Pellet* pellet;
    HINSTANCE hInstance;
    DOUBLE_BUFFER* buffer;

    void (*createWindow)(struct Game* game, HINSTANCE hInstance, int nShowCmd);
    void (*update)(struct Game* game);
    void (*destroy)(struct Game* game);
    void (*setupDoubleBuffering)(HWND hwnd, DOUBLE_BUFFER* lpDoubleBuffer, int cx, int cy);
    void (*resizeDoubleBuffer)(HWND hwnd, DOUBLE_BUFFER *lpDoubleBuffer, int cx, int cy);
    void (*doubleBufferingCleanup)(DOUBLE_BUFFER* lpDoubleBuffer);
    void (*render)(struct Game* game, int32_t cx, int32_t cy);
    void (*restart)(struct Game* game);

    bool isRunning;
    bool isMuted;
    float deltatime;
}Game;


void setRandomSeed();
Game* InitializeGame();
void FatalAllocError(LPCWSTR what);
DIRECTIONS getReversedDirection(DIRECTIONS direction);
float interpolateScale(float start, float end, float t);
void moveSnake(Snake* snake, int nextX, int nextY, DIRECTIONS dir);
bool isCollisionSnakeBody(Snake* snake);
bool isCollisionSnakePellet(Game* game);
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
void resetGame(Game* game);
void startGame(Game* game);
void manageSound(Game* game);
void debugStat(HDC hdc, Game* game);
void animateButton(HWND hwnd, const RECT* rect, const SPRITE* sprite, Frame* frame);
void processGameOverClick(Game *game, const RECT* rect, bool isRestarting);
void handleButtonClick(Game* game);
void processGameOverButton(HWND hwnd);
void redirectMenu(Game* game);
