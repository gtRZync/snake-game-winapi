#pragma once
#include "double_buffer.h"
#include "snake.h"
#include "pellet.h"
#include "font.h"
#include "menu.h"

extern SPRITE title;
extern SPRITE keys;
extern SPRITE sound;
extern SPRITE trophy;
extern SPRITE restart_sprite;
extern SPRITE home_sprite;

void renderMenu(HWND hwnd, HDC hdc, const GAMESTATE* gameState);
void renderGrid(HDC hdc);
void renderHeader(HDC hdc, SPRITE* sprite);
void drawSnake(HDC hdc, Snake* snake);
void drawPellet(HDC hdc, Pellet* pellet);
void renderToBackBuffer(HWND hwnd, GAMESTATE* gameState, HDC back_buffer, Pellet* pellet, Snake* snake);
void copyToFrontBuffer(HDC back_buffer, HDC front_buffer, int32_t cx, int32_t cy);
void renderOnGameOver(HDC hdc, Snake* snake, Pellet* pellet, GAMESTATE* gameState);
void renderSprite(HDC hdc, SPRITE* sprite, uint32_t cx, uint32_t cy, float scale, const Frame frame, UINT transparentColorKey);
void setupRectAndRenderSprite(HDC hdc, SPRITE *sprite, uint32_t posX, uint32_t posY, float scale, const Frame frame, UINT transparentColorKey, RECT* outRect, int8_t inflateX, int8_t inflateY);
void renderTransparentLayer(HDC hdc, BOOL is_rounded, RECT* rect);
void renderControlKeysOverlay(HDC hdc, SPRITE* sprite, GAMESTATE* gameState);
int32_t renderTitle(HDC hdc, SPRITE *sprite, float scaleFactor);
void renderSoundIcon(HDC hdc, SPRITE *sprite, float scaleFactor);
Frame getCornerFrame(DIRECTIONS fromDir, DIRECTIONS toDir);
void test(HDC hdc, const RECT* lprect);
void renderGameOverText(HDC hdc, RECT* rect);
void renderTrophies(HDC hdc, int center_x, int center_y, float scale, Pellet* pellet) ;
void renderButton(HDC hdc, SPRITE* sprite, int posX, int posY, float scale, Frame frame, COLORREF color, RECT* outRect, int inflateX, int inflateY);
void renderScoreValue(HDC hdc, int value, int posX, int posY, int spriteW);
void renderButtonsText(HDC hdc, int center_x, int center_y, int homeW, int restartW, int font_size);
void renderScores(HDC hdc, int center_x, int center_y, int trophyW, int pelletW);
