#include <stdio.h>
#include <signal.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "../pointit/pointit.h"

SDL_Surface* screen = NULL;
SDL_Surface* cam = NULL;
SDL_Surface* target = NULL;

struct pointit_context pointit;

void sigtrap() {
    pointit_destroy();
    SDL_Quit();
    exit(0);
}

static SDL_Surface* load_image(char* filename ) {
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;

  loadedImage = IMG_Load( filename );

  if (loadedImage != NULL) {
    optimizedImage = SDL_DisplayFormat( loadedImage );
    SDL_FreeSurface(loadedImage);


    Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0x0, 0xFF );
    SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
  }

  return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
  SDL_Rect offset;

  offset.x = x;
  offset.y = y;

  SDL_BlitSurface( source, NULL, destination, &offset);
}

int main(void) {
    int oldTicks, newTicks;
    int frames = 0;

    SDL_Init( SDL_INIT_EVERYTHING ); 
    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
    target = load_image("./target.png");
    
    if (pointit_init() != 0) {
        printf("An error ocurred while starting pointit\n");
        return 0;
    }
    pointit = pointit_get_green_context();

    signal(SIGINT, sigtrap);
    signal(SIGTERM, sigtrap);
   
    oldTicks = SDL_GetTicks(); 
    newTicks = SDL_GetTicks();
    for (;;) {
        newTicks = SDL_GetTicks();
        pointit_detect(&pointit);

//        printf("%d, %d\n", pointit_get_x(), pointit_get_y());
        SDL_FillRect( SDL_GetVideoSurface(), NULL, 0 );
        
        cam = pointit_sdlcam_surf();
        SDL_BlitSurface( cam, NULL, screen, NULL );
        
        lineRGBA(screen, pointit.l , 0, pointit.l, screen->h, 0xff, 0x00, 0xff, 0xff);
        lineRGBA(screen, pointit.r, 0, pointit.r, screen->h, 0xff, 0x00, 0xff, 0xff);
        lineRGBA(screen, 0, pointit.t, screen->w, pointit.t, 0xff, 0x00, 0xff, 0xff);
        lineRGBA(screen, 0, pointit.b, screen->w, pointit.b, 0xff, 0x00, 0xff, 0xff);

        apply_surface(
            ((pointit.x * 640) / pointit.w)  - (target->w / 2),
            ((pointit.y * 480) / pointit.h) - (target->h / 2),
            target, screen);
        SDL_Flip( screen );

        frames++;

        if (newTicks - oldTicks > 1000) {
            printf("FPS %d\n", frames);   
            frames = 0;
            oldTicks = newTicks;
        }
    }

    pointit_destroy();
    SDL_Quit();
    return 0;
}
