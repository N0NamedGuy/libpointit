#include <stdio.h>
#include <signal.h>
#include <SDL/SDL.h>

#include "../pointit/pointit.h"

SDL_Surface* screen = NULL;
SDL_Surface* cam = NULL;

void sigtrap() {
    pointit_destroy();
    SDL_Quit();
    exit(0);
}

int main(void) {
    int oldTicks, newTicks;
    int frames = 0;

    SDL_Init( SDL_INIT_EVERYTHING ); 
    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
    
    if (pointit_init() != 0) {
        printf("An error ocurred while starting pointit\n");
        return 0;
    }

    signal(SIGINT, sigtrap);
    signal(SIGTERM, sigtrap);
   
    oldTicks = SDL_GetTicks(); 
    newTicks = SDL_GetTicks();
    for (;;) {
        newTicks = SDL_GetTicks();
        pointit_detect();
//        cam = pointit_sdlcam_surf();
//        SDL_BlitSurface( cam, NULL, screen, NULL );
//        SDL_Flip( screen );

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
