#include "ball.h"

Ball::Ball() {
  x = SCREEN_WIDTH / 2;
  y = SCREEN_HEIGHT / 2;
  sx = 0;
  sy = 0;

  d = 32;

  scored = 0;
  paused_time = SDL_GetTicks();
}

int Ball::getX() { return (int)x; }
int Ball::getY() { return (int)y; }

float Ball::getSpeedX() { return sx;}
float Ball::getSpeedY() { return sy;}


int Ball::render(Pad* p1, Pad* p2) {

  int w = SCREEN_WIDTH;
  int h = SCREEN_HEIGHT;

  int deflect = 0;

  if (SDL_GetTicks() - paused_time < PAUSE_TIME) {
    return 0;
  }

  if ((/*x >= (p1.getX() - d)  &&*/ x <= p1->getX() + 16 + (d/2)) &&
      (y > p1->getY() - 40 - d && y < p1->getY() + 40 + d)) {
    sx = -sx * (1.1);

    deflect = (p1->getY() - y) ;

    sy = -(deflect) * (sx / 50);
    play_phit();

  } else if ((x >= p2->getX() - (d/2) /* && x <= (p2.getX() + 16) + d*/) &&
	     (y > p2->getY() - 40 - d && y < p2->getY() + 40 + d/2)) {
    sx = -sx * (1.1);
    
    deflect = (p2->getY() - y) ;

    sy = -(deflect) * (sx / 50);
    play_phit();



  } else if (x > w - d) {
    sx = -sx;
    x = w - d;
    play_crash();
    scored = 1;
    p1->score();
    
    paused_time = SDL_GetTicks();
    //printf("%d : %d\n",p1->get_score(),p2->get_score());
    return 1;
  } else if (x < d) {
    sx = -sx;
    x = d;
    scored = 2;
    play_crash();
    p2->score();
    
    paused_time = SDL_GetTicks();
    //printf("%d : %d\n",p1->get_score(),p2->get_score());
    return 2;
  }

  if (y > h - d) {
    sy = -sy;
    y = h - d;
    play_whit();
  } else if (y < d) {
    sy = -sy;
    y = d;
    play_whit();
  }

  x += sx;
  y += sy;

  //play_moving();

  return 0;
}

void Ball::resetPos() {
  x = SCREEN_WIDTH / 2;
  y = SCREEN_HEIGHT / 2;
  sx = -5;
  sy = 5;

  if (scored == 1) {
    sx = -sx;
  }

  scored = 0;
}

