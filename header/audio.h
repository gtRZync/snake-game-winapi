#ifndef AUDIO_H
#define AUDIO_H
#include "sprite.h"
#include "utilis.h"

void muteGame(SPRITE* sprite);
void playGameSound(const GAMESTATE* gameState, SPRITE* sprite, boolean isMuted);


#endif