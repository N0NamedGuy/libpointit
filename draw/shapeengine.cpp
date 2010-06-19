#include "shapeengine.h"

#define TOLERANCE 10  // Degree tolerance
#define PI 3.14159265

ShapeDetect::ShapeDetect(int num_sides) {
  sides = num_sides;
  base_angle = 0;
}

void ShapeDetect::add_point(int x, int y) {
  Dot tmp_dot(x,y);
  
  if (points.size() > 0) {
    int ang_diff = 360 / sides;
    long double angle = cos(x) / 
      std::abs( (float)(x - points[points.size() -1 ].x));

    angle = angle * 180 / PI;

    if (base_angle - angle < ang_diff - TOLERANCE &&
	base_angle - angle > ang_diff + TOLERANCE) {
      
      return;
    }
  }  

  points.push_back(tmp_dot);
}
