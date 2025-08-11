#include "utilis.h"

boolean isPointInRect(const RECT* rect, int32_t clickedX, int32_t clickedY)
{
    return (clickedX > rect->left && clickedX < rect->right && clickedY > rect->top && clickedY < rect->bottom);
}

uint8_t directionToIndex(DIRECTIONS dir) {
    switch(dir)
    {
        case UP: return (uint8_t)UP;
        case RIGHT: return (uint8_t)RIGHT;
        case LEFT: return (uint8_t)LEFT;
        case DOWN: return (uint8_t)DOWN;
        default: return -1;
    }
}

const char* directionToString(DIRECTIONS dir) {
    switch(dir)
    {
        case UP: return "UP";
        case RIGHT: return "RIGHT";
        case LEFT: return "LEFT";
        case DOWN: return "DOWN";
        default: return "None";
    }
}