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
#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include "../pointit/pointit.h"
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
