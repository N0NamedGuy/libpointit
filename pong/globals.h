#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include "pointit.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

using namespace std;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const int PAD_WIDTH;
extern const int PAD_HEIGHT;

extern PointIt pntIt;
/*
extern Ball b;
extern Pad p1;
extern Pad p2;
extern SDL_Rect pointing;
*/
extern int oldticks;
extern int fps;
extern float time_delta;

extern bool paused;
extern bool fullscreen;
extern bool quit;
extern bool using_pointit;

extern SDL_Surface *pad1;
extern SDL_Surface *pad2;
extern SDL_Surface *ball;
extern SDL_Surface *pointer;

extern SDL_Surface *screen;

extern SDL_Event event;

extern TTF_Font *font;
extern SDL_Color textColor;
extern SDL_Surface *scoreboard;


#endif
