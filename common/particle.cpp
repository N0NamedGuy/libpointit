#include "particle.h"

const int MAX_SPEED = 10;
const int MIN_SPEED = 4;
const int MAX_PARTICLES = 1000000;

Particle::Particle(int ix, int iy, float iax, float iay, SDL_Surface* dest) {
  int angle;
  float speed;

  x = (float)ix;
  y = (float)iy;

  ax = iax;
  ay = iay;

  speed = (float)(rand() % ( (MAX_SPEED * 1000) - MIN_SPEED ) + MIN_SPEED )
    / 1000;

  angle = rand() % 360;
 
  sx = speed * cos(angle * PI/180);
  sy = speed * sin(angle * PI/180);

  dest_surf = dest;

  alpha = 200;
}

void Particle::render() {
  sy += ax;
  sy += ay;

  x += sx;
  y += sy;

  alpha -= 0.1;
 
  if (x >= 0 && y >= 0) {

    pixelRGBA(dest_surf,
	      (signed int)x, (signed int)y,
	      rand() % 255,
	      rand() % 255,
	      /*rand() %*/ 255,
	      (int)alpha); 
  }
}


void Particle::render_surf(SDL_Surface* particle) {
  SDL_Rect offset;

  sy += ax;
  sy += ay;

  x += sx;
  y += sy;

  offset.x = (int)x;
  offset.y = (int)y;

  alpha -= 0.1;
 
  if (x >= 0 && y >= 0) {
    SDL_SetAlpha( particle, SDL_SRCALPHA, alpha ); 
    SDL_BlitSurface(particle, NULL, dest_surf, &offset);
  }
}

bool Particle::is_done() {
  return (x > dest_surf->w || x < -20 ||
	  y > dest_surf->h || y < -20 || 
	  alpha <= 0);
}

ParticleEngine::ParticleEngine() {
  srand ( time(NULL) );
  quit = false;
}

ParticleEngine::~ParticleEngine() {
  quit = true;
}

void ParticleEngine::do_particles
(int x, int y, float ax, float ay, int num_parts, SDL_Surface* dest_surf) {
  
  for (int i = 0; i < num_parts; i++) {
    Particle tmp(x,y,ax,ay,dest_surf);
    particles.push_back(tmp);
  }
  
}

void ParticleEngine::render() {
  std::vector<Particle>::iterator it;

  for (it = particles.begin(); it < particles.end(); it++) {
    it->render();

    if (it->is_done()) {
      particles.erase(it);
    }
  }
}


void ParticleEngine::render(SDL_Surface* particle) {
  std::vector<Particle>::iterator it;

  for (it = particles.begin(); it < particles.end(); it++) {
    it->render_surf(particle);

    if (it->is_done()) {
      particles.erase(it);
    }
  }
}
