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
#ifndef PAD_H
#define PAD_H

#include "globals.h"
#include "utils.h"


class Pad {
  float x,y;
  float sy;
  int ph;
  int sc; // Score
public:
  Pad(int play);
  int getX();
  int getY();
  void moveTo(int desty);
  void score() {sc++;}
  void reset_score() {sc = 0;}
  int get_score() {return sc;}
};


#endif
