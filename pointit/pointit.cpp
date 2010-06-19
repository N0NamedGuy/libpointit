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
#include "pointit.h"
#define dist(x1,y1,x2,y2) (sqrt( ((x2-x1)*(x2-x1))	\
				 + ((y2-y1)*(y2-y1)) ))

/*
int PointIt::TOLERANCE = 60;
int PointIt::MAX_DIFF = 80;
int PointIt::MAX_DIST = 80;
int PointIt::POINT_W = 20;
*/

void rgb2hsv(int ir, int ig, int ib,
	     int* h, int* s, int* v) { 

  float r = ((float)(ir)) / 255; 
  float g = ((float)(ig)) / 255; 
  float b = ((float)(ib)) / 255; 


  // RGB are from 0..1, H is from 0..360, SV from 0..1
  double maxC = b;
  if (maxC < g) maxC = g;
  if (maxC < r) maxC = r;
  double minC = b;
  if (minC > g) minC = g;
  if (minC > r) minC = r;

  double delta = maxC - minC;

  double V = maxC;
  double S = 0;
  double H = 0;

  if (delta == 0) {
    H = 0;
    S = 0;
  } else {
    S = delta / maxC;
    double dR = 60*(maxC - r)/delta + 180;
    double dG = 60*(maxC - g)/delta + 180;
    double dB = 60*(maxC - b)/delta + 180;
    if (r == maxC)
      H = dB - dG;
    else if (g == maxC)
      H = 120 + dR - dB;
    else
      H = 240 + dG - dR;
  }

  if (H<0)
    H+=360;
  if (H>=360)
    H-=360;

  *h = H;
  *s = S * 100;
  *v = V * 100;
}

#ifdef USING_CV
void PointIt::init_cv() {
  capture = cvCaptureFromCAM( CV_CAP_ANY );
  if( !capture ) {
    //fprintf( stderr, "ERROR: capture is NULL \n" );
    err = 1;
    return;
  }
  
  err = 0;

  camImg* frame = cvQueryFrame( capture );

  if( !frame ) {
    //fprintf( stderr, "ERROR: frame is null...\n" );
    err=2;
    w = -1;
    h = -1;
  } else {

    w = frame->width;
    h = frame->height;
  }
}
#endif

PointIt::PointIt() {
  TOLERANCE = 10;
  MAX_DIFF = 80;
  MAX_DIST = 800;
  POINT_W = 20;

  //printf("Starting PointIt\n");

#ifdef USING_CV
  init_cv();
  if (err != 0) {
    return;
  }
#endif

  detected_x = get_width() / 2;
  detected_y = get_height() / 2;
  
  detected_diff_x = 0;
  detected_diff_y = 0;

  last_seconds = 0;
  fps = 0;
  last_fps = 0;

  showcam = 0;
  showlines = 0;

  err = 0;


  //  toggle_cam();
}

PointIt::~PointIt() {
  //  printf("PointIt ended\n");

#ifdef USING_CV
  if (showcam) cvDestroyWindow( "defcam" );
  cvReleaseCapture( &capture );
#endif
}


int PointIt::is_color(camImg* img, int x, int y) {
#ifdef USING_CV

  int blue=0,green=0,red=0;

#define USING_EFFICIENT
#ifdef USING_EFFICIENT
  blue = ((uchar *)(img->imageData + y*img->widthStep))[x*img->nChannels + 0];
  green = ((uchar *)(img->imageData + y*img->widthStep))[x*img->nChannels + 1];
  red = ((uchar *)(img->imageData + y*img->widthStep))[x*img->nChannels + 2];
  /*
    printf("Coord: (%d %d)  RGB:(%d %d %d)\n",
	   x,y,
	   red,green,blue);
  */
#else
  CvScalar color;
  color = cvGet2D(img,y,x);
 
  blue = (int)color.val[0];
  green = (int)color.val[1];
  red = (int)color.val[2];
#endif

  int h,s,v;

  rgb2hsv(red,green,blue,&h,&s,&v);

  
  /*
  if (h >= 75 + 120 && h <= 140 + 120 &&   // green
      //h <= 30 && h >= 330 //&& // red
      
      s > 40 && v > 30) {
    printf("Coord: (%d %d)  RGB:(%d %d %d) HSV:(%d %d %d)\n",
	   x,y,
	   red,green,blue,
	   h,s,v);

    cvCircle(img, cvPoint(x,y), 5, cvScalar(0,0,255), 1);
    }
  */
  
  return 
    h >= 75 && h <= 140 && // For green
    // h >= 240 - 30 && h <= 240 + 30 && // For "theorical" blue

    // h <= 30 && h >= 330 && // For red
    s > 40 && v > 30;

#else
  return 0;

#endif


  /*  return(
    red > 0 && red < 120 &&
    green > 120 && green < 255 &&
    blue > 0 && blue < 120 &&
    green > red &&
    green > blue);
  */
}

void PointIt::count_fps() {
  time_t cur_sec = time(NULL);

  if (cur_sec > last_seconds) {
    last_seconds = cur_sec;
    last_fps = fps;
    fps = 0;
  }

  fps++;
}

void PointIt::do_detection(camImg* img) {
  int i,j;
  
  int l=-1,r=-1,t=-1,b=-1;
  int get_out = 0;

  int num_pixels = 0;

  int x,y;

  for (i = 0; i < img->width; i++ ) {
    for (j = 0; j < img->height; j++) {
      if (is_color(img,i,j)) {
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
    return;
  }

  num_pixels = 0;
  get_out = 0;

  for (i = img->width - 1; i >= 0 ; i-- ) {
    for (j = 0; j < img->height; j++) {

      if (is_color(img,i,j)) {
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
  for (i = 0; i < img->height; i++ ) {
    for (j = 0; j < img->width; j++) {
      if (is_color(img,j,i)) {
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
  for (i = img->height-1; i >= 0; i--) {
    for (j = 0; j < img->width; j++) {
      if (is_color(img,j,i)) {
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

    if (showlines) {
#ifdef USING_CV
      cvLine(img, cvPoint(l,0), cvPoint(l,img->height-1), cvScalar(0,255,0), 1);
      cvLine(img, cvPoint(r,0), cvPoint(r,img->height-1), cvScalar(0,255,0), 1);
      cvLine(img, cvPoint(0,t), cvPoint(img->width-1,t), cvScalar(0,255,0), 1);
      cvLine(img, cvPoint(0,b), cvPoint(img->width-1,b), cvScalar(0,255,0), 1);
#endif
    }

  }

}

void PointIt::do_detect() {
#ifdef USING_CV
  IplImage* frame = cvQueryFrame( capture );

  if( !frame ) {
    //fprintf( stderr, "ERROR: frame is null...\n" );
    err=2;
  } else {

    IplImage* to_process;

    to_process = cvCloneImage(frame);
    cvFlip(frame,to_process,1);
    do_detection(to_process);

    if (showcam) {
      cvShowImage( "defcam", to_process );
      cvWaitKey(10);
    }

    // Save memory...
    cvReleaseImage(&to_process);
    //count_fps();

    //printf("FPS: %d\n",get_fps());

    // Do not release the frame!

    err=0;
  }
#endif
}

void PointIt::toggle_cam() {
#ifdef USING_CV
  if (showcam)
    cvDestroyWindow( "defcam" );
  else
    cvNamedWindow( "defcam", CV_WINDOW_AUTOSIZE);
  showcam = !showcam;
#endif
}

int PointIt::get_error() {
  return err;
}

void PointIt::toggle_lines() {
  showlines = !showlines;
}

int PointIt::get_width() {
  return w;
}

int PointIt::get_height() {
  return h;
}
