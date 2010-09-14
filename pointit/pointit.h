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
#ifndef POINTIT_H
#define POINTIT_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "color.h"

#ifdef USING_V4L2
#include "v4l2/v4l2grabber.h"
#elif USING_CV
#include "cv/cvgrabber.h"
#endif

struct pointit_context {
    int x, y;        /* Detected coords */
    int l, r, t, b;  /* Detected lines */
    int dx, dy;      /* Detected diffs */
    int w, h;        /* Detected width and height */

    /* Tolerances */
    int tolerance; /* How many correct pixels must be jumped */
    int max_dist;  /* Mas distance between lines */

    /* Performance jumps */
    int step_y;
    int step_x;
    int dynamic_step;

    /* Color ranges */
    int min_h, max_h;
    int min_s, max_s;
    int min_v, max_v;
};

extern int pointit_init(void); 
extern int pointit_destroy(void); 
extern void pointit_detect(struct pointit_context* context);
 
extern struct pointit_context pointit_get_default_context(void);
extern struct pointit_context pointit_get_green_context(void);
extern struct pointit_context pointit_get_orange_context(void);
extern struct pointit_context pointit_get_yellow_context(void);
extern struct pointit_context pointit_get_blue_context(void);
extern struct pointit_context pointit_get_pink_context(void);
#endif
