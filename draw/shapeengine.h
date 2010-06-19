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
