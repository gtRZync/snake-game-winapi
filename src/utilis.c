#include "utilis.h"

boolean isPointInRect(const RECT* rect, int32_t clickedX, int32_t clickedY)
{
    return (clickedX > rect->left && clickedX < rect->right && clickedY > rect->top && clickedY < rect->bottom);
}