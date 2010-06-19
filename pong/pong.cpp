// Copyright (C) 2010 David Serrano <david.nonamedguy@gmail.com> 
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
/* Point Pong
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
#include "pong.h"

float max(float a, float b) {
  if (b > a) {
    return a;
  } else return b;
}

void cleanup() {
  clean_audio();
  SDL_FreeSurface(pad1);
  SDL_FreeSurface(pad2);
  SDL_FreeSurface(ball);
  SDL_FreeSurface(pointer);

  thread_wait();

  SDL_Quit();
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
 
  SDL_WM_SetCaption( "Pong", NULL );
  
  fullscreen = !fullscreen;
}

void do_render() {
  string strscore;
  ostringstream outs;
  

  Utils::clear_surf(screen);

  if (!paused) {
    pe.do_particles(b.getX(), b.getY(),b.getSpeedX()/100, b.getSpeedY()/100, 
		    max((b.getSpeedX() * b.getSpeedX()) + 
			(b.getSpeedY() * b.getSpeedY()),200), screen);
  }

  pe.render();
  
  Utils::apply_surface(b.getX() - 16, b.getY() - 16, ball, screen);
  Utils::apply_surface(pointing.x - 16, pointing.y - 16, pointer, screen);
  Utils::apply_surface(p1.getX(), p1.getY() - (PAD_HEIGHT / 2),pad1, screen);
  Utils::apply_surface(p2.getX(), p2.getY() - (PAD_HEIGHT / 2),pad2, screen);
  
  outs << p1.get_score() << ":" << p2.get_score();
  strscore = outs.str();

  scoreboard = TTF_RenderText_Solid(font, strscore.c_str(), textColor ); 
  Utils::apply_surface((SCREEN_WIDTH - scoreboard->w) / 2,1,scoreboard, screen);

  if( SDL_Flip( screen ) == -1 ) { exit(1); }
}

void do_logic() {
  int score = 0;
  if (!paused) {
    p1.moveTo(pointing.y);
    p2.moveTo(b.getY());

    score = b.render(&p1, &p2);

  
    if (score) {
      b.resetPos();
    }
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
      case SDLK_p: paused = !paused; break;
      case SDLK_d: pntIt.toggle_lines(); break;
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
    cerr << "Errors happened while initializing the game stuff" << endl;
    return 1;
  }

  printf("Runing at %f FPS\n", 1/(time_delta) * 1000);
  SDL_Delay(500);
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
  stop_music();
  cleanup();

  return 0;
}
