// Copyright (C) 2010 David Serrano <david.nonamedguy@gmail.com> 
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
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
