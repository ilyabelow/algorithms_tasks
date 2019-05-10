#include <iostream>
#include <random>
#include "ListGraph.h"
#include "Salesman.h"
#include "Point.h"
#include <iomanip>
const ListGraph generateRandomGraph(int vertices, double center, double dispersion);
double standardDeviation(const std::vector<double> &set);
double mean(const std::vector<double> &set);

int main() {
  std::cout << std::fixed;

  //Test 5 configurations for vertices cont from 4 to 8
  for (int i = 4; i < 9; ++i) {
    //Tabel header
    std::cout << "| " << i << " vestices                                    |" << std::endl;
    std::cout << "| optimum |  2 appr  | error | 1,5 appr | error |" << std::endl;
    std::vector<double> errors_1_5(5);
    std::vector<double> errors_2(5);
    for (int j = 0; j < 5; ++j) {
      ListGraph graph = generateRandomGraph(i, 0, 1);
      //Calculating approximations and optimum
      auto opt = Optimum(graph);
      auto approx2 = Approximation2(graph);
      auto approx1_5 = Approximation1_5(graph);
      double optWeight = opt.GraphWeight();
      double appr2Weight = approx2.GraphWeight();
      double appr1_5Weight = approx1_5.GraphWeight();
      errors_2[j] = (appr2Weight - optWeight) / optWeight * 100;
      errors_1_5[j] = (appr1_5Weight - optWeight) / optWeight * 100;

      //Nice printing
      std::cout << "|";
      std::cout << std::setw(8) << std::setprecision(2) << optWeight << " |";
      std::cout << std::setw(9) << std::setprecision(2) << appr2Weight << " |";
      std::cout << std::setw(5) << std::setprecision(0) << (appr2Weight - optWeight) / optWeight * 100 << "% |";
      std::cout << std::setw(9) << std::setprecision(2) << appr1_5Weight << " |";
      std::cout << std::setw(5) << std::setprecision(0) << (appr1_5Weight - optWeight) / optWeight * 100 << "% |";
      std::cout << std::endl;
    }
    //Statistics printing
    std::cout << "| 2 mean | 2 std dev |  1,5 mean |  1,5 std dev |" << std::endl;
    std::cout << "| ";
    std::cout << std::setw(5) << std::setprecision(0) << mean(errors_2) << "% | ";
    std::cout << std::setw(8) << std::setprecision(0) << standardDeviation(errors_2) << "% |";
    std::cout << std::setw(9) << std::setprecision(0) << mean(errors_1_5) << "% | ";
    std::cout << std::setw(11) << std::setprecision(0) << standardDeviation(errors_1_5) << "% |";
    std::cout << std::endl;

    std::cout << std::endl;
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

//Calculate mean for set of values
double mean(const std::vector<double> &set) {
  double sum = 0;
  for (auto i : set) {
    sum += i;
  }
  return sum / set.size();
}

//Calculate standart deviation for set of values
double standardDeviation(const std::vector<double> &set) {
  double sum = 0;
  double mean_value = mean(set);
  for (auto i : set) {
    sum += std::pow(i - mean_value, 2);
  }
  return std::pow(sum / set.size(), .5);
}
