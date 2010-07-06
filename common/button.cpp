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
#include "button.h"

int max3(int a, int b, int c) {
  if (a > b && a > c)
    return a;
  else if (b > a && b > c)
    return b;
  else
    return c;
}
/*
SDL_Surface*
load_image( std::string filename ) {
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;

  loadedImage = IMG_Load( filename.c_str() );

  if (loadedImage != NULL) {
    optimizedImage = SDL_DisplayFormat( loadedImage );
    SDL_FreeSurface(loadedImage);


    Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0x0, 0xFF ); 
    SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey ); 
  }

  return optimizedImage;
  }*/

void Button::build_button() {
  SDL_Rect offset = {0,0,surf_button->w,surf_button->h};
  SDL_FillRect(surf_button, &offset,
	       SDL_MapRGB(surf_button->format, 0xFF, 0x0, 0xFF));
  
  
  // Draw left side
  offset.x = 0;
  offset.y = (button.h - surf_side_left->h) / 2;
  SDL_BlitSurface( surf_side_left, NULL, surf_button, &offset);

  // Draw center side
  offset.x = (button.w - surf_center->w) / 2;
  offset.y = (button.h - surf_center->h) / 2;
  SDL_BlitSurface( surf_center, NULL, surf_button, &offset);

  // Draw right side
  offset.x = button.w - surf_side_right->w;
  offset.y = (button.h - surf_side_right->h) / 2;
  SDL_BlitSurface( surf_side_right, NULL, surf_button, &offset);

  // Draw text
  offset.x = (button.w - surf_caption->w) / 2;
  offset.y = (button.h - surf_caption->h) / 2;
  SDL_BlitSurface( surf_caption, NULL, surf_button, &offset);
  
  // Draw left hotspot
  if (!left_hit)
    SDL_BlitSurface( surf_hotspot, NULL, surf_button, &hotspot_lf);
  else
    SDL_BlitSurface( surf_hotspot_hit , NULL, surf_button, &hotspot_lf);

  // Draw right hotspot
  if (!right_hit)
    SDL_BlitSurface( surf_hotspot, NULL, surf_button, &hotspot_rt);
  else
    SDL_BlitSurface( surf_hotspot_hit , NULL, surf_button, &hotspot_rt);

}

Button::Button(std::string caption, int ix, int iy) {
  TTF_Font* font;
  font = TTF_OpenFont( "FreeSans.ttf", 20 );
  SDL_Color textColor = { 255, 255, 255 };


  surf_side_left = NULL;
  surf_side_right = NULL;
  surf_center = NULL;
  surf_hotspot = NULL;
  surf_hotspot_hit = NULL;
  surf_caption = NULL;

  surf_side_left = load_image("gfx/btn_side_lf.png");
  surf_side_right = load_image("gfx/btn_side_rt.png");
  surf_center = load_image("gfx/btn_center.png");
  surf_hotspot = load_image("gfx/btn_hotspot.png");
  surf_hotspot_hit = load_image("gfx/btn_hotspot_hit.png");
  surf_caption = TTF_RenderText_Solid(font, caption.c_str(), textColor );

  if (surf_side_left == NULL ||surf_side_right == NULL)  {
    printf("Sides not loaded...\n");
    exit(1);
  }

  if (surf_center == NULL)  {
    printf("Center not loaded...\n");
    exit(1);
  }

  if (surf_caption == NULL)  {
    printf("Text not loaded...\n");
    exit(1);
  }

  button.x = ix; button.y = iy;
  button.w = surf_side_left->w + surf_center->w + surf_side_right->w;
  button.h = max3(surf_side_left->h, surf_center->h, surf_side_right->h);

  hotspot_lf.w = surf_hotspot->w; hotspot_lf.h = surf_hotspot->h;
  hotspot_rt.w = surf_hotspot->w; hotspot_rt.h = surf_hotspot->h;
  hotspot_lf.x = (surf_side_left->w - hotspot_lf.w) / 2;
  hotspot_lf.y = (surf_side_left->h - hotspot_lf.h) / 2;
  hotspot_rt.x = ((surf_side_right->w - hotspot_rt.w) / 2) + 
    (button.w - surf_side_right->w);
  hotspot_rt.y = ((surf_side_right->h - hotspot_rt.h) / 2);

  left_hit = false;
  right_hit = false;
  hover = false;

  surf_button = SDL_CreateRGBSurface(SDL_SWSURFACE, 
				     button.w,button.h,SCREEN_BPP,
				     0,0,0,0);

  SDL_SetColorKey( surf_button, SDL_SRCCOLORKEY, 
		   SDL_MapRGB( surf_button->format, 0xFF, 0x0, 0xFF )); 


  build_button();

}

void Button::render(SDL_Surface* dest) {
  SDL_BlitSurface( surf_button, NULL, dest, &button);
}

void Button::render_pos(int x, int y) {
  hover = (x > button.x && x < button.x + button.w &&
	   y > button.y && y < button.y + button.h);

  if (!hover) {
    if (left_hit || right_hit) {
      left_hit = false;
      right_hit = false;
      build_button();
    }
    
    return;
  }

  if (x > button.x + hotspot_lf.x && 
      x < button.x + hotspot_lf.x + hotspot_lf.w &&
      y > button.y + hotspot_lf.y && 
      y < button.y + hotspot_lf.y + hotspot_lf.h) {
    left_hit = true;
  }

  if (x > button.x + hotspot_rt.x && 
      x < button.x + hotspot_rt.x + hotspot_rt.w &&
      y > button.y + hotspot_rt.y && 
      y < button.y + hotspot_rt.y + hotspot_rt.h) {
    right_hit = true;
  }

  build_button();
}

bool Button::is_clicked() {
  
  if (left_hit && right_hit) {
    left_hit = false;
    right_hit = false;
    build_button();
    return true;
  }

  return false;
}
