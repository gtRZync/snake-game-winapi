#pragma once

#include "config.h"

#define SCALE_RECT(cx, cy, scale_x, scale_y) (RECT){(cx - scale_x) * TILE_SIZE, (cy - scale_y) * TILE_SIZE, (cx + scale_x) * TILE_SIZE, (cy + scale_y) * TILE_SIZE}
#define SETUP_RECT(cx, cy, scaled_size) (RECT){(cx) * TILE_SIZE, (cy) * TILE_SIZE, (cx + scaled_size) * TILE_SIZE, (cy + scaled_size) * TILE_SIZE}
#define SAFE_FREE(p) do {if((p) != NULL){ free(p); p = NULL;}} while(0)

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
#define golden_brown     RGB(194,144,48);
#define head_sprite_COLOR RGB(85, 107, 47)    // Olive
#define SNAKE_BODY_COLOR RGB(139, 69, 19)    // SaddleBrown
#define champagneSilver  RGB(230, 232, 234)
#define MENU_BG          RGB(36,94,99)
#define turquoise        RGB(0, 162, 232) 


boolean isPointInRect(const RECT* rect, int32_t clickedX, int32_t clickedY);
uint8_t directionToIndex(DIRECTIONS dir);
const char* directionToString(DIRECTIONS dir);

