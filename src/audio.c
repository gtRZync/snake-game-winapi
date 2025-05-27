#include "audio.h"

void muteGame(SPRITE *sprite)
{
    if(sprite)
        setSpriteFrame(sprite, 0);
    }
    
    void playGameSound(const GAMESTATE *gameState, SPRITE *sprite, boolean isMuted)
    {
        if(!isMuted)
        setSpriteFrame(sprite, 1);
        //! sound logic coming soon
    }
