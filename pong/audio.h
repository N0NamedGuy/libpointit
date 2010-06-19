#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"



extern int init_audio();
extern void clean_audio();
extern void play_moving();
extern void play_whit();
extern void play_phit();
extern void play_crash();
extern void play_music();
extern void stop_music();
#endif
