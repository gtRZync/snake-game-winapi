#include "audio.h"

void muteGame(const SPRITE *sprite)
{
    if(sprite)
        setFrameCol(sprite, &frames[SOUND], 0);
}

void playGameSound(const GAMESTATE *gameState, const SPRITE *sprite, bool isMuted)
{
    if(!isMuted)
        setFrameCol(sprite, &frames[SOUND], 1);
    //! sound logic coming soon
}
