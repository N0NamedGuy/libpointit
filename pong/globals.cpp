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
