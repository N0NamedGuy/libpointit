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
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "globals.h"

class Utils {
 public:
  static SDL_Surface* load_image( std::string filename );
  static void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
  static void clear_surf(SDL_Surface* surf);
};



#endif
