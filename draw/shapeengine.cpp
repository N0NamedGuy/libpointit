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
