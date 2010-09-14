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
#include <string>
#include <sstream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_gfxPrimitives.h"

#include "../pointit/pointit.h"
#include "../common/thread.h"
#include "../common/utils.h"
#include "../common/particle.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_BPP 32
#define PI 3.14159265

using namespace std;

struct pointit_context pntIt;

SDL_Rect lastpos = {-1,-1,1,1};
SDL_Rect pointing = {0,0,1,1};

int oldticks=0;
int fps=30;
float time_delta = (1.0/(float(fps))) * 1000.0;
bool do_particle=true;
int diffticks=0;

bool fullscreen=false;
bool quit = false;
bool using_pointit=true;

SDL_Surface *pointer = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *rockets[360];
SDL_Surface *explosion = NULL;

double rocket_pos_x;
double rocket_pos_y;

double rocket_angle = 90;
double rocket_speed = 8;


#define MAX_ROCKET_SECS 5
long old_rocket_ticks = 0;
bool exploded = false;

SDL_Event event;

ParticleEngine pe;

bool
load_images() {

  pointer = load_image("gfx/pointer.png");
  background = load_image("gfx/background.jpg");
  explosion = load_image("puff.png");

  for (int i = 0; i < 360; i++) {
    
    std::ostringstream filename;

    rockets[i] = NULL;
    filename << "gfx/rocket_" << i << ".png";
    rockets[i] = load_image(filename.str());

    //    cout << "loading " << filename.str() << endl;

    if (rockets[i] == NULL) {
      std::cerr << "ROCKET NOT FOUND @ " << i << endl;
      rockets[i] = load_image("gfx/rocket_1.png");
      exit(1);
    }

  }

  //  rocket = load_image("gfx/rocket.png");


  return true;
}

bool
init_logic() {

  SDL_ShowCursor(0);
  pointing.x = SCREEN_WIDTH / 2 + 100;
  pointing.y = SCREEN_HEIGHT / 2 + 1;

  SDL_WarpMouse(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  rocket_pos_x = SCREEN_WIDTH;
  rocket_pos_y = SCREEN_HEIGHT;

  old_rocket_ticks = SDL_GetTicks();

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
 
  SDL_WM_SetCaption( "Demo", NULL );


  if (!load_images()) {
    cerr << "Images not loaded" << endl;
    return false;
  }

  if (!init_pointit()) {
    cerr << "No point it..." << endl;
    //    return false;
  }

  if (!init_logic()) {
    cerr << "Logic not loaded... (WTF?)" << endl;
    return false;
  }
  return true;
}

void cleanup() {
  SDL_FreeSurface(pointer);
  SDL_FreeSurface(background);
  SDL_FreeSurface(explosion);

  for (int i = 0; i < 360; i++) {
    SDL_FreeSurface(rockets[i]);
  }

  thread_wait();
  SDL_Quit();
}

void clear_surf(SDL_Surface* img) {
  SDL_Rect rect = {0,0,img->w,img->h};
  SDL_FillRect(img, &rect, 0);
}

void toggle_pointit() {
  if (!using_pointit) {
    using_pointit = !init_pointit();
  }

  using_pointit = !using_pointit;
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

void do_render() {
  //  clear_surf(screen);
  
  apply_surface(0,0,background,screen);
  apply_surface(pointing.x - 16, pointing.y - 16, pointer, screen);

  if (!exploded)
    apply_surface((int)rocket_pos_x - (rockets[(int)rocket_angle]->w / 2), 
		  (int)rocket_pos_y - (rockets[(int)rocket_angle]->h / 2),
		  rockets[(int)rocket_angle], screen);

  pe.render(explosion);

  if( SDL_Flip( screen ) == -1 ) { exit(1); }
}

void do_logic() {

  //  rocket_rot = rotozoomSurface(rocket, rocket_angle * (180 / PI) ,6,0);

  if (exploded) {
    return;
  }

  double diff_x = (pointing.x - rocket_pos_x);
  double diff_y = -(pointing.y - rocket_pos_y);
  /*
  cout << "WTF IS GOING ON???? HELP!" << endl;

  cout << "dx: " << diff_x << 
    " dy: " << diff_y << endl;

  cout << "px: " << pointing.x << 
    " py: " << pointing.y << endl;

  cout << "rx: " << rocket_pos_x << 
  " ry: " << rocket_pos_y << endl;*/

  double rad_ang = atan( diff_y / diff_x);

  if (isnan(rad_ang)) {
    return;
  }

  if (diff_x < 0 && diff_y < 0) {
    rad_ang += PI;
  } else if (diff_x < 0 && diff_y > 0) {
    rad_ang -= PI / 2;
    rad_ang += 3 * PI / 2;
  } else if (diff_x > 0 && diff_y < 0) {
    rad_ang += 4 * PI / 2;
  }

  rocket_angle = rad_ang * 180 / PI;

  rocket_pos_x += rocket_speed * cos(rad_ang) ;
  rocket_pos_y -= rocket_speed * sin(rad_ang) ;

  pe.do_particles((int)rocket_pos_x - (cos(rad_ang) * 51), (int)rocket_pos_y + (sin(rad_ang) * 51), 0, 0, 1, screen);

  double radius = sqrt((diff_x * diff_x) + (diff_y * diff_y));
  if (radius < 10) {
    exploded = true;
    pe.do_particles(rocket_pos_x, rocket_pos_y, 0, 0, 100, screen);
  }


  /*  cout << "cos(ang): " << cos(rad_ang) << 
    " sin(ang): " << sin(rad_ang) <<
    " npos_x: " << rocket_pos_x + cos(rad_ang) <<
    " npos_y: " << rocket_pos_y + sin(rad_ang) <<
    " rad_ang: " << rad_ang << endl; 
  
  
    cout << "cur_angle: " << rocket_angle << endl;*/

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
      //case SDLK_d: pntIt.toggle_lines(); break;

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
    
    do_pointit();
    do_input();
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
