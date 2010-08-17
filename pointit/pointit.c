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

#include <math.h>

#include "pointit.h"
#include "color.h"

#define dist(x1,y1,x2,y2) (sqrt( (((x2)-(x1))*((x2)-(x1)))	\
				 + (((y2)-(y1))*((y2)-(y1))) ))

#ifdef USING_V4L2
#include "v4l2/v4l2grabber.h"
#elif USING_CV
#include "cv/cvgrabber.h"
#endif

#define WIDTH 320 
#define HEIGHT 240

int pointit_init(void) {
  if (pointit_init_cap(WIDTH, HEIGHT) == -1) {
    printf("Couldn't init video device.\n");   
    return -1;
  }
  return 0;
}

int pointit_destroy(void) {
  return pointit_destroy_cap();
}


int pointit_is_color(struct pointit_context* context, int x, int y) {
  struct hsv_color hsv;
  hsv = pointit_get_color(x, y);
 
  return 
    hsv.h >= context->min_h && hsv.h <= context->max_h && 
    hsv.s >= context->min_s && hsv.s <= context->max_s &&
    hsv.v >= context->min_v && hsv.v <= context->max_v;
}

void pointit_detect(struct pointit_context* context) {
  int i,j;
  
  int l=-1,r=-1,t=-1,b=-1;
  int get_out = 0;

  int num_pixels = 0;

  int x,y;

  pointit_capture();

  for (i = 0; i < context->w; i += context->step_x) {
    for (j = 0; j < context->h; j += context->step_y) {
      if (pointit_is_color(context, i, j)) {
        
        num_pixels++;

        if (num_pixels > context->tolerance) {
          l = i;
	        get_out = 1;
	        break;
	      }
      }
    }
    if (get_out) break;
  }

  if (!get_out) {
    if (context->step_x >= (context->tolerance / 4)) context->step_x /= 2;
    if (context->step_y >= (context->tolerance / 4)) context->step_y /= 2;
    return;
  }

  num_pixels = 0;
  get_out = 0;

  for (i = context->w - 1; i >= 0 ; i -= context->step_x ) {
    for (j = 0; j < context->h; j += context->step_y) {

      if (pointit_is_color(context, i, j)) {
        num_pixels++;

        if (num_pixels > context->tolerance) {
          r = i;
	        get_out = 1;
	        break;
	      }
      }
    }
    if (get_out) break;
  }

  num_pixels = 0;
  get_out = 0;
  for (i = 0; i < context->h; i += context->step_y ) {
    for (j = 0; j < context->w; j += context->step_x) {
      if (pointit_is_color(context, j, i)) {
        num_pixels++;

	      if (num_pixels > context->tolerance) {
	        t = i;
	        get_out = 1;
	        break;
	      }
      }
    }
    if (get_out) break;
  }

  num_pixels = 0;
  get_out = 0;
  for (i = context->h - 1; i >= 0; i -= context->step_y) {
    for (j = 0; j < context->w; j += context->step_x) {
      if (pointit_is_color(context, j, i)) {
        num_pixels++;

        if (num_pixels > context->tolerance) {
          b = i;
	        get_out = 1;
	        break;
	      }
      }
    }
    if (get_out) break;
  }

  x = l + ((r-l)/2);
  y = t + ((b-t)/2);

  if (get_out
      && (r-l < context->max_dist && b-t < context->max_diff)
      /* && ( (dist(x,y,context->dx,context->dy) <= context->max_dist) ) */
      ) {

    context->dx = x - context->x;
    context->dy = y - context->y;

    context->x = x;
    context->y = y;

    context->l = l;
    context->r = r;
    context->t = t;
    context->b = b;

    context->step_x *= 2;
    context->step_y *= 2;

    if (context->step_x >= (r-l) / 2) {
      context->step_x /= 2;
    }

    if (context->step_y >= (b-t) / 2) {
      context->step_y /= 2;
    }

  } else {
    if (context->step_x >= (context->tolerance / 4)) context->step_x /= 2;
    if (context->step_y >= (context->tolerance / 4)) context->step_y /= 2;
  }

}
    
struct pointit_context pointit_get_green_context(void) {
  struct pointit_context context;

  context.x = 0;
  context.y = 0;

  context.dx = 0;
  context.dy = 0;
  
  context.l = 0;
  context.r = 0;
  context.t = 0;
  context.b = 0;
  
  context.w = WIDTH;
  context.h = HEIGHT;

  context.tolerance = 10;
  context.max_diff = 80;
  context.max_dist = 240;
  context.point_w = 20;

  context.step_x = context.tolerance / 4;
  context.step_y = context.tolerance / 4;

  context.min_h = 75; context.max_h = 140;
  context.min_s = 40; context.max_s = 255;
  context.min_v = 30; context.max_v = 255;

  return context;
}

// vim:ts=2:expandtab:cindent
