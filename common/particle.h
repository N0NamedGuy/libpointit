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
#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <pthread.h>
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

#define PI 3.14159265

class Particle {
private:
  float x,y;
  float sx, sy;
  float ax, ay;
  float w, h;
  float alpha;
    
  bool done;

  SDL_Surface* dest_surf;

public:

  Particle(int ix,int iy, float iax, float iay, SDL_Surface* dest);
  void render();
  void render_surf(SDL_Surface* particle);
  bool is_done();
};

class ParticleEngine {
  std::vector<Particle> particles;
  bool quit;
public:

  ParticleEngine();
  ~ParticleEngine();

  void do_particles(int x, int y, float ax, float ay, int num_parts, SDL_Surface* dest_surf);
  void render();
  void render(SDL_Surface* particle);
};
#endif

