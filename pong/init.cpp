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
#include "init.h"

bool
load_images() {
  pad1 = Utils::load_image("gfx/pad1.png");
  pad2 = Utils::load_image("gfx/pad2.png");
  ball = Utils::load_image("gfx/ball.png");
  pointer = Utils::load_image("gfx/pointer.png");
  return true;
}

bool
init_logic() {

  b.resetPos();


  play_music();
  

  SDL_ShowCursor(0);
  SDL_WarpMouse(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  return true;
}

bool
init_pointit() {
  
  using_pointit = 0;
  if (pointit_init() == 0) {
    pntIt = pointit_get_green_context();
    quit = false;
    using_pointit = true;
    init_thread(&pntIt, &using_pointit);

    using_pointit = 1;
  }

   return true;
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
init() {
  if (SDL_Init( SDL_INIT_EVERYTHING ) == -1) {
    cerr << "SDL Not initialized" << endl;
    return false;
  }

  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			     SDL_SWSURFACE /*| SDL_FULLSCREEN*/ );

  if (screen == NULL) {
    cerr << "Screen not initialized" << endl;
    return false;
  }
 
  SDL_WM_SetCaption( "Pong", NULL );


  if (!load_images()) {
    cerr << "Images not loaded" << endl;
    return false;
  }

  if (init_audio() != 0) {
    cerr << "No sound" << endl;
  }

  //Initialize SDL_ttf 
  if(!init_font() ) {
    cerr << "No fonts..." << endl;
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

  return true;
}

