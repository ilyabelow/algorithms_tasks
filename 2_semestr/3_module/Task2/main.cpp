#include <iostream>
#include <random>
#include "ListGraph.h"
#include "Salesman.h"
#include "Point.h"
#include <iomanip>
const ListGraph generateRandomGraph(int vertices, double center, double dispersion);

int main() {
  std::cout << std::fixed;
  //Tabel header
  std::cout << "vertices | optimum |  2 appr  | error | 1,5 appr | error |" << std::endl;
  std::cout << "-----------------------------------------------------------" << std::endl;
  //Test 5 configurations for vertices cont from 4 to 8
  for (int i = 4; i < 9; ++i) {
    for (int j = 0; j < 5; ++j) {
      ListGraph graph = generateRandomGraph(i, 0, 1);
      //Calculating approximations and optimum
      auto opt = Optimum(graph);
      auto approx2 = Approximation2(graph);
      auto approx1_5 = Approximation1_5(graph);
      double optWeight = opt.GraphWeight();
      double appr2Weight = approx2.GraphWeight();
      double appr1_5Weight = approx1_5.GraphWeight();
      //Nice printing
      std::cout << std::setw(8) << i << " |";
      std::cout << std::setw(8) << std::setprecision(2) << optWeight << " |";
      std::cout << std::setw(9) << std::setprecision(2) << appr2Weight << " |";
      std::cout << std::setw(5) << std::setprecision(0) << (appr2Weight - optWeight) / optWeight * 100 << "% |";
      std::cout << std::setw(9) << std::setprecision(2) << appr1_5Weight << " |";
      std::cout << std::setw(5) << std::setprecision(0) <<(appr1_5Weight - optWeight) / optWeight * 100 << "% |" ;
      std::cout << std::endl;
    }
  }
  return 0;
}

//Generate euclidean graph with normally distributed points
const ListGraph generateRandomGraph(const int vertices, const double center, const double dispersion) {
  //Some stuff for random generator
  std::random_device rand_dev{};
  std::mt19937 generator{rand_dev()};
  std::normal_distribution<> distribution{center, dispersion};
  //Point of the graph
  std::vector<Point> points;
  for (int i = 0; i < vertices; ++i) {
    points.emplace_back(distribution(generator), distribution(generator));
  }
  //Filling the graph
  ListGraph graph(vertices);
  for (int i = 0; i < graph.VerticesCount(); ++i) {
    for (int j = 0; j < graph.VerticesCount(); ++j) {
      if (i == j) {
        continue;
      }
      //weight = distance
      graph.EmplaceEdge(i, j, points[i].DistanceTo(points[j]));
    }
  }
  return graph;
}