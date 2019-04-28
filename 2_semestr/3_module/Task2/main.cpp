#include <iostream>

#include "ArcGraph.h"
int main() {
  int vertices, edges;
  std::cin >> vertices >> edges;
  //Reading the graph
  ArcGraph graph;
  for (int i = 0; i < edges; ++i) {
    int from, to, weight;
    std::cin >> from >> to >> weight;
    graph.EmplaceFullEdge(from - 1, to - 1, weight);
  }
  //Finding MST
  auto MST = graph.FindMST();
  std::cout << MST.GraphWeight();
  return 0;
}
