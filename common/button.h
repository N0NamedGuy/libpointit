/* nGp Application Suite - Presentation Sample
 * Copyright (C) 2009 David Serrano
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "utils.h"


#define SCREEN_BPP 32
/**
   It is assumed that you have the SDL fonts already initialized!
 */

class Button {
private:
  SDL_Surface* surf_side_left;
  SDL_Surface* surf_side_right;
  SDL_Surface* surf_center;
  SDL_Surface* surf_hotspot;
  SDL_Surface* surf_hotspot_hit;
  SDL_Surface* surf_caption;

  bool left_hit;
  bool right_hit;
  bool hover;

  SDL_Rect button;

  SDL_Rect hotspot_lf;
  SDL_Rect hotspot_rt;

  SDL_Surface* surf_button;


  void build_button();
public:
  Button(std::string caption, int ix, int iy);

  // Informs the position of the pointer
  void render_pos(int x, int y);
  bool is_clicked();
  bool is_hover();
  void render(SDL_Surface* dest);
};

#endif
