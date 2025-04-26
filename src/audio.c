#include "audio.h"

void muteGame(SPRITE *sprite)
{
    if(sprite)
        sprite->currentFrame = 0;
        //sound logic coming soon
}

void playGameSound(const GAMESTATE *gameState, SPRITE *sprite, boolean isMuted)
{
    if(!isMuted)
        sprite->currentFrame = 1;
}
