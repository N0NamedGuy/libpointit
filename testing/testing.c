#include <stdio.h>
#include <signal.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "../pointit/pointit.h"

SDL_Surface* screen = NULL;
SDL_Surface* cam = NULL;

SDL_Surface* target1 = NULL;
SDL_Surface* target2 = NULL;

struct pointit_context pointit1;
struct pointit_context pointit2;

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
    target1 = load_image("./pointer1.png");
    target2 = load_image("./pointer2.png");
    
    if (pointit_init() != 0) {
        printf("An error ocurred while starting pointit\n");
        return 0;
    }
    pointit1 = pointit_get_green_context();
    pointit2 = pointit_get_blue_context();
    //pointit = pointit_get_orange_context();
    //pointit = pointit_get_blue_context();
    //pointit = pointit_get_pink_context();
    //pointit = pointit_get_yellow_context();

    signal(SIGINT, sigtrap);
    signal(SIGTERM, sigtrap);
   
    oldTicks = SDL_GetTicks(); 
    newTicks = SDL_GetTicks();
    for (;;) {
        newTicks = SDL_GetTicks();
        pointit_detect(&pointit1);
        pointit_detect(&pointit2);

//        printf("%d, %d\n", pointit_get_x(), pointit_get_y());
        SDL_FillRect( SDL_GetVideoSurface(), NULL, 0 );
        
        cam = pointit_sdlcam_surf();
        SDL_BlitSurface( cam, NULL, screen, NULL );
        
        lineRGBA(screen, pointit1.l , 0, pointit1.l, screen->h, 0xff, 0x00, 0xff, 0xff);
        lineRGBA(screen, pointit1.r, 0, pointit1.r, screen->h, 0xff, 0x00, 0xff, 0xff);
        lineRGBA(screen, 0, pointit1.t, screen->w, pointit1.t, 0xff, 0x00, 0xff, 0xff);
        lineRGBA(screen, 0, pointit1.b, screen->w, pointit1.b, 0xff, 0x00, 0xff, 0xff);
/*
        lineRGBA(screen, pointit2.l , 0, pointit2.l, screen->h, 0xff, 0x00, 0x00, 0xff);
        lineRGBA(screen, pointit2.r, 0, pointit2.r, screen->h, 0xff, 0x00, 0x00, 0xff);
        lineRGBA(screen, 0, pointit2.t, screen->w, pointit2.t, 0xff, 0x00, 0x00, 0xff);
        lineRGBA(screen, 0, pointit2.b, screen->w, pointit2.b, 0xff, 0x00, 0x00, 0xff);
*/        
        apply_surface(
            ((pointit1.x * 640) / pointit1.w)  - (target1->w / 2),
            ((pointit1.y * 480) / pointit1.h) - (target1->h / 2),
            target1, screen);
        
/*        apply_surface(
            ((pointit2.x * 640) / pointit2.w)  - (target2->w / 2),
            ((pointit2.y * 480) / pointit2.h) - (target2->h / 2),
            target2, screen);*/
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
