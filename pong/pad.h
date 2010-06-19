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
