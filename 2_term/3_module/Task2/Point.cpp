
#include "Point.h"
#include <cmath>

double Point::DistanceTo(const Point &other) const {
  return std::pow(std::pow(x - other.x, 2) + std::pow(y - other.y, 2), .5);
}
Point::Point(double x, double y) : x(x), y(y) {}