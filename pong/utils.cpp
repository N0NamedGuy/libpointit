#include "utils.h"

SDL_Surface*
Utils::load_image( std::string filename ) {
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
}

void 
Utils::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
  SDL_Rect offset;
  
  offset.x = x;
  offset.y = y;

  SDL_BlitSurface( source, NULL, destination, &offset);
}

void
Utils::clear_surf(SDL_Surface* surf) {
  SDL_Rect rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
  SDL_FillRect(surf, &rect, 0);
}
