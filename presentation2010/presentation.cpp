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
#include <iostream>
#include <sstream>
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"

#include "../pointit/pointit.h"
#include "../common/thread.h"
#include "../common/particle.h"
#include "../common/button.h"
#include "../common/utils.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32
#define MAX_SLIDES 8

using namespace std;

struct pointit_context pntIt;

SDL_Rect lastpos = {-1,-1,1,1};
SDL_Rect pointing = {0,0,1,1};


int oldticks=0;
int fps=30;
float time_delta = (1.0/(float(fps))) * 1000.0;
bool do_particle=true;
int diffticks=0;

bool paused=true;
bool fullscreen=false;
bool quit = false;
bool using_pointit=true;

SDL_Surface *pointer = NULL;
SDL_Surface *drawing = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *puff = NULL;
SDL_Surface *curslide = NULL;
int slidenum = 1;

SDL_Event event;

ParticleEngine pe;
TTF_Font* font;

Button* btn_quit;
Button* btn_erase;
Button* btn_fs;

Button* btn_prev;
Button* btn_next;


void
load_slide(int slide) {
  std::ostringstream filename;
  filename << "slides/slide" << slide << ".png";

  curslide = load_image(filename.str());
  apply_surface(0,0,curslide,drawing);
}

void
next_slide() {
  if (slidenum < MAX_SLIDES) {
    slidenum++;
  }
  load_slide(slidenum);
}

void
prev_slide() {
  if (slidenum > 0) {
    slidenum--;
  }

  load_slide(slidenum);
}

bool
init_font() {
  if( TTF_Init() == -1 ) {
    return false;
  }

  font = TTF_OpenFont( "FreeSans.ttf", 28 );

  if (font == NULL) {
    return false;
  }

  return true;
}

bool
load_images() {

  pointer = load_image("gfx/pointer.png");
  puff = load_image("puff.png");
  drawing = SDL_CreateRGBSurface(SDL_SWSURFACE, 
				 screen->w,screen->h,SCREEN_BPP,
				 0,0,0,0);

  return true;
}

bool
init_logic() {

  SDL_ShowCursor(0);
  SDL_WarpMouse(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  load_slide(slidenum);

  return true;
}


bool
init_pointit() {
  quit = false;
  if (pointit_init() == 0) {
    using_pointit = true;
    pntIt = pointit_get_green_context();
    init_thread(&pntIt, &using_pointit);
  } else {
    using_pointit = false;
  }

  return using_pointit;
}

bool
init_gui() {
  btn_quit = new Button("Quit",SCREEN_WIDTH - 200, SCREEN_HEIGHT - 100);
  btn_erase = new Button("Erase",SCREEN_WIDTH - 400, SCREEN_HEIGHT - 100);
  btn_fs = new Button("Full Screen",SCREEN_WIDTH - 600, SCREEN_HEIGHT - 100);

  btn_next = new Button("NEXT",SCREEN_WIDTH - 800, SCREEN_HEIGHT - 100);
  btn_prev = new Button("PREV",SCREEN_WIDTH - 1000, SCREEN_HEIGHT - 100);
  

  return true;
}

void toggle_fullscreen() {
  if (fullscreen) {

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			       SDL_SWSURFACE /*| SDL_FULLSCREEN*/ );
  } else {
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			       SDL_SWSURFACE | SDL_FULLSCREEN);
  }
 
  if (screen == NULL) {
    cerr << "Screen not initialized" << endl;
    //return false;
  }
 
  SDL_WM_SetCaption( "Draw", NULL );
  
  fullscreen = !fullscreen;
}

bool
init() {
  if (SDL_Init( SDL_INIT_EVERYTHING ) == -1) {
    cerr << "SDL Not initialized" << endl;
    return false;
  }

  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			     SDL_HWSURFACE /*| SDL_FULLSCREEN*/ );

  if (screen == NULL) {
    cerr << "Screen not initialized" << endl;
    return false;
  }
 
  SDL_WM_SetCaption( "Draw", NULL );


  if (!load_images()) {
    cerr << "Images not loaded" << endl;
    return false;
  }

  if (!init_font()) {
    cerr << "TrueType fonts not loaded" << endl;
    return false;
  }

  if (!init_gui()) {
    cerr << "No GUI";
    return false;
  }

  if (!init_logic()) {
    cerr << "Wth? No logic??" << endl;
    return false;
  }

  if (!init_pointit()) {
    cerr << "No point it..." << endl;
    //    return false;
  }

  toggle_fullscreen();

  return true;
}

void cleanup() {
  SDL_FreeSurface(pointer);
  SDL_FreeSurface(curslide);
  SDL_FreeSurface(drawing);
  SDL_FreeSurface(puff);
  thread_wait();
  SDL_Quit();
}

void clear_surf(SDL_Surface* img) {
  //SDL_Rect rect = {0,0,img->w,img->h};
  apply_surface(0,0,curslide,img);
}

void toggle_pointit() {
  using_pointit = !using_pointit;
}



void clear_draw() {
  clear_surf(drawing);
  lastpos.x = -1; lastpos.y = -1;
}

void do_render() {
  //  clear_surf(screen);
  apply_surface(0,0,curslide,screen);

  if (!paused) {
    if (lastpos.x != -1 && lastpos.y != -1) {
      lineRGBA(drawing, lastpos.x, lastpos.y,
	       pointing.x, pointing.y,
	       0xFF, 0xFF, 0x00, 0xFF);

      //      apply_surface(pointing.x - 16, pointing.y - 16, pointer, drawing);
    }

    if (lastpos.x != pointing.x && 
	lastpos.y != pointing.y) {
      pe.do_particles(pointing.x, pointing.y, 
		      (-lastpos.x + pointing.x) / 500,
		      (-lastpos.y + pointing.y) / 500, 20, screen);
    }

    lastpos = pointing;
  } else {
    lastpos.x = -1;
    lastpos.y = -1;
  }

  apply_surface(0,0,drawing,screen);
  
  if (do_particle) {
    pe.render(puff);
    do_particle = false;
  }
  
  if (paused) {
    btn_fs->render_pos(pointing.x, pointing.y);
    btn_fs->render(screen);

    btn_erase->render_pos(pointing.x, pointing.y);
    btn_erase->render(screen);

    btn_quit->render_pos(pointing.x, pointing.y);
    btn_quit->render(screen);
  }


  btn_prev->render_pos(pointing.x, pointing.y);
  btn_prev->render(screen);

  btn_next->render_pos(pointing.x, pointing.y);
  btn_next->render(screen);

  apply_surface(pointing.x - 16, pointing.y - 16, pointer, screen);
  

  if( SDL_Flip( screen ) == -1 ) { exit(1); }
}

void do_logic() {
  if (btn_fs->is_clicked()) {
    toggle_fullscreen();
  }

  if (btn_erase->is_clicked()) {
    clear_draw();
  }

  if (btn_quit->is_clicked()) {
    quit = true;
  }

  if (btn_prev->is_clicked()) {
    prev_slide();
  }

  if (btn_next->is_clicked()) {
    next_slide();
  }
}

void do_pointit() {
  if (using_pointit) {
    //pntIt.do_detect();
    pointing.x = (pntIt.x * SCREEN_WIDTH) / pntIt.w;
    pointing.y = (pntIt.y * SCREEN_HEIGHT) / pntIt.h;
  }
}

void do_input() {
  while ( SDL_PollEvent( &event ) ) {
    if (event.type == SDL_QUIT ) {
      quit = true;
    }
      
    if (event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
/*      case SDLK_UP: pointing.y-=10; break;
      case SDLK_DOWN: pointing.y+=10; break;
      case SDLK_LEFT: pointing.x-=10; break;
      case SDLK_RIGHT: pointing.x+=10; break;*/
      case SDLK_f: toggle_fullscreen(); break;
      case SDLK_l: toggle_pointit(); break;
      //case SDLK_c: pntIt.toggle_cam(); break;
      case SDLK_SPACE: paused = !paused; break;
      //case SDLK_d: pntIt.toggle_lines(); break;
      case SDLK_e: clear_draw(); break;

      case SDLK_s:
	SDL_SaveBMP(drawing, "cur_drawing.bmp");
	break;

      case SDLK_ESCAPE: quit = true; break;
      default: break;
      }
    }
      
    if (!using_pointit && event.type == SDL_MOUSEMOTION) {
      pointing.x = event.motion.x;
      pointing.y = event.motion.y;
    }
      
  }
}

int main() {

  if (!init()) {
    cerr << "Errors happened while initializing the demo stuff" << endl;
    return 1;
  }

  //printf("Runing at %f FPS\n", 1/(time_delta) * 1000);
  while (quit == false) {
    oldticks = SDL_GetTicks();
    
    do_input();
    do_pointit();
    do_logic();
    do_render();
    
    diffticks += (SDL_GetTicks() - oldticks);
    if (diffticks >= time_delta) {
      do_particle = true;
    }

    //SDL_Delay(20);
  }
  cleanup();

  return 0;
}
