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
#include <cmath>
#include <vector>

class Dot {
public:
  Dot() {}
  Dot(int x_, int y_) {x = x_; y = y_;}
  int x,y;
};

class ShapeDetect {
private:
  std::vector<Dot> points;

  float base_angle;
  int sides;
public:
  ShapeDetect(int num_sides);
  void add_point(int x, int y);
  bool detected();
};
