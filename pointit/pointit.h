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

#define USING_CV

#ifdef USING_CV
#include "cv.h"
#include "highgui.h"
#define camImg IplImage
#endif


//#define SHOWCAM

class PointIt {
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

  // Detected differences
  int detected_diff_x;
  int detected_diff_y;

  // Step for improved performance
  int step_x;
  int step_y;

  int w,h;
#ifdef USING_CV
  // Our webcam handle
  CvCapture *capture;
#endif

private:
  int is_color(camImg* img, int x, int y);
  void count_fps();
  void do_detection(camImg* img);
  

#ifdef USING_CV
  void init_cv();
#endif

public:
  PointIt();
  ~PointIt();
  void do_detect();
  int get_x() { return detected_x; }
  int get_y() { return detected_y; }
  int get_diff_x() { return detected_diff_x; }
  int get_diff_y() { return detected_diff_y; }
  int get_fps() { return last_fps; }
  void toggle_cam();
  int get_error();
  void toggle_lines();

  int get_width();
  int get_height();
};


#endif

// vim:ts=2:expandtab:cindent
