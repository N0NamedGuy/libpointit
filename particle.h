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

