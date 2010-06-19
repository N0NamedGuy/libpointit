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
