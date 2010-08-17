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

#define WIDTH 640 
#define HEIGHT 480

int err;

int TOLERANCE;
int MAX_DIFF;
int MAX_DIST;

int POINT_W;

int showcam;
int showlines;
// FPS Measuring
time_t last_seconds;
int fps;
int last_fps;

// Detected coordinates
int detected_x;
int detected_y;

// Detected lines
int detected_l;
int detected_r;
int detected_t;
int detected_b;

// Detected differences
int detected_diff_x;
int detected_diff_y;

// Step for improved performance
int step_x;
int step_y;

int w,h;

int pointit_init(void) {
  TOLERANCE = 10;
  MAX_DIFF = 80;
  MAX_DIST = 800;
  POINT_W = 20;

  //printf("Starting PointIt\n");

  if (pointit_init_cap(WIDTH, HEIGHT) == -1) {
    printf("Couldn't init video device.\n");   
    return -1;
  }

  detected_x = pointit_get_width() / 2;
  detected_y = pointit_get_height() / 2;
  
  detected_diff_x = 0;
  detected_diff_y = 0;

  last_seconds = 0;
  fps = 0;
  last_fps = 0;

  showcam = 0;
  showlines = 0;

  err = 0;

  step_x = TOLERANCE / 4;
  step_y = TOLERANCE / 4;

  return 0;
}

int pointit_destroy(void) {
  return pointit_destroy_cap();
}


int pointit_is_color(int x, int y) {
  struct hsv_color hsv = pointit_get_color(x, y);
 
  return 
    hsv.h >= 75 && hsv.h <= 140 && // For green
    // hsv.h >= 240 - 30 && hsv.h <= 240 + 30 && // For "theorical" blue

    // hsv.h <= 30 && hsv.h >= 330 && // For red
    hsv.s > 40 && hsv.v > 30;
}

void pointit_count_fps(void) {
  time_t cur_sec = time(NULL);

  if (cur_sec > last_seconds) {
    last_seconds = cur_sec;
    last_fps = fps;
    fps = 0;
  }

  fps++;
}

void pointit_detect(void) {
  int i,j;
  
  int l=-1,r=-1,t=-1,b=-1;
  int get_out = 0;

  int num_pixels = 0;

  int x,y;

  pointit_capture();

  for (i = 0; i < WIDTH; i += step_x) {
    for (j = 0; j < HEIGHT; j += step_y) {
      if (pointit_is_color(i,j)) {
        
        num_pixels++;

        if (num_pixels > TOLERANCE) {
          l = i;
	        get_out = 1;
	        break;
	      }
      }
    }
    if (get_out) break;
  }

  if (!get_out) {
    if (step_x >= (TOLERANCE / 4)) step_x /= 2;
    if (step_y >= (TOLERANCE / 4)) step_y /= 2;
    return;
  }

  num_pixels = 0;
  get_out = 0;

  for (i = WIDTH - 1; i >= 0 ; i -= step_x ) {
    for (j = 0; j < HEIGHT; j += step_y) {

      if (pointit_is_color(i,j)) {
        num_pixels++;

        if (num_pixels > TOLERANCE) {
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
  for (i = 0; i < HEIGHT; i += step_y ) {
    for (j = 0; j < WIDTH; j += step_x) {
      if (pointit_is_color(j,i)) {
        num_pixels++;

	      if (num_pixels > TOLERANCE) {
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
  for (i = HEIGHT-1; i >= 0; i -= step_y) {
    for (j = 0; j < WIDTH; j += step_x) {
      if (pointit_is_color(j,i)) {
        num_pixels++;

        if (num_pixels > TOLERANCE) {
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
      && (r-l < MAX_DIFF && b-t < MAX_DIFF)
      && (   (detected_x == -1 && detected_y == -1) ||
	     (dist(x,y,detected_x,detected_y) <= MAX_DIST)
	      )
      ) {

    detected_diff_x = x - detected_x;
    detected_diff_y = y - detected_y;

    detected_x = x;
    detected_y = y;

    detected_l = l;
    detected_r = r;
    detected_t = t;
    detected_b = b;

    step_x *= 2;
    step_y *= 2;

    if (step_x >= (r-l) / 2) {
      step_x /= 2;
    }

    if (step_y >= (b-t) / 2) {
      step_y /= 2;
    }

  } else {
    if (step_x >= (TOLERANCE / 4)) step_x /= 2;
    if (step_y >= (TOLERANCE / 4)) step_y /= 2;
  }

}
    
void pointit_toggle_cam() {
  if (showcam)
    pointit_hide_cam();
  else
    pointit_show_cam();
  showcam = !showcam;
}

int pointit_get_x(void) {
  return detected_x; 
}

int pointit_get_y(void) {
  return detected_y; 
}

int pointit_get_left(void) {
  return detected_l; 
}

int pointit_get_right(void) {
  return detected_r;
}

int pointit_get_top(void) {
  return detected_t;
} 

int pointit_get_bottom(void) {
  return detected_b;
}

int pointit_get_diff_x(void) {
  return detected_diff_x; 
}

int pointit_get_diff_y(void) {
  return detected_diff_y;
}

int pointit_get_width(void) {
  return WIDTH;
}

int pointit_get_height(void) {
  return HEIGHT;
}

// vim:ts=2:expandtab:cindent
