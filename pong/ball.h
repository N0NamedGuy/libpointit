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
#ifndef BALL_H
#define BALL_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "pad.h"
#include "audio.h"


#include "globals.h"
#include "utils.h"

#define PAUSE_TIME 3000

class Ball {
  float x,y,sx,sy,d,scored;
  unsigned int paused_time;

public:
  Ball();
  int getX();
  int getY();
  float getSpeedX();
  float getSpeedY();  

/**
 * w -> playfield width
 * h -> playfield height
 */
  int render(Pad* p1, Pad* p2);
  void resetPos();
};



#endif /* BALL_H */
