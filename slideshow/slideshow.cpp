/* Point Draw
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
#include <vector>
#include <dirent.h>
#include <cmath>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"

#include "pointit.h"
#include "thread.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_BPP 32

using namespace std;

PointIt pntIt;

SDL_Rect lastpos = {-1,-1,1,1};
SDL_Rect pointing = {0,0,1,1};


int oldticks=0;
int fps=30;

float time_delta = (1.0/(float(fps))) * 1000.0;

bool paused=true;
bool fullscreen=false;
bool quit = false;
bool using_pointit=true;

SDL_Surface *pointer = NULL;
SDL_Surface *drawing = NULL;
SDL_Surface *screen = NULL;

SDL_Event event;


// Directory stuff
vector<string> file_list = vector<string>();
int cur_file = 0;

bool init_dir () {
  int cnt = 0;

  string dir = "./images";

  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(dir.c_str())) == NULL) {
    cout << "Error opening " << dir << endl;
    return false;
  }

  while ((dirp = readdir(dp)) != NULL) {
    cnt++;

    if (cnt > 2) {    
      file_list.push_back(string(dirp->d_name));
    }

  }
  closedir(dp);
  return true;
}


SDL_Surface
*load_image( string filename ) {
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
apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
  SDL_Rect offset;
  
  offset.x = x;
  offset.y = y;

  SDL_BlitSurface( source, NULL, destination, &offset);
}

bool
load_images() {

  pointer = load_image("gfx/pointer.png");
  drawing = SDL_CreateRGBSurface(SDL_SWSURFACE, 
				 screen->w,screen->h,SCREEN_BPP,
				 0,0,0,0);

  return true;
}

bool
init_logic() {

  if (pntIt.get_error()) {
    using_pointit = 0;
  }  

  SDL_ShowCursor(0);
  SDL_WarpMouse(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  return true;
}


bool
init_pointit() {
  quit = false;
  using_pointit = true;
  init_thread(&pntIt, &using_pointit);

  return pntIt.get_error() == 0;
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

  if (!init_logic()) {
    cerr << "Wth? No logic??" << endl;
    return false;
  }

  if (!init_pointit()) {
    cerr << "No point it..." << endl;
    //return false;
  }

  init_dir();
  return true;
}

void cleanup() {
  SDL_FreeSurface(pointer);
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
  clear_surf(screen);
  
  if (!paused) {
    if (lastpos.x != -1 && lastpos.y != -1) {
      lineRGBA(drawing, lastpos.x, lastpos.y,
	       pointing.x, pointing.y,
	       0xFF, 0xFF, 0xFF, 0xFF);

      //      apply_surface(pointing.x - 16, pointing.y - 16, pointer, drawing);
    }

    lastpos = pointing;
  } else {
    lastpos.x = -1;
    lastpos.y = -1;
  }

  apply_surface(0,0,drawing,screen);
  apply_surface(pointing.x - 16, pointing.y - 16, pointer, screen);
  
  if( SDL_Flip( screen ) == -1 ) { exit(1); }
}

void do_logic() {
  int diff = 0;

  if (using_pointit) {
    diff = pntIt.get_diff_x();
  } else {
    diff = pointing.x - lastpos.x;
    lastpos.x = pointing.x;
    lastpos.y = pointing.y;
  }

  if (abs(diff) > 300) {
    if (diff > 0) {
      cur_file++;
      std::cout << "gone right " << endl;
    } else if (diff < 0) {
      cur_file--;
      std::cout << "gone left" << endl;
    }

    if (cur_file < 0) {
      cur_file = (signed int)(file_list.size()) - 1;
      
    } else if (cur_file >= (signed int)(file_list.size())) {
      cur_file = 0;
    }

    std::cout << "Cur file:" << cur_file 
	      << " name: " << file_list[cur_file]
	      << " Diff: " << diff << endl;

    diff = 0;
  } 

  
}

void do_pointit() {
  if (using_pointit) {
    //pntIt.do_detect();
    if (pntIt.get_error() != 0) {
      using_pointit = 0;
    } else {
      pointing.x = (pntIt.get_x() * SCREEN_WIDTH) / pntIt.get_width();
      pointing.y = (pntIt.get_y() * SCREEN_HEIGHT) / pntIt.get_height();
    }
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
      case SDLK_c: pntIt.toggle_cam(); break;
      case SDLK_SPACE: paused = !paused; break;
      case SDLK_d: pntIt.toggle_lines(); break;
      case SDLK_e: 
	clear_surf(drawing);
	lastpos.x = -1; lastpos.y = -1;
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

  printf("Runing at %f FPS\n", 1/(time_delta) * 1000);
  while (quit == false) {
    oldticks = SDL_GetTicks();
    
    do_input();
    do_pointit();
    do_logic();
    do_render();

    while ((SDL_GetTicks() - oldticks) < time_delta) {

    }

    //SDL_Delay(20);
  }
  cleanup();

  return 0;
}
