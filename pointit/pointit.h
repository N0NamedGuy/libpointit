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
