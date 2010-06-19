#include "pad.h"

Pad::Pad(int player) {
  if (player == 1) {
    x = 0;
  } else if (player == 2) {
    x = SCREEN_WIDTH - PAD_WIDTH;
  } else {
    x = (SCREEN_WIDTH - PAD_WIDTH) / 2;
  }


  y = 0;
  ph = PAD_HEIGHT;
  sy = 0;
  sc = 0;
}


int Pad::getX() { return x; }
int Pad::getY() { return y; }

void Pad::moveTo(int desty) {
  if (desty > y) {
    sy+=1;
  } else if (desty < y) {
    sy-=1;
  }

  //  if (sy > 10) sy = 10; // Speed limiter

  if (((int)y) == desty) {
    sy = 0;

  } else if (sy > 0 && y > desty) {
    sy = 0;
  } else if (sy < 0 && y < desty) {
    sy = 0;
  } 

  y += sy;

  if (y < ph / 2) {
    y = ph/2;
    sy = 0;
  }

  if (y > SCREEN_HEIGHT - (ph/2)) {
    y = SCREEN_HEIGHT - (ph/2);
    sy = 0;
  } 
}
