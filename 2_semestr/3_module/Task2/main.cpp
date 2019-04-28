#include <iostream>
#include <random>
#include "ListGraph.h"
#include "2Approximation.h"

struct Point {
  double x;
  double y;

  Point(double x, double y);
  double DistanceTo(const Point &other) const;
  double operator-(const Point &other) const;
};
double Point::DistanceTo(const Point &other) const {
  return std::pow(std::pow(x - other.x, 2) + std::pow(y - other.y, 2), .5);
}
double Point::operator-(const Point &other) const {
  return DistanceTo(other);
}
Point::Point(double x, double y) : x(x), y(y) {}

int main() {
  std::random_device rand_dev{};
  std::mt19937 generator{rand_dev()};
  std::normal_distribution<> distribution{0, 1};
  std::vector<Point> points;
  for (int i = 0; i < 5; ++i) {
    points.emplace_back(distribution(generator), distribution(generator));
  }
  ListGraph graph;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (i == j) {
        continue;
      }
      graph.EmplaceFullEdge(i, j, points[i] - points[j]);
    }
  }
  auto approx2 = Salesman2(graph);

  return 0;
}
