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
#include "globals.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

const int PAD_WIDTH = 16;
const int PAD_HEIGHT = 80;

PointIt pntIt;



int oldticks=0;
int fps=30;
float time_delta = (1.0/(float(fps))) * 1000.0;

bool paused=false;
bool fullscreen=false;
bool quit = false;
bool using_pointit = true;

SDL_Surface *pad1 = NULL;
SDL_Surface *pad2 = NULL;
SDL_Surface *ball = NULL;
SDL_Surface *pointer = NULL;

SDL_Surface *screen = NULL;

SDL_Event event;

TTF_Font *font = NULL;
SDL_Color textColor = { 255, 255, 255 };
SDL_Surface *scoreboard = NULL;
