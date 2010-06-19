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
