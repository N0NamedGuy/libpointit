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
#include "audio.h"

Mix_Music *music = NULL; 

Mix_Chunk *ball_moving;
Mix_Chunk *wall_hit;
Mix_Chunk *pad_hit;
Mix_Chunk *ball_crash;
  
int err;


int
init_audio() {
  ball_moving = NULL;
  wall_hit = NULL;
  pad_hit = NULL;
  ball_crash = NULL;
  err = 0;

  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) { 
    return 1; 
  } 

  ball_moving = Mix_LoadWAV( "sfx/tshh.wav" );
  wall_hit = Mix_LoadWAV( "sfx/ting.wav" );
  pad_hit = Mix_LoadWAV( "sfx/pong.wav" );
  ball_crash = Mix_LoadWAV( "sfx/crash.wav" );
  
  music = Mix_LoadMUS( "music/glow-in-the-dark.mp3" );

  if( ( ball_moving == NULL ) || ( wall_hit == NULL ) || 
      ( pad_hit == NULL ) || ( ball_crash == NULL )
      || ( music == NULL) ) { 
    return 2;  
  }

  return 0;
}

void
clean_audio() {
  Mix_FreeChunk(ball_moving);
  Mix_FreeChunk(wall_hit);
  Mix_FreeChunk(pad_hit);
  Mix_FreeChunk(ball_crash);

  Mix_FreeMusic( music );

  Mix_CloseAudio();
}

void
play_moving() {
  Mix_PlayChannel( -1, ball_moving, 1 );
}

void
play_whit() {
  Mix_PlayChannel( -1, wall_hit, 0 );
}

void
play_phit() {
  Mix_PlayChannel( -1, pad_hit, 0 );
}

void
play_crash() {
  Mix_PlayChannel( -1, ball_crash, 0 );
}

void
play_music() {
  if (Mix_PlayingMusic() == 0) {
    Mix_PlayMusic( music, -1 );
  }
}

void
stop_music() {
  Mix_HaltMusic( );
}
