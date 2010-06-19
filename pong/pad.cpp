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
