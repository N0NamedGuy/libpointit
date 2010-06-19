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
#ifndef OBJS_H
#define OBJS_H

#include "SDL/SDL.h"
#include "ball.h"
#include "pad.h"
#include "../common/particle.h"

extern Ball b;
extern Pad p1;
extern Pad p2;
extern SDL_Rect pointing;
extern ParticleEngine pe;

#endif
