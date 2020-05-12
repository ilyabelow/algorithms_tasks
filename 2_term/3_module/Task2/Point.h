#pragma once

//Struct for a point on a R^2
struct Point {
  double x;
  double y;

  Point(double x, double y);
  //Mesure euclidean distance to another point
  double DistanceTo(const Point &other) const;
};
