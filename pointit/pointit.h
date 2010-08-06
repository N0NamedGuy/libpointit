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

#define USING_V4L2
#ifdef USING_V4L2
#include "v4l2/v4l2grabber.h"
#endif

extern static int pointit_init(void); 
extern static int pointit_destroy(void); 
extern static int pointit_detect(void); 
extern static int pointit_toggle_cam(void); 

extern static int pointit_get_x(void); 
extern static int pointit_get_y(void); 
extern static int pointit_get_diff_x(void); 
extern static int pointit_get_diff_y(void); 
extern static int pointit_get_width(void); 
extern static int pointit_get_height(void); 
// vim:ts=2:expandtab:cindent
